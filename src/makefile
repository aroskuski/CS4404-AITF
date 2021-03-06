# File: Makefile
# Description: A file that can be used with the make utility in order to
# generate two targets. One target is the executable binary used by all
# hosts in the AITF implementation and the other target is the executable
# binary used by all gateways in the AITF implementation.
# Version: 1.0

all: hostAITF gatewayAITF

hostAITF: hostRunnable.o Flow.o Hash.o Host.o HostFilter.o HostPolicyModule.o Logger.o Packet.o ShadowTable.o
	g++ hostRunnable.o Flow.o Hash.o Host.o HostFilter.o HostPolicyModule.o Logger.o Packet.o ShadowTable.o -o hostAITF -lnetfilter_queue -lpthread -lcrypto

hostRunnable.o: hostRunnable.cpp
	g++ -c hostRunnable.cpp

gatewayAITF: gatewayRunnable.o Gateway.o GatewayPolicyModule.o GatewayFilter.o Flow.o Hash.o ShadowTable.o Logger.o Packet.o
	g++ gatewayRunnable.o Gateway.o GatewayPolicyModule.o GatewayFilter.o Flow.o Hash.o ShadowTable.o Logger.o Packet.o -o gatewayAITF -lpthread -lssl -lcrypto -lnetfilter_queue

gatewayRunnable.o: gatewayRunnable.cpp
	g++ -c gatewayRunnable.cpp

# All host object files
Host.o: Host.cpp Host.h
	g++ -c Host.cpp

HostPolicyModule.o: HostPolicyModule.cpp HostPolicyModule.h
	g++ -c HostPolicyModule.cpp

HostFilter.o: HostFilter.cpp HostFilter.h
	g++ -c HostFilter.cpp

# All gateway object files
Gateway.o: Gateway.cpp Gateway.h
	g++ -c Gateway.cpp

GatewayPolicyModule.o: GatewayPolicyModule.cpp GatewayPolicyModule.h
	g++ -c GatewayPolicyModule.cpp

GatewayFilter.o: GatewayFilter.cpp GatewayFilter.h
	g++ -c GatewayFilter.cpp

# All commonly shared object files
Flow.o: Flow.cpp Flow.h
	g++ -c Flow.cpp

Hash.o: Hash.cpp Hash.h
	g++ -c Hash.cpp

ShadowTable.o: ShadowTable.cpp ShadowTable.h
	g++ -c ShadowTable.cpp

Logger.o: Logger.cpp Logger.h
	g++ -c Logger.cpp

Packet.o: Packet.cpp Packet.h
	g++ -c Packet.cpp

clean:
	rm -f *.o
	rm -f hostAITF
	rm -f gatewayAITF
