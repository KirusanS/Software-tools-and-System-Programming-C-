#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <float.h>
#include "worker.h"
//by kirusan s 1000558434
int main(int argc, char **argv) {
	
	char ch;
	char path[PATHLENGTH];
	char *startdir = ".";
    char *image_file = NULL;

	while((ch = getopt(argc, argv, "d:")) != -1) {
		switch (ch) {
			case 'd':
			startdir = optarg;
			break;
			default:
			fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME] FILE_NAME\n");
			exit(1);
		}
	}

        if (optind != argc-1) {
	     fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME] FILE_NAME\n");
        } else
             image_file = argv[optind];


	// Open the directory provided by the user (or current working directory)
	
	DIR *dirp;
	if((dirp = opendir(startdir)) == NULL) {
		perror("opendir");
		exit(1);
	} 
	DIR *dirp2;
	if((dirp2 = opendir(startdir)) == NULL) {
		perror("opendir");
		exit(1);
	} 
	
	/* For each entry in the directory, eliminate . and .., and check
	* to make sure that the entry is a directory, then call run_worker
	* to process the image files contained in the directory.
	*/
		
	struct dirent *dp;
	struct dirent *dp2;
	CompRecord rec_two;
	strcpy(rec_two.filename, "");        
	rec_two.distance = FLT_MAX;
    Image * starter_image = read_image(image_file);
	int number_dir = 0;
	while ((dp2 = readdir(dirp2)) != NULL){
		if(strcmp(dp2->d_name, ".") == 0 || 
		    strcmp(dp2->d_name, "..") == 0 ||
		    strcmp(dp2->d_name, ".svn") == 0){
			continue;
		}
		strncpy(path, startdir, PATHLENGTH);
		strncat(path, "/", PATHLENGTH - strlen(path) - 1);
		strncat(path, dp2->d_name, PATHLENGTH - strlen(path) - 1);
		struct stat sbuf;
		if(stat(path, &sbuf) == -1) {
			//This should only fail if we got the path wrong
			// or we don't have permissions on this entry.
			perror("stat");
			exit(1);
		}
		//only call if it is a directory then increase by 1
		if (S_ISDIR(sbuf.st_mode)){
		   number_dir++;
		}
	}
	int pipefd[number_dir][2];
	int j;
	j = 0;
	while((dp = readdir(dirp)) != NULL && j!=number_dir) {
		
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
		
		//call pipe for the sub
		if ((pipe(pipefd[j])) == -1) {
			perror("pipe failed");
			exit(1);
		}
		//call fork
		int result = fork();
		if (result < 0){
			//sys-call error
			perror("fork failed");
			exit(1);

		}else if (result == 0){
			if (close(pipefd[j][0]) == -1) {
				
				perror("reading end close");
			}
			//child
			int num_c;
			for (num_c = 1; num_c < j; num_c++) {
				if (close(pipefd[num_c][0]) == -1) {
					perror("reading ends close for forked children");
				}
			}

			// Only call process_dir if it is a directory
			// Otherwise ignore it.
			if(S_ISDIR(sbuf.st_mode)) {
				printf("Processing all images in directory: %s \n", path);
				rec_two = process_dir(path, starter_image, pipefd[j][1]);
				if (write(pipefd[j][1], &rec_two, sizeof(CompRecord)) != sizeof(CompRecord)) {
					perror("write to pipe");
				}
				if (close(pipefd[j][1]) == -1) {
					perror("writing pipe to close");
				}
			}
			exit(0);
		}else{

			if (close(pipefd[j][1]) == -1) {
				perror("writing parent pipe to close");
			}
		}
		if(S_ISDIR(sbuf.st_mode)) {
			j++;
		}
	}
	CompRecord CRec;
	strcpy(CRec.filename, "");        
	CRec.distance = FLT_MAX;
	CompRecord curr_CRec;	
				
	for (int i=0; i< number_dir; i++) {
		if (read(pipefd[i][0], &curr_CRec, sizeof(CompRecord)) == -1) {
			perror("child pipe reading");
		}
		if (CRec.distance < 0 || curr_CRec.distance < CRec.distance){
			strcpy(CRec.filename, curr_CRec.filename);
			CRec.distance = curr_CRec.distance;
		}
	}
    printf("The most similar image is %s with a distance of %f\n", CRec.filename, CRec.distance);
	
	return 0;
}