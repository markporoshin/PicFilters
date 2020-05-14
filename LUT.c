//
// Created by Марк Порошин on 22.04.2020.
//

#include "LUT.h"


void LUTSetNegative( byte * LUT ) {
    for (int i = 0; i < 255; ++i) {
        LUT[i] = 255 - i;
    }
}


void LUTApply(PIC * PicDest, PIC * PicSrc, byte * LUT) {
    for (int y = 0; y < PicSrc->H; ++y) {
        for (int x = 0; x < PicSrc->W; ++x) {

            PicDest->Pixels[x + PicDest->W * y][0] = LUT[PicSrc->Pixels[x + PicSrc->W * y][0]];
            PicDest->Pixels[x + PicDest->W * y][1] = LUT[PicSrc->Pixels[x + PicSrc->W * y][1]];
            PicDest->Pixels[x + PicDest->W * y][2] = LUT[PicSrc->Pixels[x + PicSrc->W * y][2]];

        }
    }
}
int Clamp( int X, int A, int B )
{
    if (X < A)
        return A;
    if (X > B)
        return B;
    return X;
}
void LutClamp (byte * LUT ,int A)
{
    int i;
    for(i = 0; i < 256; i++ )
    LUT[i] = Clamp(i + A, 0, 255);
}
int Contrast(int X, int A, int B)
{
    return X *(B - A)/255 + A;
}
void LUTContrast (byte * LUT ,int A, int B)
{
    int i;
    for(i = 0; i < 256; i++ )
        LUT[i] = Contrast(i + A, 0, 255);
}