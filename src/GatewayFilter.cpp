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
void GatewayFilter::startFilterThread() {
    
}

/**
 * This is the main function used by each worker thread that is spawned
 * when the startFilterthread() method of the GatewayFilter class is used.
 * @return 0 upon success, 1 upon failure.
 */
int gatewayFilterMain() {
    
    // declaration of local variables
    int fd;
    int bytes;
    char packetBuffer[4096];
    struct nfq_handle *handle;
    struct nfq_q_handle *queueHandle;
    
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
    if (!(queueHAndle = nfq_create_queue(handle, 0, &cb, NULL))) {
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
        id = ntohl(header->packet_id);
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
        // AITFPkt->routeRecord.pktFlow[AITFPkt->routeRecord.position + 1].ip = AITFPkt->ipHeader.ip_dst;
        // AITFPkt->routeRecord.pktFlow[AITFPkt->routeRecord.position + 1].nonce = 1;
        
        return nfq_set_verdict(qh, id, NF_ACCEPT, sizeof(struct RRPacket), RRPkt);
    }
    
    // manage the packet when it is recognized as an AITF control packet w/ RR
    if (protocol == 143) {
        
        // drop the recieved packet if the route record is malformed
        if (AITFPkt->routeRecord.position + 1 > AITFPkt->routeRecord.length
            || AITFPkt->routeRecord.length != 6) {
            return nfq_set_verdict(qh, id, NF_DROP, 0, NULL);
        }
        
        // if we are the destination or the gateway to the destination read
        // otherwise just add to RR and forward
        
        // if the packet is a filtering request packet respond to the policy module
        // if the packet is a counter connection initialization respond to the policy module
        // if the packet is a counter connection response respond to the policy module
        
        // if the packet is headed to a legacy host drop the RR
        if (false) {
            newPkt->ipHeader = AITFPkt->ipHeader;
            newPkt->ipHeader.ip_p = 
            memcpy(newPkt->payload, AITFPkt->payload, 1500);
            return nfq_set_verdict(qh, id, NF_ACCEPT, sizeof(struct ipPacket), newPkt);
        }
    }
}
