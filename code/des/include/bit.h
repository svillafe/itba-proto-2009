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
*  --------------------------------- bit.h --------------------------------  *
*                                                                            *
*****************************************************************************/

#ifndef BIT_H
#define BIT_H

/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/

int bit_get(const unsigned char *bits, int pos);

void bit_set(unsigned char *bits, int pos, int state);

void bit_xor(const unsigned char *bits1, const unsigned char *bits2, unsigned
   char *bitsx, int size);

void bit_rot_left(unsigned char *bits, int size, int count);

#endif
