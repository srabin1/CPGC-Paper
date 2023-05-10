/*On windows follow this instruction to get a running time:
  1.clock_t start = clock();
  2.clock_t stop = clock();
  3.elapsed = ((double)(stop - start)) / CLOCKS_PER_SEC * 1000.0;
 */

 /*On Grid follow this instruction to get a running time:
   0. struct timespec begin, end;
   1.clock_gettime(CLOCK_REALTIME, &begin);
   2.clock_gettime(CLOCK_REALTIME, &end);         
   3.long seconds = end.tv_sec - begin.tv_sec;
   4.long nanoseconds = end.tv_nsec - begin.tv_nsec;
   5.elapsed = (seconds + nanoseconds * 1e-9) * 1000;
  */

#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


#define MAXCHAR 100000
#define split true
FILE* u_file_p;   // Stores Left partitions of the cliques
FILE* v_file_p;   // Stores right partitions of the cliques
FILE* trivial_c;  // Stores trivial cliques
FILE* k_values;   // Stores k_hat values
FILE* m_hat_values;
FILE* results;



struct timespec begin, end;
char f_name[100]; // name of the adjacency matrix file
int nodes[] = { 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384 };
int density[] = { 80, 85, 90, 95, 98 };
int experiments[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
float compression_ratio;
int** adj_matrix;
int* d_v;  // Degree of vertices in W
int** K;  // Stores initial selection of vertices from W
int** U;    // Stores common vertices for right partition
int** K_split;     // Stores the right partition for each delta-clique
int** U_split;      // Stores the left partition for each delta-clique
int* clique_u_size;     // Size of the left partition of the delta-clique
int* clique_v_size;      // Size of the right partition of the delta-clique
int k_split;    // starting index of q in each CPGC iteration
int k_temp;     // ending index of q in each CPGC iteration
int m_hat; // number of remaining edges
int d_K;  // degree of set K in iteration k
int Gamma;     // # of cliques extracted in each iteration of CPGC
int k_hat;
float delta = 1;
int* temp_psi;  // stores the degrees of vertices w for sorting 
int* temp_psi_idx;  // stores the sorted indices of degrees of vertices w 
int initial_edges;
int total_edges;    // Sum fo edges in delta-clique and trivial edges
int graph_nodes;
double run_time;


/* Algo2: CPGC lines 5 and 13 */
void get_k_hat() {
    float de = (2 * pow((double)graph_nodes, 2)) / m_hat;
    float nu = delta * log2((double)graph_nodes);
    k_hat = floor((double)nu / (log2((double)de)));
}


void load_adj_matrix() {
    FILE* fpointer = fopen(f_name, "r");
    char line[MAXCHAR];
    if (!fpointer)
        printf("Cann't open the file\n");
    else {
        int row = 0;
        int col = 0;
        while (fgets(line, MAXCHAR, fpointer)) {
            col = 0;
            //strtok break down each line into smaller string
            char* value = strtok(line, ",");
            while (value != NULL) {
                adj_matrix[row][col] = atoi(value);
                //adj_matrix_fix[row][col] = atoi(value);
                value = strtok(NULL, ",");
                col++;
            }
            row++;
        }
        fclose(fpointer);
    }
}


/* Algo2: CPGC lines 1 and 11 */
void get_edges() {
    m_hat = 0;
    int i;
    for (i = 0; i < graph_nodes; i++)
        m_hat += d_v[i];
}

/* Used by partition function */
void swap(int* xp, int* yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}



// Partition the array and return the pivot index
int partition(int* arr, int* temp_psi_idx, int low, int high) {
    int pivot = arr[low]; // Choose the first element as the pivot
    int i = low + 1; // Start from the second element
    int j = high;

    while (i <= j) {
        // Find an element greater than the pivot from the left side
        while (i <= j && arr[i] >= pivot) {
            i++;
        }
        // Find an element smaller than the pivot from the right side
        while (i <= j && arr[j] < pivot) {
            j--;
        }
        // Swap the elements if they are out of order
        if (i < j) {
            swap(&arr[i], &arr[j]);
            swap(&temp_psi_idx[i], &temp_psi_idx[j]);
        }
    }

    // Move the pivot to its correct position
    swap(&arr[low], &arr[j]);
    swap(&temp_psi_idx[low], &temp_psi_idx[j]);

    return j;
}


/* Algo3: CSA line 2*/
// Quicksort implementation
void quicksort(int* arr, int* temp_psi_idx, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, temp_psi_idx, low, high);

        //#pragma omptask shared(arr, temp_psi_idx) firstprivate(low, pivotIndex)
        quicksort(arr, temp_psi_idx, low, pivotIndex - 1);

        //#pragma omptask shared(arr, temp_psi_idx) firstprivate(high, pivotIndex)	
        quicksort(arr, temp_psi_idx, pivotIndex + 1, high);
        //#pragma omptaskwait
    }
}


/* Algo3: CSA lines 4 to 6*/
void get_new_K(int k) {
    int i = 0;
    for (i = 0; i < graph_nodes; i++)
        temp_psi_idx[i] = i;
    memcpy(temp_psi, d_v, graph_nodes * sizeof(int));
    (void)quicksort(temp_psi, temp_psi_idx, 0, graph_nodes - 1);

    i = 0;
    while (d_v[temp_psi_idx[i]] >= temp_psi[k_hat - 1]) {
        K[k][i] = temp_psi_idx[i] + 1;
        i++;
    }
    K[k][i] = -1; // to define end of selection of vertex v in V
    d_K = i;
}

/* Algo3: CSA lines 8 to 11 */
void get_U_with_k_hat(int k) {

    int div_K = floor(((double)d_K / k_hat));
    int start = 0;
    int end = k_hat;
    int div = 0;
    int set_size = k_hat;
    k_temp = k_temp + div_K;
    int i, j, d;

    /* Algo3: CSA line 9 */

    for (d = k_split; d < k_temp; d++) {
        for (j = 0; j < set_size; j++) {
            K_split[d][j] = K[k][j + start];
        }
        K_split[d][set_size] = -1;
        clique_v_size[d] = j;
        start = start + k_hat;
    }

    /* Algo3: CSA line 10 */
    for (d = k_split; d < k_temp; d++) {
        int u = 0;
        for (i = 0; i < graph_nodes; i++) {
            int flag = 0;
            for (j = 0; j < k_hat + 1 && !flag; j++) {
                if (K_split[d][j] != -1) {
                    if (adj_matrix[i][K_split[d][j] - 1] == 0) {
                        flag = 1;
                    }
                }
                else {
                    U_split[d][u] = i + 1;
                    u++;
                    flag = 1;
                }
            }
            U_split[d][u] = -1;
        }
        clique_u_size[d] = u;
    }
}

/* Algo2: CPGC lines 2 to 4*/
void get_d_v() {
    int j, i;
    for (j = 0; j < graph_nodes; j++) {
        d_v[j] = 0;
        for (i = 0; i < graph_nodes; i++) {
            d_v[j] = d_v[j] + adj_matrix[i][j];
        }
    }
}


void display_adj_matrix() {
    printf("\nADJACENCY MATRIX\n");
    int i, j;
    for (i = 0; i < graph_nodes; i++) {
        for (j = 0; j < graph_nodes; j++) {
            printf(" %d  ", adj_matrix[i][j]);
        }
        printf("\n");
    }
}


/* Algo3: CSA lines 12 and 13 */
void get_updates() {
    int d, j, i;
    for (d = k_split; d < k_temp; d++) {
        for (j = 0; j < clique_v_size[d]; j++) {
            d_v[K_split[d][j] - 1] = d_v[K_split[d][j] - 1] - clique_u_size[d];
            m_hat -= clique_u_size[d];
            for (i = 0; i < clique_u_size[d]; i++) {
                adj_matrix[U_split[d][i] - 1][K_split[d][j] - 1] = 0;
            }
        }
    }
}

/* Calculating compression ratio */
void get_compression_ratio() {
    get_edges();
    int edges_in_clique = 0;
    int p;
    for (p = 0; p < k_temp; p++) {
        if (clique_v_size[p] < 2)
            edges_in_clique += clique_u_size[p];
        else
            edges_in_clique += clique_u_size[p] + clique_v_size[p];
    }
    total_edges = m_hat + edges_in_clique;
    compression_ratio = (float)initial_edges / (float)total_edges;
}



void sequentialCPA() {
    load_adj_matrix();
    //clock_t start = clock();
    clock_gettime(CLOCK_REALTIME, &begin);
    int k = 0;
    get_d_v();
    get_edges();
    initial_edges = m_hat;
    get_k_hat();
    int flag = 0;

    while (k_hat > 1 && !flag)
    {
        int m = m_hat;

        get_new_K(k);

        get_U_with_k_hat(k);
        get_updates();
        get_k_hat();

        k++;
        k_split = k_temp;
        if (m_hat == m) { // k_hat == 1
            // break;
            flag = 1;
        }
    }
    //clock_t stop = clock();
    clock_gettime(CLOCK_REALTIME, &end);
    //run_time = ((double)(stop - start)) / CLOCKS_PER_SEC * 1000.0;

    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    run_time = (seconds + nanoseconds * 1e-9) * 1000;
    get_compression_ratio();
}

int** getAllocate(int n) {
    int i;
    int** arr = (int**)malloc((n) * sizeof(int*));
    for (i = 0; i < n; i++) {
        arr[i] = (int*)malloc((n) * sizeof(int));
    }
    return arr;
}

void getDeAllocate(int n, int** arr) {
    int i;
    for (i = 0; i < n; i++) {
        free(arr[i]);
    }
    free(arr);
}

/* Run on windows without batch script */
/*
int main() {
    char core[5] = "seq";
    results = fopen("Grid_Sequential_CPA_results.csv", "w");
    fprintf(results, "nodes,density,experimentNo,compressionRatio,elapsedTime,cores\n");
    int i = 0;
    int k = 0;
    int j = 0;
    int multiplier;
    for (i = 9; i < 10; i++) {
        for (j = 0; j < 1; j++) {
            for (k = 0; k < 1; k++) {
                k_temp = 0;
                k_split = 0;
                graph_nodes = nodes[i];
                multiplier = ceil(log10((double)graph_nodes));
                d_v = (int*)malloc(graph_nodes * sizeof(int));
                K = getAllocate(multiplier * graph_nodes);
                U = getAllocate(multiplier * graph_nodes);
                K_split = getAllocate(multiplier * graph_nodes);
                U_split = getAllocate(multiplier * graph_nodes);
                adj_matrix = getAllocate(graph_nodes);
                clique_u_size = (int*)malloc((multiplier * graph_nodes) * sizeof(int));
                clique_v_size = (int*)malloc((multiplier * graph_nodes) * sizeof(int));
                temp_psi = (int*)malloc(graph_nodes * sizeof(int));
                temp_psi_idx = (int*)malloc(graph_nodes * sizeof(int));
                printf("____Density: %d _______ Experimtent: %d ______________", density[j], experiments[k]);
                sprintf(f_name, "New_generated_data/Bipartite_%dX%d/%d/Bipartite_%dX%d_%d_%d.csv", nodes[i], nodes[i], density[j], nodes[i], nodes[i], density[j], experiments[k]);
                printf("%s \n ", f_name);
                sequentialCPA();
                printf("\n*******************END******************* \n\n\n");
                fprintf(results, "%d, %d, %d, %f, %f,%s\n", nodes[i], density[j], experiments[k], compression_ratio, run_time, core);
                free(d_v);
                getDeAllocate(multiplier * graph_nodes, K);
                getDeAllocate(multiplier * graph_nodes, U);
                getDeAllocate(multiplier * graph_nodes, K_split);
                getDeAllocate(multiplier * graph_nodes, U_split);
                getDeAllocate(graph_nodes, adj_matrix);
                free(clique_u_size);
                free(clique_v_size);
                free(temp_psi);
                free(temp_psi_idx);
            }
        }
    }
    fclose(results);
    return 0;
}
*/


/* For batch script */
int main(int argc, char* argv[]) {
    //char result_file[50];
    char cores[5] = "seq";
    int nodes = atoi(argv[1]);
    int density = atoi(argv[2]);
    int exp = atoi(argv[3]);
    int multiplier;
    sprintf(f_name, "New_generated_data/Bipartite_%dX%d/%d/Bipartite_%dX%d_%d_%d.csv", nodes, nodes, density, nodes, nodes, density, exp);

    graph_nodes = nodes;  // nodes
    multiplier = ceil(log10((double)graph_nodes));
    d_v = (int*)malloc(graph_nodes * sizeof(int));
    K = getAllocate(multiplier * graph_nodes);
    U = getAllocate(multiplier * graph_nodes);
    K_split = getAllocate(multiplier * graph_nodes);
    U_split = getAllocate(multiplier * graph_nodes);
    adj_matrix = getAllocate(graph_nodes);
    clique_u_size = (int*)malloc((multiplier * graph_nodes) * sizeof(int));
    clique_v_size = (int*)malloc((multiplier * graph_nodes) * sizeof(int));
    temp_psi = (int*)malloc(graph_nodes * sizeof(int));
    temp_psi_idx = (int*)malloc(graph_nodes * sizeof(int));
    sequentialCPA();
    printf("%d,%d,%d,%lf, %lf,%s\n", nodes, density, exp, compression_ratio, run_time, cores);
    free(d_v);
    getDeAllocate(multiplier * graph_nodes, K);
    getDeAllocate(multiplier * graph_nodes, U);
    getDeAllocate(multiplier * graph_nodes, K_split);
    getDeAllocate(multiplier * graph_nodes, U_split);
    getDeAllocate(graph_nodes, adj_matrix);
    free(clique_u_size);
    free(clique_v_size);
    free(temp_psi);
    free(temp_psi_idx);
    return 0;
}
