#include <stdio.h>

#include <stdlib.h>

/*
 * Include the function prototypes of all those functions implemented in image_processing.c.
 * Do not remove or modify them in any way.
 */

void remove_red();
void convert_to_black_and_white();
void instagram_square();

int main(int argc, char * argv[]) {
  /* Task 1 needs to be completed here: Make sure that the command line arguments are correct. In case an error exists,
   * print the error message and terminate the program. Otherwise, invoke the corresponding
   * image processing function. */

  /* if argument count does not equal 2 the error message is printed
   * and the program is terminated*/
  if (argc != 2) {
    printf("Error: Expecting one command-line argument, which needs to be either 1, 2, or 3.\n");
  } else {
    //storing the option chosen into an integer variable
    int opt = strtol(argv[1], NULL, 10);

    /* the function relevant to the choice will be ran
     * 1 indicates remove_red, 2 for convert_to_black_and_white
     * 3 for instagram_square
     * if not one of the three choices an error message
     * will be printed and the program will terminate */

    //remove red
    if (opt == 1) {
      remove_red();
    }
    //black and white conversion
    else if (opt == 2) {
      convert_to_black_and_white();
    }
    //instagram square
    else if (opt == 3) {
      instagram_square();
    }
    // error message
    else {
      printf("Error: Expecting one command-line argument, which needs to be either 1, 2, or 3.\n");
    }
  }
  return 0;
}
