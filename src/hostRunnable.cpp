//
//  main.cpp
//  hostAITF
//
//  Created by Alexander W. Witt on 5/1/15.
//  Copyright (c) 2015 ___Alexander W. Witt & Andrew J. Roskuski___. All rights reserved.
//

#include <iostream>
#include "Host.h"
#include "HostFilter.h"

/**
 * This main() function is used for executing all code
 * used by an AITF host in our AITF implementation.
 */
int main(int argc, const char * argv[]) {

    // declaration of local variables for main function
    bool isVerbose = false;

    // determine whether or not the user has entered any arguments
    if (argc != 2 && argc != 1) {
        std::cout << "usage: ./hostAITF [--verbose]" << std::endl;
        exit(1);
    }

    // indicate that the verbose option has been set
    if (argc == 2 && strncmp(argv[1], "--verbose", 9) == 0) {
        isVerbose = true;
    }

    // begin executing the code for operating the AITF host
    if (isVerbose) {
        std::cout << std::endl;
        std::cout << "+------------------------------+" << std::endl;
        std::cout << "|  LAUCHING AITF HOST PROGRAM  |" << std::endl;
        std::cout << "+------------------------------+" << std::endl;
        std::cout << std::endl;
        std::cout << "* Remember to enter ^C to kill program when done with use" << std::endl;
        std::cout << "- Starting code for running AITF host..." << std::endl;
    }

    // declaration of local variables for AITF host code
    Host host = Host();
    HostFilter filter = HostFilter();
    filter.startFilterThread(&host);

    // declaration of threads used for the AITF program
    pthread_t taskThread;
    pthread_t blockThread;

    sem_init(&Host::sem, 0, 0);
    sem_init(&Host::qsem, 0, 1);
    sem_init(&Host::lsem, 0, 1);

    pthread_create(&taskThread, NULL, &hostTaskThread, (void*) &host);
    pthread_create(&blockThread, NULL, &hostBlockCleanupThread, (void*) &host);


    // network communication code used by the AITF host
    Host::s = -1;
    while(Host::s < 0) {
        Host::s = socket(AF_INET, SOCK_STREAM, 0);
    }

    addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        std::cerr << "ERROR: getaddrinfo()" << std::endl;
        exit(1);
    }
    if (bind(Host::s, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "ERROR: bind()" << std::endl;
        exit(1);
    };
    listen(Host::s, 10);

    sockaddr_storage their_addr;

    // print out information to indicate progress to user
    if (isVerbose) {
        std::cout << "- Entering accept loop..." << std::endl;
    }

    while(true) {
        unsigned int addr_size = sizeof(their_addr);
        int sock = accept(Host::s, (sockaddr *) &their_addr, &addr_size);
        if (sock != -1) {
            pthread_t t;
            pthread_create(&t, NULL, &hostRecvThread, (void*) sock);
        }
        else {
            std::cerr << "ERROR: accept()" << std::endl;
        }
    }

    // return never called because ^C is required to kill program
    return 0;
}
