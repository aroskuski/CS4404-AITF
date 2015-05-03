#!/bin/bash

for i in `seq 1 4`; do
    scp -r /home/andrew/git/CS4404-AITF/src cs4404@10.4.20.$i:/home/cs4404/CS4404-AITF/
    #scp -r /home/andrew/git/CS4404-AITF/starthost.sh cs4404@10.4.20.$i:/home/cs4404/CS4404-AITF/
    #scp -r /home/andrew/git/CS4404-AITF/startgateway.sh cs4404@10.4.20.$i:/home/cs4404/CS4404-AITF/
    #scp -r /home/andrew/git/CS4404-AITF/route$i.sh cs4404@10.4.20.$i:/home/cs4404/CS4404-AITF/route.sh
done

