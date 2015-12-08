/*
*  Copyright (c) 1999 O'Reilly & Associates, Inc.
*
*  Mastering Algorithms with C
*  Kyle Loudon
*
*  Permission is hereby granted, free of charge, to any person obtaining a
*  copy of this software and associated documentation files (the "Software"),
*  to deal in the Software without restriction, including without limitation
*  the rights to use, copy, modify, merge, publish, distribute, sublicense,
*  and/or sell copies of the Software, and to permit persons to whom the
*  Software is furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. IN NO EVENT SHALL
*  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*  FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE, OR ANY OTHER
*  DEALINGS IN THE SOFTWARE.
*/

/*****************************************************************************
*                                                                            *
*  --------------------------------- bit.c --------------------------------  *
*                                                                            *
*****************************************************************************/

#include <string.h>

#include "../include/bit.h"

/*****************************************************************************
*                                                                            *
*  -------------------------------- bit_get -------------------------------  *
*                                                                            *
*****************************************************************************/

int bit_get(const unsigned char *bits, int pos) {

unsigned char      mask;

int                i;

/*****************************************************************************
*                                                                            *
*  Set a mask for the bit to get.                                            *
*                                                                            *
*****************************************************************************/

mask = 0x80;

for (i = 0; i < (pos % 8); i++)
   mask = mask >> 1;

/*****************************************************************************
*                                                                            *
*  Get the bit.                                                              *
*                                                                            *
*****************************************************************************/

return (((mask & bits[(int)(pos / 8)]) == mask) ? 1 : 0);

}

/*****************************************************************************
*                                                                            *
*  -------------------------------- bit_set -------------------------------  *
*                                                                            *
*****************************************************************************/

void bit_set(unsigned char *bits, int pos, int state) {

unsigned char      mask;

int                i;

/*****************************************************************************
*                                                                            *
*  Set a mask for the bit to set.                                            *
*                                                                            *
*****************************************************************************/

mask = 0x80;

for (i = 0; i < (pos % 8); i++)
   mask = mask >> 1;

/*****************************************************************************
*                                                                            *
*  Set the bit.                                                              *
*                                                                            *
*****************************************************************************/

if (state)
   bits[pos / 8] = bits[pos / 8] | mask;
else
   bits[pos / 8] = bits[pos / 8] & (~mask);

return;

}

/*****************************************************************************
*                                                                            *
*  -------------------------------- bit_xor -------------------------------  *
*                                                                            *
*****************************************************************************/

void bit_xor(const unsigned char *bits1, const unsigned char *bits2, unsigned
   char *bitsx, int size) {

int                i;

/*****************************************************************************
*                                                                            *
*  Compute the bitwise XOR (exclusive OR) of the two buffers.                *
*                                                                            *
*****************************************************************************/

for (i = 0; i < size; i++) {

   if (bit_get(bits1, i) != bit_get(bits2, i))
      bit_set(bitsx, i, 1);
   else
      bit_set(bitsx, i, 0);

}

return;

}

/*****************************************************************************
*                                                                            *
*  ----------------------------- bit_rot_left -----------------------------  *
*                                                                            *
*****************************************************************************/

void bit_rot_left(unsigned char *bits, int size, int count) {

int                fbit,
                   lbit,
                   i,
                   j;

/*****************************************************************************
*                                                                            *
*  Rotate the buffer to the left the specified number of bits.               *
*                                                                            *
*****************************************************************************/

if (size > 0) {

   for (j = 0; j < count; j++) {

      for (i = 0; i <= ((size - 1) / 8); i++) {

         /********************************************************************
         *                                                                   *
         *  Get the bit about to be shifted off the current byte.            *
         *                                                                   *
         ********************************************************************/

         lbit = bit_get(&bits[i], 0);

         if (i == 0) {

            /*****************************************************************
            *                                                                *
            *  Save the bit shifted off the first byte for later.            *
            *                                                                *
            *****************************************************************/

            fbit = lbit;

            }

         else {

            /*****************************************************************
            *                                                                *
            *  Set the rightmost bit of the previous byte to the leftmost    *
            *  bit about to be shifted off the current byte.                 *
            *                                                                *
            *****************************************************************/

            bit_set(&bits[i - 1], 7, lbit);

         }

         /********************************************************************
         *                                                                   *
         *  Shift the current byte to the left.                              *
         *                                                                   *
         ********************************************************************/

         bits[i] = bits[i] << 1;

      }

      /***********************************************************************
      *                                                                      *
      *  Set the rightmost bit of the buffer to the bit shifted off the      *
      *  first byte.                                                         *
      *                                                                      *
      ***********************************************************************/

      bit_set(bits, size - 1, fbit);

   }

}

return;

}
