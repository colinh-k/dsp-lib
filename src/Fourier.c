#include "Signal.h"
#include "Fourier.h"
#include <math.h>
#include <memory.h>
// #include <stdio.h>

// NOTE: trig functions are calculated by C in radians.

// takes a signal, calculates its Discrete Fourier Transform by correlation, then
//  returns the real and imaginary signals through the output parameter.
void DFT_Correlation(Signal *s_in, FD_Signal_Rect *fd_sig) {
  // size of frequency domain signals
  fd_sig->size = s_in->size / 2 + 1;

  // TODO: error checking:
  fd_sig->real = Signal_Create(fd_sig->size);
  fd_sig->img = Signal_Create(fd_sig->size);

  // zero out the output signals to prepare for accumulation
  memset(fd_sig->real->samples, 0, fd_sig->size * sizeof(Sample));
  memset(fd_sig->img->samples, 0, fd_sig->size * sizeof(Sample));

  for (int32_t i = 0; i < fd_sig->size; i++) {
    for (int32_t j = 0; j < fd_sig->size; j++) {
      fd_sig->real->samples[i] += s_in->samples[j] * cos(2 * M_PI * i * j / s_in->size);
      fd_sig->img->samples[i] += s_in->samples[j] * sin(2 * M_PI * i * j / s_in->size);
    }
  }
}

// takes the real and imaginary parts of a signal and calculates
//  the inverse Discrete Fourier Transform by correlation, returning the
//  signal through the output parameter. this algorithm is slow (?)
void DFT_Correlation_Inverse(FD_Signal_Rect *fd_rect, Signal *s_out) {
  // set the size of the original signal
  s_out->size = fd_rect->size * 2 - 2;

  for (uint32_t i = 0; i < fd_rect->size; i++) {
    fd_rect->real->samples[i] /= s_out->size / 2;  // == <...> - 1
    fd_rect->img->samples[i] /= - s_out->size / 2;
  }

  // correct the first and last real signal components
  // TODO: account for there being no samples in the real signal at idx 0:
  fd_rect->real->samples[0] /= 2;
  fd_rect->real->samples[fd_rect->real->size - 1] /= 2;

  // zero out the output signal to prepare for synthesis
  memset(s_out->samples, 0, s_out->size * sizeof(Sample));

  // synthesize the frequency domain signals
  for (uint32_t i = 0; i < fd_rect->size; i++) {
    for (uint32_t j = 0; j < s_out->size; j++) {
      s_out->samples[j] += fd_rect->real->samples[i] * cos(2 * M_PI * i * j / s_out->size);
      s_out->samples[j] += fd_rect->img->samples[i] * sin(2 * M_PI * i * j / s_out->size);
    }
  }
}

void FD_Signal_Rect_To_Polar(FD_Signal_Rect *fd_rect, FD_Signal_Polar *fd_polar) {
  for (int32_t i = 0; i < fd_rect->size; i++) {
    fd_polar->mag->samples[i] = sqrt(pow(fd_rect->real->samples[i], 2) + pow(fd_rect->img->samples[i], 2));

    // guard against 0 in the denominator. this happens when the real
    //  sample is 0, but the imaginary sample is not. this indicates
    //  a cooresponding phase value of +-90 degrees exactly.
    if (fd_rect->real->samples[i] == 0) {
      if (fd_rect->img->samples[i] > 0) {
        fd_polar->phase->samples[i] = M_PI / 2;
      } else if (fd_rect->img->samples[i] < 0) {
        fd_polar->phase->samples[i] = - M_PI / 2;
      }
      // fd_rect->real->samples[i] = 1e-20;
    }

    fd_polar->phase->samples[i] = atan(fd_rect->img->samples[i] / fd_rect->real->samples[i]);

    // adjust arctan calculation whenver the real part is negative
    //  (see vector diagram for real to polar conversion for intuition)
    if (fd_rect->real->samples[i] < 0 && fd_rect->img->samples[i] < 0)
      fd_polar->phase->samples[i] -= M_PI;
    if (fd_rect->real->samples[i] < 0 && fd_rect->img->samples[i] >= 0)
      fd_polar->phase->samples[i] += M_PI;
  }
}

void FD_Signal_Polar_To_Rect(FD_Signal_Polar *fd_polar, FD_Signal_Rect *fd_rect) {
  for (int32_t i = 0; i < fd_polar->size; i++) {
    fd_rect->real->samples[i] = fd_polar->mag->samples[i] * cos(fd_polar->phase->samples[i]);
    fd_rect->img->samples[i] = fd_polar->mag->samples[i] * sin(fd_polar->phase->samples[i]);
  }
}

void Unwrap_Phase(FD_Signal_Polar *fd_polar) {
  Signal *phase = fd_polar->phase;
  // the first sample is 0, both for the original and unwrapped signal
  for (int32_t i = 1; i < fd_polar->size; i++) {
    int32_t factor = roundtol((phase->samples[i] - phase->samples[i]) / (2 * M_PI));
  }
}
