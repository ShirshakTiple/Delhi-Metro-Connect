

typedef struct node{
    char *name;
    char *colour;
    struct node *next;
    struct node *prev;
    int index;
}node;

typedef node* line;

extern line red;
extern line yellow;
extern line blue;
extern line airport_exp;
extern line green;
extern line rapid;
extern line violet;
extern line magenta;
extern line pink;
extern line aqua;
extern line grey;
extern line blue2;
extern line green2;

void init_line(line *l);
void insert_stop(line *l , char *name , char *colour , int index);
void display(line l);
void file_store();

