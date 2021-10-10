#include <stdio.h>
#include <memory.h>

#include "Signal.h"

#define BUF_SIZE 1028  // 4096

Signal* Signal_Create(size_t s_size) {
  Signal *signal = malloc(sizeof(Signal));
  if (signal == NULL)
    return NULL;  // malloc failed

  signal->samples = malloc(s_size * sizeof(Sample));
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

ComplexSignal* ComplexSignal_Create(size_t s_size) {
  ComplexSignal *c_signal = malloc(sizeof(ComplexSignal));
  if (c_signal == NULL)
    return NULL;  // malloc failed

  c_signal->real = malloc(s_size * sizeof(Sample));
  c_signal->imag = malloc(s_size * sizeof(Sample));
  if (c_signal->real == NULL || c_signal->imag == NULL)
    return NULL;  // malloc failed
  
  c_signal->size = s_size;
  return c_signal;
}
void ComplexSignal_Free(ComplexSignal *signal) {
  free(signal->real);
  free(signal->imag);
  free(signal);
}

// returns a complex signal composed of the real samples
//  in the given signal and all complex values equal to 0.
ComplexSignal* Signal_ToComplexSignal(Signal *signal) {
  ComplexSignal *c_sig = ComplexSignal_Create(signal->size);

  // copy over the real part of the signal
  memcpy(c_sig->real, signal->samples, signal->size);
  // initialize the imaginary part to all 0's
  memset(c_sig->imag, 0, c_sig->size * sizeof(Sample));

  return c_sig;
}