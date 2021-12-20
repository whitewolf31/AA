#include <stdio.h>
#include <stdlib.h>
int n, k;

typedef struct Node {
  int vertex;
  struct Node *next;
} Node;

typedef struct Graph {
  int size;
  int edges_num;
  struct Node **adjList;
} Graph;

Node *alloc_list(int vertex) {
  Node *new_list = (Node *) malloc(sizeof(Node));
  new_list->vertex = vertex;
  new_list->next = NULL;

  return new_list;
}

Graph *alloc_graph(int size, int edges_num) {
  Graph *new_graph = (Graph *) malloc(sizeof(Graph));
  new_graph->size = size;
  new_graph->edges_num = edges_num;
  new_graph->adjList = (Node **) malloc(size * sizeof(Node *));
  for (int i = 0; i < size; i++) {
    new_graph->adjList[i] = alloc_list(i);
  }

  return new_graph;
}

void add_edge(Graph *graph, int from, int to) {
  Node *new_list = (Node *) alloc_list(to);
  Node *looper = graph->adjList[from];
  while (looper->next != NULL) looper = looper->next;
  looper->next = new_list;
}

int findIndex(char item, char *arr, int size) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == item) return i;
  }

  return -1;
}

char is_isomorphic(Graph **graphs, char **order, char **modified, int graph_index) {
  int idx1 = graph_index;
  int idx2 = graph_index - 1;
  Graph *graph1 = graphs[graph_index];
  Graph *graph2 = graphs[graph_index - 1];
  if (graph1->size < graph2->size) {
    Graph *copy = graph1;
    graph1 = graph2;
    graph2 = copy;
    idx1 = graph_index - 1;
    idx2 = graph_index;
  }
  char **adjList1 = (char **) malloc(k * sizeof(char *));
  char **adjList2 = (char **) malloc(k * sizeof(char *));
  for (int i = 0; i < k; i++) {
    adjList1[i] = (char *) calloc(k, sizeof(char));
    adjList2[i] = (char *) calloc(k, sizeof(char));
  }
  // Creating adjacent matrixes for comparing
  for (int i = 0; i < k; i++) {
    int current_node1 = order[idx1][i];
    int current_node2 = order[idx2][i];
    Node *looper1 = graphs[idx1]->adjList[current_node1];
    Node *looper2 = graphs[idx2]->adjList[current_node2];
    while (looper1 != NULL) {
      int nextVertex = looper1->vertex;
      if (modified[idx1][nextVertex] != 0) {
        int actualNextIndex = findIndex(nextVertex, order[idx1], k);
        adjList1[i][actualNextIndex] = 1;
        adjList1[actualNextIndex][i] = 1;

      }
      looper1 = looper1->next;
    }
    while (looper2 != NULL) {
      int nextVertex = looper2->vertex;
      if (modified[idx2][nextVertex] != 0) {
        int actualNextIndex = findIndex(nextVertex, order[idx2], k);
        adjList2[i][actualNextIndex] = 1;
        adjList2[actualNextIndex][i] = 1;
      }
      looper2 = looper2->next;
    }
  }
  char res = 1;
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < k; j++) {
      if (adjList1[i][j] != adjList2[i][j]) res = 0;
    }
  }
  for (int i = 0; i < k; i++) {
    free(adjList1[i]);
    free(adjList2[i]);
  }
  free(adjList1);
  free(adjList2);

  return res;
};

char find_isomorphic_graphs(Graph **graphs, char **modified, char **order, int graph_index, int recurs_index) {
  if (recurs_index == k) {
    if (graph_index == 0) return find_isomorphic_graphs(graphs, modified, order, graph_index + 1, 0);
    // Compare subgraph graph_index with subgraph graph_index - 1
    if (is_isomorphic(graphs, order, modified, graph_index) > 0) return find_isomorphic_graphs(graphs, modified, order, graph_index + 1, 0);
    else return -1;
  }
  if (graph_index == n) 
    return 1;
  for (int i = 0; i < graphs[graph_index]->size; i++) {
    // Generating Arrangements
    if (modified[graph_index][i] == 0) {
      modified[graph_index][i] = 1;
      order[graph_index][recurs_index] = i;
      if (find_isomorphic_graphs(graphs, modified, order, graph_index, recurs_index + 1) > 0) return 1;
      modified[graph_index][i] = 0;
    }
  }

  return -1;
}

int main(void) {
  FILE *in = fopen("grafuri.in", "r");
  FILE *out = fopen("grafuri.out", "w");
  fscanf(in, "%d %d", &n, &k);
  Graph **graphs = (Graph **) malloc(n * sizeof(Graph *));

  // ------------ Generation Part Start ------------

  for (int i = 0; i < n; i++) {
    int vertices_num, edges_num;
    fscanf(in, "%d %d", &vertices_num, &edges_num);
    graphs[i] = alloc_graph(vertices_num, edges_num);
    for (int j = 0; j < edges_num; j++) {
      int from, to;
      fscanf(in, "%d %d", &from, &to);
      from--;
      to--;
      add_edge(graphs[i], from, to);
    }
  }

  // ------------ Generation Part End ------------

  char **modified = (char **) malloc(n * sizeof(char *));
  for (int i = 0; i < n; i++)
    modified[i] = (char *) calloc(graphs[i]->size, sizeof(char));
  char **order = (char **) malloc(n * sizeof(char *));
  for (int i = 0; i < n; i++) {
    order[i] = (char *) calloc(k, sizeof(char));
  }
  if (find_isomorphic_graphs(graphs, modified, order, 0, 0) > 0) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < k; j++) {
        fprintf(out, "%d ", order[i][j]);
      }
      fprintf(out, "\n");
    }
  } else {
    fprintf(out, "-1\n");
  }

  // ------------ Output Part Start ------------

  for (int i = 0; i < n; i++) {
    // printf("Graph %d, size: %d:\n", i, graphs[i]->size); TODO: Uncomment for graph info
    for (int j = 0; j < graphs[i]->size; j++) {
      Node *looper = graphs[i]->adjList[j];
      while (looper->next != NULL) {
        Node *prev = looper;
        looper = looper->next;
        free(prev);
        // printf("(%d, %d)\n", j, looper->vertex); TODO: Uncomment for graph info
      }
      free(looper);
    }
    free(graphs[i]->adjList);
    free(graphs[i]);
  }
  free(graphs);

  // ------------ Output Part End ------------

  fclose(in);
  fclose(out);

  return 0;
}