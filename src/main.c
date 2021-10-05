#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#define _USE_MATH_DEFINES  // for math constants (like M_PI)
#include <math.h>

#include "Signal.h"
#include "Fourier.h"
#include "FileUtils.h"
#include "SignalGenerator.h"
#include "Impulse.h"

int main(int argc, char const *argv[]) {
  // double fq, double amp, double ph, uint32_t n, uint32_t sr
  Signal *sig = SigGen_Sinusoid(&sin, 10, 1, 0, 330, 1000);
  File_WriteSignal(sig, "sin_og.sig");

  FD_Signal_Rect fd;
  DFT_Correlation(sig, &fd);

  int k = 1;
  Signal *real1 = SigGen_Sinusoid(&cos, (double)k, fd.real->samples[k], 0, fd.size * 2 - 2, fd.size * 2 - 2);
  // fd.size * 2 - 2, 1, fd.real->samples[1], 0, fd.size);

  File_WriteSignal(real1, "real1.sig");

  Signal *sig2 = Signal_Create(330);
  DFT_Correlation_Inverse(&fd, sig2);
  File_WriteSignal(sig2, "inverse.sig");


  // Signal *sig = SigGen_Sin(128, 3, 1, 0, 64);

  // Signal *sig = Signal_Create(16);
  // memset(sig->samples, 0, 16 * sizeof(Sample));
  // sig->samples[3] = 50;
  // sig->samples[4] = -18;
  // sig->samples[5] = -9;
  // sig->samples[6] = -6;
  // sig->samples[7] = -2;
  // File_WriteSignal(sig, "og.sig");

  // FD_Signal_Rect fd;
  // DFT_Correlation(sig, &fd);

  // Signal *real1 = SigGen_Cos(fd.size * 2 - 2, 1, fd.real->samples[1], 0, fd.size);

  // uint32_t sr = 32;

  // for (uint32_t k = 0; k < 32 / 2; k++) {
  //   Signal *c = Signal_Create(32);
  //   for (uint32_t i = 0; i < 32; i++) {
  //     c->samples[i] = cos(2 * M_PI * k * i / 32);
  //   }

  //   char r_name[16];
  //   sprintf(r_name, "%dreal.sig", k);

  //   File_WriteSignal(c, r_name);
  // }

  // File_WriteSignal(real1, "real_wave.sig");

// FD_RectToWaveform(FD_Signal_Rect *fd_rect, Signal **re_waves, Signal **im_waves);
  // Signal *reals, *imgs;
  // FD_RectToWaveform(&fd, &reals, &imgs);
  // for (int i = 0; i < fd.size; i++) {
  //   char i_name[16], r_name[16];
  //   sprintf(i_name, "%dimg.sig", i);
  //   sprintf(r_name, "%dreal.sig", i);

  //   File_WriteSignal(&(imgs[i]), i_name);
  //   File_WriteSignal(&(reals[i]), r_name);
  // }

  // File_WriteSignal(fd.real, "real.sig");
  // File_WriteSignal(fd.img, "img.sig");

  // Signal *sig2 = Signal_Create(16);
  // DFT_Correlation_Inverse(&fd, sig2);
  // File_WriteSignal(sig2, "inversed.sig");

  return EXIT_SUCCESS;
}
