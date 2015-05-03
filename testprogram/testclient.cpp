#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netinet/tcp.h>

int main(int argc, char** argv){
	//int s = socket(AF_INET, SOCK_STREAM, 0);
	
	addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE;
	std::cout << "about to getaddrinfo\n";
	std::cout << argv[1] << "\n";
	int status = getaddrinfo(argv[1], "9001", &hints, &res);
	//std::cout << gai_strerror(status) << "\n";
	std::cout << res << "\n";
	std::cout << "about to socket\n";
	int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	std::cout << "about to connect\n";
	connect(s, res->ai_addr, res->ai_addrlen);
	
	
	char buf[9000];
	timeval tv;
	timeval tv2;
	timeval tres;
	std::cout << "about to gettimeofday\n";
	gettimeofday(&tv, 0);
	
	gettimeofday(&tv2, 0);
	timersub(&tv2, &tv, &tres);
	std::cout << "about to loop\n";
	std::cout << tres.tv_sec << "\n";
	int c = 0;
	long long bytes = 0;
	int len;
	while(tres.tv_sec < 60){
		//std::cout << "about to send\n";
		len = send(s, buf, 100, MSG_NOSIGNAL);
		//std::cout << "about to gettimeofday\n";
		gettimeofday(&tv2, 0);
		//std::cout << "about to timersub\n";
		timersub(&tv2, &tv, &tres);
		//std::cout << tres.tv_sec << "\n";
		c++;
		bytes += len;
	}
	std::cout << "Sent " << bytes << " bytes with " << c << " calls to send()\n";
	socklen_t optlen = sizeof(struct tcp_info);
	struct tcp_info tcpinfo;
	getsockopt(s, SOL_TCP, TCP_INFO, (void *)(&tcpinfo), &optlen);
	std::cout << "RTT: " << tcpinfo.tcpi_rtt << " usec\n";
	
	shutdown(s, 2);
	exit(0);
	
	
}