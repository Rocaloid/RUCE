#ifndef MATFUNC
#define MATFUNC

double * wavread(const char* filename, int *fs, int *nbit, int *wav_length);
void wavwrite(double *x, int x_length, int fs, int nbit, const char *filename);

#endif

