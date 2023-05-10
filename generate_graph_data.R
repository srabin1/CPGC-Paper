#install.packages("igraph")
#install.packages("ggplot2")
library(igraph)
#library(ggplot2)

################################bipartite graph##################################

#sample_bipartite(10, 5, p=.1)

#set.seed(123)
#options(digits=2)
# generate random bipartite graph.
P <- c(.98, .95, .90, .85, .80)
E <- c ( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10)

nodes <- c(32, 32768, 65536, 131072)

for (n in nodes){
  for (i in P){
    for (e in E){
      g <- sample_bipartite(n, n, p=i)
      bipartite_graph <- as_incidence_matrix(g)
      write.table(bipartite_graph, file = paste0("New_generated_data/Bipartite_",n,"X",n,"/",i*100,"/Bipartite_",n,"X",n,"_", i*100,"_",e,".csv"), sep = ",", row.names=FALSE, col.names=FALSE)
    }
  }
}
# 
