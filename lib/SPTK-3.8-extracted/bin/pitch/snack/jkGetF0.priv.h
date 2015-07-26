#include <stdlib.h>
#include "sigproc.priv.h"
#include "../../nrand/_nrand.pub.h"
#include "../../../lib/getmem.pub.h"


static void usage(int status) {
    abort();
}

typedef struct f0_params F0_params;
typedef struct cross_rec Cross;
typedef struct frame_rec Frame;
typedef struct sta_rec Stat;
typedef struct _float_list float_list;

static int check_f0_params(F0_params *par, double sample_freq);
static void get_fast_cands(float *fdata, float *fdsdata, int ind, int step, int size, int dec, int start, int nlags, float *engref, int *maxloc, float *maxval, Cross *cp, float *peaks, int *locs, int *ncand, F0_params *par);
static float *downsample(float *input, int samsin, int state_idx, double freq, int *samsout, int decimate, int first_time, int last_time);
static void get_cand(Cross *cross, float *peak, int *loc, int nlags, int *ncand, float cand_thresh);
static int downsamp(float *in, float *out, int samples, int *outsamps, int state_idx, int decimate, int ncoef, float fc[], int init);
static void do_ffir(register float *buf, register int in_samps, register float *bufo, register int *out_samps, int idx, register int ncoef, float *fc, register int invert, register int skip, register int init);
static int lc_lin_fir(register float fc, int *nf, float *coef);
static void peak(float *y, float *xp, float *yp);
static int get_Nframes(long buffsize, int pad, int step);
static int init_dp_f0(double freq, F0_params *par, long *buffsize, long *sdstep);
static int dp_f0(float *fdata, int buff_size, int sdstep, double freq, F0_params *par, float **f0p_pt, float **vuvp_pt, float **rms_speech_pt, float **acpkp_pt, int *vecsize, int last_time);
static Frame *alloc_frame(int nlags, int ncands);
static int save_windstat(float *rho, int order, float err, float rms);
static int retrieve_windstat(float *rho, int order, float *err, float *rms);
static float get_similarity(int order, int size, float *pdata, float *cdata, float *rmsa, float *rms_ratio, float pre, float stab, int w_type, int init);
static Stat *get_stationarity(float *fdata, double freq, int buff_size, int nframes, int frame_step, int first_time);
static int eround(double flnum);
static void free_dp_f0(void);
void rapt(float_list *input, int length, double sample_freq, int frame_shift, double minF0, double maxF0, double voice_bias, int otype);
