#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

//Self descriptive function name
int get_file_size(int fd) {
  struct stat stat_struct;
  if(fstat(fd, &stat_struct) == -1)
    return -1;
  return (int) stat_struct.st_size;
}

int main(void) {
  const char* filePath = "test.txt";
  char* fileBuffer;

  int fdSize = open(filePath, O_RDONLY, 0);
  int fileSize = get_file_size(fdSize);
  fileBuffer = (char*)malloc(fileSize); //Allocated buffer of the size of the file
  close(fdSize);

  FILE* inputFS = fopen(filePath, "r");
  char* line = NULL;
  size_t len = 0;
  ssize_t numRead;
  
  if (inputFS == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while ((numRead = getline(&line, &len, inputFS)) != -1) {
    printf("Retrieved line of length %zu:\n", numRead);
    fwrite(line, numRead, 1, stdout);
  }

  free(line);
  fclose(inputFS);
  exit(EXIT_SUCCESS);

}
