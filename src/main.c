#define _USE_MATH_DEFINES  // for math constants (like M_PI)
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include "Signal.h"
#include "SignalGenerator.h"
#include "Fourier.h"
#include "FileUtils.h"

// tests the dft with a sum of sin waves. outputs the frequency domain
//  cos and sin waves to .sig files
void TestDFT_Sinusoid(void);
// does the same as TestDFT_Sinusoid, except with a square wave
void TestDFT_Square(void);
// writes each cos and sin wave from the given frequency 
//  domain signal to files named "[1-9]+[real|imag].sig"
void WriteFDWaves(FDSignal_Rect *fd, Signal *og);

int main(int argc, char const *argv[]) {
  TestDFT_Sinusoid();
  // TestDFT_Square();

  return EXIT_SUCCESS;
}

void TestDFT_Sinusoid(void) {
  // double fq, double amp, double ph, uint32_t n, uint32_t sr
  Signal *sig1 = SigGen_Sinusoid(&sin, 10, 0.25, 0, 300, 1000);
  Signal *sig2 = SigGen_Sinusoid(&sin, 5, 0.5, 0, 300, 1000);
  Signal *sig3 = SigGen_Sinusoid(&sin, 15, 0.5, 0, 300, 1000);
  Signal *sigs[3] = {sig1, sig2, sig3};

  Signal *sig = SigGen_SinusoidSynth(3, sigs);
  File_WriteSignal(sig, "og_signal.sig");

  FDSignal_Rect *fd = DFT_Correlation(sig);
  WriteFDWaves(fd, sig);

  Signal *sig_inv = DFT_Correlation_Inverse(fd);
  File_WriteSignal(sig_inv, "inverse_signal.sig");

  Signal_Free(sig);
  Signal_Free(sig1);
  Signal_Free(sig2);
  Signal_Free(sig3);
  Signal_Free(sig_inv);
  
  free(fd);
}

void TestDFT_Square(void) {
  Signal *square = SigGen_Square(10, 1, 300, 1000);
  File_WriteSignal(square, "square.sig");

  FDSignal_Rect *fd_square = DFT_Correlation(square);
  WriteFDWaves(fd_square, square);

  Signal *inv_square = DFT_Correlation_Inverse(fd_square);
  File_WriteSignal(inv_square, "square_inv.sig");

  Signal_Free(square);
  Signal_Free(inv_square);
  free(fd_square);
}

void WriteFDWaves(FDSignal_Rect *fd, Signal *og) {
  size_t s_size = fd->size * 2 - 2;

  Signal *build = Signal_Create(s_size);
  memset(build->samples, 0, build->size * sizeof(Sample));

  // copy the frequency domain signals
  Signal *r_cpy = Signal_Create(fd->size);
  Signal *i_cpy = Signal_Create(fd->size);
  for (uint32_t i = 0; i < fd->size; i++) {
    r_cpy->samples[i] = fd->real->samples[i];
    i_cpy->samples[i] = fd->img->samples[i];
  }

  for (uint32_t i = 0; i < fd->size; i++) {
    r_cpy->samples[i] /= (s_size / 2.0);
    i_cpy->samples[i] /= - (s_size / 2.0);
  }

  // correct the first and last real signal components
  r_cpy->samples[0] /= 2.0;
  r_cpy->samples[r_cpy->size - 1] /= 2.0;

  // Sample r_amp = fd->real->samples[i] / (s_size / 2.0);
  // Sample i_amp = - fd->img->samples[i] / (s_size / 2.0);

  // if (i == 0 || i == fd->real->size - 1) {
  //   r_amp /= 2.0;
  // }

  // generate samples from the real and imaginary components from
  //  the dft signal and output them as cos and sin waves to .sig files
  for (uint32_t i = 0; i < fd->size; i++) {
    char name[16];

    sprintf(name, "%dreal.sig", i);
    Signal *real = SigGen_Sinusoid(&cos, (double) i, r_cpy->samples[i], 0, fd->size * 2 - 2, fd->size * 2 - 2);
    File_WriteSignal(real, name);

    Signal *imag = SigGen_Sinusoid(&sin, (double) i, i_cpy->samples[i], 0, fd->size * 2 - 2, fd->size * 2 - 2);
    sprintf(name, "%dimag.sig", i);
    File_WriteSignal(imag, name);

    Signal *sum = SigGen_SinusoidSynth(3, (Signal*[3]) {real, imag, build});
    Signal_Free(build);
    build = sum;

    Signal_Free(real);
    Signal_Free(imag);
  }

  for (uint32_t i = 0; i < build->size; i++) {
    printf("%lf\t%lf\n", build->samples[i], og->samples[i]);
  }
}
