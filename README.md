


# ReadMe: A Clique Partitioning-Based Algorithm for Graph Compression

## The provided Git repository includes the following:

- Implementation of both Feder-Motwani (FM) and CPGC algorithm in C `fm.c` and `cpgc.c`, respectively.
- Implementation of Dinitz's algorithm for both original bipartite and the compressed graph in C `dinics_bi.c` and `dinics_tri.c`, respectively.
- Batch scripts to test FM for multiple experiments: `fmbatchScript.sh`
- Combined batch scripts to test CPGC and Dinitz's algorithm for multiple experiments: `cpgcbatchScript.sh`
- Folder with name `datasets` which includes the Python code to generate bipartite graphs in .mtx format with the corresponding code `simpleGraphGenerator.py`, a batch script to test the Python code for bipartite graphs for multiple experiments: `simpleGraphGenerator.sh`, and a sample bipartite graph in .mtx format: `bipartite_graph_32_80_1.mtx`.

**Note**: To test the above algorithms’ code, run them in the following order:

1. Run the `simpleGraphGenerator.py` code via its batch script `simpleGraphGenerator.sh` to generate bipartite graphs and use them as inputs for FM (`fm.c`), CPGC (`cpgc.c`), and Dinitz's algorithm (`dinics_bi.c` and `dinics_tri.c`) code.
2. Compile and run FM, CPGC and Dinitz’s algorithms for both bipartite and tripartite graphs.


## The implementation of the algorithms in the paper have been organized as follows:

### Generating datasets

**Programming Language**: Python  
**Version**: 3 and above
**Folder**: datasets
1. We generated the original bipartite graphs in Python for instances with |U| = |W| = n number of nodes equal to $2^i$, where i = 5, 6, ..., 15 and having five different densities: p = 0.80, 0.85, 0.90, 0.95, and 0.98.

2. The corresponding code `simpleGraphGenerator.py` in folder `datasets` generates such bipartite graphs.

3. To generate a input bipartite graph do the following steps:  
-  change the current directoy to `datasets` and
-  run the `simpleGraphGenerator.py` python file, it takes three arguments in the following sequence:
   - nodes, i.e., the number of vertices in the left partition of given graph (eg. 32768),  
   - density, i.e., the density of the given graph (eg. 98), and  
   - experimentNo, i.e., the experiment number (eg. 1).  
-  run the following command ```python3 simpleGraphGenerator.py 32768 98 1```,
-  it will generate `bipartite_graph_nodes_density_experimentNo.mtx` bipartite graph in the current `dataset` directory. 

4. To generate multiple input bipartite graphs do the following steps:
- updatet nodes, density and experiment number in the `simpleGraphGenerator.sh` bash file as required
- change the current directoy to `datasets` and
- run the following command ```bash simpleGraphGenerator.sh```
- this will generate the requested input bipartite graphs in the current `dataset` directory.

### Executing the C programs
**Programming Language**: C  
**Compiler**: gnu  
**Version**: tested with version 7 and 9

1. We implemented Feder-Motwani (FM) algorithm (`fm.c`), CPGC algorithm (`cpgc.c`), and Dinitz’s algorithm for both original bipartite (`dinics_bi.c`) and compressed graph (`dinics_tri.c`).

2. To compile the FM, CPGC, and Dinics algorithm code use the following commands, respectively:
- change the directory to the main directory (i.e. the main folder of the repo) in the terminal,
- for compiling FM code use ```gcc fm.c -lm -o fm```
- for compiling CPGC code use ```gcc cpgc.c -lm cpgc```
- for compiling Dinics for bipartite code use ```gcc dinics_bi.c -lm dinics_bi```
- for compiling Dinics for tripartite code use ```gcc dinics_tri.c -lm dinics_tri```

3. To run the FM and CPGC code use the following commands, respectively:
- change the directory to the main directory (i.e. the main folder of the repo) in the terminal,
- for executing FM code use ```./fm nodes density experiment delta```
- for executing CPGC code use ```./cpgc nodes density experimentNo delta```
- where, 
   a) nodes, i.e., the number of vertices in the left partition of given graph,  
   b) density, i.e., the density of the given graph,  
   c) experimentNo, i.e., the experiment number,  
   d) delta, i.e., the constant δ ($0 < \delta \leq 1$).  

4. To run the FM executable file for multiple experiments through a batch script use the following steps:
- generate the fm execuitable file by following the steps procedure in the step 2 above
- update the nodes, density and experiment number in the `fmbatchScript.sh` script for the generated datasets
- run the following command ```bash fmbatchScript.sh```
- this store the fm resutls in the `fm_results.csv` in the main directory

5. To run the CPGC and Dinics executable files for multiple experiments through a batch script use the following steps:
- generate the cpgc, dinics_bi, and dinics_tri execuitable files by following the steps procedure in the step 2 above
- update the nodes, density and experiment number in the `cpgcbatchScript.sh` script for the generated datasets
- run the following command ```bash cpgcbatchScript.sh```
- this store the cpgc resutls in the `cpgc_results.csv` in the main directory and dinics_bi and dinics_tri in `bipartite_dinics_results.csv` and `tripartite_dinics_results.csv`, respectively.

6. To run the Dinics executable files independently use the following steps:
- generate the fm, cpgc, dinics_bi, and dinics_tri execuitable files by following the steps procedure in the step 2 above
- executing the `dinics_bi` program needs nodes, density and experiment number as input arguments in respective order
- executing the `dinics_tri` program needs nodes, density, experiment number, $\delta$, and algorithm (fm or cpgc) as input arguments in respective order
- ```./dinics_bi 1024 80 1```

7. The results for FM and CPGC stored as csv files with names: `fm_results.csv` and `cpgc_results.csv`, respectively. Both outputs includes the following information:  
a) nodes, i.e., the number of vertices in the left partition of given graph,  
b) density, i.e., the density of the given graph,  
c) experimentNo, i.e., the experiment number,  
d) delta, i.e., the constant $\delta$,  
e) compression_ratio, i.e., compression ratio of FM or CPGC algorithm, and  
f) execution_time, i.e., the execution time of FM or CPGC algorithm.  

8. The output for Dinitz’s algorithms stored as `bipartite_dinics_results.csv` and `tripartite_dinics_results.csv` includes the following information:  
a) nodes, i.e., the number of vertices in the left partition of given graph  
b) total_nodes, i.e., the total vertices in the graph, which includes the vertices in source, left partition, middle partition, right partition, and sink,  
c) density, i.e., the density of the given graph,  
d) experimentNo, i.e., the experiment number,  
e) delta, i.e., the constant δ,  
f) maximumFlow, i.e., maximum matching in a given graph,  
g) run_time, i.e., execution time for the Dinitz's algorithm,  
h) total_run_time, i.e., total execution time including reading the .mtx files.  


### Discussion and Limitations:
- The obtained compression ratio depends on the number of nodes, density and $\delta$, which determines the size of the right partition of the $\delta$-clique.
- The FM algorithm computes a large number to select the vertices for the right parition of a $\delta$-clique. This number increasing with increasing nodes in each partition of the graph and thus exceeds the machines capaticy to store this number. Therefore, the FM program is limited to compress graphs with upto 128 vertices in each partition. 




