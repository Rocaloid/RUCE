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

/****************************************************************
  $Id: _nrand.c,v 1.16 2014/12/11 08:30:43 uratec Exp $

    Generate Normal Distributed Random Value
        nrand(p, leng, seed)

        double *p     :  generated normal distributed random value
        int    leng   :  length
        int    seed   :  seed of random value

****************************************************************/

#include <stdio.h>
#include <math.h>

/*
#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif
*/
#include "_nrand.priv.h"

#define RAND_MAX 32767


static double rnd(unsigned long *next)
{
   double r;

   *next = *next * 1103515245L + 12345;
   r = (*next / 65536L) % 32768L;

   return (r / RAND_MAX);
}

int nrand(double *p, const int leng, const int seed)
{
   int i;
   unsigned long next;

   if (seed != 1)
      next = srnd((unsigned int) seed);
   for (i = 0; i < leng; i++)
      p[i] = (double) nrandom(&next);

   return (0);
}

double nrandom(unsigned long *next)
{
   static int sw = 0;
   static double r1, r2, s;

   if (sw == 0) {
      sw = 1;
      do {
         r1 = 2 * rnd(next) - 1;
         r2 = 2 * rnd(next) - 1;
         s = r1 * r1 + r2 * r2;
      }
      while (s > 1 || s == 0);
      s = sqrt(-2 * log(s) / s);
      return (r1 * s);
   } else {
      sw = 0;
      return (r2 * s);
   }
}

unsigned long srnd(const unsigned int seed)
{
   return (seed);
}
