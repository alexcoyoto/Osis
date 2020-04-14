#include "pch.h"
#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct Node {
	int data;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
} Node;


Node* getFreeNode(int value, Node *parent) {
	Node* tmp = (Node*)malloc(sizeof(Node));
	tmp->left = tmp->right = NULL;
	tmp->data = value;
	tmp->parent = parent;
	return tmp;
}


void insert(Node **head, int value) {
	Node *tmp = NULL;
	Node *ins = NULL;
	if (*head == NULL) {
		*head = getFreeNode(value, NULL);
		return;
	}

	tmp = *head;
	while (tmp) {
		if (value > tmp->data) {
			if (tmp->right) {
				tmp = tmp->right;
				continue;
			}
			else {
				tmp->right = getFreeNode(value, tmp);
				return;
			}
		}
		else if (value < tmp->data) {
			if (tmp->left) {
				tmp = tmp->left;
				continue;
			}
			else {
				tmp->left = getFreeNode(value, tmp);
				return;
			}
		}
		else {
			exit(2);
		}
	}
}


Node* getMinNode(Node *root) {
	while (root->left) {
		root = root->left;
	}
	return root;
}


Node* getMaxNode(Node *root) {
	while (root->right) {
		root = root->right;
	}
	return root;
}


Node *getNodeByValue(Node *root, int value) {
	while (root) {
		if (root->data > value) {
			root = root->left;
			continue;
		}
		else if (root->data < value) {
			root = root->right;
			continue;
		}
		else {
			return root;
		}
	}
	return NULL;
}

void removeNodeByPtr(Node *target) {
	if (target->left && target->right) {
		Node *localMax = getMaxNode(target->left);
		target->data = localMax->data;
		removeNodeByPtr(localMax);
		return;
	}
	else if (target->left) {
		if (target == target->parent->left) {
			target->parent->left = target->left;
			target->left->parent = target->parent;
		}
		else {
			target->parent->right = target->left;
			target->left->parent = target->parent;
		}
	}
	else if (target->right) {
		if (target == target->parent->right) {
			target->parent->right = target->right;
			target->right->parent = target->parent;
		}
		else {
			target->parent->left = target->right;
			target->right->parent = target->parent;
		}
	}
	else {
		if (target == target->parent->left) {
			target->parent->left = NULL;
		}
		else {
			target->parent->right = NULL;
		}
	}
	free(target);
}


void deleteValue(Node *root, int value) {
	Node *target = getNodeByValue(root, value);
	removeNodeByPtr(target);
}


void printTree(Node *root)
{
	if (root) {
		printf("%d ", root->data);
		printTree(root->left);
		printTree(root->right);
	}
}

void clearTree(Node* root)
{
	if (root) {
		clearTree(root->left);
		clearTree(root->right);
		if (root->parent)
		{
			root->parent->right = NULL;
			root->parent->left = NULL;
		}
		free(root);
	}
}


void main() {
	Node *root = NULL;
	insert(&root, 10);
	insert(&root, 12);
	insert(&root, 8);
	insert(&root, 9);
	insert(&root, 7);
	insert(&root, 3);
	insert(&root, 4);
	printTree(root);
	deleteValue(root, 4);
	deleteValue(root, 8);
	deleteValue(root, 3);

	printf("\n");

	printTree(root);

	clearTree(root);
	root = NULL;

	printf("\n");

	printTree(root);

	system("pause");
}