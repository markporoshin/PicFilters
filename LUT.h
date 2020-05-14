//
// Created by Марк Порошин on 22.04.2020.
//

#ifndef CLASSWORKOPENGL_LUT_H
#define CLASSWORKOPENGL_LUT_H

#include "pic.h"

void LUTSetNegative( byte *  LUT );
void LUTApply(PIC * PicDest, PIC * PicSrc, byte * LUT);
void LutClamp(byte * LUT, int A);
void LUTContrast(byte * LUT, int A, int B);

#endif //CLASSWORKOPENGL_LUT_H
