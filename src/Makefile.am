lib_LIBRARIES=libcav.a
bin_PROGRAMS=yuvShow yuvComp yuvCopy videoEffects

# Definition of the configuration flag
AM_CXXFLAGS=-std=c++0x
AM_CFLAGS=$(DEPS_CFLAGS)

libcav_a_SOURCES = block.h block.cc \
              frame.h frame.cc \
              frame422.h frame422.cc \
              frame420.h frame420.cc \
              frame444.h frame444.cc \
              framergb.h framergb.cc \
              video.h video.cc \
              video-formats.h \
              exceptions/cav-exceptions.h
libcav_a_LDFLAGS=-static 
#libcav_a_LIBADD=$(DEPS_LIBS)
#libcav_a_CPPFLAGS=$(DEPS)

# yuvShow program 
yuvShow_LDADD=$(DEPS_LIBS) libcav.a
yuvShow_SOURCES=yuvShow.cc


# yuvComp program 
yuvComp_LDADD=$(DEPS_LIBS) libcav.a
yuvComp_SOURCES=yuvComp.cc


# yuvComp program 
yuvCopy_LDADD=$(DEPS_LIBS) libcav.a
yuvCopy_SOURCES=yuvCopy.cc

# videoEffects program 
videoEffects_LDADD=$(DEPS_LIBS) libcav.a
videoEffects_SOURCES=videoEffects.cc