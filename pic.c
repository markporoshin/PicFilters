//
// Created by Марк Порошин on 08.04.2020.
//

#include <stdlib.h>
#include <stdio.h>
#include "pic.h"


int PicLoad( PIC*P, char*FileName )
{
    short w, h; //поменять на int
    FILE * F ;
    P->W = P->H = 0;
    P->Pixels= NULL;
    F = fopen(FileName,"rb");
    fread(&h, 2, 1, F);
    fread(&w, 2, 1, F);
    P->H = h;
    P->W = w;
    if(!PicCreate(P, w, h))
    {
        fclose(F);
        return 0;
    }
    fread(P->Pixels, 3, w * h, F);
    fclose(F);
    return 1;
}


int PicGet( PIC*P, int X, int Y, int C)
{
    if (X < 0 || Y < 0 || X >= P->W || Y >= P->H)
        return 0;
    return P->Pixels[X + Y * P->W][C];
}


void PicFree( PIC*P)
{
    if(P->Pixels != NULL)
        free(P->Pixels);
    P->W = P->H = 0;
    P->Pixels = NULL;
}


int PicCreate( PIC*P, int NewW, int NewH) {
    P->Pixels = malloc(sizeof(RGB) * NewH * NewW);
    P->H = NewH;
    P->W = NewW;
    if (P->Pixels == NULL)
        return 0;
    return 1;
}

