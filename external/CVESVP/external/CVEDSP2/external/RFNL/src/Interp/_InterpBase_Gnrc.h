//Generic Template Base Interpolation Functions Header & Implementation

RTFunc(RInline _T1, RFNL_Interp_Non_Gnrc, _T1 a0, _T1 a1, _T1 r)
{
    return 0.5f * (a0 + a1);
}

RTFunc(RInline _T1, RFNL_Interp_Linear_Gnrc, _T1 a0, _T1 a1, _T1 r)
{
    return a0 + (a1 - a0) * r;
}

RTFunc(RInline _T1, RFNL_Interp_Cosine_Gnrc, _T1 a0, _T1 a1, _T1 r)
{
    return a0 - 0.5f * (a1 - a0) * 
        (RCall(RFNL_PCos_LPrec_Gnrc, _T1)(r * M_PI) - 1.0f);
}

