//
// Created by Марк Порошин on 26.04.2020.
//

#include <stdlib.h>
#include <stdio.h>

#include "LINFIL.h"
#include "pic.h"

#define N_FILTERS (sizeof(Filters) / sizeof(Filters[0]))

FILTER Filters[] =
        {
                {
                        "Negative",
                        {
                                {0, 0, 0},
                                {0, -1, 0},
                                {0, 0, 0}
                        },
                        1,
                        255
                },
                {
                        "Copy",
                        {
                                {0, 0, 0},
                                {0, 1, 0},
                                {0, 0, 0}
                        },
                        1,
                        0
                },
                {
                        "Blur-box",
                        {
                                {1, 1, 1},
                                {1, 1, 1},
                                {1, 1, 1}
                        },
                        9,
                        0
                },
                {
                        "Small blur",
                        {
                                {0, 1, 0},
                                {1, 1, 1},
                                {0, 1, 0}
                        },
                        5,
                        0
                },
                {
                        "Gauss blur",
                        {
                                {1, 2, 1},
                                {2, 4, 2},
                                {2, 1, 2}
                        },
                        16,
                        0
                },
                {
                        "Hi-pass big",
                        {
                                {-1, -1, -1},
                                {-1, 9, -1},
                                {-1, -1, -1}
                        },
                        1,
                        0
                },
                {
                        "Hi-pass small",
                        {
                                {0, -1, 0},
                                {-1, 5, -1},
                                {0, -1, 0}
                        },
                        1,
                        0
                },
                {
                        "Grad",
                        {
                                {-1, -1, -1},
                                {-1, 8, -1},
                                {-1, -1, -1}
                        },
                        1,
                        128
                },
                {
                        "Laplass small",
                        {
                                {0, -1, 0},
                                {-1, 4, -1},
                                {0, -1, 0}
                        },
                        1,
                        128
                },
                {
                        "Small N",
                        {
                                {0, -1, 0},
                                {0, 1, 0},
                                {0, 0, 0}
                        },
                        1,
                        128
                },
                {
                        "Small NW",
                        {
                                {-1, 0, 0},
                                {0, 1, 0},
                                {0, 0, 0}
                        },
                        1,
                        128
                },
                {
                        "Small W",
                        {
                                {0, 0, 0},
                                {-1, 1, 0},
                                {0, 0, 0}
                        },
                        1,
                        128
                },
                {
                        "Small SW",
                        {
                                {0, 0, 0},
                                {0, 1, 0},
                                {-1, 0, 0}
                        },
                        1,
                        128
                },
                {
                        "Small S",
                        {
                                {0, 0, 0},
                                {0, 1, 0},
                                {0, -1, 0}
                        },
                        1,
                        128
                },
                {
                        "Small SE",
                        {
                                {0, 0, 0},
                                {0, 1, 0},
                                {0, 0, -1}
                        },
                        1,
                        128
                },
                {
                        "Small E",
                        {
                                {0, 0, 0},
                                {0, 1, -1},
                                {0, 0, 0}
                        },
                        1,
                        128
                },
                {
                        "Small NE",
                        {
                                {0, 0, -1},
                                {0, 1, 0},
                                {0, 0, 0}
                        },
                        1,
                        128
                },
                {
                        "Big NW",
                        {
                                {-1, -1, -1},
                                {-1, 2, 1},
                                {-1, 1, 1}
                        },
                        1,
                        128
                },
                {
                        "Big N",
                        {
                                {-1, -1, -1},
                                {-1, 2, -1},
                                {1, 1, 1}
                        },
                        1,
                        128
                },
                {
                        "Big W",
                        {
                                {-1, -1, 1},
                                {-1, 2, 1},
                                {-1, -1, 1}
                        },
                        1,
                        128
                },
                {
                        "Big SW",
                        {
                                {-1, 1, 1},
                                {-1, 2, 1},
                                {-1, -1, -1}
                        },
                        1,
                        128
                },
                {
                        "Big SE",
                        {
                                {-1, -1, -1},
                                {1, 2, -1},
                                {1, 1, -1}
                        },
                        1,
                        128
                },
                {
                        "Big E",
                        {
                                {1, -1, -1},
                                {1, 2, -1},
                                {1, -1, -1}
                        },
                        1,
                        128
                },
                {
                        "Big NE",
                        {
                                {-1, -1, -1},
                                {1, 2, -1},
                                {1, 1, -1}
                        },
                        1,
                        128
                }

        };
int NumOfFilters = N_FILTERS;

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

void PicFilter( PIC *PDest, PIC *PSrc, FILTER *F )
{
    int x, y, i, j;
    for ( y = 0; y < PSrc->H; ++y )
    {
        for ( x = 0; x < PSrc->W; ++x )
        {
            int c = 0;
            for ( c = 0; c < 3; ++c )
            {
                int sum = 0;
                for ( i = 0; i < 3; ++i )
                {
                    for ( j = 0; j < 3; ++j )
                    {
                        int cur_x = getPoint(PSrc->W, x + i - 1);
                        int cur_y = getPoint(PSrc->H, y + j - 1);
                        sum += PSrc->Pixels[cur_x + cur_y * PSrc->W][c] * F->A[i][j];
                    }
                }
                PDest->Pixels[x + PDest->W * y][c] = sum / F->Frac + F->Bias;
            }
        }
    }
}