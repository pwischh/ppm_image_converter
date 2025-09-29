/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING A TUTOR,
CODE WRITTEN BY OTHER STUDENTS, OR CODE DERIVED FROM AN AI TOOL - PARKER WISCHHOVER
*/

#include "pbm.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum Transformation {bitmap, grayscale, isolate, remove_channel, sepia, mirror, thumbnail, tile};

unsigned int clamp(unsigned int val, unsigned int max) {
    return val > max ? max : val;
}

void bitmapTransformation(PPMImage *in, PBMImage *out) {
    for (unsigned int row = 0; row < in->height; row++) {
        for (unsigned int col = 0; col < in->width; col++) {
            unsigned int redPixel = in->pixmap[0][row][col];
            unsigned int greenPixel = in->pixmap[1][row][col];
            unsigned int bluePixel = in->pixmap[2][row][col];

            double average = ((double)(redPixel + greenPixel + bluePixel))/3.0;
            out->pixmap[row][col] = (average < (in->max/2.0)) ? 1 : 0;
        }
    }
}

void grayscaleTransformation(PPMImage *in, PGMImage *out) {
    for (unsigned int row = 0; row < in->height; row++) {
        for (unsigned int col = 0; col < in->width; col++) {
            unsigned int redPixel = in->pixmap[0][row][col];
            unsigned int greenPixel = in->pixmap[1][row][col];
            unsigned int bluePixel = in->pixmap[2][row][col];

            double average = ((double)(redPixel + greenPixel + bluePixel))/3.0;
            unsigned int grayscaleVal = (unsigned int)((average/(double)in->max) * out->max);
            out->pixmap[row][col] = grayscaleVal;
        }
    }
}

void isolateTransformation(PPMImage *in, PPMImage *out, char *channel) {
    int zeroChannel1 = -1;
    int zeroChannel2 = -1;
    int isolatedChannel = -1;

    if (strcmp(channel, "red") == 0) {
        isolatedChannel = 0;
        zeroChannel1 = 1;
        zeroChannel2 = 2;
    }
    else if (strcmp(channel, "green") == 0) {
        zeroChannel1 = 0;
        isolatedChannel = 1;
        zeroChannel2 = 2;
    }
    else {
        zeroChannel1 = 0;
        zeroChannel2 = 1;
        isolatedChannel = 2;
    }

    for (unsigned int row = 0; row < in->height; row++) {
        for (unsigned int col = 0; col < in->width; col++) {
            out->pixmap[zeroChannel1][row][col] = 0;
            out->pixmap[zeroChannel2][row][col] = 0;
            out->pixmap[isolatedChannel][row][col] = in->pixmap[isolatedChannel][row][col];
        }
    }
}

void removeTransformation(PPMImage *in, PPMImage *out, char *channel) {
    int zeroChannel = -1;
    int nonzeroChannel1 = -1;
    int nonzeroChannel2 = -1;

    if (strcmp(channel, "red") == 0) {
        zeroChannel = 0;
        nonzeroChannel1 = 1;
        nonzeroChannel2 = 2;
    }
    else if (strcmp(channel, "green") == 0) {
        nonzeroChannel1 = 0;
        zeroChannel = 1;
        nonzeroChannel2 = 2;
    }
    else {
        nonzeroChannel1 = 0;
        nonzeroChannel2 = 1;
        zeroChannel = 2;
    }

    for (unsigned int row = 0; row < in->height; row++) {
        for (unsigned int col = 0; col < in->width; col++) {
            out->pixmap[zeroChannel][row][col] = 0;
            out->pixmap[nonzeroChannel1][row][col] = in->pixmap[nonzeroChannel1][row][col];
            out->pixmap[nonzeroChannel2][row][col] = in->pixmap[nonzeroChannel2][row][col];
        }
    }
}

void sepiaTransformation(PPMImage *in, PPMImage *out) {
    for (unsigned int row = 0; row < in->height; row++) {
        for (unsigned int col = 0; col < in->width; col++) {
            unsigned int red = in->pixmap[0][row][col];
            unsigned int green = in->pixmap[1][row][col];
            unsigned int blue = in->pixmap[2][row][col];

            unsigned int newRed = (unsigned int)((0.393 * red) + (0.769 * green) + (0.189 * blue));
            unsigned int newGreen = (unsigned int)((0.349 * red) + (0.686 * green) + (0.168 * blue));
            unsigned int newBlue = (unsigned int)((0.272 * red) + (0.534 * green) + (0.131 * blue));
        
            out->pixmap[0][row][col] = clamp(newRed, in->max);
            out->pixmap[1][row][col] = clamp(newGreen, in->max);
            out->pixmap[2][row][col] = clamp(newBlue, in->max);
        }
    }
}

void mirrorTransformation(PPMImage *in, PPMImage *out) {
    int w = in->width;
    for (unsigned int row = 0; row < in->height; row++) {
        for (unsigned int col = 0; col < w/2; col++) {
            out->pixmap[0][row][col] = in->pixmap[0][row][col];
            out->pixmap[1][row][col] = in->pixmap[1][row][col];
            out->pixmap[2][row][col] = in->pixmap[2][row][col]; 

            out->pixmap[0][row][w - col - 1] = in->pixmap[0][row][col];
            out->pixmap[1][row][w - col - 1] = in->pixmap[1][row][col];
            out->pixmap[2][row][w - col - 1] = in->pixmap[2][row][col];
        }
    }
}

void thumbnailTransformation(PPMImage *in, PPMImage *out, unsigned int scaleFactor) {
    for (unsigned int row = 0; row < out->height; row++) {
        for (unsigned int col = 0; col < out->width; col++) {
            out->pixmap[0][row][col] = in->pixmap[0][row * scaleFactor][col * scaleFactor];
            out->pixmap[1][row][col] = in->pixmap[1][row * scaleFactor][col * scaleFactor];
            out->pixmap[2][row][col] = in->pixmap[2][row * scaleFactor][col * scaleFactor];
        }
    }
}

void tileTransformation(PPMImage *thumbnail, PPMImage *out, unsigned int scaleFactor) {
    for (unsigned int row = 0; row < out->height; row++) {
        for (unsigned int col = 0; col < out->width; col++) {
            unsigned int thumbnailRow = row % thumbnail->height;
            unsigned int thumbnailCol = col % thumbnail->width;

            out->pixmap[0][row][col] = thumbnail->pixmap[0][thumbnailRow][thumbnailCol];
            out->pixmap[1][row][col] = thumbnail->pixmap[1][thumbnailRow][thumbnailCol];
            out->pixmap[2][row][col] = thumbnail->pixmap[2][thumbnailRow][thumbnailCol];
        }
    }
}

int main( int argc, char *argv[] )
{
    int opt;

    char* outputName = NULL;
    char* optionVal = NULL;
    unsigned int pgmMax = 0;
    unsigned int scaleFactor = 0;

    enum Transformation transformation = bitmap;
    int optionCount = 0;

    while ((opt = getopt(argc, argv, "bg:i:r:smt:n:o:")) != -1) {
        switch (opt) {
            case 'b':
                optionCount++;
                break;
            case 'g':
                transformation = grayscale;
                pgmMax = (unsigned int) strtol(optarg, NULL, 10);
                optionVal = optarg;
                optionCount++;
                break;
            case 'i':
                transformation = isolate;
                optionVal = optarg;
                optionCount++;
                break;
            case 'r':
                transformation = remove_channel;
                optionVal = optarg;
                optionCount++;
                break;
            case 's':
                transformation = sepia;
                optionCount++;
                break;
            case 'm':
                transformation = mirror;
                optionCount++;
                break;
            case 't':
                transformation = thumbnail;
                scaleFactor = (unsigned int) strtol(optarg, NULL, 10);
                optionCount++;
                break;
            case 'n':
                transformation = tile;
                scaleFactor = (unsigned int) strtol(optarg, NULL, 10);
                optionCount++;
                break;
            case 'o':
                outputName = optarg;
                break;
            default:
                fprintf(stderr, "Usage: ppmcvt [-bgirsmtno] [FILE]\n");
                exit(1);
        }
    }

    if (optionCount > 1) {
        fprintf(stderr, "Error: Multiple transformations specified\n");
        exit(1);
    }

    if (!outputName) {
        fprintf(stderr, "Error: No output file specified\n");
        exit(1);
    }

    if (transformation == isolate || transformation == remove_channel) {
        if (strcmp(optionVal, "red") != 0 && strcmp(optionVal, "blue") != 0 && strcmp(optionVal, "green") != 0) {
            fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green' or 'blue'\n", optionVal);
            exit(1);
        }
    }

    if (transformation == grayscale) {
        if (pgmMax > 65535) {
            fprintf(stderr, "Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n", optionVal);
            exit(1);
        }
    }

    if (transformation == thumbnail || transformation == tile) {
        if (scaleFactor < 1 || scaleFactor > 8) {
            fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", scaleFactor);
            exit(1);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Error: No input file specified\n");
        exit(1);
    }

    char *inputName = argv[optind];
    PPMImage *in = read_ppmfile(inputName);

    switch (transformation) {
        case bitmap: {
            PBMImage *out = new_pbmimage(in->width, in->height);
            bitmapTransformation(in, out);
            write_pbmfile(out, outputName);
            del_pbmimage(out);
            break;
        }
        case grayscale: {
            PGMImage *out = new_pgmimage(in->width, in->height, pgmMax);
            grayscaleTransformation(in, out);
            write_pgmfile(out, outputName);
            del_pgmimage(out);
            break;
        }
        case isolate: {
            PPMImage *out = new_ppmimage(in->width, in->height, in->max);
            isolateTransformation(in, out, optionVal);
            write_ppmfile(out, outputName);
            del_ppmimage(out);
            break;
        }
        case remove_channel: {
            PPMImage *out = new_ppmimage(in->width, in->height, in->max);
            removeTransformation(in, out, optionVal);
            write_ppmfile(out, outputName);
            del_ppmimage(out);
            break;
        }
        case sepia: {
            PPMImage *out = new_ppmimage(in->width, in->height, in->max);
            sepiaTransformation(in, out);
            write_ppmfile(out, outputName);
            del_ppmimage(out);
            break;
        }
        case mirror: {
            PPMImage *out = new_ppmimage(in->width, in->height, in->max);
            mirrorTransformation(in, out);
            write_ppmfile(out, outputName);
            del_ppmimage(out);
            break;
        }
        case thumbnail: {
            PPMImage *out = new_ppmimage(in->width/scaleFactor, in->height/scaleFactor, in->max);
            thumbnailTransformation(in, out, scaleFactor);
            write_ppmfile(out, outputName);
            del_ppmimage(out);
            break;
        }
        case tile: {
            PPMImage *thumbnail = new_ppmimage(in->width/scaleFactor, in->height/scaleFactor, in->max);
            thumbnailTransformation(in, thumbnail, scaleFactor);

            PPMImage *out = new_ppmimage(in->width, in->height, in->max);
            tileTransformation(thumbnail, out, scaleFactor);
            write_ppmfile(out, outputName);
            del_ppmimage(thumbnail);
            del_ppmimage(out);
            break;
        }
    }
    del_ppmimage(in);
    return 0;
}