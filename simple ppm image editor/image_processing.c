#include <stdio.h>

/*
 * Read an image from the standard input and set the red value of each pixel to
 * zero.
 */
void remove_red() {
  /*declare variables for the magic number(header)row number,column number
   *and max colour
   */
  char magic_num[2];
  int col_num, row_num, max_col;
  //int i =1;
  scanf("%s %d %d %d", magic_num, & col_num, & row_num, & max_col);
  /*print all the info in the header of the image */
  printf("%s\n%d %d\n%d\n", magic_num, col_num, row_num, max_col);
  /*declare variables for the RGB values*/
  int red, green, blue;
  /*while loop through each pixel and change all red values to zero*/
  while (scanf("%d %d %d", & red, & green, & blue) != EOF) {
    /*setting all red values to 0*/
    red = 0;
    /*printing the values*/
    printf("%d %d %d\n", red, green, blue);
}
}

/*
 * Read an image from the standard input and convert it from colour to black and white.
 */
void convert_to_black_and_white() {
  /*declare variables for the magic number(header)row number,column number
   *and max colour
   */
  char magic_num[2];
  int col_num, row_num, max_col;
  //int i = 1;
  scanf("%s %d %d %d", magic_num, & col_num, & row_num, & max_col);
  /*print all the info in the header of the image */
  printf("%s\n%d %d\n%d\n", magic_num, col_num, row_num, max_col);
  /*declare variables for the RGB values and average*/
  int red, green, blue, average;
  /*while loop through each pixel to grayscale*/
  while (scanf("%d %d %d", & red, & green, & blue) != EOF) {
    // getting the average of the rgb vals
    average = (red + green + blue) / 3;
    /*printing the values*/
    printf("%d %d %d\n", average, average, average);
  }
}

/*
 * Read an image from the standard input and convert it to a square image.
 */
void instagram_square() {
  /*declare variables for the magic number(header)row number,column number
   *and max colour
   */
  char magic_num[2];
  int col_num, row_num, max_col;
  //i will be used as a counter
  int i =0;
  //declare if row is larger than column
  //int row_largest = 0;
  //declare if col is larger than row_num
  int col_largest = 0;
  scanf("%s %d %d %d", magic_num, & col_num, & row_num, & max_col);
  /*declare red,green,blue*/
  int red, green, blue;
  /* change the col and row to match that of the future squre
   * the lower value will become the basis of the
   * square
   */
  int square_basis;
  if (col_num > row_num) {
    //the basis for square becomes the row
    square_basis = row_num;
    //row largest declared as false
    //row_largest = 1;
  } else {
    //the basis for square becomes the column
    square_basis = col_num;
    //column largest declared as false
    col_largest = 1;
  }
  /*print all the info in the header of the image */
  printf("%s\n%d %d\n%d\n", magic_num, square_basis, square_basis, max_col);
  if (col_largest == 1){
      while (scanf("%d %d %d", & red, & green, & blue) != EOF && i < square_basis*square_basis) {
        i++;
        printf("%d %d %d\n", red, green, blue);
  }
}
  else{
    //items that are not present in the square
    int removed_square = 0;
    while (scanf("%d %d %d", & red, & green, & blue) != EOF){
      i++;
      removed_square ++;
      if (removed_square <= square_basis){
          printf("%d %d %d\n", red, green, blue);
      }
      //if max column number is reached the removed_square is reset to 0
      if (i >= col_num && i%col_num==0){
          removed_square = 0;
      }
    }
  }
}
