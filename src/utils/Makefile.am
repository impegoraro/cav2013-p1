

# Definition of the configuration flag
AM_CXXFLAGS=-std=c++0x -Wall -I../
AM_CFLAGS=$(DEPS_CFLAGS) 

bin_PROGRAMS=yuvShow yuvComp yuvCopy yuvConvert videoEffects yuvResize frameExtractor

# yuvShow program 
yuvShow_LDADD=../libcav/libcav.a $(DEPS_LIBS)
yuvShow_SOURCES=yuvShow.cc


# yuvComp program 
yuvComp_LDADD=../libcav/libcav.a $(DEPS_LIBS)
yuvComp_SOURCES=yuvComp.cc

# yuvCopy program 
yuvCopy_LDADD=../libcav/libcav.a $(DEPS_LIBS)
yuvCopy_SOURCES=yuvCopy.cc

# yuvConvert program 
yuvConvert_LDADD=../libcav/libcav.a $(DEPS_LIBS)
yuvConvert_SOURCES=yuvConvert.cc

# videoEffects program 
videoEffects_LDADD=../libcav/libcav.a $(DEPS_LIBS)
videoEffects_SOURCES=videoEffects.cc

# yuvResize program 
yuvResize_LDADD=../libcav/libcav.a $(DEPS_LIBS)
yuvResize_SOURCES=yuvResize.cc

# frameExtractor program
frameExtractor_LDADD=../libcav/libcav.a $(DEPS_LIBS)
frameExtractor_SOURCES=frameExtractor.cc