/* ----------------------------------------------------------------- */
/*             The Speech Signal Processing Toolkit (SPTK)           */
/*             developed by SPTK Working Group                       */
/*             http://sp-tk.sourceforge.net/                         */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 1984-2007  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/*                1996-2014  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the SPTK working group nor the names of its */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/********************************************************
 $Id: _fftr.c,v 1.17 2014/12/11 08:30:35 uratec Exp $                     
 
 NAME:                     
        fftr - Fast Fourier Transform for Double sequence      
 SYNOPSIS:                                             
        int   fftr(x, y, m)            
                     
        double  x[];   real part of data      
        double  y[];   working area         
        int     m;     number of data(radix 2)      
                Naohiro Isshiki    Dec.1995   modified
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif
*/

#include "../../lib/getmem.pub.h"

#define PI 3.14159265358979323846

static double *_sintbl = 0;
static int maxfftsize = 0;

static int fft(double *x, double *y, const int m);

int sptk_fftr(double *x, double *y, const int m)
{
   int i, j;
   double *xp, *yp, *xq;
   double *yq;
   int mv2, n, tblsize;
   double xt, yt, *sinp, *cosp;
   double arg;

   mv2 = m / 2;

   /* separate even and odd  */
   xq = xp = x;
   yp = y;
   for (i = mv2; --i >= 0;) {
      *xp++ = *xq++;
      *yp++ = *xq++;
   }

   if (fft(x, y, mv2) == -1)    /* m / 2 point fft */
      return (-1);


   /***********************
   * SIN table generation *
   ***********************/

   if ((_sintbl == 0) || (maxfftsize < m)) {
      tblsize = m - m / 4 + 1;
      arg = PI / m * 2;
      if (_sintbl != 0)
         free(_sintbl);
      _sintbl = sinp = dgetmem(tblsize);
      *sinp++ = 0;
      for (j = 1; j < tblsize; j++)
         *sinp++ = sin(arg * (double) j);
      _sintbl[m / 2] = 0;
      maxfftsize = m;
   }

   n = maxfftsize / m;
   sinp = _sintbl;
   cosp = _sintbl + maxfftsize / 4;

   xp = x;
   yp = y;
   xq = xp + m;
   yq = yp + m;
   *(xp + mv2) = *xp - *yp;
   *xp = *xp + *yp;
   *(yp + mv2) = *yp = 0;

   for (i = mv2, j = mv2 - 2; --i; j -= 2) {
      ++xp;
      ++yp;
      sinp += n;
      cosp += n;
      yt = *yp + *(yp + j);
      xt = *xp - *(xp + j);
      *(--xq) = (*xp + *(xp + j) + *cosp * yt - *sinp * xt) * 0.5;
      *(--yq) = (*(yp + j) - *yp + *sinp * yt + *cosp * xt) * 0.5;
   }

   xp = x + 1;
   yp = y + 1;
   xq = x + m;
   yq = y + m;

   for (i = mv2; --i;) {
      *xp++ = *(--xq);
      *yp++ = -(*(--yq));
   }

   return (0);
}

/********************************************************
   $Id: _fft.c,v 1.17 2014/12/11 08:30:34 uratec Exp $               
       NAME:               
                fft - fast fourier transform    
       SYNOPSIS:               
                int   fft(x, y, m);         
                     
                double   x[];   real part      
                double   y[];   imaginary part      
                int      m;     data size      
   
                return : success = 0
                         fault   = -1
       Naohiro Isshiki          Dec.1995    modified   
********************************************************/

static int checkm(const int m)
{
   int k;

   for (k = 4; k <= m; k <<= 1) {
      if (k == m)
         return (0);
   }
   fprintf(stderr, "fft : m must be a integer of power of 2!\n");

   return (-1);
}

static int fft(double *x, double *y, const int m)
{
   int j, lmx, li;
   double *xp, *yp;
   double *sinp, *cosp;
   int lf, lix, tblsize;
   int mv2, mm1;
   double t1, t2;
   double arg;
   int checkm(const int);

   /**************
   * RADIX-2 FFT *
   **************/

   if (checkm(m))
      return (-1);

   /***********************
   * SIN table generation *
   ***********************/

   if ((_sintbl == 0) || (maxfftsize < m)) {
      tblsize = m - m / 4 + 1;
      arg = PI / m * 2;
      if (_sintbl != 0)
         free(_sintbl);
      _sintbl = sinp = dgetmem(tblsize);
      *sinp++ = 0;
      for (j = 1; j < tblsize; j++)
         *sinp++ = sin(arg * (double) j);
      _sintbl[m / 2] = 0;
      maxfftsize = m;
   }

   lf = maxfftsize / m;
   lmx = m;

   for (;;) {
      lix = lmx;
      lmx /= 2;
      if (lmx <= 1)
         break;
      sinp = _sintbl;
      cosp = _sintbl + maxfftsize / 4;
      for (j = 0; j < lmx; j++) {
         xp = &x[j];
         yp = &y[j];
         for (li = lix; li <= m; li += lix) {
            t1 = *(xp) - *(xp + lmx);
            t2 = *(yp) - *(yp + lmx);
            *(xp) += *(xp + lmx);
            *(yp) += *(yp + lmx);
            *(xp + lmx) = *cosp * t1 + *sinp * t2;
            *(yp + lmx) = *cosp * t2 - *sinp * t1;
            xp += lix;
            yp += lix;
         }
         sinp += lf;
         cosp += lf;
      }
      lf += lf;
   }

   xp = x;
   yp = y;
   for (li = m / 2; li--; xp += 2, yp += 2) {
      t1 = *(xp) - *(xp + 1);
      t2 = *(yp) - *(yp + 1);
      *(xp) += *(xp + 1);
      *(yp) += *(yp + 1);
      *(xp + 1) = t1;
      *(yp + 1) = t2;
   }

   /***************
   * bit reversal *
   ***************/
   j = 0;
   xp = x;
   yp = y;
   mv2 = m / 2;
   mm1 = m - 1;
   for (lmx = 0; lmx < mm1; lmx++) {
      if ((li = lmx - j) < 0) {
         t1 = *(xp);
         t2 = *(yp);
         *(xp) = *(xp + li);
         *(yp) = *(yp + li);
         *(xp + li) = t1;
         *(yp + li) = t2;
      }
      li = mv2;
      while (li <= j) {
         j -= li;
         li /= 2;
      }
      j += li;
      xp = x + j;
      yp = y + j;
   }

   return (0);
}
