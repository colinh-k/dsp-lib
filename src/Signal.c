#include <stdio.h>

#include "Signal.h"

#define BUF_SIZE 1028  // 4096

Signal* Signal_Create(size_t s_size) {
  Signal *signal = (Signal *) malloc(sizeof(Signal));
  if (signal == NULL)
    return NULL;  // malloc failed

  signal->samples = (Sample *) malloc(s_size * sizeof(Sample));
  if (signal->samples == NULL)
    return NULL;  // malloc failed
  
  signal->size = s_size;
  return signal;
}

// not implemented:
// int8_t Signal_Load(Signal *signal, char *filename) {
//   FILE *fd = fopen(filename, "r");
//   if (!fd)
//     return -1;  // fopen failed

//   Signal *buf[BUF_SIZE];
//   fread(buf, BUF_SIZE, sizeof(Signal), fd);

//   fclose(fd);
// }

void Signal_Free(Signal *signal) {
  free(signal->samples);
  free(signal);
}