#ifndef FOURIER_H_
#define FOURIER_H_

#include "Signal.h"

// rectangular coordinates for a frequency domain signal pair
typedef struct {
  size_t size;  // number of samples in the real/img signals
  Signal *real;
  Signal *img;
} FDSignal_Rect;

// polar coordinates for a frequency domain signal pair
typedef struct {
  size_t size;
  Signal *mag;
  Signal *phase;  // the angle between the positive x axis and the segment
} FD_Signal_Polar;

// takes a signal, calculates its Discrete Fourier Transform, then
//  returns the real and imaginary signals through the output parameter.
FDSignal_Rect* DFT_Correlation(Signal *s_in);

// takes the real and imaginary parts of a signal and calculates
//  the inverse Discrete Fourier Transform, returning the
//  signal through the output parameter. this algorithm is slow
Signal* DFT_Correlation_Inverse(FDSignal_Rect *fd_rect);

// converts the given rectangular frequency domain signal into polar domain
void FDSignal_RectToPolar(FDSignal_Rect *fd_rect, FD_Signal_Polar *fd_polar);
void FDSignal_PolarToRect(FD_Signal_Polar *fd_polar, FDSignal_Rect *fd_rect);

// void Unwrap_Phase(FD_Signal_Polar *fd_polar);

// void FDSignal_RectToWaveform(FDSignal_Rect *fd_rect, Signal **re_waves, Signal **im_waves, uint32_t n_waves);

#endif  // FOURIER_H_