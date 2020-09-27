/*
 * Created by Sotirios-Efstathios Maneas on 2017-01-02.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"


/**
 *  A helper function that allocates a new tree node.
 *
 *  @param value The entry's value. It represents either an attribute or a filename.
 *  @return A pointer to the newly allocated node.
 *  (for now it returns NULL, so the compiler does not complain)
 */
struct TreeNode *allocate_node(const char *value) {
	//memory allocated for the node size of a treeNode
	struct TreeNode *node = malloc(sizeof(struct TreeNode));
	node->value = malloc(strlen(value) + 1);
	//string is copied 
	strcpy((*node).value, value);
	//setting child to null
	(*node).child = NULL;
	//setting sibling to null
	(*node).sibling = NULL;
	//node is returned as wanted
	return node;
}

/**
 *  Insert a new image to a tree
 *
 *  @param tree A pointer to the root of the tree.
 *  @param values An array, whose first three members are the attribute values for 
 *                the image and the last one is the filename
 */
void tree_insert(struct TreeNode *root, char **values) {
	//start at the root get a reference for it
	struct TreeNode *root_ref = root;
	//get a reference to the child we work with
	struct TreeNode *wk_node = (*root_ref).child;
	
	//integer looking at the difference between two values
	int dif;

	//loop counter finding the attributes
	for (int i = 0; i < 4; i++) {

		// if the row below the root is empty
		if (wk_node == NULL) {

			// create a node and set it to be the working node
			root_ref->child = allocate_node(values[i]);
			//working node 
			wk_node = (*root_ref).child;

		}
		//taking two strings and returning the int for dif
		dif = strcmp((*wk_node).value, values[i]);

		// if the row that is below
		// is larger than the value
		if (dif > 0) {

			// a node will be created and the sibling value will be
			// set to it
			root_ref->child = allocate_node(values[i]);
			//sibling value is set to the node
			root_ref->child->sibling = wk_node;
			wk_node = root_ref->child;
			//comparing the value of the strings
			dif = strcmp((*wk_node).value, values[i]);

		}


		// while loop going through the
		// row of siblings
		while (dif < 0 && wk_node != NULL) {
			//the reference root becomes the working node
			root_ref = wk_node;
			//working node is set to the sibling
			wk_node= wk_node->sibling;

			//if the working node is empty
			if (wk_node != NULL)
				//string are compared
				dif = strcmp((*wk_node).value, values[i]);
		}
		// if the difference is not zero 
		// the node will be placed between the root and the working
		// node
		if (dif != 0) {
			//
			root_ref->sibling = allocate_node(values[i]);
			root_ref->sibling->sibling = wk_node;
			wk_node = root_ref->sibling;
		}
		//at end of row if the working node is empty
		else if (wk_node == NULL) {
			root_ref->sibling = allocate_node(values[i]);
			wk_node = root_ref->sibling;
		}
		//root reference is set to the current working node
		root_ref = wk_node;
		wk_node = wk_node->child;
	}

}

/** 
 *  Searches a tree to print all files with matching attribute values.
 * 
 *  @param tree A pointer to the root of the tree. 
 *  @param values An array of attribute values 
 */
void tree_search(const struct TreeNode *root, char **values) {

	// setting a pointer for the first node with the file values
	struct TreeNode *fnode; 
	//setting it to the root of the child
	fnode = ( * root).child;
	int dif;
	//if the child of the root is null print null as specified in guidelines
	if (( * root).child != NULL) {
	//loop through the rows
	for (int i = 0; i < 3; i++) {
		//difference between the values
		dif = strcmp(fnode->value, values[i]);

		// while loop through siblings to find an atrribute
		while (dif < 0 && fnode != NULL) {
			//current node is sibling
			fnode = fnode->sibling;
			//if it is null look at diff of diff values
			if (fnode != NULL)
				dif = strcmp(fnode->value, values[i]);
		}
		
		// the attribute declared does not exist in the database so null will be
		// printed as specified on the asssignment sheet
		if (fnode == NULL){
			//null printed
			printf("(NULL)");
			//print a new line
			printf("\n");
			//return as expected
			return;
		}
		else if (dif != 0)
			{
			//null printed
			printf("(NULL)");
			//print a new line
			printf("\n");
			//return as expected
			return;
		}
		//else if the node is not empty
		//the node will become the child
		else
			//first node 
			fnode = fnode->child;
	}
	//if the node is not empty
	while (fnode != NULL) {
		//the values are printed
		printf("%s ", fnode->value);
		fnode = fnode->sibling;
	}
	//print a new line
	printf("\n");
}
//else
	else {
		//if the root-> child is empty null is returned as expected
		printf("(NULL)");
		//print a new line
		printf("\n");
		return;
	}
}

//helper to print the db in increasing order as specified in assignment sheet
void db_printer(struct TreeNode *tree, char **info, int print_num, int stage){
	//for the stage at 5
	if (stage!=5){
		// store current values from tree to be used when the printer hits stage 5
		info[stage-2] = tree->value;
		//use the printer with a value increased to the last line and the current node child
		db_printer(tree->child, info,1, stage+1);
	}else{
		// obtain the name of the file from the array of info
		info[3] = tree->value;
		// for loop through all info and print them to the stdout
		int i;
		int counter = 4;
		for(i = 0; i < counter; i++) {
			printf("%s ", info[i]);
		}
		// after each file a new line is declared
		printf("\n");
	} 
	//each file and its info will only be returned once
	if(print_num == 1) {
		// Loops through until the sibling is NULL
		while(tree->sibling != NULL) {
			int print_num_stop_var = 0;
			db_printer(tree->sibling, info,print_num_stop_var, stage);
			//tree will become its sibling
			tree = tree->sibling;
		}
	}
}
/**
 *  Prints a complete tree to the standard output.
 *
 *  @param tree A pointer to the root of the tree.
 */
void tree_print(const struct TreeNode *tree) {
	// an empty array decalred for values
	char *info[4];
	//if the first nodes child is not null
	//print the database
	if((*tree).child != NULL) {
		//int i declared to be 1 for 1 recursive call
		int i = 1;
		db_printer((*tree).child, info,i, 2);
	} else {
		printf("(NULL)");
		//printing a new line
		printf("\n");
		//int i declared to be 1 for 1 recursive call
		int i = 1;
		//call dbprinter to print the info
		//and filename
		db_printer((*tree).child, info,i, 2);
	}
}
