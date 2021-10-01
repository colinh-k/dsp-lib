#ifndef SIGNAL_GENERATOR_H_
#define SIGNAL_GENERATOR_H_

#include "Signal.h"

// returns a Signal containing samples taken at the given
//  frequency from a sine wave with the given parameters
// sf - sampling frequency
// freq - sine frequency
// amp - sine amplitude
// phase - sine phase
// n - number of samples
Signal* SigGen_Sin(uint32_t sf, uint32_t freq, uint32_t amp,
                        uint32_t phase, uint32_t n);

Signal* SigGen_Cos(uint32_t sf, uint32_t freq, uint32_t amp,
                        uint32_t phase, uint32_t n);

// returns a Signal containing samples taken at the given
//  frequency from a sine wave with the given parameters
// sf - sampling frequency
// freq - sine frequency
// amp - sine amplitude
// phase - sine phase
// n - number of samples
Signal* SigGen_SinSum();

#endif // SIGNAL_GENERATOR_H_
