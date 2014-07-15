#!/bin/bash



while :
do
date 
squeue  -o "%.18i %.9P %.30j %.8u %.2t %.10M %.6D %R"


nJob_a_R=(`squeue -h -o "%.8u %.2t"  |grep R  | wc -l`)
nJob_a_PD=(`squeue -h  -o "%.8u %.2t" |grep PD  | wc -l`)
nJob_u_R=(`squeue -h -u ${USER} -o "%.8u %.2t"  |grep R  | wc -l`)
nJob_u_PD=(`squeue -h -u ${USER} -o "%.8u %.2t" |grep PD  | wc -l`)

echo "Number of total running jobs $nJob_a_R  "
echo "Number of total pending jobs $nJob_a_PD  "
echo "Number of user's running jobs $nJob_u_R  "
echo "Number of user's pending jobs $nJob_u_PD "
echo ""
sleep 10
done