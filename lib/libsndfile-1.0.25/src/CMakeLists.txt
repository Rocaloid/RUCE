file(GLOB_RECURSE LIBG72X_SRC_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "G72x/*.c")
file(GLOB_RECURSE LIBGSM610_SRC_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "GSM610/*.c")
add_library(sndfile STATIC common.c file_io.c command.c pcm.c ulaw.c alaw.c float32.c 
                double64.c ima_adpcm.c ms_adpcm.c dwvw.c vox_adpcm.c 
                interleave.c strings.c dither.c broadcast.c audio_detect.c 
                ima_oki_adpcm.c ima_oki_adpcm.h chunk.c ogg.c chanmap.c 
                windows.c id3.c
                sndfile.c aiff.c au.c avr.c caf.c dwd.c flac.c htk.c ircam.c 
                macbinary3.c macos.c mat4.c mat5.c nist.c paf.c pvf.c raw.c rx2.c sd2.c 
                sds.c svx.c txw.c voc.c wve.c w64.c wav_w64.c wav.c xi.c mpc2k.c rf64.c 
                ogg_vorbis.c ogg_speex.c ogg_pcm.c g72x.c gsm610.c
                ${LIBG72X_SRC_SOURCES} ${LIBGSM610_SRC_SOURCES}
)
target_include_directories(sndfile PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}")

