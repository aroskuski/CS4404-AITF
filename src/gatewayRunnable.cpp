#include <iostream>
#include "GatewayFilter.h"
#include "Gateway.h"


int main(int argc, const char * argv[]) {
    Gateway gateway = Gateway();
    GatewayFilter filter = GatewayFilter();
    struct gwFilterData* gwFD;
    gwFD->st = &gateway.st;
    gwFD->gw = &gateway;

    filter.startFilterThread(gwFD);

    gateway.initBlacklist();

    pthread_t taskThread;
    pthread_t blockThread;

    sem_init(&Gateway::sem, 0 , 0);
    sem_init(&Gateway::qsem, 0 , 1);
    sem_init(&Gateway::lsem, 0, 1);

    pthread_create(&taskThread, NULL, &gatewayTaskThread, (void *) &gateway);
    pthread_create(&blockThread, NULL, &gatewayBlockCleanupThread, (void *) &gateway);

    int s = -1;
    while(s < 0){
        s = socket(AF_INET, SOCK_STREAM, 0);
    }

    addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "1025", &hints, &res);

    bind(Gateway::s, res->ai_addr, res->ai_addrlen);

    listen(Gateway::s,10);

    int len;
    char buf[1024];
    sockaddr_storage their_addr;
    for(;;){
        unsigned int addr_size = sizeof(their_addr);
        int sock = accept(Gateway::s, (sockaddr *) &their_addr, &addr_size);
        pthread_t t;
        pthread_create(&t, NULL, &recvBlockReq, (void *)sock);
    }
}
