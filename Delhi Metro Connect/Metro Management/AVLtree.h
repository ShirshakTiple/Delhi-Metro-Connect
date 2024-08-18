

typedef struct avlnode{
    char *name;
    char **line_list;
    int size;
    struct avlnode * parent , *right , *left;
    int bf;
}avlnode;

typedef avlnode* AVLtree;

void init_AVL(AVLtree *t);
void insert_AVL(AVLtree *t , avlnode *n);
void display_AVL(AVLtree t);
avlnode* search_station(AVLtree t,char *val);