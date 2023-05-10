#!/bin/bash

echo "nodes,density,experimentNo,compressionRatio,elapsedTime,cores" > Grid_fm_CPA_results.csv
for node in  32 64 128 
do
    	for density in 80 85 90 95 98 
    	do
        	for exp in 1 2 3 4 5 6 7 8 9 10
        	do
			#echo "$node,$density,$exp" >> Grid_fm_CPA_results.csv
            		./fm  $node $density $exp >> Grid_fm_CPA_results.csv
            		echo $node $density $exp
        	done
    	done
done


