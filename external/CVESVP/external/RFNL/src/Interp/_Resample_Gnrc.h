//Generic Template Resampling Header

#define __RFNL_Define_Resample_Gnrc(Post) \
    RTFunc(void, _C(RFNL_Resample_, Post, _Gnrc), \
                 _T1* Dest, _T1* Sorc, \
                 int DestSize, int SorcSize)

__RFNL_Define_Resample_Gnrc(Non);
__RFNL_Define_Resample_Gnrc(Linear);
__RFNL_Define_Resample_Gnrc(Cosine);

