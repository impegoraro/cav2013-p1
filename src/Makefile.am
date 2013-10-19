bin_PROGRAMS=cavp1

AM_CXXFLAGS=-std=c++0x
AM_CFLAGS=$(DEPS_CFLAGS) 
cavp1_LDADD=$(DEPS_LIBS)

cavp1_SOURCES=block.h block.cc \
              frame.h frame.cc \
              frame422.h frame422.cc \
              frame420.h frame420.cc \
              frame444.h frame444.cc \
              video.h video.cc \
              video-formats.h \
              exceptions/cav-exceptions.h \
              yuvShow.cc
