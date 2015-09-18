//-----------------------------------------------------------------------------
// Copyright 2012-2015 Masanori Morise. All Rights Reserved.
// Author: mmorise [at] yamanashi.ac.jp (Masanori Morise)
//
// Matlab functions implemented for WORLD
// Since these functions are implemented as the same function of Matlab,
// the source code does not follow the style guide (Names of variables
// and functions).
// Please see the reference of Matlab to show the usage of functions.
// Caution:
//   Since these functions (wavread() and wavwrite()) are roughly implemented,
//   we recommend more suitable functions provided by other organizations.
//
// Sept 17, 2015
//   Add IEEE Float and fmt extension support for wavread.
//   Add 8bit/24bit/32bit PCM support for wavwrite.
//     k.hua.kanru [at] ieee.org (Kanru Hua)
//
//-----------------------------------------------------------------------------

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define false 0
#define true  1
typedef int bool;

static int MyMax(int x, int y) {
  return x > y ? x : y;
}

static int MyMin(int x, int y) {
  return x < y ? x : y;
}

#define FORMAT_PCM          1
#define FORMAT_IEEE_FLOAT   3
#define FORMAT_EXT        254

//-----------------------------------------------------------------------------
// GetParameters() extracts fp, nbit, wav_length from the .wav file
// This function is only used in wavread().
//-----------------------------------------------------------------------------
static bool GetParameters(FILE *fp, int *fs, int *nbit, int *wav_length, int *format_tag) {
  unsigned char data_check[5] = {0};
  char str_check[5] = {0};
  str_check[4] = '\0';
  int fmt_size = 16;
  
  fread(str_check, 1, 4, fp);  // "RIFF"
  str_check[4] = '\0';
  if (0 != strcmp(str_check, "RIFF")) {
    printf("RIFF error.\n");
    return false;
  }
  fseek(fp, 4, SEEK_CUR);
  fread(str_check, 1, 4, fp);  // "WAVE"
  if (0 != strcmp(str_check, "WAVE")) {
    printf("WAVE error.\n");
    return false;
  }
  fread(str_check, 1, 4, fp);  // "fmt "
  if (0 != strcmp(str_check, "fmt ")) {
    printf("fmt error.\n");
    return false;
  }
  fread(data_check, 1, 4, fp);  // cksize: 16/18/40 0 0 0
  if (!((16 == data_check[0] || 18 == data_check[0] || 40 == data_check[0])
   && 0 == data_check[1] && 0 == data_check[2] && 0 == data_check[3])) {
    printf("fmt (2) error.\n");
    return false;
  }
  fmt_size = data_check[0];
  fread(data_check, 1, 2, fp);  // wFormatTag: 1(PCM)/3(IEEE Float) 0 | FE FF(extended format)
  if (!(((1 == data_check[0] || 3 == data_check[0]) && 0 == data_check[1]) ||
       (254 == data_check[0] && 255 == data_check[1]))) {
    printf("Format ID error.\n");
    return false;
  }
  if (1 == data_check[0])
    *format_tag = FORMAT_PCM;
  else if (3 == data_check[1])
    *format_tag = FORMAT_IEEE_FLOAT;
  else
    *format_tag = FORMAT_EXT;
  fread(data_check, 1, 2, fp);  // nChannels: 1 0
  if (!(1 == data_check[0] && 0 == data_check[1])) {
    printf("This function cannot support stereo file\n");
    return false;
  }
  
  fread(data_check, 1, 4, fp); // fs
  *fs = 0;
  for (int i = 3; i >= 0; --i) *fs = *fs * 256 + data_check[i];
  fread(data_check, 1, 4, fp); // nAvgBytesPerSec
  fread(data_check, 1, 2, fp); // nBlockAlign
  int block_align = data_check[0];
  fread(data_check, 1, 2, fp); // nBitsPerSample
  *nbit = data_check[0];
  
  if ((*nbit) / 8 != block_align) {
    printf("Unsupported block align.\n");
    return false;
  }
  
  short int extsize = 0;
  if (fmt_size > 16) {
    fread(&extsize, 1, 2, fp); // cbSize
    if (0 != extsize && 22 != extsize) {
      printf("Unsupported extension.\n");
      return false;
    }
  }
  if(extsize > 0) {
    char extension[22];
    fread(extension, 1, extsize, fp);
    if (extension[6] == 1 && extension[7] == 0)
      *format_tag = FORMAT_EXT;
    else if (extension[6] == 3 && extension[7] == 0)
      *format_tag = FORMAT_IEEE_FLOAT;
    else {
      printf("Unsupported format.\n");
      return false;
    }
  }
  
  // Skip until "data" is found. 2011/03/28
  while (0 != fread(str_check, 1, 1, fp)) {
    if (str_check[0] == 'd') {
      fread(&str_check[1], 1, 3, fp);
      if (0 != strcmp(str_check, "data")) {
        fseek(fp, -3, SEEK_CUR);
      } else {
        break;
      }
    }
  }
  if (0 != strcmp(str_check, "data")) {
    printf("data error.\n");
    return false;
  }

  fread(data_check, 1, 4, fp);  // "data"
  *wav_length = 0;
  for (int i = 3; i >= 0; --i)
    *wav_length = *wav_length * 256 + data_check[i];
  *wav_length /= (*nbit / 8);
  return true;
}

void wavwrite(double *x, int x_length, int fs, int nbit, const char *filename) {
  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("File cannot be opened.\n");
    return;
  }

  if (nbit % 8 != 0 || nbit > 32) {
    printf("Unsupported bit per sample.\n");
    return;
  }
  
  int nbyte = nbit / 8;
  
  char text[4] = {'R', 'I', 'F', 'F'};
  uint32_t long_number = 36 + x_length * nbyte;
  fwrite(text, 1, 4, fp);
  fwrite(&long_number, 4, 1, fp);

  text[0] = 'W';
  text[1] = 'A';
  text[2] = 'V';
  text[3] = 'E';
  fwrite(text, 1, 4, fp);
  text[0] = 'f';
  text[1] = 'm';
  text[2] = 't';
  text[3] = ' ';
  fwrite(text, 1, 4, fp);

  long_number = 16;
  fwrite(&long_number, 4, 1, fp);
  int16_t short_number = FORMAT_PCM;
  fwrite(&short_number, 2, 1, fp);
  short_number = 1; // nChannels
  fwrite(&short_number, 2, 1, fp);
  long_number = fs; // nSamplesPerSec
  fwrite(&long_number, 4, 1, fp);
  long_number = fs * nbyte; // nAvgBytesPerSec
  fwrite(&long_number, 4, 1, fp);
  short_number = nbyte; // nBlockAlign
  fwrite(&short_number, 2, 1, fp);
  short_number = nbit; // nBitsPerSample
  fwrite(&short_number, 2, 1, fp);

  text[0] = 'd';
  text[1] = 'a';
  text[2] = 't';
  text[3] = 'a';
  fwrite(text, 1, 4, fp);
  long_number = x_length * nbyte;
  fwrite(&long_number, 4, 1, fp);

  if (nbyte == 1)
  for (int i = 0; i < x_length; ++i) {
    uint8_t tmp_signal;
    tmp_signal = (uint8_t)(MyMax(0, MyMin(255, (int)((x[i] + 1.0) * 127))));
    fwrite(&tmp_signal, 1, 1, fp);
  }
  else if (nbyte == 2)
  for (int i = 0; i < x_length; ++i) {
    int16_t tmp_signal;
    tmp_signal = (int16_t)(MyMax(-32768, MyMin(32767, (int)(x[i] * 32767))));
    fwrite(&tmp_signal, 2, 1, fp);
  }
  else if (nbyte == 3)
  for (int i = 0; i < x_length; ++i) {
    int32_t tmp_signal;
    tmp_signal = (int32_t)(MyMax(-8388608, MyMin(8388607, (int)(x[i] * 8388607))));
    fwrite(&tmp_signal, 3, 1, fp);
  }
  else if (nbyte == 4)
  for (int i = 0; i < x_length; ++i) {
    int32_t tmp_signal;
    tmp_signal = (int32_t)(MyMax(-2147483648, MyMin(2147483647, (int)(x[i] * 2147483647))));
    fwrite(&tmp_signal, 4, 1, fp);
  }

  fclose(fp);
}

double * wavread(const char* filename, int *fs, int *nbit, int *wav_length) {
  FILE *fp = fopen(filename, "rb");
  if (NULL == fp) {
    printf("File not found.\n");
    return NULL;
  }

  int format_tag = 0;
  if (GetParameters(fp, fs, nbit, wav_length, & format_tag) == false) {
    fclose(fp);
    return NULL;
  }

  double *waveform = calloc(*wav_length, sizeof(double));
  if (waveform == NULL) return NULL;

  int quantization_byte = *nbit / 8;
  double zero_line = pow(2.0, *nbit - 1);
  double tmp, sign_bias, nbitpow;
  unsigned char for_int_number[8];
  nbitpow = pow(2.0, *nbit - 1);
  for (int i = 0; i < *wav_length; ++i) {
    sign_bias = tmp = 0.0;
    fread(for_int_number, 1, quantization_byte, fp);  // "data"
    if (format_tag == FORMAT_PCM) {
      if (quantization_byte == 1) {
        waveform[i] = for_int_number[0] / 128.0 - 0.5;
        continue;
      }
      if (for_int_number[quantization_byte-1] >= 128) {
        sign_bias = nbitpow;
        for_int_number[quantization_byte - 1] =
          for_int_number[quantization_byte - 1] & 0x7F;
      }
      for (int j = quantization_byte - 1; j >= 0; --j)
        tmp = tmp * 256.0 + for_int_number[j];
      waveform[i] = (tmp - sign_bias) / zero_line;
    } else
    if (format_tag == FORMAT_IEEE_FLOAT) {
      if (quantization_byte == 4)
        waveform[i] = *((float*)for_int_number);
      else if (quantization_byte == 8)
        waveform[i] = *((double*)for_int_number);
    }
  }
  fclose(fp);
  return waveform;
}

