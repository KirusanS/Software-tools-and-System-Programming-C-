**Image retrieval program**

Using a provided name of a ppm image file and the name of the directory where we want to look for the similar. The program will search the directory for sub-directories and fork a new process for each sub-directory. Each child process will compare the original image to all the images in the sub-directory that it was assigned. It will then send to the parent process information on the most similar image in its sub-directory.
