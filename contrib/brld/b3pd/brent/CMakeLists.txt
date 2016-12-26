# This is /db3p/brent/CMakeLists.txt
# Two popular Brent algorithms: root solver and minimizer for 1D function
# This library is downloaded from www.netlib.org, the library name is c/brent.shar.
# The file brent.shar has instruction on how to create these source files.


set ( brent_sources
  brent.h    zeroin.c fminbr.c
)
 
add_library(brent ${brent_sources})
