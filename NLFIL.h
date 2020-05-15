//
// Created by Марк Порошин on 12.05.2020.
//


#include "pic.h"

#ifndef CLASSWORKOPENGL_NLFIL_H
#define CLASSWORKOPENGL_NLFIL_H


void PicFilterRoberts( PIC *PDest, PIC *PSrc, double Alpha );
void PicFilterSobel(PIC *PDest, PIC *PSrc, double Alpha );
void PicFilterMed( PIC *PDest, PIC *PSrc, int N );
void PicFilterKuwahara( PIC *PDest, PIC *PSrc, int R );

#endif //CLASSWORKOPENGL_NLFIL_H
