//
// Created by Марк Порошин on 12.05.2020.
//

#include "NLFIL.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>




static int getPoint( int axis, int value )
{
    if ( value >= 0 && value < axis )
        return value;
    else if ( value < 0 )
    {
        return axis + value;
    }
    else
    {
        return value - axis;
    }
}

void PicFilterRoberts( PIC *PDest, PIC *PSrc, double Alpha )
{
    int i, j;
    int c = 0;
    for (c = 0; c < 3; ++c)
    {
        for (i = 0; i < PSrc->H ; ++i)
        {
            for (j = 0; j < PSrc->W ; ++j)
            {
                int bj = getPoint(PSrc->W, j + 1);
                int ci = getPoint(PSrc->H, i + 1);
                int A = PSrc->Pixels[i * PDest->W + j][c];

                int B = PSrc->Pixels[i * PDest->W + bj][c];
                int C = PSrc->Pixels[(ci) * PDest->W + j][c];
                int D = PSrc->Pixels[(ci) * PDest->W + (bj)][c];

                PDest->Pixels[i * PDest->W + j][c] = Alpha * (abs(A - D) + abs(B - C));

            }
        }
    }
}
void PicFilterSobel(PIC *PDest, PIC *PSrc, double Alpha )
{
    int i, j, f, g;
    int c = 0;
    for (c = 0; c < 3; ++c)
    {
        for (i = 0; i < PSrc->H ; ++i)
        {
            for (j = 0; j < PSrc->W ; ++j)
            {
                int bj = getPoint(PSrc->W, j + 1);
                int ci = getPoint(PSrc->H, i + 1);
                int aj = getPoint(PSrc->W, j - 1);
                int di = getPoint(PSrc->H, i - 1);

                int A = PSrc->Pixels[(di) * PDest->W + aj][c];
                int B = PSrc->Pixels[i * PDest->W + aj][c];
                int C = PSrc->Pixels[(ci) * PDest->W + (aj)][c];
                int E = PSrc->Pixels[i * PDest->W + bj][c];
                int H = PSrc->Pixels[(ci) * PDest->W + j][c];
                int G = PSrc->Pixels[(ci) * PDest->W + (bj)][c];
                int D = PSrc->Pixels[(di) * PDest->W + j][c];
                int F = PSrc->Pixels[(di) * PDest->W + bj][c];

                f = (A + 2 * B + C) - (F + 2 * G + H);
                g = (A + 2 * D + F) - (C + 2 * E + H);

                PDest->Pixels[i * PDest->W + j][c] = Alpha * sqrt(f * f + g * g);

            }
        }
    }
}

void BubleSort(int *list, int n)
{
    int c, d, t;

    for (c = 0 ; c < n - 1; c++) {
        for (d = 0 ; d < n - c - 1; d++) {
            if (list[d] > list[d+1]) {
                /* Swapping */
                t         = list[d];
                list[d]   = list[d+1];
                list[d+1] = t;
            }
        }
    }
}

void QuickSort(int *s_arr, int first, int last)
{
    if (first < last)
    {
        int left = first, right = last, middle = s_arr[(left + right) / 2];
        do
        {
            while (s_arr[left] < middle) left++;
            while (s_arr[right] > middle) right--;
            if (left <= right)
            {
                int buf = s_arr[left];
                s_arr[left] = s_arr[right];
                s_arr[right] = buf;
                left++;
                right--;
            }
        } while (left <= right);
        QuickSort(s_arr, first, right);
        QuickSort(s_arr, left, last);
    }
}


void PicFilterMed( PIC *PDest, PIC *PSrc, int N )
{
    int c, i, j;
    int x_offset;
    int * array = malloc(sizeof(int) * N * N);
    printf("N*N%d\n", N*N);
    for (c = 0; c < 3; c++)
    {
        for (i = 0; i < PSrc->H; i++)
        {
            for (j = 0; j < PSrc->W; j++)
            {
                int array_cursor = 0;
                for (x_offset = -N / 2; x_offset < N - N / 2; ++x_offset)
                {
                    for (int y_offset = -N / 2; y_offset < N - N / 2; ++y_offset)
                    {
                        int x = j + x_offset;
                        int y = i + y_offset;

                        if (x < 0 || y < 0 || x >= PSrc->W || y >= PSrc->H)
                            continue;

                        array[array_cursor] = PSrc->Pixels[y * PSrc->W + x][c];
                        array_cursor++;
                    }
                }
                printf("array_cursor%d\n", array_cursor);
                //BubleSort(array, array_cursor);
                QuickSort(array,0, array_cursor);
                PDest->Pixels[j + i * PDest->W][c] = array[array_cursor / 2];
            }
        }
    }

    free(array);
}