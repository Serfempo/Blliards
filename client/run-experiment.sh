#!/bin/sh
echo ${HOST} `uptime` >> exp-host.list
cd ~/FastFiz-0.2/client
./client -c experiment.conf -m f -f 2 -t v &> ${PBS_JOBID}.out
