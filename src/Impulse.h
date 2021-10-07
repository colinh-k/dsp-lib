#ifndef IMPULSE_H_
#define IMPULSE_H_

#include <memory.h>

#include "Signal.h"

// contains misc. utility functions for signal manipulation

// produces an output signal that is the given input signal convolved
//  with the given impulse response (IR).
// s_in - the input signal
// s_ir - the impulse response
Signal* Impulse_Convolve(Signal *s_in, Signal *s_ir);

#endif  // IMPULSE_H_
