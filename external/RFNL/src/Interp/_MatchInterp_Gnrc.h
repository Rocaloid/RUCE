//Generic Template PMatch Interpolation Header

RTFunc(void, RFNL_MatchInterp_Non_Gnrc,
             _T2* Dest, _C(PMatch_, _T1, _, _T2)* Sorc, _T2 Fill, int Size);
             
RTFunc(void, RFNL_MatchInterp_Linear_Gnrc,
             _T2* Dest, _C(PMatch_, _T1, _, _T2)* Sorc, _T2 Fill, int Size);

RTFunc(void, RFNL_MatchInterp_Cosine_Gnrc,
             _T2* Dest, _C(PMatch_, _T1, _, _T2)* Sorc, _T2 Fill, int Size);

