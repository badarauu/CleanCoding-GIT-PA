#include <stdio.h>
#include <stdlib.h>

// Structura pentru un nod din lista de adiacenta / coada
typedef struct Node {
    int data;
    struct Node *next;
} NODE;

// Structura pentru graf
typedef struct Graph {
    int vertices;
    int *visited;
    NODE **adjacency_lists;
} GPH;

// --- FUNCTII UTILS ---

// Aloca memorie pentru un nod nou
NODE *create_node(int v) {
    NODE *new_node = malloc(sizeof(NODE));
    new_node->data = v;
    new_node->next = NULL;
    return new_node;
}

// Initializeaza graful cu liste goale si vectorul de vizitati cu 0
GPH *create_graph(int vertices) {
    GPH *graph = malloc(sizeof(GPH));
    graph->vertices = vertices;
    graph->adjacency_lists = malloc(vertices * sizeof(NODE *));
    graph->visited = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// Adauga o muchie neorientata in graf
void add_edge(GPH *graph, int src, int dest) {
    // Adauga dest in lista lui src
    NODE *new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;

    // Adauga src in lista lui dest (graf neorientat)
    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

// Citeste muchiile de la tastatura
void insedg(int nr_of_vertices, int nr_of_edges, GPH *graph) {
    int src, dest;
    printf("Adauga %d muchii (noduri de la 0 la %d):\n", nr_of_edges, nr_of_vertices - 1);
    for (int i = 0; i < nr_of_edges; i++) {
        printf("Muchia %d (sursa destinatie): ", i + 1);
        scanf("%d %d", &src, &dest);
        add_edge(graph, src, dest);
    }
}

// --- FUNCTII COADA (Pentru BFS) ---

int is_empty(NODE *queue) {
    return queue == NULL;
}

// Adauga un element la sfarsitul cozii
void enqueue(NODE **queue, int data) {
    NODE *new_node = create_node(data);
    if (is_empty(*queue)) {
        *queue = new_node;
    } else {
        NODE *temp = *queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Scoate un element de la inceputul cozii
int dequeue(NODE **queue) {
    int data = (*queue)->data;
    NODE *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

// Reseteaza vectorul de vizitati intre parcurgeri
void wipe_visited_list(GPH *graph, int nr_of_vertices) {
    for (int i = 0; i < nr_of_vertices; i++) {
        graph->visited[i] = 0;
    }
}

// --- PARCURGERI IN GRAF ---

// Parcurgerea in Adancime (DFS) - Foloseste recursivitate
void DFS(GPH *graph, int vertex_nr) {
    NODE *temp = graph->adjacency_lists[vertex_nr];

    graph->visited[vertex_nr] = 1;
    printf("%d ", vertex_nr);

    while (temp != NULL) {
        int connected_vertex = temp->data;
        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

// Parcurgerea in Latime (BFS) - Foloseste o coada
void BFS(GPH *graph, int start) {
    NODE *queue = NULL;

    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        NODE *temp = graph->adjacency_lists[current];
        while (temp) {
            int adj_vertex = temp->data;
            if (graph->visited[adj_vertex] == 0) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

// --- FUNCTIA MAIN ---
int main() {
    int nr_of_vertices;
    int nr_of_edges;
    int starting_vertex;

    printf("Cate noduri are graful? ");
    scanf("%d", &nr_of_vertices);

    printf("Cate muchii are graful? ");
    scanf("%d", &nr_of_edges);

    GPH *graph = create_graph(nr_of_vertices);
    insedg(nr_of_vertices, nr_of_edges, graph);

    printf("\nDe unde plecam in DFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu DFS: ");
    DFS(graph, starting_vertex);
    printf("\n");

    // Resetam istoricul vizitelor pentru a putea rula corect si BFS
    wipe_visited_list(graph, nr_of_vertices);

    printf("De unde plecam in BFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu BFS: ");
    BFS(graph, starting_vertex);
    printf("\n");

    return 0;
}
