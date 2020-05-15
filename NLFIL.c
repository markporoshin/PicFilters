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


int Mean(int * array, int size) {
    int i, sum = 0;
    for (i = 0; i < size; ++i) {
        sum += array[i];
    }
    return sum / size;
}

int Var(int * array, int size, int mean) {
    int i, sum = 0;
    for (i = 0; i < size; ++i) {
        sum += (array[i] - mean) * (array[i] - mean);
    }
    return sum / (size - 1);
}

void setArray(PIC * p, int * array, int r, int x, int y, int c)
{
    int size = r * r;
    int k = 0, i, j;
    for (i = x; i < x + r; ++i) {
        for (j = y; j < y + r; ++j) {
            int px = getPoint(p->W, i);
            int py = getPoint(p->H, j);
            array[k] = p->Pixels[px + py * p->W][c];
            k++;
        }
    }
}

void PicFilterKuwahara( PIC *PDest, PIC *PSrc, int R ) {
    int i, j, c, x, y;
    int mean[4], var[4];
    int x_offset, y_offset;
    for (c = 0; c < 3; c++)
    {
        for (i = 0; i < PSrc->H; i++)
        {
            for (j = 0; j < PSrc->W; j++)
            {
                int * array = malloc(sizeof(int) * R * R);

                setArray(PSrc, array, R, j - R, i - R, c);
                mean[0] = Mean(array, R * R);
                var[0] = Var(array, R * R, mean[0]);

                setArray(PSrc, array, R, j - R, i - 1, c);
                mean[1] = Mean(array, R * R);
                var[1] = Var(array, R * R, mean[1]);

                setArray(PSrc, array, R, j + 1, i - R, c);
                mean[2] = Mean(array, R * R);
                var[2] = Var(array, R * R, mean[2]);

                setArray(PSrc, array, R, j + 1, i + 1, c);
                mean[3] = Mean(array, R * R);
                var[3] = Var(array, R * R, mean[3]);

                int min = 0;
                for (int k = 0; k < 4; ++k) {
                    if (var[k] < var[min]) {
                        min = k;
                    }
                }

                PDest->Pixels[j + i * PDest->W][c] = mean[min];
                free(array);
            }
        }
    }
}