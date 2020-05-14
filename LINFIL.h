//
// Created by Марк Порошин on 26.04.2020.
//

#ifndef CLASSWORKOPENGL_LINFIL_H
#define CLASSWORKOPENGL_LINFIL_H

#include "pic.h"

typedef struct
{
    char *Name;
    int A[3][3], Frac, Bias;
} FILTER;

void PicFilter( PIC *PDest, PIC *PSrc, FILTER *F );

#endif //CLASSWORKOPENGL_LINFIL_H
