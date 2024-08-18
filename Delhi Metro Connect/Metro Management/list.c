#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"

line red;
line yellow;
line blue;
line airport_exp;
line green;
line rapid;
line violet;
line magenta;
line pink;
line aqua;
line grey;
line blue2;
line green2;

void process_file(char* filename, line* l);
void file_store();

void init_line(line *l){
    *l = NULL;
    return;
}


void file_store() {
    char* filenames[] = {
        "red.txt", "yellow.txt", "blue.txt", "airport_exp.txt", "green.txt",
        "rapid.txt", "violet.txt", "magenta.txt", "pink.txt", "aqua.txt", "grey.txt",
        "blue2.txt", "green2.txt"
    };
    init_line(&red);
    process_file(filenames[0], &red);

    init_line(&yellow);
    process_file(filenames[1], &yellow);

    init_line(&blue);
    process_file(filenames[2], &blue);

    init_line(&airport_exp);
    process_file(filenames[3], &airport_exp);

    init_line(&green);
    process_file(filenames[4], &green);

    init_line(&rapid);
    process_file(filenames[5], &rapid);

    init_line(&violet);
    process_file(filenames[6], &violet);

    init_line(&magenta);
    process_file(filenames[7] , &magenta);

    init_line(&pink);
    process_file(filenames[8], &pink);

    init_line(&aqua);
    process_file(filenames[9], &aqua);

    init_line(&grey);
    process_file(filenames[10], &grey);

    init_line(&blue2);
    process_file(filenames[11] , &blue2);

    init_line(&green2);
    process_file(filenames[12], &green2);

    return;
}

void process_file(char* filename, line* l) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Empty file\n");
        return;
    }
    
    char name[50];
    char colour[50];
    fgets(colour, 50, fp);
    colour[strcspn(colour, "\n")] = '\0';
    int index = 1;
    while (fgets(name, 50, fp) != NULL) {
        name[strcspn(name, "\n")] = '\0';
        insert_stop(l, name, colour , index);
        index++;
    }
    fclose(fp);
   //display(*l);
    return;
}

void insert_stop(line *l , char*name , char*colour , int index){
    node* nn = (node*)malloc(sizeof(node));
    if (!nn) return;

    nn->name = NULL;
    nn->colour = NULL;
    nn->next = NULL;
    nn->prev = NULL;
    nn->index = 0;

    if (name && colour) {
        nn->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
        nn->colour = (char*)malloc(sizeof(char) * (strlen(colour) + 1));
        strcpy(nn->name, name);
        strcpy(nn->colour, colour);
        nn->index = index;
    }
    if(!(*l)){
        *l = nn;
        return;
    }
    node *p = *l;
    
    while(p->next)
        p = p->next;
    
    nn->prev = p;
    p->next = nn;
    return;
}

void display(line l){
    if(!l)
        return;
    node *p = l;
    while(p){
        printf("%s-%s\n" , p->name , p->colour);
        p = p->next;
    }
    return;
}
