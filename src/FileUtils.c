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

// filename becomes <filename>_real and <filename>_imag
//  for the real and imaginary components of the complex signal
void File_WriteComplexSignal(ComplexSignal *signal, char *filename) {
  char r_name[32];
  sprintf(r_name, "%s_real.sig", filename);
  char i_name[32];
  sprintf(i_name, "%s_imag.sig", filename);

  FILE *r_fd = fopen(r_name, "w");
  FILE *i_fd = fopen(i_name, "w");

  for (int32_t i = 0; i < signal->size; i++) {
    // fwrite(&(signal->samples[i]), sizeof(Sample), 1, fd);
    fprintf(r_fd, "%lf\n", signal->real[i]);
    fprintf(i_fd, "%lf\n", signal->imag[i]);
  }

  fclose(r_fd);
  fclose(i_fd);
}
