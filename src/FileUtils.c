#include <stdio.h>

#include "Signal.h"

// TODO: error checking for system calls
void File_ReadSignal(Signal *signal, char *filename) {
  FILE *fd = fopen(filename, "r");

  for (int32_t i = 0; i < signal->size; i++) {
    // fread(&(signal->samples[i]), sizeof(Sample), 1, fd);
  }

  fclose(fd);
}

void File_WriteSignal(Signal *signal, char *filename) {
  FILE *fd = fopen(filename, "w");

  for (int32_t i = 0; i < signal->size; i++) {
    // fwrite(&(signal->samples[i]), sizeof(Sample), 1, fd);
    fprintf(fd, "%lf\n", signal->samples[i]);
  }

  fclose(fd);
}