//
//  HostFilter.cpp
//  Acquiring IP Address
//
//  Created by Alexander W. Witt on 4/28/15.
//  Copyright (c) 2015 ___Alexander W. Witt & Andrew J. Roskuski___. All rights reserved.
//

#include "HostFilter.h"

/**
 * This method is used spawning a thread for carrying out the
 * three responsibilities listed for the HostFilter class.
 */
void HostFilter::startFilterThread() {
    
}

/**
 * This is the main function used by each worker thread that is spawned
 * when the startFilterThread() method of the HostFilter class is used.
 * @return 0 upon success, 1 upon failure.
 */
int hostFilterMain() {
    
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
    if (nfq_unbind_pf(handle, AF_INET < 0)) {
        std::cerr << "ERROR: nfq_unbind_pf()" << std::endl;
        exit(1);
    }
    
    // bind the handle so that it can process IPv4 packets
    if (nfq_bind_pf(handle, AF_INET) < 0) {
        std::cerr << "ERROR: nfq_bind_pf()" << std::endl;
        exit(1);
    }
    
    // create the handle for the nfq queue and ensure that it is linked to a callback
    if (!(queueHandle = nfq_create_queue(handle, 0, &cb, NULL))) {
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
            std::cout << "Packet received..." << std::endl;
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
    u_int16_t id;
    u_int8_t protocol;
    unsigned char* pktData;
    
    struct ipPacket* regularPkt;
    struct ipPacket* newPkt;
    struct AITFPacket* AITFPkt;
    struct routeRecord* routeRecord;
    struct nfqnl_msg_packet_hdr *header;

    // initialization of local variables
    int len = nfq_get_payload(pkt, &pktData);
    // ipAddress = (u_int32_t) cbData;
    
    // acquire the netfilter id of the received packet
    if ((header = nfq_get_msg_packet_hdr(pkt))) {
        id = ntohl(header->packet_id);
    }

    // acquire the received packet and read in its contents
    regularPkt = (struct ipPacket*) pktData;
    AITFPkt = (struct AITFPacket*) pktData;

    // determine if the packet is an AITF packet
    protocol = (u_int8_t) regularPkt->ipHeader.ip_p;
    
    // manage the packet when it is recognized as an AITF packet
    if (protocol == 61) {
        
        // drop the received packet if the route record is malformed
        if (AITFPkt->rr.position + 1 > AITFPkt->rr.length) {
            return nfq_set_verdict(qh, id, NF_DROP, 0, NULL);
        }
        
        // add to the route record of the received AITF packet
        AITFPkt->rr.pktFlow[AITFPkt->rr.position + 1].ip = AITFPkt->ipHeader.ip_dst;
        AITFPkt->rr.pktFlow[AITFPkt->rr.position + 1].nonce = 1;
        
        // produce the flow to send to the policy module
        Flow flow = Flow(&AITFPkt->rr.pktFlow, AITFPkt->rr.length);

        // provide observed packet flow to host policy module
        HostPolicyModule policyModule = HostPolicyModule();
        policyModule.checkHighFlowPolicy(flow);

        // remove the route record shim from the packet
        newPkt->ipHeader = AITFPkt->ipHeader;
        newPkt->ipHeader.ip_p = AITFPkt->rr.protocol;
        newPkt->ipHeader.ip_sum =
        memcpy(newPkt->payload, AITFPkt->payload, 1500);
    }
    return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}
