//Generic Template Series Mapping, Compression & Expansion Header

#define __RFNL_Define_MapStretch_Gnrc(Post) \
    RTFunc(void, _C(RFNL_MapStretch_, Post, _Gnrc), \
                _T2* Dest, _T2* Sorc, _T1* DestAnchor, _T1* SorcAnchor, \
                int INum, int Size)

__RFNL_Define_MapStretch_Gnrc(Non);
__RFNL_Define_MapStretch_Gnrc(Linear);
__RFNL_Define_MapStretch_Gnrc(Cosine);

