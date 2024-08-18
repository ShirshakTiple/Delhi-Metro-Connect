#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"graph.h"
#include"AVLtree.h"
#include"list.h"
#include"limits.h"

typedef struct qnode{
    graph_node *n;
    struct qnode *next;
}qnode;

typedef qnode* Queue;

void initq(Queue *q);
void enq(Queue *q , graph_node *name);

graph_node *deq(Queue *q);

int isEmpty(Queue q);

void initq(Queue *q){
    *q = NULL;
    return;
}

void enq(Queue *q , graph_node *n){
    qnode *nn = (qnode*)malloc(sizeof(qnode));
    nn->n = n;
    nn->next = NULL;
    if(isEmpty(*q)){
        *q = nn;
        return;
    }
    qnode *p = *q;
    while(p->next)
        p = p->next;
    p->next = nn;
    return;
}

graph_node *deq(Queue *q){
    if(isEmpty(*q))
        return NULL;
    qnode *p = *q;
    graph_node *n = p->n ;
    *q = p->next;
    free(p);
    return n;
}

int isEmpty(Queue q){
    return (q == NULL);
}

typedef struct stack{
    int *A;
    int top;
    int size;
}stack;

void init_stack(stack *s , int size);
void push_stack(stack *s , int key);
int pop_stack(stack *s);
int isEmpty_stack(stack s);

void init_stack(stack *s , int size){
    s->A = (int*)malloc(sizeof(int) * size);
    s->top = -1;
    s->size = size;
    return;
}

void push_stack(stack *s , int key){
    if(s->top == (s->size -1)){
        s->A = (int*)realloc(s->A , sizeof(int)*(s->size+1));
        s->size++;
    }
    s->top ++;
    s->A[s->top] = key;
    return;
}

int pop_stack(stack *s){
    if(isEmpty_stack(*s))
        return INT_MIN;
    int c = s->A[s->top];
    s->top--;
    return c;
}

int isEmpty_stack(stack s){
    return(s.top == -1);
}

void insert_graph(graph *g, char *key, graph_node *nn);
int getIndex(graph *g , char *colour);

void init_graph(graph *g, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    // Initialize graph variables
    int max_colors = 100;
    g->num_colors = 13;
    g->adjacency_list = (graph_array **)malloc(max_colors * sizeof(graph_array *));
    for (int i = 0; i < g->num_colors; i++) {
        g->adjacency_list[i] = NULL;
    }

    // Set the keys
    char *keys[] = {
        "Aqua",
        "Blue",
        "Blue2",
        "Green",
        "Green2",
        "Grey",
        "Airport Express",
        "Pink",
        "Magenta",
        "Violet",
        "Yellow",
        "Red",
        "Rapid"
    };

    // Add the keys to the adjacency list
    for (int i = 0; i < g->num_colors; i++) {
        graph_array *arr = (graph_array *)malloc(sizeof(graph_array));
        arr->color_name = strdup(keys[i]);
        arr->head = NULL;
        arr->garr_size = 0;

        g->adjacency_list[i] = arr;
    }

    char line[500];
    char *token;
    char *key;
    char *color;
    char *stop_list;
    while (fgets(line, sizeof(line), file)) {

        token = strtok(line, ",");
        key = strdup(token);

        token = strtok(NULL, ",");
        color = strdup(token);

        token = strtok(NULL, ",");
        stop_list = strdup(token);

        graph_node *node = (graph_node *)malloc(sizeof(graph_node));
        node->color = (char *)malloc((strlen(color) + 1) * sizeof(char));
        strcpy(node->color, color);
        node->next = NULL;
        node->size = 0;
        // Tokenize stop_list based on semicolon
        char *stop_token = strtok(stop_list, ";");
        while (stop_token != NULL) {
            if (node->size == 0)
                node->stop_list = (char **)malloc(sizeof(char *) * 1);
            else
                node->stop_list = (char **)realloc(node->stop_list, (node->size + 1) * sizeof(char *));

            node->stop_list[node->size] = (char *)malloc((strlen(stop_token) + 1) * sizeof(char));
            strcpy(node->stop_list[node->size], stop_token);
            node->size++;
            stop_token = strtok(NULL, ";");
        }
        insert_graph(g , key , node);
    }
    fclose(file);
    return;
}

void insert_graph(graph *g, char *key, graph_node *nn) {
    int key_found = 0;
    int index = -1;
    
    // Find the index of the key in the adjacency list
    for (int i = 0; i < g->num_colors; i++) {
        if (strcmp(g->adjacency_list[i]->color_name, key) == 0) {
            key_found = 1;
            index = i;
            break;
        }
    }
    
    if (key_found) {
        graph_array *arr = g->adjacency_list[index];
        graph_node *last_node = arr->head;
        
        if (last_node == NULL) {
            arr->head = nn; // Update the head of the linked list
        } else {
            while (last_node->next != NULL) {
                last_node = last_node->next;
            }
            last_node->next = nn;
        }
    } 
    else {
        // Key not found, create a new graph array
        graph_array *arr = (graph_array *)malloc(sizeof(graph_array));
        arr->color_name = strdup(key);
        arr->head = nn;
        arr->garr_size = 1;
        
        // Add the new graph array to the adjacency list
        g->adjacency_list[g->num_colors] = arr;
        g->num_colors++;
    }
}

void displayGraph(graph *g) {
    printf("Graph:\n");
    for (int i = 0; i < g->num_colors; i++) {
        graph_array *arr = g->adjacency_list[i];
        printf("Key: %s\n", arr->color_name);
        printf("%d\n" , g->num_colors);

        graph_node *node = arr->head;
        while (node != NULL) {
            printf("Color: %s, Stops: ", node->color);
            for (int j = 0; j < node->size; j++) {
                printf("%s", node->stop_list[j]);
                if (j < node->size - 1)
                    printf(", ");
            }
            //printf("\n");
            node = node->next;
        }
    }
}

graph_node *findGraphNode(graph *g, char *color) {
    for (int i = 0; i < g->num_colors; i++) {
        graph_array *arr = g->adjacency_list[i];
        graph_node *node = arr->head;
        while (node != NULL) {
            if (strcmp(node->color, color) == 0) {
                return node;
            }
            node = node->next;
        }
    }
    return NULL;  // Color not found
}

int getIndex(graph *g , char *colour){
    for (int i = 0; i < g->num_colors; i++) {
        if (strcmp(g->adjacency_list[i]->color_name, colour) == 0) 
            return i;
    }
    return INT_MIN;
}

char **bfs(graph *g , char *source  , char *destination){
    char **path = (char**)malloc(sizeof(char*)*13);
    int s = 0;
    Queue q;
    initq(&q);
    int Visited[13];
    for (int i = 0 ; i < 13 ; i++)
        Visited[i] = 0;
    
    graph_node *source_node = findGraphNode(g, source);
    graph_node *destination_node = findGraphNode(g, destination);
    graph_node *nn;
    enq(&q , source_node);
    int index = 0;
    index = getIndex(g,source);
    if(index == INT_MIN)
        return NULL;
    Visited[index] = 1;
    while(!(isEmpty(q))){
        nn = deq(&q);
        index = getIndex(g , nn->color);
        if(nn!=source_node){
            path[s]= (char*)malloc(sizeof(char)*(strlen(nn->stop_list[0])));
            strcpy(path[s] , nn->stop_list[0]);
            s++;
        }
        graph_array *arr = g->adjacency_list[index];
        graph_node *node = arr->head;
        while(node){
            if(strcmp(node->color , destination) == 0){
                path[s]= (char*)malloc(sizeof(char)*(strlen(node->stop_list[0])));
                strcpy(path[s] , node->stop_list[0]);
                s++;
                return path;
            }
            index = getIndex(g,node->color);
            if(!Visited[index]){
                enq(&q , node);
                Visited[index] = 1;
            }
            node = node->next;
        }
    }
    return NULL;
}
