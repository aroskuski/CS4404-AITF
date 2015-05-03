#!/bin/bash

apt-get update
apt-get install -y g++ libnetfilter-queue-dev make zlib1g-dev libssl-dev
#dpkg -i libssl-dev_1.0.1f-1ubuntu2.8_amd64.deb
ip route flush cache
sysctl -w net.ipv4.ip_forward=1
sysctl -w net.ipv4.conf.eth0.accept_redirects=0
sysctl -w net.ipv4.conf.all.send_redirects=0
sysctl -p /etc/sysctl.conf
./route.sh
cd src
make clean
make
cd ..

