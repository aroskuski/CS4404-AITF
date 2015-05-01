//
//  GatewayFilter.cpp
//  
//
//  Created by Alexander W. Witt on 4/29/15.
//
//

#include "GatewayFilter.h"

/**
 * This method is used spawning a thread for carrying out the
 * six responsibilities listed for the GatewayFilter class.
 */
void GatewayFilter::startFilterThread(gwFilterData* gwFD) {
	pthread_t gatewayFilterThread;
	    if (pthread_create(&gatewayFilterThread, NULL, &gatewayFilterMain, (void*) gwFD)) {
	    	std::cout << "ERROR: pthread_create()" << std::endl;
	    	exit(1);
	    }
}

/**
 * This is the main function used by each worker thread that is spawned
 * when the startFilterthread() method of the GatewayFilter class is used.
 * @return 0 upon success, 1 upon failure.
 */
void* gatewayFilterMain(void* arg) {
    
    // declaration of local variables
    int fd;
    int bytes;
    char packetBuffer[4096];
    struct nfq_handle *handle;
    struct nfq_q_handle *queueHandle;
    gwFilterData* gwFD = (gwFilterData*) arg;
    
    // attempt to open a queue connection handle from the netfilter module
    if (!(handle = nfq_open())) {
        std::cerr << "ERROR: nfq_open()" << std::endl;
        exit(1);
    }
    
    // unbind the handle to prevent any inability to bind
    if (nfq_unbind_pf(handle, AF_INET) < 0) {
        std::cerr << "ERROR: nfq_unbind_pf()" << std::endl;
        exit(1);
    }
    
    // bind the handle so that it can process IPv4 packets
    if (nfq_bind_pf(handle, AF_INET) < 0) {
        std::cerr << "ERROR: nfq_bind_pf()" << std::endl;
        exit(1);
    }
    
    // create the handle for the nfq queue and ensure that it is linked to a callback
    if (!(queueHandle = nfq_create_queue(handle, 0, &cb, (void*) gwFD))) {
        std::cerr << "ERROR: nfq_create_queue()" << std::endl;
        exit(1);
    }
    
    // enable all packet data for every queued packet to be copied and read into user space
    if (nfq_set_mode(queueHandle, NFQNL_COPY_PACKET, 0xffff) < 0) {
        std::cerr << "ERROR: packet copy mode" << std::endl;
        exit(1);
    }
    
    // create a file descriptor that can be used to read from the queue
    fd = nfq_fd(handle);
    if (fd == -1) {
        std::cerr << "ERROR: nfq_fd()" << std::endl;
        exit(1);
    }
    
    // when a packet is received from the queue go and handle it with the callback
    while (true) {
        if ((bytes = recv(fd, packetBuffer, sizeof(packetBuffer), 0)) >= 0) {
            nfq_handle_packet(handle, packetBuffer, bytes);
        }
    }
    return 0;
}

/**
 * This is a callback function that is used by the netfilter module
 * whenever a packet is intercepted and read from the netfilter queue.
 * @param qh a pointer to the handle for the nfq queue
 * @param msg a pointer to an nfgenmsg structure
 * @param pkt a pointer to the packet retrieved from the nfq queue
 * @param cbData a pointer to the data of the callback function
 * @return 0 upon success
 */
static int cb(struct nfq_q_handle *qh, struct nfgenmsg *msg, struct nfq_data *pkt, void *cbData) {
    
    // declaration of local variables
    uint16_t id;
    uint8_t protocol;
    unsigned char* pktData;
    gwFilterData* gwFD = (gwFilterData*) cbData;
    
    struct ipPacket* regularPkt;
    struct ipPacket* newPkt;
    struct RRPacket* RRPkt;
    struct AITFPacket* AITFPkt;
    struct routeRecord* routeRecord;
    struct nfqnl_msg_packet_hdr* header;
    
    // initialization of local variables
    int len = nfq_get_payload(pkt, &pktData);
    
    // acquire the netfilter id of the received packet
    if ((header = nfq_get_msg_packet_hdr(pkt))) {
        id = ntohl((uint32_t)header->packet_id);
    }
    
    // acquire the received packet and read in its contents
    regularPkt = (struct ipPacket*) pktData;
    AITFPkt = (struct AITFPacket*) pktData;
    RRPkt = (struct RRPacket*) pktData;
    
    // determine if the packet is an AITF packet
    protocol = (uint8_t) regularPkt->ipHeader.ip_p;
    
    // manage the packet when it is recognized as an RR packet
    if (protocol == 61) {
        
        // drop the received packet if the route record is malformed
        if (RRPkt->routeRecord.position + 1 > RRPkt->routeRecord.length
            || RRPkt->routeRecord.length != 6) {
            return nfq_set_verdict(qh, id, NF_DROP, 0, NULL);
        }
        
        // add to the route record of the received AITF packet
        // AITFPkt->rr.pktFlow[AITFPkt->rr.position + 1].ip = htonl()
        AITFPkt->rr.pktFlow[AITFPkt->rr.position + 1].nonce = htons(checksum(AITFPkt, sizeof(AITFPacket)));

        // create a flow object from the observed route record
        Flow flow = Flow(AITFPkt->rr.pktFlow, AITFPkt->rr.length - (AITFPkt->rr.length - AITFPkt->rr.position + 1));
        
        // determine if the flow is in the shadowtable and respond to the gateway if it is
        if (gwFD->st->containsFlow(flow)) {
        	gwFD->gw->sendMessage(flow);
        	return nfq_set_verdict(qh, id, NF_DROP, 0, NULL);
        }

        // determine whether the next hop is legacy and remove the route record if it is
        if (gwFD->gw->checkBlacklist(ntohl((uint32_t)AITFPkt->ipHeader.ip_dst))) {
        	RRPkt = (struct RRPacket*) AITFPkt;
        	regularPkt->ipHeader = RRPkt->ipHeader;
        	memset(regularPkt->payload, 0, 1500);
        	memcpy(regularPkt->payload, RRPkt->payload, 1500);
        	regularPkt->ipHeader.ip_sum = checksum(regularPkt, sizeof(ipPacket));
        	return nfq_set_verdict(qh, id, NF_ACCEPT, sizeof(struct ipPacket), (unsigned char*)regularPkt);
        }

        return nfq_set_verdict(qh, id, NF_ACCEPT, sizeof(struct RRPacket), (unsigned char*)RRPkt);
    }
    
    // add a route record shim to the received packet if the next hop is not the destination
    if (!(gwFD->gw->checkBlacklist(ntohl((uint32_t)regularPkt->ipHeader.ip_dst)))) {
    	RRPkt = (struct RRPacket*) regularPkt;
    	RRPkt->ipHeader = regularPkt->ipHeader;
    	RRPkt->ipHeader.ip_p = 61;
    	memset(RRPkt->payload, 0, 1500);
    	memcpy(RRPkt->payload, regularPkt->payload, 1500);
    	RRPkt->routeRecord.length = 10;
    	// RRPkt->routeRecord.pktFlow[0] = our IP
    	RRPkt->routeRecord.position = 0;
    	RRPkt->routeRecord.protocol = regularPkt->ipHeader.ip_p;
    	RRPkt->ipHeader.ip_sum = checksum(regularPkt, sizeof(ipPacket));
    	return nfq_set_verdict(qh, id, NF_ACCEPT, sizeof(struct RRPkt), (unsigned char*)RRPkt);
    }
}
