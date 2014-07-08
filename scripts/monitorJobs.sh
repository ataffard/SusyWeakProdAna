#!/bin/bash



while :
do
date 
squeue  -o "%.18i %.9P %.30j %.8u %.2t %.10M %.6D %R"

nJob_R=(`squeue -h -u ataffard |grep R  | wc -l`)
nJob_PD=(`squeue -h -u ataffard |grep PD  | wc -l`)

echo "Number of running jobs $nJob_R  "
echo "Number of pending jobs $nJob_PD "
echo ""
sleep 10
done