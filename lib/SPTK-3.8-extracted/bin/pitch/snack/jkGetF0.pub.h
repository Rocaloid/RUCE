#ifdef __cplusplus
extern "C" {
#endif

typedef struct _float_list {
    float f;
    struct _float_list *next;
} float_list;

void rapt(float_list *input, int length, double sample_freq, int frame_shift, double minF0, double maxF0, double voice_bias, int otype, void (*callback)(void *callback_param, float result), void *callback_param);

#ifdef __cplusplus
}
#endif
