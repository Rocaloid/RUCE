CVESVP
======

CVE Spectral Voice Processing Library

Todo & Plan List
===

####Step1: Voice(Low Level) Modeling

- [x] Spectrum filter utilities
- [x] Implement `PSOLAIterlyzer`(in: `Wave`, out: `PulseList`)
- [x] Implement `PSOLAItersizer`(in: `Position` `DataFrame`, out: `Wave`)
- [x] Implement `VOTFromWave` based on `STFTIterlyzer`(in: `Wave`, out: `Position`)
- [X] Implement `Sinusoid` structure and `List_Sinusoid` and `Sinusoid_ToReal`
- [x] Implement `HNMFrame` `HNMContour` structure and `List_HNMFrame`
- [x] Implement `SinusoidIterlyzer`(in: `Wave`, out: `List_Sinusoid`)
- [x] Implement `SinusoidItersizer`(in: `Position` `Sinusoid`, out: `Wave`)
- [x] Implement `HNMIterlyzer`(in: `Wave`, out: `List_HNMFrame`)
- [x] Glottal pulse & phase reconstruction in `SinusoidItersizer`
- [x] Implement `HNMItersizer` based on `SinusoidItersizer`(in: `Position` `HNMFrame`, out: `Wave`)
- [x] Implement `F0Iterlyzer`(in: `Wave`, out: `Real`)
- [x] Phase control points for `SinusoidItersizer`
- [ ] SNT analysis
- [ ] Turbulent Noise reconstruction in `HNMItersizer`

####Step2: Structural Changes & Minor Improvements

- [x] Replace `CDSP2_If_Debug_Check` with `RAssert`
- [x] Down integrate Lists to CVEDSP2
- [x] Conversion between `HNMFrame` and `HNMContour`
- [x] Implement `F0FromWave_YIN`
- [x] Implement `GainIterfector` in CVEDSP2(in: `Wave` `Wave`, out: `Wave`)
- [ ] Implement `MixIterfector` in CVEDSP2(in: `Wave` `Wave`, out: `Wave`)
- [ ] Implement `PulseItersizer` in CVEDSP2(in: `Position`, out: `Wave`)

####Step3: Voice Manipulation

- [x] Timbre adjustment related to pitch scaling
- [ ] Implement `GenKlatt` based on `FWindow`
- [ ] Implement `EpRParam` structure
- [ ] Implement `EpRParam_ToHNMContour`
- [ ] EpR fitting algorithm
- [ ] EpR manipulating utilities
- [ ] PSOLA manipulating utilities

####Others/External

- [ ] Implement `VMaxIndex`, `VMinIndex`, `VMaxEI`, `VMinEI`, `VLog` in RFNL
- [ ] More interpolation kernels(Cubic, Sinc) in RFNL
- [ ] Default analysis window for `_F0.rc`/Window Cache

Bibilography
===

* Serra, X. 1989. "A System for Sound Analysis/Transformation/Synthesis based on a Deterministic plus Stochastic Decomposition" Ph.D. Thesis. Stanford University.

* Sanjaume, Jordi Bonada. Voice processing and synthesis by performance sampling and spectral models. Diss. Universitat Pompeu Fabra, 2008.

* Quatieri, Thomas F., and R. McAulay. "Phase coherence in speech reconstruction for enhancement and coding applications." Acoustics, Speech, and Signal Processing, 1989. ICASSP-89., 1989 International Conference on. IEEE, 1989.

* De Cheveigné, Alain, and Hideki Kawahara. "YIN, a fundamental frequency estimator for speech and music." The Journal of the Acoustical Society of America 111.4 (2002): 1917-1930.

* Childers, Donald G., and C. K. Lee. "Vocal quality factors: Analysis, synthesis, and perception." the Journal of the Acoustical Society of America 90.5 (1991): 2394-2410.

