#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 100

typedef struct {
    int v;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} LinkedList;

typedef struct {
    int numNodes;
    LinkedList* adjacencyList;
} Graph;

Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->v = v;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int numNodes) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numNodes = numNodes;
    graph->adjacencyList = (LinkedList*)malloc(numNodes * sizeof(LinkedList));

    for (int i = 0; i < numNodes; ++i) {
        graph->adjacencyList[i].head = NULL;
    }

    return graph;
}

void addEdge(Graph* graph, int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = graph->adjacencyList[src].head;
    graph->adjacencyList[src].head = newNode;
}

void topologicalSortUtil(Graph* graph, int v, int visited[], int stack[], int* stackIndex) {
    visited[v] = 1;

    Node* currentNode = graph->adjacencyList[v].head;
    while (currentNode != NULL) {
        int adjacentNode = currentNode->v;
        if (!visited[adjacentNode]) {
            topologicalSortUtil(graph, adjacentNode, visited, stack, stackIndex);
        }
        currentNode = currentNode->next;
    }

    stack[(*stackIndex)++] = v;
}

void topologicalSort(Graph* graph) {
    int* stack = (int*)malloc(graph->numNodes * sizeof(int));
    int stackIndex = 0;
    int* visited = (int*)malloc(graph->numNodes * sizeof(int));

    for (int i = 0; i < graph->numNodes; ++i) {
        visited[i] = 0;
    }

    for (int i = 0; i < graph->numNodes; ++i) {
        if (!visited[i]) {
            topologicalSortUtil(graph, i, visited, stack, &stackIndex);
        }
    }

    printf("Ordre optimal des opérations:\n");
    for (int i = stackIndex - 1; i >= 0; --i) {
        printf("%d ", stack[i]);
    }

    free(stack);
    free(visited);
}

int main() {
    FILE* file = fopen("precedences.txt", "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return -1;
    }

    int numNodes = 0;
    int src, dest;
    while (fscanf(file, "%d %d", &src, &dest) == 2) {
        numNodes = numNodes > src ? numNodes : src;
        numNodes = numNodes > dest ? numNodes : dest;
    }
    fclose(file);

    Graph* graph = createGraph(numNodes + 1);

    file = fopen("precedences.txt", "r");
    while (fscanf(file, "%d %d", &src, &dest) == 2) {
        addEdge(graph, src, dest);
    }
    fclose(file);

    topologicalSort(graph);

    free(graph->adjacencyList);
    free(graph);

    return 0;
}