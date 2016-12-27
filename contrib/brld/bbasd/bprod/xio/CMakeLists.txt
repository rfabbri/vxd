# basic/bprod/xio/CMakeLists.txt
# Development Brown Processing XML IO Library

include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )

set(bprod_xio_sources

  bprod_xio_source.cxx            bprod_xio_source.h
  bprod_xio_sink.cxx              bprod_xio_sink.h

)


#aux_source_directory(Templates bprod_xio_sources)

add_library(bprod_xio  ${bprod_xio_sources})

target_link_libraries(bprod_xio bprod bxml)

