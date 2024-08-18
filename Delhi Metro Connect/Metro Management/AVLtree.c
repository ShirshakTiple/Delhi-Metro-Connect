#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include"AVLtree.h"

avlnode* adjust_imbal(AVLtree *t);
void LL_rotate(AVLtree *t , avlnode *A);
void LR_rotate(AVLtree *t , avlnode *A);
void RR_rotate(AVLtree *t , avlnode *A);
void RL_rotate(AVLtree *t , avlnode *A);


int max(int a , int b){
	if(a>b)
		return a;
	else
		return b;
}

int absolute(int a){
	if(a>=0)
		return a;
	else
		return -a;	
}

int height(AVLtree t){
	if(!t) return -1;
	return(1 + max(height(t->left) , height(t->right)));
}


void init_AVL(AVLtree *t) {
    *t = NULL;
    FILE *fp;
    avlnode *nn;
    char line[100];
    char *name;
    char *colour;
    
    fp = fopen("all_stops.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        
        name = strtok(line, "-");
        colour = strtok(NULL, "-");
        
        nn = (avlnode *)malloc(sizeof(avlnode));
        nn->parent = NULL;
        nn->right = NULL;
        nn->left = NULL;
        nn->bf = 0;
        nn->size = 0;
        nn->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(nn->name, name);
        nn->line_list = (char **)malloc(sizeof(char *) * 1);
        nn->line_list[0] = (char *)malloc(sizeof(char) * (strlen(colour) + 1));
        strcpy(nn->line_list[0], colour);
        nn->size++;
        
        insert_AVL(t, nn);
    }
    
    fclose(fp);
}



void insert_AVL(AVLtree *t, avlnode *n) {
    avlnode *p = *t;
    avlnode *q = NULL;
    if (!p) {
        *t = n;
        return;
    }
    while (p) {
        q = p;
        if (strcmp(p->name, n->name) == 0) {
            int newSize = p->size + n->size;
            p->line_list = realloc(p->line_list, sizeof(char *) * newSize);
            for (int i = 0; i < n->size; i++) {
                p->line_list[p->size + i] = strdup(n->line_list[i]);
            }
            p->size = newSize;
            return;
        }
        if (strcmp(p->name, n->name) > 0)
            p = p->left;
        else
            p = p->right;
    }
    n->parent = q;
    if (strcmp(q->name, n->name) > 0)
        q->left = n;
    else
        q->right = n;

    // Perform AVL tree rotations if necessary
    avlnode *imbal = NULL;
    imbal = adjust_imbal(&q);
    if (imbal) {
        if (strcmp(n->name, imbal->name) < 0) {
            if (imbal->left && strcmp(n->name, imbal->left->name) < 0)
                LL_rotate(t, imbal);
            else
                LR_rotate(t, imbal);
        }
        else {
            if (imbal->right && strcmp(n->name, imbal->right->name) < 0)
                RL_rotate(t, imbal);
            else
                RR_rotate(t, imbal);
        }
    }
}


avlnode* adjust_imbal(AVLtree *t){
    avlnode *p = *t;
    avlnode *imb = NULL;
    int flag = 0;
    while(p){
        p->bf =height(p->left) - height(p->right);
        if(absolute(p->bf) > 1 && !flag){
            imb = p;
            flag = 1;
        }
        if(imb && flag)
            break;
            p = p->parent;
        }
    return imb;
}

void LL_rotate(AVLtree *t , avlnode *A){
    avlnode *B = A->left;
	avlnode *Br = B->right;
	avlnode *Ap = A->parent;
	B->parent = Ap;
	if(!Ap){
		*t = B;	
	}
	else{
		if(Ap->left == A)
			Ap->left = B;
		else
			Ap->right = B;
	}
	B->right = A;
	A->left = Br;
	A->parent = B;
	if(Br){
		Br->parent = A;
	}
	B->bf = 0;
	A->bf = 0;
    return;
}

void LR_rotate(AVLtree *t , avlnode *A){
	avlnode *B = A->left;
	avlnode *C = B->right;
	avlnode *Cl = C->left;
	avlnode *Cr = C->right;
	avlnode *Ap = A->parent;
	C->parent = Ap;
	if(!Ap){
		*t = C;
	}
	else{
		if(Ap->left == A)
			Ap->left = C;
		else
			Ap->right = C;
	}
	C->right = A;
	C->left = B;
	A->parent = C;
	B->parent = C;
	B->right = Cl;
	if(Cl)
		Cl->parent = B;
	A->left = Cr;
	if(Cr)
		Cr->parent = A;
	if(C->bf == 0){
		A->bf = 0;
		B->bf = 0;
		return;
	}
	else if(C->bf == 1){
		A->bf = -1;
		B->bf = 0;
		C->bf = 0;
		return;
	}
	else{
		A->bf = 0;
		B->bf = 1;
		C->bf = 0;
		return;
	}
	return;
}

void RR_rotate(AVLtree *t , avlnode *A){
	avlnode *B = A->right;
	avlnode *Bl = B->left;
	avlnode *Ap = A->parent;
	B->parent = Ap;
	if(!Ap){
		*t = B;
	}
	else{
		if(Ap->left==A)
			Ap->left = B;
		else
			Ap->right = B;
	}
	B->left = A;
	A->right = Bl;
	A->parent = B;
	if(Bl){
		Bl->parent = A;
	}
	B->bf = 0;
	A->bf = 0;
	return;
}

void RL_rotate(AVLtree *t , avlnode *A){
	avlnode *B = A->right;
	avlnode *C = B->left;
	avlnode *Cl = C->left;
	avlnode *Cr = C->right;
	avlnode *Ap = A->parent;
	C->parent = Ap;
	if(!Ap)
		*t = C;
	else{
		if(Ap->left == A)
			Ap->left = C;
		else
			Ap->right = C;
	}
	C->left = A;
	C->right = B;
	A->parent = C;
	B->parent = C;
	A->right = Cl;
	if(Cl)
		Cl->parent = A;
	B->left = Cr;
	if(Cr)
		Cr->parent = B;
	if(C->bf == 0){
		A->bf = 0;
		B->bf = 0;
		return;
	}
	else if(C->bf == 1){
		A->bf = 0;
		B->bf = -1;
		C->bf = 0;
		return;
	}
	else{
		A->bf = 1;
		B->bf = 0;
		C->bf = 0;
		return;
	}
	return;
}

void display_AVL(AVLtree t) {
    if (!t)
        return;
    display_AVL(t->left);
    printf("%s - ", t->name);
    if (t->size > 0) {
        for (int i = 0; i < t->size; i++)
            printf("%s ", t->line_list[i]);
    }
    printf("\n");
    display_AVL(t->right);
}

avlnode* search_station(AVLtree t, char *val){
	avlnode *temp = t ;
	if((!t)) 
		return NULL ;
	while(temp){
		if(strcmp(temp->name,val) == 0){
			return temp;
		}
		else if(strcmp(temp->name,val) > 0){
			temp = temp->left ;
		}
		else {
			temp = temp->right ;
		}
	}
	if(!temp) 
		return NULL ;
	
	return NULL;
}
