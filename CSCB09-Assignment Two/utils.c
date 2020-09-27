/*
 * Created by Sotirios-Efstathios Maneas on 2017-01-02.
 */

#include <stdio.h>
#include <string.h>

#include "utils.h"


/*
 * Tokenize the string stored in cmd based on DELIMITERS as separators.
 * Return the number of tokens, and store pointers to them in cmd_argv.
 */
int tokenize(char *cmd, char **cmd_argv) {

		int args_count = 0;
		//retrieving the first token
		char *tkn = strtok(cmd, DELIMITERS);

		//loop to find all tokens
		while (tkn!= NULL){
			//pointer stored in cmd_argv
			cmd_argv[args_count] = tkn;
			//increment the count by one to get all tokens
			args_count+=1;
			tkn = strtok(NULL,DELIMITERS);
		}


        // return number of tokens stored
	return args_count;
}