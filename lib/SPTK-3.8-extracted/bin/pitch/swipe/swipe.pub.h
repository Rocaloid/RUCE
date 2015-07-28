#ifdef __cplusplus
extern "C" {
#endif

typedef struct _float_list {
    float f;
    struct _float_list *next;
} float_list;

void swipe(float_list *input, int length, double samplerate, int frame_shift, double min, double max, double st, int otype, void (*callback)(void *callback_param, float result), void *callback_param);

#ifdef __cplusplus
}
#endif
