#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_

#include <stdio.h>

#include "Signal.h"

void File_ReadSignal(Signal *signal, char *filename);
void File_WriteSignal(Signal *signal, char *filename);
void File_WriteComplexSignal(ComplexSignal *signal, char *filename);

#endif  // FILE_UTILS_H_