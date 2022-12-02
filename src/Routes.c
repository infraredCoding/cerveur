#include "Routes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Route * initRoute(char* key, char* value) {
	struct Route * temp = (struct Route *) malloc(sizeof(struct Route));

	temp->key = key;
	temp->value = value;

	temp->left = temp->right = NULL;
	return temp;
}

void inorder(struct Route* root)
{

    if (root != NULL) {
        inorder(root->left);
        printf("%s -> %s \n", root->key, root->value);
        inorder(root->right);
    }
}

struct Route * addRoute(struct Route * root, char* key, char* value) {
	if (root == NULL) {
		return initRoute(key, value);
	}

	if (strcmp(key, root->key) == 0) {
		printf("============ WARNING ============\n");
		printf("A Route For \"%s\" Already Exists\n", key);
	}else if (strcmp(key, root->key) > 0) {
		root->right = addRoute(root->right, key, value);
	}else {
		root->left = addRoute(root->left, key, value);
	}
}

struct Route * search(struct Route * root, char* key) {
	struct Route *selected=root;
	while (selected != NULL) {
	if (strcmp(key, selected->key) == 0) {
		return selected;
	}
	else if (strcmp(key, selected->key) > 0) {
		selected=selected->right;
	}else if (strcmp(key, selected->key) < 0) {
		selected=selected->left;
		}  
	}
	return NULL;
}

