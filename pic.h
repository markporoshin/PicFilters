//
// Created by Марк Порошин on 08.04.2020.
//

#ifndef CLASSWORKOPENGL_PIC_H
#define CLASSWORKOPENGL_PIC_H

typedef unsigned char byte;
typedef byte RGB[3];
typedef struct {
    int H, W;
    RGB * Pixels;
} PIC;


void PutPixel( int x, int y, byte r, byte g, byte b );
int PicCreate( PIC*P, int NewW, int NewH);
int PicSave( PIC*P, char*FileName );
int PicLoad( PIC*P, char*FileName );
void PicFree( PIC *P );
void PicDraw( PIC *P, int X0, int Y0 );




#endif //CLASSWORKOPENGL_PIC_H
