# this is a configure file to use large file support
if(WIN32)

else(WIN32)
  exec_program(getconf ARGS "LFS_CFLAGS" OUTPUT_VARIABLE FileOffsetBits)
  add_definitions(${FileOffsetBits} -D_LARGEFILE_SOURCE)
endif(WIN32)
