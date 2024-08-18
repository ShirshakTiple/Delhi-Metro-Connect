#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"
#include"AVLtree.h"
#include"graph.h"

line getLine(char *colour);
void line_traverse(char *source , char *destination , line l);
void search_list(avlnode *source, avlnode *destination);
char *getCommonColour(avlnode *source , avlnode *destination);
graph g;
AVLtree t;
int abs(int a){
    return (a < 0) ? -a : a;
}
int main(){
    file_store();
    init_AVL(&t);
    init_graph(&g, "crossings.txt");
    //displayGraph(&g);
    // char *val = "Bahdurgarh City";
    // char *val2 = "Bata Chowk";
    char input[100];
    char* val;
    char* val2;

    // Prompt the user for input
    printf("Enter the source value: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove the trailing newline
    val = strdup(input);  // Create a dynamically allocated copy of the input

    printf("Enter the destination value: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove the trailing newline
    val2 = strdup(input);  // Create a dynamically allocated copy of the input
    avlnode* source = search_station(t,val);
    avlnode* destination = search_station(t,val2);
    search_list(source , destination);

    return 0;
}


void search_list(avlnode *source, avlnode *destination) {
    if (source == NULL || destination == NULL) {
        printf("Invalid source or destination node.\n");
        return;
    }

    char *colour = NULL;
    char *s_name = malloc(sizeof(char) * (strlen(source->name) + 1));
    strcpy(s_name, source->name);
    char *d_name = malloc(sizeof(char) * (strlen(destination->name) + 1));
    strcpy(d_name, destination->name);

    colour = getCommonColour(source , destination);

    if (colour != NULL) {
        printf("\nColour : %s\n\n" , colour);
        line_traverse(s_name, d_name, getLine(colour));
        free(s_name);
        free(d_name);
        return;
    }
    char** path = bfs(&g, source->line_list[0], destination->line_list[0]);
    int path_length = 0;
    if (path != NULL) {
        while (path[path_length] != NULL) {
            path_length++;
        }
    }
    for (int i = 0; i < path_length ; i++) {
        if (path[i] != NULL) {
            int newline_index = strcspn(path[i], "\n");
            if (newline_index < strlen(path[i])) {
                path[i][newline_index] = '\0';
            }

            if (i == 0) {
                colour = getCommonColour(source, search_station(t, path[i]));
                printf("\nStart Colour = %s\n", colour);
                printf("\n");
                line_traverse(s_name, path[i], getLine(colour));
                if (i < path_length-1) {
                    int newline_index = strcspn(path[i+1], "\n");
                    if (newline_index < strlen(path[i+1])) {
                        path[i+1][newline_index] = '\0';
                    }
                    colour = getCommonColour(search_station(t, path[i]), search_station(t, path[i + 1]));
                    printf("\nSwitch Colour = %s\n", colour);
                    printf("\n");
                    line_traverse(path[i], path[i + 1], getLine(colour));
                }
            } 
            else if (i == path_length - 1) {
                colour = getCommonColour(search_station(t, path[i]), destination);
                printf("\nFinal Colour = %s\n", colour);
                printf("\n");
                line_traverse(path[i], d_name, getLine(colour));
            } 
            else {
                colour = getCommonColour(search_station(t, path[i]), search_station(t, path[i + 1]));
                printf("\nSwitch Colour = %s\n", colour);
                printf("\n");
                line_traverse(path[i], path[i + 1], getLine(colour));
            }
        } 
        else {
            printf("NULL\n");
        }
    }
    for (int i = 0; i < path_length; i++) {
        free(path[i]);
    }
    free(path);
    return;
}

char *getCommonColour(avlnode *source , avlnode *destination){
    char *colour;
    for (int i = 0; i < source->size; i++) {
        for (int j = 0; j < destination->size; j++) {
            if (strcmp(source->line_list[i], destination->line_list[j]) == 0) {
                colour = destination->line_list[j];
                return colour;
            }
        }
    }
    return NULL;
}
line getLine(char *colour){
    if (colour) {
        if (strcmp(colour, "Red") == 0) 
            return red;
        else if (strcmp(colour, "Blue") == 0) 
            return blue;
        else if (strcmp(colour, "Blue2") == 0) 
            return blue2;
        else if (strcmp(colour, "Green") == 0) 
            return green;
        else if (strcmp(colour, "Green2") == 0) 
            return green2;
        else if (strcmp(colour, "Grey") == 0) 
            return grey;
        else if (strcmp(colour, "Magenta") == 0) 
            return magenta;
        else if (strcmp(colour, "Pink") == 0) 
            return pink;
        else if (strcmp(colour, "Aqua") == 0) 
            return aqua;
        else if (strcmp(colour, "Violet") == 0) 
            return violet;
        else if (strcmp(colour, "Yellow") == 0) 
            return yellow;
        else if (strcmp(colour, "Rapid") == 0) 
            return rapid;
        else if (strcmp(colour, "Airport Express") == 0) 
            return airport_exp;
    }
    return NULL;
}
void line_traverse(char *source, char *destination, line l) {
    node *p = l;
    while (strcmp(p->name, source) != 0)
        p = p->next;
    int index1 = p->index;

    node *q = l;
    while (strcmp(q->name, destination) != 0)
        q = q->next;
    int index2 = q->index;

    int diff = abs(index2 - index1);
    char **list = (char **)malloc(sizeof(char *) * (diff + 2));
    int i = 0;
    if (index2 > index1) {
        while (strcmp(p->name, q->name) != 0) {
            list[i] = malloc((strlen(p->name) + 1) * sizeof(char));
            strcpy(list[i], p->name);
            printf("%s\n" , list[i]);
            i++;
            p = p->next;
        }
    } else {
        while (strcmp(p->name, q->name) != 0) {
            list[i] = malloc((strlen(p->name) + 1) * sizeof(char));
            strcpy(list[i], p->name);
            printf("%s\n" , list[i]);
            i++;
            p = p->prev;
        }
    }
    list[i] = malloc((strlen(destination) + 1) * sizeof(char));
    strcpy(list[i], destination);
    printf("%s\n" , list[i]);
    return;
}

