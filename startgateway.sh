#!bin/bash

GatewayAITF &
sleep 5
iptables -A INPUT -J NFQUEUE--queue-num 0
iptables -A OUTPUT -J NFQUEUE--queue-num 0
