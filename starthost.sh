#!/bin/bash

/usr/bin/time ./src/hostAITF > hstd.txt 2> time.txt &
sleep 5
iptables -A INPUT -j NFQUEUE --queue-num 0
iptables -A OUTPUT -j NFQUEUE --queue-num 0
sleep 60
iptables -D INPUT -j NFQUEUE --queue-num 0
iptables -D OUTPUT -j NFQUEUE --queue-num 0
killall hostAITF
