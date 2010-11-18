/* 
 * gif.c --
 *
 *	This file provides access to gif pixmaps.
 *
 * Copyright (c) 1993 by Sven Delmas
 * All rights reserved.
 * See the file COPYRIGHT for the copyright notes.
 *
 *
 * adapted from code by Alfred Kayser, DNPAP (the package is named xgif).
 *
 * Copyright by Alfred Kayser, DNPAP
 *
 *
 * minor pieces of code have been taken from the code published by
 *   jim frost and kirk johnson (xloadimage)
 *
 * Copyright 1989, 1990 Kirk L. Johnson
 *
 * Permission to use, copy, modify, distribute, and sell this
 * software and its documentation for any purpose is hereby granted
 * without fee, provided that the above copyright notice appear in
 * all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation. The
 * author makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express
 * or implied warranty.
 *
 * THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *
 * Copyright 1989, 1990 Jim Frost
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no representations
 * about the suitability of this software for any purpose.  It is
 * provided "as is" without express or implied warranty.
 *
 * THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
 * USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef __GNUC__
#include <string.h>
#endif

#ifndef WINNT
extern "C" {
#endif
#include <stdlib.h>
#include <stdio.h>
#ifndef __GNUC__
#include <string.h>
#endif
#include <assert.h>
#ifndef WINNT
}
#endif

#include "img.hpp"
#include "gif.hpp"

#define True 1
#define False 0

/* global vars */
int            quiet=1, local=1, DEBUG=0;

#define IMAGESEP 0x2c
#define EXTENSION 0x21
#define INTERLACEMASK 0x40
#define COLORMAPMASK 0x80
#define ADDTOPIXEL(a) if (Quick) Image[Offset++]=a; else AddToPixel(a)

#define ALLOCATED 3

static int EGApalette[16][3] = {
  {0,0,0},       {0,0,128},     {0,128,0},     {0,128,128}, 
  {128,0,0},     {128,0,128},   {128,128,0},   {200,200,200},
  {100,100,100}, {100,100,255}, {100,255,100}, {100,255,255},
  {255,100,100}, {255,100,255}, {255,255,100}, {255,255,255} };

static int BitOffset,		/* Bit Offset of next code */
    XC, YC,                     /* Output X and Y coords of current pixel */
    Offset,                     /* Offset in output array */
    Pass,			/* Used by output routine if interlaced pic */
    Width, Height,		/* image dimensions */
    BytesPerScanline,		/* Bytes per scanline in output raster */
    ColorMapSize,		/* number of colors */
    Background,			/* background color */
    NumUsed,			/* Number of colors really used */
    CodeSize,			/* Code size, read from GIF header */
    ReadMask,			/* Code AND mask for current code size */
    Interlace,
    HasColormap;

static Byte *Image;		/* The result array */
static Byte *Raster;		/* The raster data stream, unblocked */

    /* The GIF headers found in the file */
static Byte gifheader[13];
static Byte imageheader[9];
static Byte colormap[3*256];

    /* The hash table used by the decompressor */
static int  Prefix[4096];
static int  Suffix[4096];

    /* An output array used by the decompressor */
static Byte OutCode[1025];

    /* The color map, read from the GIF header */
static Byte Red[256], Green[256], Blue[256], used[256], Intensity[256];

static char id[] = "GIF";


int ReadImageData(FILE *fp)
{
/* Read the raster data.  Here we just transpose it from the GIF array
 * to the Raster array, turning it from a series of blocks into one long
 * data stream, which makes life much easier for ReadCode().
 */
  long filesize, filepos;
  int ch;
  Byte *ptr1;

  /* find the size of the file */
  filepos = ftell(fp);
  fseek(fp, 0L, 2);
  filesize = ftell(fp)-filepos;
  fseek(fp, filepos, 0);

  if (!(Raster = (Byte *) malloc(filesize)))
    return 1;

  ptr1 = Raster;
  while ((ch = getc(fp))>0)
    {
      if ((int) fread(ptr1, 1, ch, fp) < (int) ch)
        return 1;
      ptr1 += ch;
    } 
  return 0;
}


/* Fetch the next code from the raster data stream.  The codes can be
 * any length from 3 to 12 bits, packed into 8-bit Bytes, so we have to
 * maintain our location in the Raster array as a BIT Offset.  We compute
 * the Byte Offset into the raster array by dividing this by 8, pick up
 * three Bytes, compute the bit Offset into our 24-bit chunk, shift to
 * bring the desired code to the bottom, then mask it off and return it. 
 */
int ReadCode()
{
  int RawCode, ByteOffset, BitShift;

  ByteOffset = BitOffset / 8;
  BitShift = BitOffset % 8;
  BitOffset += CodeSize;
  if (BitShift+CodeSize<8)
    return (Raster[ByteOffset]>>BitShift) & ReadMask;
  else
    {
      RawCode = Raster[ByteOffset] + (0x100 * Raster[ByteOffset + 1]);
      if (BitShift+CodeSize >= 16)
        RawCode += (0x10000 * Raster[ByteOffset + 2]);
      return((RawCode>>BitShift) & ReadMask);
    }
}


void AddToPixel(Byte Index)
{
  if (YC<Height) /* Might be of importance when reading interlaced gifs */
    Image[YC*BytesPerScanline+XC] = Index;
  if (!used[Index]) { used[Index]=True; NumUsed++; }
  if (++XC == Width)
    {
      XC = 0;
      if (Interlace)
	{
          switch (Pass) 
            {
              case 0: YC += 8; if (YC >= Height) { Pass++; YC = 4; } break;
              case 1: YC += 8; if (YC >= Height) { Pass++; YC = 2; } break;
              case 2: YC += 4; if (YC >= Height) { Pass++; YC = 1; } break;
              case 3: YC += 2; break;
              default: break;
	    }
	}
      else
        YC++;
    }
}


int DecodeImage()
{
/* Start reading the raster data. First we get the intial code size
 * and compute decompressor constant values, based on this code size.
 */
  int Quick = 0,                /* True, when not interlaced and local Cmap */
    InitCodeSize = 0,		/* Starting code size, used during Clear */
    InCode = 0,			/* Value returned by ReadCode */
    MaxCode = 0,		/* limiting value for current code size */
    ClearCode = 0,		/* GIF clear code */
    EOFCode = 0,		/* GIF end-of-information code */
    CurCode = 0, OldCode = 0,	/* Decompressor variables */
    FreeCode = 0,		/* Decompressor, next free slot in hashtable */
    OutCount = 0,		/* Decompressor output 'stack count' */
    FinChar = 0,		/* Decompressor variable */
    BitMask = 0;		/* AND mask for data size */

  BitMask = ColorMapSize - 1;

  ClearCode = (1 << CodeSize);
  EOFCode = ClearCode + 1;
  FreeCode = ClearCode + 2;

/* The GIF spec has it that the code size is the code size used to
 * compute the above values is the code size given in the file, but the
 * code size used in compression/decompression is the code size given in
 * the file plus one. (thus the ++).
 */

  CodeSize++;
  InitCodeSize = CodeSize;
  MaxCode = (1 << CodeSize);
  ReadMask = MaxCode - 1;

/* Allocate the X Image */
  if (!(Image = (Byte *) malloc(Width*Height)))
    return 1;

  BytesPerScanline = Width;

/* Decompress the file, continuing until you see the GIF EOF code.
 * One obvious enhancement is to add checking for corrupt files here.
 */
  Quick = (local && !Interlace);
  Offset = 0; 
  if (DEBUG) fprintf(stderr,"Decoding...\n");
  InCode = ReadCode();
  while (InCode != EOFCode) {

/* Clear code sets everything back to its initial value, then reads the
 * immediately subsequent code as uncompressed data.
 */

    if (InCode == ClearCode) {
      CodeSize = InitCodeSize;
      MaxCode = (1 << CodeSize);
      ReadMask = MaxCode - 1;
      FreeCode = ClearCode + 2;
      CurCode = OldCode = InCode = ReadCode();
      FinChar = CurCode & BitMask;
      ADDTOPIXEL(FinChar);
    }
    else {

/* If not a clear code, then must be data: save same as CurCode */

      CurCode = InCode;

/* If greater or equal to FreeCode, not in the hash table yet;
 * repeat the last character decoded
 */

      if (CurCode >= FreeCode) {
        CurCode = OldCode;
        OutCode[OutCount++] = FinChar;
      }

/* Unless this code is raw data, pursue the chain pointed to by CurCode
 * through the hash table to its end; each code in the chain puts its
 * associated output code on the output queue.
 */

      while (CurCode > BitMask) {
        if (OutCount >= 1024) {
          fprintf(stderr,"\nCorrupt GIF file (OutCount)!\n");
          exit(1);  
        }
        OutCode[OutCount++] = Suffix[CurCode];
        CurCode = Prefix[CurCode];
      }

/* The last code in the chain is treated as raw data. */

      /* OutCode[OutCount++] = FinChar = CurCode &BitMask*/;
      FinChar = CurCode & BitMask;
      ADDTOPIXEL(FinChar);

/* Now we put the data out to the Output routine.
 * It's been stacked LIFO, so deal with it that way...  */
      while (OutCount>0)
        ADDTOPIXEL(OutCode[--OutCount]);

/* Build the hash table on-the-fly. No table is stored in the file. */

      Prefix[FreeCode] = OldCode;
      Suffix[FreeCode] = FinChar;
      OldCode = InCode;

/* Point to the next slot in the table.  If we exceed the current
 * MaxCode value, increment the code size unless it's already 12.  If it
 * is, do nothing: the next code decompressed better be CLEAR
 */

      FreeCode++;
      if (FreeCode >= MaxCode) {
        if (CodeSize < 12) {
          CodeSize++;
          MaxCode *= 2;
          ReadMask = (1 << CodeSize) - 1;
        }
      }
    }
    InCode = ReadCode();
  }
  free(Raster);
  return 0;
}


int ColorDicking()
{
  /* we've got the picture loaded, we know what colors are needed. get 'em */
  int i,j;
  register Byte *ptr;

  if (!HasColormap)
    {
      if (DEBUG) fprintf(stderr,"Using EGA palette as default\n");
      for (i=0; i<ColorMapSize; i++) {
        Red[i] = EGApalette[i&15][0];
        Green[i] = EGApalette[i&15][1];
        Blue[i] = EGApalette[i&15][2];
        used[i] = True;
      }
    }

  for (i=j=0; i<ColorMapSize; i++)
    {
      int val=(int)Red[i]+(int)Green[i]+(int)Blue[i];
      Intensity[i]=val/3;
    }

  ptr = Image;
  for (i=0; i<Height; i++)
    for (j=0; j<Width; j++,ptr++) 
      *ptr = (Byte) Intensity[*ptr];
  return 0;
}


void ReadColormap(FILE *fp)
{
  Byte *ptr=colormap;
  int i;

  if (DEBUG) fprintf(stderr,"Reading Color map...\n");
  fread(colormap, ColorMapSize, 3, fp);
  for (i = 0; i < ColorMapSize; i++) {
    Red[i] = (*ptr++);
    Green[i] = (*ptr++);
    Blue[i] = (*ptr++);
    used[i] = 0;
  }
  NumUsed=0;
}


/*****************************/
int LoadGIF(char *fname)
/*****************************/
{
  register Byte ch;
  FILE *fp;

  BitOffset = 0,		/* Bit Offset of next code */
    XC = 0, YC = 0,		/* Output X and Y coords of current pixel */
    Offset = 0,                 /* Offset in output array */
    Pass = 0;			/* Used by output routine if interlaced pic */
  ColorMapSize = 0;
  Image = NULL;

  fp = fopen(fname,"rb");

  if (!fp) return 1;

  if ( (fread(gifheader, sizeof(gifheader), 1, fp)!=1)
       || (strncmp((char *) gifheader, id, 3)!=0)) {
    fclose(fp);
    return 1;
  }
  if (strncmp((char *) gifheader+3, "87a", 3) &&
      strncmp((char *) gifheader+3, "89a", 3))
    fprintf(stderr,"Warning: %s contains unknown version %c%c%c",
            fname,gifheader[3],gifheader[4],gifheader[5]);
  HasColormap = ((gifheader[10] & COLORMAPMASK) ? True : False);
  ColorMapSize = 1 << (gifheader[10]&7)+1;

  Background = gifheader[11];		/* background color... not used. */

/* Read in global colormap. */
  if (HasColormap) ReadColormap(fp);

/* Check for image extension */
  while ((ch=getc(fp)) == EXTENSION)
    {
      getc(fp);			/* skip extension code */
      while ((ch=getc(fp))>0)
        fseek(fp, ch, 1);		/* skip it */
    }
	
  if (ch != IMAGESEP) {
    fclose(fp);
    return 1;
  }

  fread(imageheader,sizeof(imageheader),1,fp);

  Width = imageheader[4] + 0x100 * imageheader[5];
  Height = imageheader[6] + 0x100 * imageheader[7];

  if (!quiet) 
    fprintf(stderr,"%s: %dx%dx%d\n", fname, Width, Height, ColorMapSize);

  Interlace = ((imageheader[8] & INTERLACEMASK) ? True : False);

  if (imageheader[8] & COLORMAPMASK) 
    {
      HasColormap = True;
      ColorMapSize = 1 << (imageheader[8]&7)+1;
      ReadColormap(fp);
    }
  CodeSize = getc(fp); 
  if (!ReadImageData(fp)) {
    fclose(fp);
    if (DecodeImage())
      return 1;
    return 0;
  } else {
    fclose(fp);
    return 1;
  }
}

void LoadByteImageGif(ByteImage *image, char *filename)
{
  if (LoadGIF(filename)) return;
  if (ColorDicking()) return;
  image->NewBuffer(Image,Width,Height);
  Image=NULL;
}

unsigned char *LoadGifToBuffer(char *filename, int *width, int *height)
{
  if (LoadGIF(filename)) return NULL;
  if (ColorDicking()) return NULL;
  *width=Width;
  *height=Height;
  return Image;
}

WidthHeight *ReadGifSize(char *filename)
{
  register Byte ch;
  FILE *fp;

  BitOffset = 0,		/* Bit Offset of next code */
    XC = 0, YC = 0,		/* Output X and Y coords of current pixel */
    Offset = 0,                 /* Offset in output array */
    Pass = 0;			/* Used by output routine if interlaced pic */
  ColorMapSize = 0;
  Image = NULL;

  fp = fopen(filename,"rb");

  assert(fp!=NULL);

  if ( (fread(gifheader, sizeof(gifheader), 1, fp)!=1)
       || (strncmp((char *) gifheader, id, 3)!=0)) {
    fclose(fp);
    exit(1);
  }
  if (strncmp((char *) gifheader+3, "87a", 3) &&
      strncmp((char *) gifheader+3, "89a", 3))
    fprintf(stderr,"Warning: %s contains unknown version %c%c%c",
            filename,gifheader[3],gifheader[4],gifheader[5]);
  HasColormap = ((gifheader[10] & COLORMAPMASK) ? True : False);
  ColorMapSize = 1 << (gifheader[10]&7)+1;

  Background = gifheader[11];		/* background color... not used. */

/* Read in global colormap. */
  if (HasColormap) ReadColormap(fp);

/* Check for image extension */
  while ((ch=getc(fp)) == EXTENSION)
    {
      getc(fp);			/* skip extension code */
      while ((ch=getc(fp))>0)
        fseek(fp, ch, 1);		/* skip it */
    }
	
  if (ch != IMAGESEP) {
    fclose(fp);
    exit(1);
  }

  fread(imageheader,sizeof(imageheader),1,fp);

  WidthHeight *wh=new WidthHeight;
  wh->width = imageheader[4] + 0x100 * imageheader[5];
  wh->height = imageheader[6] + 0x100 * imageheader[7];
  fclose(fp);
  return wh;
}

