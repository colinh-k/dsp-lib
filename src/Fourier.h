#ifndef FOURIER_H_
#define FOURIER_H_

#include "Signal.h"

// rectangular coordinates for a frequency domain signal pair
typedef struct {
  size_t size;  // number of samples in the real/img signals
  Signal *real;
  Signal *img;
} FD_Signal_Rect;

// polar coordinates for a frequency domain signal pair
typedef struct {
  size_t size;
  Signal *mag;
  Signal *phase;  // the angle between the positive x axis and the segment
} FD_Signal_Polar;

// takes a signal, calculates its Discrete Fourier Transform, then
//  returns the real and imaginary signals through the output parameter.
void DFT_Correlation(Signal *s_in, FD_Signal_Rect *fd_sig);

// takes the real and imaginary parts of a signal and calculates
//  the inverse Discrete Fourier Transform, returning the
//  signal through the output parameter. this algorithm is slow
void DFT_Correlation_Inverse(FD_Signal_Rect *fd_rect, Signal *s_out);

// converts the given rectangular frequency domain signal into polar domain
void FD_Signal_Rect_To_Polar(FD_Signal_Rect *fd_rect, FD_Signal_Polar *fd_polar);
void FD_Signal_Polar_To_Rect(FD_Signal_Polar *fd_polar, FD_Signal_Rect *fd_rect);

// 
void Unwrap_Phase(FD_Signal_Polar *fd_polar);

void FD_RectToWaveform(FD_Signal_Rect *fd_rect, Signal **re_waves, Signal **im_waves);

#endif  // FOURIER_H_