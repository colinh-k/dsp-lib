#include "Signal.h"
#include "SignalGenerator.h"
#include <stdint.h>
#include <stdio.h>
#include <memory.h>

#define _USE_MATH_DEFINES  // for math constants (like M_PI)
#include <math.h>

// sr: sample rate, fq: freqency of sin, ph: phase shift of sin, n: number of samples
Signal* SigGen_Sinusoid(sinusoid_fn fn, double fq, double amp, double ph, uint32_t n, uint32_t sr) {
  Signal *s_out = Signal_Create(n);

  for (uint32_t i = 0; i < n; i++) {
    double theta = 2.0 * M_PI * fq * (double) i / (double) sr + ph;
    s_out->samples[i] = amp * fn(theta);
  }

  return s_out;
}

Signal* SigGen_SinusoidSynth(size_t n_sigs, Signal **sigs) {
  Signal *s_out = Signal_Create(sigs[0]->size);
  memset(s_out->samples, 0, sigs[0]->size * sizeof(Sample));

  for (uint32_t i = 0; i < n_sigs; i++) {
    for (uint32_t j = 0; j < sigs[0]->size; j++) {
      s_out->samples[j] += sigs[i]->samples[j];
    }
  }

  return s_out;
}
