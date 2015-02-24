//Generic Template Basic Vectorization Header

RTFunc(void, RFNL_VAdd_Gnrc, _T1* Dest, _T1* Sorc1, _T1* Sorc2, int Size);
RTFunc(void, RFNL_VSub_Gnrc, _T1* Dest, _T1* Sorc1, _T1* Sorc2, int Size);
RTFunc(void, RFNL_VSubBy_Gnrc, _T1* Dest, _T1* Sorc1, _T1* Sorc2, int Size);
RTFunc(void, RFNL_VMul_Gnrc, _T1* Dest, _T1* Sorc1, _T1* Sorc2, int Size);
RTFunc(void, RFNL_VDiv_Gnrc, _T1* Dest, _T1* Sorc1, _T1* Sorc2, int Size);
RTFunc(void, RFNL_VDivBy_Gnrc, _T1* Dest, _T1* Sorc1, _T1* Sorc2, int Size);

RTFunc(void, RFNL_VMax_Gnrc, _T1* Dest, _T1* Sorc1, _T1* Sorc2, int Size);
RTFunc(void, RFNL_VMin_Gnrc, _T1* Dest, _T1* Sorc1, _T1* Sorc2, int Size);

RTFunc(void, RFNL_VCAdd_Gnrc, _T1* Dest, _T1* Sorc1, _T1 Sorc2, int Size);
RTFunc(void, RFNL_VCSub_Gnrc, _T1* Dest, _T1* Sorc1, _T1 Sorc2, int Size);
RTFunc(void, RFNL_VCSubBy_Gnrc, _T1* Dest, _T1* Sorc1, _T1 Sorc2, int Size);
RTFunc(void, RFNL_VCMul_Gnrc, _T1* Dest, _T1* Sorc1, _T1 Sorc2, int Size);
RTFunc(void, RFNL_VCDiv_Gnrc, _T1* Dest, _T1* Sorc1, _T1 Sorc2, int Size);
RTFunc(void, RFNL_VCDivBy_Gnrc, _T1* Dest, _T1* Sorc1, _T1 Sorc2, int Size);

RTFunc(void, RFNL_VCMax_Gnrc, _T1* Dest, _T1* Sorc1, _T1 Sorc2, int Size);
RTFunc(void, RFNL_VCMin_Gnrc, _T1* Dest, _T1* Sorc1, _T1 Sorc2, int Size);

RTFunc(void, RFNL_VSet_Gnrc, _T1* Dest, _T1 Sorc1, int Size);
RTFunc(void, RFNL_VAbs_Gnrc, _T1* Dest, _T1* Sorc, int Size);
RTFunc(void, RFNL_VCopy_Gnrc, _T1* Dest, _T1* Sorc, int Size);

RTFunc(_T1, RFNL_VMaxElmt_Gnrc, _T1* Sorc, int From, int To);
RTFunc(_T1, RFNL_VMinElmt_Gnrc, _T1* Sorc, int From, int To);

#ifdef _T1_IntLike
RTFunc(int64_t, RFNL_VSum_Gnrc, _T1* Sorc, int From, int To);
#else
RTFunc(_T1, RFNL_VSum_Gnrc, _T1* Sorc, int From, int To);
#endif

#if _T1_TID == _TID_Float
RTFunc(_T1, RFNL_VAvg_Gnrc, _T1* Sorc, int From, int To);
#else
RTFunc(double, RFNL_VAvg_Gnrc, _T1* Sorc, int From, int To);
#endif

