#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <stdint.h>
#include <stdlib.h>  // for size_t

// defines the type of a sample, which should be able
//  to hold fractional values
typedef double Sample;

typedef struct {
  size_t size;  // the size of the samples array
  // the rate at which the samples were taken
  uint32_t sample_rate;
  Sample *samples;  // the array of samples defining the signal
} Signal;

// a signal composed of complex numbers
typedef struct {
  size_t size;  // both real and imag are the same size
  Sample *real;  // real part of the signal
  Sample *imag;  // imaginary part of the signal
} ComplexSignal;

// allocates space for a signal of the given size and initializes its
//  size field. returns NULL on failure, or a pointer to the allocated
//  signal struct otherwise
Signal* Signal_Create(size_t s_size);

// loads as many samples into the signal as will fit from the given
//  file. the expected filetype is .csv containing only float values.
//  returns -1 on failure, else returns the number of
//  samples loaded into the signal
int8_t Signal_Load(Signal *signal, char *filename);

// free the allocated signal. 'signal' is not longer a valid
//  pointer when this function returns.
void Signal_Free(Signal *signal);

ComplexSignal* ComplexSignal_Create(size_t s_size);
void ComplexSignal_Free(ComplexSignal *signal);

// returns a complex signal composed of the real samples
//  in the given signal and all complex values equal to 0.
ComplexSignal* Signal_ToComplexSignal(Signal *signal);

#endif  // SIGNAL_H_
