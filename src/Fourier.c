#include <math.h>
#include <memory.h>
#include <stdio.h>

#include "Signal.h"
#include "Fourier.h"
#include "SignalGenerator.h"

// NOTE: trig functions are calculated by C in radians.

// takes a signal, calculates its Discrete Fourier Transform by correlation, then
//  returns the real and imaginary signals in rectangular form.
FDSignal_Rect* DFT_Correlation(Signal *s_in) {
  FDSignal_Rect *fd_sig = malloc(sizeof(FDSignal_Rect));

  // size of frequency domain signals
  fd_sig->size = s_in->size / 2.0 + 1.0;

  // TODO: error checking:
  fd_sig->real = Signal_Create(fd_sig->size);
  fd_sig->img = Signal_Create(fd_sig->size);

  // zero out the output signals to prepare for accumulation
  memset(fd_sig->real->samples, 0, fd_sig->size * sizeof(Sample));
  memset(fd_sig->img->samples, 0, fd_sig->size * sizeof(Sample));

  for (int32_t i = 0; i < fd_sig->size; i++) {
    for (int32_t j = 0; j < s_in->size; j++) {
      fd_sig->real->samples[i] += s_in->samples[j] * cos(2 * M_PI * (double) i * (double) j / s_in->size);
      // printf("real: %lf\n", fd_sig->real->samples[i]);
      fd_sig->img->samples[i] -= s_in->samples[j] * sin(2 * M_PI * (double) i * (double) j / s_in->size);
      // printf("imag: %lf\n", fd_sig->img->samples[i]);
    }
  }

  return fd_sig;
}

// takes the real and imaginary parts of a signal and calculates
//  the inverse Discrete Fourier Transform by correlation, returning the
//  original signal as a new signal. this algorithm is slow
Signal* DFT_Correlation_Inverse(FDSignal_Rect *fd_rect) {
  // set the size of the original signal
  size_t s_out_size = fd_rect->size * 2.0 - 2.0;

  Signal *s_out = Signal_Create(s_out_size);

  // adjust the amplitudes in the frequency domain
  for (uint32_t i = 0; i < fd_rect->size; i++) {
    fd_rect->real->samples[i] /= (s_out->size / 2.0);
    fd_rect->img->samples[i] /= - (s_out->size / 2.0);
  }

  // correct the first and last real signal components
  // TODO: account for there being no samples in the real signal at idx 0:
  fd_rect->real->samples[0] /= 2.0;
  fd_rect->real->samples[fd_rect->real->size - 1] /= 2.0;

  // zero out the output signal to prepare for synthesis
  memset(s_out->samples, 0, s_out->size * sizeof(Sample));

  // synthesize the frequency domain signals
  for (uint32_t i = 0; i < fd_rect->size; i++) {
    for (uint32_t j = 0; j < s_out->size; j++) {
      s_out->samples[j] += fd_rect->real->samples[i] * cos(2 * M_PI * (double) i * (double) j / s_out->size);
      s_out->samples[j] += fd_rect->img->samples[i] * sin(2 * M_PI * (double) i * (double) j / s_out->size);
    }
  }

  return s_out;
}

void FDSignal_RectToPolar(FDSignal_Rect *fd_rect, FD_Signal_Polar *fd_polar) {
  for (int32_t i = 0; i < fd_rect->size; i++) {
    fd_polar->mag->samples[i] = sqrt(pow(fd_rect->real->samples[i], 2) + pow(fd_rect->img->samples[i], 2));

    // guard against 0 in the denominator. this happens when the real
    //  sample is 0, but the imaginary sample is not. this indicates
    //  a cooresponding phase value of +-90 degrees exactly.
    if (fd_rect->real->samples[i] == 0) {
      // if (fd_rect->img->samples[i] > 0) {
      //   fd_polar->phase->samples[i] = M_PI / 2;
      // } else if (fd_rect->img->samples[i] < 0) {
      //   fd_polar->phase->samples[i] = - M_PI / 2;
      // }
      fd_rect->real->samples[i] = 1e-20;
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

void FDSignal_PolarToRect(FD_Signal_Polar *fd_polar, FDSignal_Rect *fd_rect) {
  for (int32_t i = 0; i < fd_polar->size; i++) {
    fd_rect->real->samples[i] = fd_polar->mag->samples[i] * cos(fd_polar->phase->samples[i]);
    fd_rect->img->samples[i] = fd_polar->mag->samples[i] * sin(fd_polar->phase->samples[i]);
  }
}

// void Unwrap_Phase(FD_Signal_Polar *fd_polar) {
//   Signal *phase = fd_polar->phase;
//   // the first sample is 0, both for the original and unwrapped signals
//   for (int32_t i = 1; i < fd_polar->size; i++) {
//     int32_t factor = lround((phase->samples[i] - phase->samples[i]) / (2 * M_PI));
//   }
// }

// converts the given frequency domain components (rectangular) to
//  their signal/waveform representations, i.e., the real part
//  is converted to cosine waves, and the imaginary part
//  is converted to sine waves
// returns an array of pointers to allocated signals for
//  cos and sin through the output params
// only converts up to n_waves each of the real and imaginary components
// void FDSignal_RectToWaveform(FDSignal_Rect *fd_rect, Signal **re_waves, Signal **im_waves, uint32_t n_waves) {
//   // if the given number of waves to convert is out of bounds, just convert all components.
//   n_waves = (n_waves >= 0 && n_waves <= fd_rect->size) ? n_waves : fd_rect->size;
//   printf("%d\n", n_waves);
//   // allocate space for the output arrays
//   *re_waves = malloc(n_waves * sizeof(Signal));
//   *im_waves = malloc(n_waves * sizeof(Signal));

//   for (uint32_t i = 0; i < n_waves; i++) {
//     // Signal *re_wave = Signal_Create(fd_rect->size * 2 - 2);
//     // re_waves[i] = SigGen_Cos(fd_rect->size * 2 - 2, i, fd_rect->real->samples[i], 0, fd_rect->size);
//     // im_waves[i] = SigGen_Sin(fd_rect->size * 2 - 2, i, fd_rect->real->samples[i], 0, fd_rect->size);
//     re_waves[i] = SigGen_Sinusoid(&cos, (double) i, fd_rect->real->samples[i], 0, fd_rect->size * 2 - 2, fd_rect->size * 2 - 2);
//     im_waves[i] = SigGen_Sinusoid(&sin, (double) i, fd_rect->img->samples[i], 0, fd_rect->size * 2 - 2, fd_rect->size * 2 - 2);
//   }
// }

// for efficiency, this function modifies the signal 'in place'.
//  upon return, the signal contains the real and imaginary
//  components of the original signal's frequency domain 
void FFT(ComplexSignal *sig) {
  // constants / aliases
  Sample *real = sig->real;
  Sample *imag = sig->imag;
  size_t n = sig->size;
  size_t n_m1 = n - 1;
  size_t n_d2 = n / 2;
  uint32_t n_stages = log(n) / log(2);
  int32_t p = n_d2;

  // Time Domain Decomposition Phase:
  // algorithm for re-arranging the samples via 'bit reversing'.
  for (uint32_t i = 1; i <= n - 2; i++) {
    if (i < p) {
      // swap real and imag samples
      Sample r_tmp = real[p];
      Sample i_tmp = imag[p];

      real[p] = real[i];
      imag[p] = imag[i];

      real[i] = r_tmp;
      imag[i] = i_tmp;
    }

    int32_t k = n_d2;

    while (k <= p) {
      p = p - k;
      k = k / 2;
    }

    p += k;
  }

  // Frequency Domain Synthesis Phase:
  // iterate for each stage to reconstruct the frequency domain signal
  for (uint32_t i = 1; i <= n_stages; i++) {
    // number of samples in current stage
    uint32_t n_in_stage = pow(2, i);
    uint32_t n_in_stage_d2 = n_in_stage / 2;

    double u_r = 1;
    double u_i = 0;

    Sample s_r = cos(M_PI / n_in_stage_d2);
    Sample s_i = - sin(M_PI / n_in_stage_d2);

    for (uint32_t j = 1; j <= n_in_stage_d2; j++) {
      uint32_t j_m1 = j - 1;
      for (uint32_t k = j_m1; k <= n_m1; k += n_in_stage) {
        uint32_t idx = k + n_in_stage_d2;

        // butterfly calculation:
        Sample r_tmp = real[idx] * u_r - imag[idx] * u_i;
        Sample i_tmp = real[idx] * u_i - imag[idx] * u_r;

        real[idx] = real[k] - r_tmp;
        imag[idx] = imag[k] - i_tmp;
        real[k] = real[k] + r_tmp;
        imag[k] = imag[k] + i_tmp;
      }

      Sample r_tmp = u_r;
      u_r = r_tmp * s_r - u_i * s_i;
      u_i = r_tmp * s_i + u_i * s_r;
    }
  }
}

void FFT_Inverse(ComplexSignal *sig) {
  // aliases
  size_t n = sig->size;
  Sample *real = sig->real;
  Sample *imag = sig->imag;

  for (uint32_t i = 0; i < sig->size; i++) {
    imag[i] = - imag[i];
  }

  FFT(sig);

  for (uint32_t i = 0; i < sig->size; i++) {
    real[i] = real[i] / ((Sample) n);
    imag[i] = -imag[i] / ((Sample) n);
  }
}
