#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "Signal.h"
#include "Fourier.h"

#define SIZE 16

int main(int argc, char const *argv[]) {
  Signal *sig = Signal_Create(SIZE);
  if (!sig)
    return EXIT_FAILURE;

  for (int i = 0; i < SIZE; i++) {
    sig->samples[i] = (double) i / SIZE;
    printf("%lf\n", sig->samples[i]);
  }

  FD_Signal_Rect fd;
  DFT_Correlation(sig, &fd);
  DFT_Correlation_Inverse(&fd, sig);

  printf("\n\nbreak\n\n");

  for (int i = 0; i < SIZE; i++) {
    sig->samples[i] = (double) i / SIZE;
    printf("%lf\n", sig->samples[i]);
  }

  return EXIT_SUCCESS;
}
