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

  // Signal *sig = SigGen_Sinusoid(&sin, 10, 1, 0, 330, 1000);
  File_WriteSignal(sig, "og_signal.sig");

  FDSignal_Rect *fd = DFT_Correlation(sig);

  // Signal *s_build = Signal_Create(300);
  // memset(s_build->samples, 0, s_build->size * sizeof(Sample));

  // adjust the amplitudes in the frequency domain
  for (uint32_t i = 0; i < fd->size; i++) {
    fd->real->samples[i] /= (sig->size / 2.0);
    fd->img->samples[i] /= - (sig->size / 2.0);
  }

  // correct the first and last real signal components
  fd->real->samples[0] /= 2.0;
  fd->real->samples[fd->real->size - 1] /= 2.0;

  // generate samples from the real and imaginary components from
  //  the dft signal and output them as cos and sin waves to .sig files
  for (uint32_t i = 0; i < fd->size; i++) {
    char name[16];

    sprintf(name, "%dreal.sig", i);
    Signal *real = SigGen_Sinusoid(&cos, (double) i, fd->real->samples[i], 0, fd->size * 2 - 2, fd->size * 2 - 2);
    File_WriteSignal(real, name);

    Signal *imag = SigGen_Sinusoid(&sin, (double) i, fd->img->samples[i], 0, fd->size * 2 - 2, fd->size * 2 - 2);
    sprintf(name, "%dimag.sig", i);
    File_WriteSignal(imag, name);
  }

  Signal *sig_inv = DFT_Correlation_Inverse(fd);
  File_WriteSignal(sig_inv, "inverse_signal.sig");

  Signal_Free(sig);
  Signal_Free(sig1);
  Signal_Free(sig2);
  Signal_Free(sig3);
  Signal_Free(sig_inv);
  // Signal_Free(s_build);
  
  free(fd);
}

void TestDFT_Square(void) {
  Signal *square = SigGen_Square(10, 1, 300, 1000);
  File_WriteSignal(square, "square.sig");

  Signal *dft_square = DFT_Correlation(square);

}

// writes each cos and sin wave from the given frequency 
//  domain signal to files named "[1-9]+[real|imag].sig"
void WriteFDWaves(FDSignal_Rect *fd) {

}
