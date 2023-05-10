#!/bin/bash
# Job name
#SBATCH --job-name sequentialCPA
# Submit to the primary QoS
#SBATCH -q primary
# Request one node
#SBATCH -N 1
# Request entire node - the job allocation can not share nodes with other running jobs
#SBATCH --exclusive
# Total number of cores
#SBATCH -n 1
# Request memory
#SBATCH --mem=128G
# Mail when the job begins, ends, fails, requeues
#SBATCH --mail-type=ALL
# Where to send email alerts
#SBATCH --mail-user=gp6989@wayne.edu
# Create an output file that will be output_<jobid>.out
#SBATCH -o output_%j.out
# Create an error file that will be error_<jobid>.out
#SBATCH -e errors_%j.err
# Set maximum time limit
#SBATCH -t 24:00:00

cd /wsu/home/gp/gp69/gp6989/graphCompression/
#cd $TMPDIR

#cp -r /wsu/home/gp/gp69/gp6989/graphCompression/New_generated_data/ $TMPDIR
#cp /wsu/home/gp/gp69/gp6989/graphCompression/parallelCpaExe  $TMPDIR


echo "nodes,density,experimentNo,compressionRatio,elapsedTime,cores" > Grid_sequential_CPA_results.csv
for node in  32 #64 128 256 512 1024 2048 4096 8192 16384
do
    	for density in 80 #85 90 95 98 
    	do
        	for exp in 1 #2 3 4 5 6 7 8 9 10
        	do
			#echo "$node,$density,$exp" >> Grid_sequential_CPA_results.csv
            		./sequential  $node $density $exp >> Grid_sequential_CPA_results.csv
            		echo $node $density $exp
        	done
    	done
done


