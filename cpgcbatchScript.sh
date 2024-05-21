#!/bin/bash
# Job name
#SBATCH --job-name parallel_random_CPGC
# Submit to the primary QoS
#SBATCH -q primary
# Request one node
#SBATCH -N 1
# Request entire node - the job allocation can not share nodes with other running jobs
#SBATCH --exclusive
# Total number of cores
#SBATCH -n 1
# Request memory
#SBATCH --mem=250G
# Mail when the job begins, ends, fails, requeues
#SBATCH --mail-type=ALL
# Where to send email alerts
#SBATCH --mail-user=gw4590@wayne.edu
# Create an output file that will be output_<jobid>.out
#SBATCH -o output_%j.out
# Create an error file that will be error_<jobid>.out
#SBATCH -e errors_%j.err
# Set maximum time limit
#SBATCH -t 96:00:00
#cd /wsu/home/gw/gw45/gw4590/Graph_Compression
echo nodes,density,exp,delta,compressionRatio,executionTime > cpgc_results.csv 
echo nodes,total_nodes,density,exp,delta,maximumFlow,run_time,total_run_time > bipartite_dinics_results.csv
echo nodes,total_nodes,density,exp,delta,maximumFlow,run_time,total_run_time > tripartite_dinics_results.csv
algorithm = "cpgc"
for node in  32 64 128 256 512 1024 2048 4096 8192 16384 32768 
do
    	for density in 80 85 90 95 98 
    	do
        	for exp in 1 2 3 4 5 6 7 8 9 10
        	do
	 		for delta in 0.5 0.6 0.7 0.8 0.9 1
    			do
       				./cpgc  $node $density $exp $delta>> cpgc_results.csv
            			echo $node $density $exp
				fileName="datasets/tripartite_graph_${node}_${density}_${exp}.mtx"
				./dinics_bi  $node $density $exp $delta >> bipartite_dinics_results.csv	
				./dinics_tri  $node $density $exp $delta $algorithm >> tripartite_dinics_results.csv
				rm "$fileName"
	       		done
        	done
    	done
done
