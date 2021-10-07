#ifndef SIGNAL_GENERATOR_H_
#define SIGNAL_GENERATOR_H_

#include "Signal.h"

typedef double (*sinusoid_fn) (double);

// returns a Signal containing samples taken at the given
//  frequency from the given sinusoid function wave with
//  the given parameters.
// fn - sinusoidal function to sample (sin or cos)
// sr - sampling rate
// fq - sine frequency
// amp - sine amplitude
// ph - sine phase
// n - number of samples
Signal* SigGen_Sinusoid(sinusoid_fn fn, double fq, double amp, double ph, uint32_t n, uint32_t sr);

// returns a new signal that is the sum of each signal in the given array of the given size.
//  all signals must be the same length
Signal* SigGen_SinusoidSynth(size_t n_sigs, Signal **sigs);

Signal* SigGen_Square(double fq, double amp, uint32_t n, uint32_t sr);

#endif // SIGNAL_GENERATOR_H_
