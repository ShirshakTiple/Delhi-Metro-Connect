

typedef struct graph_node {
    char *color;
    char **stop_list;
    struct graph_node *next;
    int size;
} graph_node;

typedef struct graph_array {
    char *color_name;
    graph_node *head;
    int garr_size;
} graph_array;

typedef struct graph {
    graph_array **adjacency_list;
    int num_colors;
} graph;



void init_graph(graph *g, char *filename);
char **bfs(graph *g , char *source  , char *destination);
void displayGraph(graph *g);
graph_node *findGraphNode(graph *g, char *color);


