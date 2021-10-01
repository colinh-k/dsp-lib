#include "Signal.h"
#include <stdint.h>

#define _USE_MATH_DEFINES  // for math constants (like M_PI)
#include <math.h>

Signal* SigGen_Sin(uint32_t sf, uint32_t freq, uint32_t amp,
                        uint32_t phase, uint32_t n) {
  Sample t = 2.0 * M_PI * freq / sf;  // 
  Sample s_cos = amp * cos(phase);  // sample from cosine
  Sample s_sin = amp * sin(phase);  // sample from sine

  Sample cs = cos(t);
  Sample sn = sin(t);

  Signal *s_out = Signal_Create(n);

  for (uint32_t i = 1; i < n; i++) {
    t = s_cos;
    s_cos = cs * s_cos - sn * s_sin;
    s_sin = sn * t + cs * s_sin;
    s_out->samples[i] = s_sin;
  }

  return s_out;
}

Signal* SigGen_Cos(uint32_t sf, uint32_t freq, uint32_t amp,
                        uint32_t phase, uint32_t n) {
  Sample t = 2.0 * M_PI * freq / sf;  // 
  Sample s_cos = amp * cos(phase);  // sample from cosine
  Sample s_sin = amp * sin(phase);  // sample from sine

  Sample cs = cos(t);
  Sample sn = sin(t);

  Signal *s_out = Signal_Create(n);

  for (uint32_t i = 1; i < n; i++) {
    t = s_cos;
    s_cos = cs * s_cos - sn * s_sin;
    s_sin = sn * t + cs * s_sin;
    s_out->samples[i] = s_cos;
  }

  return s_out;
}