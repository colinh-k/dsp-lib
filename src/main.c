#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "Signal.h"
#include "Fourier.h"
#include "FileUtils.h"
#include "SignalGenerator.h"
#include "Impulse.h"

int main(int argc, char const *argv[]) {
  Signal *s_in = Signal_Create(8);

  for (int i = 0; i < 8; i++) {
    if (i >= 3) {
      s_in->samples[i] = 5-i;
    } else 
    s_in->samples[i] = -i;
  }

  Signal *s_ir = Signal_Create(4);
  s_ir->samples[0] = 1;
  s_ir->samples[1] = -1;
  s_ir->samples[2] = -0.5;
  s_ir->samples[3] = -0.25;

  Signal *res = Impulse_Convolve(s_in, s_ir);

  File_WriteSignal(s_in, "sin.sig");
  File_WriteSignal(s_ir, "sir.sig");
  File_WriteSignal(res, "res.sig");



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
