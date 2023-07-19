/*This code is the implementation of Dinic's algorithm*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

struct Edge {
    int v;
    int flow;
    int C;
    int rev;
};

struct Graph {
    int V;
    int* level;
    struct Edge** adj;
    int* edgeCount;
};

struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->adj = (struct Edge**)malloc(V * sizeof(struct Edge*));
    for (int i = 0; i < V; i++) {
        graph->adj[i] = (struct Edge*)malloc(V * sizeof(struct Edge));
    }
    graph->level = (int*)malloc(V * sizeof(int));
    graph->edgeCount = (int*)calloc(V, sizeof(int));
    return graph;
}

void addEdge(struct Graph* graph, int u, int v, int C) {
    graph->adj[u][graph->edgeCount[u]] = (struct Edge){ v, 0, C, graph->edgeCount[v] };
    graph->adj[v][graph->edgeCount[v]] = (struct Edge){ u, 0, 0, graph->edgeCount[u] };
    graph->edgeCount[u]++;
    graph->edgeCount[v]++;
}

bool BFS(struct Graph* graph, int s, int t) {
    for (int i = 0; i < graph->V; i++)
        graph->level[i] = -1;

    graph->level[s] = 0;
    int* q = (int*)malloc(graph->V * sizeof(int));
    int front = 0, rear = 0;
    q[rear++] = s;

    while (front < rear) {
        int u = q[front++];
        for (int i = 0; i < graph->edgeCount[u]; i++) {
            struct Edge e = graph->adj[u][i];
            if (graph->level[e.v] < 0 && e.flow < e.C) {
                graph->level[e.v] = graph->level[u] + 1;
                q[rear++] = e.v;
            }
        }
    }

    free(q);
    return graph->level[t] < 0 ? false : true;
}

int sendFlow(struct Graph* graph, int u, int flow, int t, int start[]) {
    if (u == t) {
        return flow;
    }

    for (; start[u] < graph->edgeCount[u]; start[u]++) {
        struct Edge* e = &graph->adj[u][start[u]];

        if (graph->level[e->v] == graph->level[u] + 1 && e->flow < e->C) {
            int curr_flow = flow < e->C - e->flow ? flow : e->C - e->flow;

            int temp_flow = sendFlow(graph, e->v, curr_flow, t, start);

            if (temp_flow > 0) {
                e->flow += temp_flow;
                graph->adj[e->v][e->rev].flow -= temp_flow;
                return temp_flow;
            }
        }
    }

    return 0;
}

int DinicMaxflow(struct Graph* graph, int s, int t) {
    if (s == t) {
        return -1;
    }

    int total = 0;

    while (BFS(graph, s, t) == true) {
        int* start = (int*)malloc(graph->V * sizeof(int));
        for (int i = 0; i < graph->V; i++)
            start[i] = 0;

        int flow;
        while ((flow = sendFlow(graph, s, INT_MAX, t, start)) > 0) {
            total += flow;
        }

        free(start);
    }

    return total;
}

int main() {
    
    /*
    //bipartite graph
    struct Graph* graph = createGraph(13);
   
    addEdge(graph, 0, 1, 1);
    addEdge(graph, 0, 2, 1);
    addEdge(graph, 0, 3, 1);
    addEdge(graph, 0, 4, 1);
    addEdge(graph, 0, 5, 1);
    addEdge(graph, 1, 7, 1);
    addEdge(graph, 2, 6, 1);
    addEdge(graph, 2, 8, 1);
    addEdge(graph, 3, 6, 1);
    addEdge(graph, 3, 7, 1);
    addEdge(graph, 4, 7, 1);
    addEdge(graph, 4, 10, 1);
    addEdge(graph, 4, 11, 1);
    addEdge(graph, 5, 7, 1);
    addEdge(graph, 5, 9, 1);
    addEdge(graph, 5, 10, 1);
    addEdge(graph, 6, 12, 1);
    addEdge(graph, 7, 12, 1);
    addEdge(graph, 8, 12, 1);
    addEdge(graph, 9, 12, 1);
    addEdge(graph, 10, 12, 1);
    addEdge(graph, 11, 12, 1);
    printf("Maximum flow: %d\n", DinicMaxflow(graph, 0, 12));
      */
    
      //tripartite graph
    struct Graph* graph = createGraph(14);
    addEdge(graph, 0, 1, 1);
    addEdge(graph, 0, 2, 1);
    addEdge(graph, 0, 3, 1);
    addEdge(graph, 0, 4, 1);
    addEdge(graph, 0, 5, 1);
    addEdge(graph, 0, 2, 1);
    addEdge(graph, 1, 8, 1);
    addEdge(graph, 2, 7, 1);
    addEdge(graph, 2, 9, 1);
    addEdge(graph, 3, 7, 1);
    addEdge(graph, 3, 8, 1);
    addEdge(graph, 4, 6, 1);
    addEdge(graph, 4, 8, 1);
    addEdge(graph, 4, 12, 1);
    addEdge(graph, 5, 6, 1);
    addEdge(graph, 5, 8, 1);
    addEdge(graph, 6, 10, 1);
    addEdge(graph, 6, 11, 1);
    addEdge(graph, 7, 13, 1);
    addEdge(graph, 8, 13, 1);
    addEdge(graph, 9, 13, 1);
    addEdge(graph, 10, 13, 1);
    addEdge(graph, 11, 13, 1);
    addEdge(graph, 12, 13, 1);
    printf("Maximum flow: %d\n", DinicMaxflow(graph, 0, 13));

    
    // Free dynamically allocated memory
    for (int i = 0; i < graph->V; i++) {
        free(graph->adj[i]);
    }
    free(graph->adj);
    free(graph->level);
    free(graph->edgeCount);
    free(graph);

    return 0;
}
