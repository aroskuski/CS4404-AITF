#!/bin/bash

ip route add 10.4.20.1 via 10.4.20.3 proto static
ip route add 10.4.20.2 via 10.4.20.3 proto static
ip route add 10.4.20.4 via 10.4.20.3 proto static
ip route add 10.4.20.5 via 10.4.20.3 proto static
ip route add 10.4.20.8 via 10.4.20.7 proto static

