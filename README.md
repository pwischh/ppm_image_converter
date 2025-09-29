# ppm_image_converter (`ppmcvt`)

**Course:** CS350 — Systems Programming  
A compact C program that reads a PPM (Portable Pixel Map) and writes a transformed image (PBM/PGM/PPM) using one of several transformations. 

---

## Features (one transformation allowed per run)
**Usage**: `ppmcvt [ -b | -g <PGMMax> | -i <channel> | -r <channel> | -s | -m | -t <scale> | -n <scale> ] -o <outfile> <infile>`
- **`-b`**: Convert PPM → PBM (bitmap) — default  
- **`-g <1-65535>`**: Convert PPM → PGM (grayscale) with specified max  
- **`-i red|green|blue`**: Isolate a color channel  
- **`-r red|green|blue`**: Remove a color channel  
- **`-s`**: Sepia  
- **`-m`**: Mirror left half onto right half  
- **`-t <1-8>`**: Thumbnail scale down by integer factor  
- **`-n <1-8>`**: N-up tiling (tile n 1/n thumbnails; output same size)
- **`-o <outfile>`**: Required output filename (will be overwritten)
- **`<infile>`**: Required input PPM file\
*Only one transformation flag (-b, -g, -i, -r, -s, -m, -t, -n) may be used per invocation.*

---

## Examples
```console
# PBM (default)
./ppmcvt -o out.pbm in.ppm

# 16-level grayscale
./ppmcvt -g 16 -o out.pgm in.ppm

# Sepia
./ppmcvt -s -o sepia.ppm in.ppm

# 1/4 thumbnail
./ppmcvt -t 4 -o thumb.ppm in.ppm

# Tile 4 quarter-sized thumbnails
./ppmcvt -n 4 -o nup.ppm in.ppm

# Isolate green channel
./ppmcvt -i green -o green_only.ppm in.ppm
```

## Attribution
Some files included with the assignment (notably the PBM library and the lab spec) are property of Dorian Arnold / Emory University and are not original work by the repository author (e.g., `pbm.c`, `pbm.h`)
