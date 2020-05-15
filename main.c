#include <stdio.h>
#include <stdlib.h>
#include <glut/glut.h>

#include "pic.h"
#include "LUT.h"
#include "LINFIL.h"
#include "NLFIL.h"

#define FRAME_W 1000
#define FRAME_H 1000

static unsigned char Frame[FRAME_H][FRAME_W][3];

extern FILTER Filters[];
extern const int NumOfFilters;
PIC pic_negative;
PIC m, *pics, robertson, sobel, med, kuwahara;

void PutPixel( int x, int y, unsigned char B, unsigned char G, unsigned char R)
{
    if(x < 0 || y < 0 || x >= FRAME_W || y >= FRAME_H)
        return;
    Frame[y][x][0] = B;
    Frame[y][x][1] = G;
    Frame[y][x][2] = R;
}

void PicDraw( PIC * P, int X0, int Y0 )
{
    int x, y;
    for(y = 0; y < P->H; y++)
        for(x = 0; x <  P->W; x++)
            PutPixel(X0 + x, Y0 + y,P->Pixels[x + (P->H-y) * P->W][2],P->Pixels[x + (P->H-y) * P->W][1],P->Pixels[x + (P->H-y) * P->W][0]);
}

void Keyboard(unsigned char Key, int x, int y)
{
    if(Key == 27)
        exit(0);
    else if(Key == 'f')
        glutFullScreen();
}

static int x = 0, y = 0, h = 0;
void AutoDraw( PIC *P )
{

    if (P == NULL)
    {
        x = 0;
        y += h;
        return;
    }
    h = P->H;
    PicDraw(P, x, y);
    //PutStr(x + 8, y + P->H, Text);
    x += P->W;
    if (x > FRAME_W - P->W)
    {
        x = 0;
        y += P->H;
    }
}

void reshape(int h, int w) {
    glutPostRedisplay();
}

void Display() {
    x = 0;
    y = 0;
    h = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.7, 1, 0.5, 0);

    AutoDraw(&m);
    //AutoDraw(&pic_negative);
    AutoDraw(&kuwahara);
//    AutoDraw(&robertson);
//    AutoDraw(&sobel);
//    AutoDraw(&med);

    glDrawPixels(FRAME_W, FRAME_H, GL_RGB, GL_UNSIGNED_BYTE,  Frame);

    glPixelZoom(5, 5);
    glutSwapBuffers();
    //glutPostRedisplay();
    glFlush();
}

int main ( int argc, char ** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(FRAME_W, FRAME_H);
    glutCreateWindow("dermo");
    glutReshapeFunc(reshape);
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);


    pics = malloc(sizeof(PIC) * NumOfFilters);


    PicLoad(&m, "/Users/markporosin/CLionProjects/ClassworkOpenGL/m.G24");

//    for (int i = 0; i < NumOfFilters; ++i) {
//        PicCreate(pics+i, m.W, m.H);
//        PicFilter(pics+i, &m, Filters+i);
//    }
//
//    PicCreate(&robertson, m.W, m.H);
//    PicFilterRoberts(&robertson, &m, 3);
//
//    PicCreate(&sobel, m.W, m.H);
//    PicFilterSobel(&sobel, &m, 1);
//
//    PicCreate(&med, m.W, m.H);
//    PicFilterMed(&med, &m, 4);

    PicCreate(&kuwahara, m.W, m.H);
    PicFilterKuwahara(&kuwahara, &m, 20);

    byte negative[256];
    LUTSetNegative(negative);
    PicCreate(&pic_negative, m.W, m.H);
    LUTApply(&pic_negative, &m, negative);

    PicSave(&pic_negative, "/Users/markporosin/CLionProjects/ClassworkOpenGL/NegativeMonkey.G24");

    glutMainLoop();

    PicFree(&m);
    return 0;
}
