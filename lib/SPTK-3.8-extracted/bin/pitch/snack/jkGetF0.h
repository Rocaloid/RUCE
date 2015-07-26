/*
 * This software has been licensed to the Centre of Speech Technology, KTH
 * by Microsoft Corp. with the terms in the accompanying file BSD.txt,
 * which is a BSD style license.
 *
 *    "Copyright (c) 1990-1996 Entropic Research Laboratory, Inc. 
 *                   All rights reserved"
 *
 * Written by:  David Talkin
 * Checked by:
 * Revised by:
 * @(#)f0.h	1.4 9/9/96 ERL
 * Brief description:
 *
 */
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

/* $Id: jkGetF0.h,v 1.6 2014/12/11 08:30:43 uratec Exp $ */


/* f0.h */
/* Some definitions used by the "Pitch Tracker Software". */
       
typedef struct f0_params {
float cand_thresh,	/* only correlation peaks above this are considered */
      lag_weight,	/* degree to which shorter lags are weighted */
      freq_weight,	/* weighting given to F0 trajectory smoothness */
      trans_cost,	/* fixed cost for a voicing-state transition */
      trans_amp,	/* amplitude-change-modulated VUV trans. cost */
      trans_spec,	/* spectral-change-modulated VUV trans. cost */
      voice_bias,	/* fixed bias towards the voiced hypothesis */
      double_cost,	/* cost for octave F0 jumps */
      mean_f0,		/* talker-specific mean F0 (Hz) */
      mean_f0_weight,	/* weight to be given to deviations from mean F0 */
      min_f0,		/* min. F0 to search for (Hz) */
      max_f0,		/* max. F0 to search for (Hz) */
      frame_step,	/* inter-frame-interval (sec) */
      wind_dur;		/* duration of correlation window (sec) */
int   n_cands,		/* max. # of F0 cands. to consider at each frame */
      conditioning;     /* Specify optional signal pre-conditioning. */
} F0_params;

/* Possible values returned by the function f0(). */
#define F0_OK		0
#define F0_NO_RETURNS	1
#define F0_TOO_FEW_SAMPLES	2
#define F0_NO_INPUT	3
#define F0_NO_PAR	4
#define F0_BAD_PAR	5
#define F0_BAD_INPUT	6
#define F0_INTERNAL_ERR	7

/* Bits to specify optional pre-conditioning of speech signals by f0() */
/* These may be OR'ed together to specify all preprocessing. */
#define F0_PC_NONE	0x00		/* no pre-processing */
#define F0_PC_DC	0x01		/* remove DC */
#define F0_PC_LP2000	0x02		/* 2000 Hz lowpass */
#define F0_PC_HP100	0x04		/* 100 Hz highpass */
#define F0_PC_AR	0x08		/* inf_order-order LPC inverse filter */
#define F0_PC_DIFF	0x010		/* 1st-order difference */

extern F0_params *new_f0_params();
extern int atoi(), eround(), lpc(), window(), get_window();
extern void get_fast_cands(), a_to_aca(), cross(), crossf(), crossfi(),
           autoc(), durbin();

#define Fprintf (void)fprintf

/* f0_structs.h */

#define BIGSORD 100

typedef struct cross_rec { /* for storing the crosscorrelation information */
	float	rms;	/* rms energy in the reference window */
	float	maxval;	/* max in the crosscorr. fun. q15 */
	short	maxloc; /* lag # at which max occured	*/
	short	firstlag; /* the first non-zero lag computed */
	float	*correl; /* the normalized corsscor. fun. q15 */
} Cross;

typedef struct dp_rec { /* for storing the DP information */
	short	ncands;	/* # of candidate pitch intervals in the frame */
	short	*locs; /* locations of the candidates */
	float	*pvals; /* peak values of the candidates */
	float	*mpvals; /* modified peak values of the candidates */
	short	*prept; /* pointers to best previous cands. */
	float	*dpvals; /* cumulative error for each candidate */
} Dprec;

typedef struct windstat_rec {  /* for lpc stat measure in a window */
    float rho[BIGSORD+1];
    float err;
    float rms;
} Windstat;

typedef struct sta_rec {  /* for stationarity measure */
  float *stat;
  float *rms;
  float *rms_ratio;
} Stat;


typedef struct frame_rec{
  Cross *cp;
  Dprec *dp;
  float rms;
  struct frame_rec *next;
  struct frame_rec *prev;
} Frame;

extern   Frame *alloc_frame();
