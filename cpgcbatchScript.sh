#!/bin/bash

echo nodes,density,exp,delta,compressionRatio,executionTime > cpgc_results.csv 
echo nodes,total_nodes,density,exp,delta,maximumFlow,run_time,total_run_time > bipartite_dinics_results.csv
echo nodes,total_nodes,density,exp,delta,maximumFlow,run_time,total_run_time > tripartite_dinics_results.csv
algorithm="cpgc"
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
				#fileName="datasets/cpgc_tripartite_graph_${node}_${density}_${exp}.mtx"
				./dinics_bi  $node $density $exp $delta >> bipartite_dinics_results.csv	
				./dinics_tri  $node $density $exp $delta $algorithm >> tripartite_dinics_results.csv
				#rm "$fileName"
	       		done
        	done
    	done
done
