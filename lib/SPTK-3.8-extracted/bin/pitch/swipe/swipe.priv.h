#include "../../fftr/_fftr.pub.h"

typedef double out_complex[2];
typedef struct _float_list float_list;

static double hz2mel(double hz);
static double hz2erb(double hz);
static double erb2hz(double erb);
static double fixnan(double x);
static void La(matrix L, vector f, vector fERBs, out_complex *fo, int w2, int hi, int i, double *fi);
static matrix loudness(vector x, vector fERBs, double nyquist, int w, int w2);
static void Sadd(matrix S, matrix L, vector fERBs, vector pci, vector mu, intvector ps, double dt, double nyquist2, int lo, int psz, int w2);
static void Sfirst(matrix S, vector x, vector pc, vector fERBs, vector d, intvector ws, intvector ps, double nyquist, double nyquist2, double dt, int n);
static void Snth(matrix S, vector x, vector pc, vector fERBs, vector d, intvector ws, intvector ps, double nyquist, double nyquist2, double dt, int n);
static void Slast(matrix S, vector x, vector pc, vector fERBs, vector d, intvector ws, intvector ps, double nyquist, double nyquist2, double dt, int n);
static vector pitch(matrix S, vector pc, double st);
void swipe(float_list *input, int length, double samplerate, int frame_shift, double min, double max, double st, int otype, void (*callback)(void *callback_param, float result), void *callback_param);
