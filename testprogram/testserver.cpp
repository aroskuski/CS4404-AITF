#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

int main(){
	
	int s = socket(AF_INET, SOCK_STREAM, 0);
	
	addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(0, "9001", &hints, &res);
	
	bind(s, res->ai_addr, res->ai_addrlen);
	listen(s, 10);
	
	struct sockaddr_storage their_addr;
    socklen_t addr_size;
	
	for(;;){
		addr_size = sizeof(their_addr);
		int cs = accept(s,(sockaddr *) &their_addr, &addr_size);
		
		if(fork() == 0){
			char buf[9000];
			int count = 0;
			long long bytes = 0;
			int len;
			while((len = recv(cs, buf, 9000, 0)) > 0){
				count++;
				bytes += len;
			}
			
			std::cout << "Recieved " << bytes << " bytes from " << count << " calls to recv()\n";
			socklen_t optlen = sizeof(struct tcp_info);
			struct tcp_info tcpinfo;
			getsockopt(cs, SOL_TCP, TCP_INFO, (void *)(&tcpinfo), &optlen);
			std::cout << "RTT: " << tcpinfo.tcpi_rtt << " usec\n";
			std::cout << "exiting\n";
			shutdown(cs, 2);
			exit(0);
		}
		//shutdown(cs, 2);
	}
	
}