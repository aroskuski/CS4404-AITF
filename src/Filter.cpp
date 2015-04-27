/*
 * Filter.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "Filter.h"

/**
 * Constructor for the Filter class.
 * @param st a pointer to the shadowtable for this filter instance
 */
Filter::Filter(ShadowTable &st) {
	//this->st = st;
	//this->h = Host();
	//this->g = Gateway();
}

/**
 * Destructor for the Filter class.
 */
Filter::~Filter() {
	// TODO Auto-generated destructor stub
}

/**
 * This method is used to initiate the thread
 * used for monitoring and modifying packets.
 * @param host whether or not the machine is a host
 */
void Filter::startFilterThread(bool host) {
	// pthread create ... and populate with the main method
}

/**
 * This method is essentially the method run by the
 * thread used for filtering in the Filter class.
 */
void Filter::filterMain(bool host) {

	// declaration of local variables
	struct nfq_handle *handle;
	struct nfq_q_handle *queueHandle;

	int fd;
	int bytes;
	char packetBuffer[4096];

	// cleanup allocated resources just in case they still exist
	nfq_close(handle);
	nfq_destroy_queue(queueHandle);

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
	if (!(queueHandle = nfq_create_queue(handle, 0, &callback, (void*) host))) {
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

	// if we received a pakcet from the queue go and have the callback handle it
	while (true) {
		if ((bytes = recv(fd, packetBuffer, sizeof(packetBuffer), 0)) >= 0) {
			nfq_handle_packet(handle, packetBuffer, bytes);
		}
	}
}

/**
 * This is a callback function that is used by netfilter
 * whehever a packet is intercepted and read from the netfilter queue.
 * @param qh a pointer to the handle for the nfq queue
 * @param msg a pointer to an nfgenmsg structure
 * @param pkt a pointer to the packet retrieved from the nfq queue
 * @param cbData a pointer to the data of the callback function
 * @return 0 upon success
 */
static int callback(struct nfq_q_handle *qh, struct nfgenmsg *msg, nfq_data *pkt, void *cbData) {

	//declaration of local variables
	u_int16_t id;
	u_int8_t protocol;
	struct nfqnl_msg_packet_hdr *header;
	unsigned char* pktData;

	// initialization of local variables
	id = 0;
	protocol = 0;

	// acquire the ID of the received packet
	if ((header = nfq_get_msg_packet_hdr(pkt))) {
		id = ntohl(header->packet_id);
	}

	// read the entire packet into memory for manipulation and reading
	int len = nfq_get_payload(pkt, &pktData);

	return 0;
}

