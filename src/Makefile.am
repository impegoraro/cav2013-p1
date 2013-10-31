

# Definition of the configuration flag
AM_CXXFLAGS=-std=c++0x -Wall
AM_CFLAGS=$(DEPS_CFLAGS)

###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################

lib_LIBRARIES=libcav.a
libcav_a_SOURCES = block.h block.cc \
              subblock.h subblock.cc \
              frame.h frame.cc \
              frame422.h frame422.cc \
              frame420.h frame420.cc \
              frame444.h frame444.cc \
              framergb.h framergb.cc \
              video.h video.cc \
              video-formats.h \
              exceptions/cav-exceptions.h
#libcav_a_LDFLAGS=-static 
#libcav_a_LIBADD=$(DEPS_LIBS)
#libcav_a_CPPFLAGS=$(DEPS)


###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

bin_PROGRAMS=yuvShow yuvComp yuvCopy yuvConvert videoEffects yuvResize

# yuvShow program 
yuvShow_LDADD=$(DEPS_LIBS) libcav.a
yuvShow_SOURCES=yuvShow.cc


# yuvComp program 
yuvComp_LDADD=$(DEPS_LIBS) libcav.a
yuvComp_SOURCES=yuvComp.cc

# yuvCopy program 
yuvCopy_LDADD=$(DEPS_LIBS) libcav.a
yuvCopy_SOURCES=yuvCopy.cc

# yuvConvert program 
yuvConvert_LDADD=$(DEPS_LIBS) libcav.a
yuvConvert_SOURCES=yuvConvert.cc

# videoEffects program 
videoEffects_LDADD=$(DEPS_LIBS) libcav.a
videoEffects_SOURCES=videoEffects.cc

# yuvResize program 
yuvResize_LDADD=$(DEPS_LIBS) libcav.a
yuvResize_SOURCES=yuvResize.cc

