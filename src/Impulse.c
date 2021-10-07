#include <memory.h>

#include "Signal.h"
#include "Impulse.h"

Signal* Impulse_Convolve(Signal *s_in, Signal *s_ir) {
  Signal *s_out = Signal_Create(s_in->size + s_ir->size - 1);
  // aliases for the output, input, and impulse response signals
  Sample *out = s_out->samples;
  Sample *in = s_in->samples;
  Sample *ir = s_ir->samples;

  memset(out, 0, s_out->size * sizeof(Sample));

  for (uint32_t i = 0; i < s_in->size; i++) {
    for (uint32_t j = 0; j < s_ir->size; j++) {
      out[i + j] += in[i] * ir[j];
    }
  }

  return s_out;
}