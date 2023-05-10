#!/bin/bash


echo "nodes,density,experimentNo,compressionRatio,elapsedTime,cores" > cpgc_results.csv
for node in  32 #64 128 256 512 1024 2048 4096 8192 16384
do
    	for density in 80 #85 90 95 98 
    	do
        	for exp in 1 #2 3 4 5 6 7 8 9 10
        	do
			#echo "$node,$density,$exp" >> Grid_sequential_CPA_results.csv
            		./cpgc  $node $density $exp >> cpgc_results.csv
            		echo $node $density $exp
        	done
    	done
done


