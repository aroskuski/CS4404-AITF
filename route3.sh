#!/bin/bash

ip route add 10.4.20.1 via 10.4.20.2 proto static
ip route add 10.4.20.5 via 10.4.20.4 proto static
ip route add 10.4.20.7 via 10.4.20.6 proto static
ip route add 10.4.20.8 via 10.4.20.6 proto static
