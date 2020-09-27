/*
 * Created by Sotirios-Efstathios Maneas on 2017-01-02.
 */

#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "utils.h"

int main(void) {

     // char array to hold a line of input
  char buf[BUFFER_SIZE] = {'\0'};

        // char* array to hold the pointers to tokens
  char *args[INPUT_ARG_MAX_NUM];

        // the root of the tree
        struct TreeNode root;
        root.value = "";
        root.child = NULL;
        root.sibling = NULL;

  struct TreeNode *root_ptr = &root;
  struct TreeNode *temp = root_ptr;
        // Add your code below:
  // number of tokens for each line of argument
  int no_token;
  // reading line by line from std
  while(fgets(buf, BUFFER_SIZE, stdin) != NULL){
    // convert each line into tokens
    no_token = tokenize(buf, args);
    // check if the argument and the number of arguments are correct
    if((args[0][0] == 'i') && (no_token == 5)){
      char *info_fname[4] = {args[1], args[2], args[3], args[4]};
      tree_insert(temp, info_fname);
      }
    else if((args[0][0]== 'q') && (no_token == 4) ){
      char *info[3] = {args[1], args[2], args[3]};
      tree_search(temp, info);
      }
    else if((args[0][0] == 'p') && (no_token == 1)){
      tree_print(temp);
      }
    else{ fprintf(stderr, "Invalid command\n");}

  }

        return 0;
}