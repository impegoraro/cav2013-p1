

# Definition of the configuration flag
AM_CXXFLAGS=-std=c++0x -Wall
AM_CFLAGS=$(DEPS_CFLAGS)

###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################

lib_LIBRARIES=libcav.a
libcav_a_SOURCES = block.h block.cc \
              frame.h frame.cc \
              frame422.h frame422.cc \
              frame420.h frame420.cc \
              frame444.h frame444.cc \
              framergb.h framergb.cc \
              video.h video.cc \
              video-encoded.h video-encoded.cc \
              video-interface.h video-interface.cc \
              video-formats.h \
              exceptions/cav-exceptions.h \
              predictor.h predictor.cc \
              linear-predictor.h linear-predictor.cc \
              nonlinear-predictor.h nonlinear-predictor.cc \
              coder.h \
              golomb.h golomb.cc \
              golomb-interframe.h golomb-interframe.cc \
              bitstream.h bitstream.cc \
              util.h util.cc
#              file.h file.cc

#libcav_a_LDFLAGS=-static 
#libcav_a_LIBADD=$(DEPS_LIBS)
#libcav_a_CPPFLAGS=$(DEPS)
