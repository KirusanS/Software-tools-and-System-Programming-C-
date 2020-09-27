#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <float.h>
#include "worker.h"

//by kirusan s 1000558434
/*
 * Read an image from a file and create a corresponding 
 * image struct 
*/
Image* read_image(char *filename)
{
        FILE *src_f = fopen(filename, "r");
        //variables to store width, height and max colour value
        //rgb values
        int row;
        int column; 
        int max_colour;
        int r;
        int g;
        int b;
        char start_image[3];
        Image *img = malloc(sizeof(Image));


        //print error if cannot open the file
        if ((src_f = fopen(filename, "r")) == NULL){
            fprintf(stderr, "file cannot be opened\n");
            perror("error: ");
            return NULL;
        }
        fscanf(src_f, "%s", start_image);
        fscanf(src_f, "%d %d",  &column, &row);
        fscanf(src_f, "%d", &max_colour);

        //check if starting image is P3
        //if not null will be returned
        if (strcmp(start_image, "P3") != 0){
            return (NULL);
        }

        img->width = column;
        img->height = row;
        img->max_value = max_colour;

        int total_size = column*row;
        //array of pixels
        Pixel *pixels = malloc(sizeof(Pixel) * total_size);
        Pixel pix;


         for(int i = 0; i < total_size; i++){
            fscanf(src_f, "%d %d %d", &r, &g, &b);
            pix.red = r;
            pix.green = g;
            pix.blue = b;
            pixels[i] = pix;
            
        }

        img->p = pixels;
        return img;
}

/*
 * Print an image based on the provided Image struct 
 */

void print_image(Image *img){
        printf("P3\n");
        printf("%d %d\n", img->width, img->height);
        printf("%d\n", img->max_value);
       
        for(int i=0; i<img->width*img->height; i++)
           printf("%d %d %d  ", img->p[i].red, img->p[i].green, img->p[i].blue);
        printf("\n");
}

/*
 * Compute the Euclidian distance between two pixels 
 */
float eucl_distance (Pixel p1, Pixel p2) {
       return sqrt( pow(p1.red - p2.red,2 ) + pow( p1.blue - p2.blue, 2) + pow(p1.green - p2.green, 2));
}

float p3_check(FILE *file, Image *img, float *difference);
float dissect_image(FILE *file, Image *img, int *total_pixels);
float acceptable_file(FILE *file);

 
/*
 * Compute the average Euclidian distance between the pixels 
 * in the image provided by img1 and the image contained in
 * the file filename
 */
float compare_images(Image *img1, char *filename) {
    FILE *file;
    float difference;
    int total_pixels;
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file\n");
        exit(1); 
    }
    //p3 check
    if (p3_check(file, img1, &difference) != 0) { 
        return FLT_MAX; 
    }
    if (difference != FLT_MAX){
        total_pixels = (img1->width)*(img1->height);
        difference = dissect_image(file, img1, &total_pixels);
    }
    if (fclose(file) != 0) {
        fprintf(stderr, "Could not close file\n");
        exit(1); 
    }
    return difference;
}

float p3_check(FILE *file, Image *img, float *difference){
    int width;
    int height;
    int colours;
    if (acceptable_file(file) != 0) {
        return 1;
    }
    fscanf(file,"%d%d%d", &width, &height, &colours);
    if (width != img->width || height != img->height) {
        *difference = FLT_MAX;
    } 
    return 0;
}


float dissect_image(FILE *file, Image *img, int *total_pixels) {
    float difference = 0;
    int ixp = 0;
    int select_colour;
    Pixel selected_p;
    int curr_num;
    for (;ixp < *total_pixels; ixp++) {
        select_colour = 0;
        for (;select_colour <3; select_colour ++){
            fscanf(file, "%d", &curr_num);
            if (select_colour == 1) {
                selected_p.green = curr_num;
            }
            else if (select_colour == 0) {
                selected_p.red = curr_num;
            } else {
                selected_p.blue = curr_num;
            }
        }
        difference += eucl_distance ((img->p)[ixp], selected_p);
    }
    return difference/(*total_pixels);
}
   

float acceptable_file(FILE *file){
    int input;
    char buffer[3];
    input = fscanf(file,"%2s", buffer);
    if (strcmp(buffer, "P3") != 0|| input != 1) {
        return 1;
    }
    return 0;
}
 

/* process all files in one directory and find most similar image among them
* - open the directory and find all files in it 
* - for each file read the image in it 
* - compare the image read to the image passed as parameter 
* - keep track of the image that is most similar 
* - write a struct CompRecord with the info for the most similar image to out_fd
*/
CompRecord process_dir(char *dirname, Image *img, int out_fd) {
    char path[PATHLENGTH];
    char *startdir = dirname;

    DIR *dirp;
    if((dirp = opendir(startdir)) == NULL) {
        perror("opendir");
        exit(1);
    }

    /* For each entry in the directory, eliminate . and .., and check
    * to make sure that the entry is a directory, then call run_worker
    * to process the image files contained in the directory.
    */

    struct dirent *dp;
    CompRecord CRec;
    CRec.distance = FLT_MAX;

    while((dp = readdir(dirp)) != NULL) {

        if(strcmp(dp->d_name, ".") == 0 ||
           strcmp(dp->d_name, "..") == 0 ||
           strcmp(dp->d_name, ".svn") == 0){
            continue;
        }
        strncpy(path, startdir, PATHLENGTH);
        strncat(path, "/", PATHLENGTH - strlen(path) - 1);
        strncat(path, dp->d_name, PATHLENGTH - strlen(path) - 1);

        struct stat sbuf;
        if(stat(path, &sbuf) == -1) {
            //This should only fail if we got the path wrong
            // or we don't have permissions on this entry.
            perror("stat");
            exit(1);
        }

        if(S_ISREG(sbuf.st_mode)) {
            printf("%s\n", path);
            if(compare_images(img, path) < CRec.distance) {
                strcpy(CRec.filename, path);
                CRec.distance = compare_images(img, path);
            }
        }
    }

    return CRec;
}
