/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR,
CODE WRITTEN BY OTHER STUDENTS, OR CODE DERIVED FROM AN AI TOOL - PARKER WISCHHOVER
*/

#include "pbm.h"
#include <stdlib.h>

PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m )
{
    PPMImage *img = (PPMImage *) malloc(sizeof(PPMImage));
    img->height = h;
    img->width = w;
    img->max = m;

    for (int color = 0; color < 3; color++) {
        img->pixmap[color] = malloc(h * sizeof(unsigned int *));

        for (unsigned int row = 0; row < h; row++) {
            img->pixmap[color][row] = malloc(w * sizeof(unsigned int));
        }
    }

    return img;
}

PBMImage * new_pbmimage( unsigned int w, unsigned int h )
{
    PBMImage *img = (PBMImage *) malloc(sizeof(PBMImage));
    img->height = h;
    img->width = w;

    img->pixmap = malloc(h * sizeof(unsigned int *));
    for (unsigned int row = 0; row < h; row++) {
        img->pixmap[row] =  malloc(w * sizeof(unsigned int));
    }

    return img;
}

PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m )
{
    PGMImage *img = (PGMImage *) malloc(sizeof(PGMImage));
    img->height = h;
    img->width = w;
    img->max = m;

    img->pixmap = malloc(h * sizeof(unsigned int *));
    for (unsigned int row = 0; row < h; row++) {
        img->pixmap[row] =  malloc(w * sizeof(unsigned int));
    }

    return img;
}

void del_ppmimage( PPMImage * p )
{
    for (int color = 0; color < 3; color++) {
        for (unsigned int row = 0; row < p->height; row++) {
            free(p->pixmap[color][row]);
        }
        free(p->pixmap[color]);
    }
    free(p);
}

void del_pgmimage( PGMImage * p )
{
    for (unsigned int row = 0; row < p->height; row++) {
        free(p->pixmap[row]);
    }
    free(p->pixmap);
    free(p);
}

void del_pbmimage( PBMImage * p )
{
    for (unsigned int row = 0; row < p->height; row++) {
        free(p->pixmap[row]);
    }
    free(p->pixmap);
    free(p);
}

