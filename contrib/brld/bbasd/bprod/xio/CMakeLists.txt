# basic/dbpro/xio/CMakeLists.txt
# Development Brown Processing XML IO Library

include_directories( ${VXL_BRL_INCLUDE_DIR}/bbas )

set(dbpro_xio_sources

  dbpro_xio_source.cxx            dbpro_xio_source.h
  dbpro_xio_sink.cxx              dbpro_xio_sink.h

)


#aux_source_directory(Templates dbpro_xio_sources)

add_library(dbpro_xio  ${dbpro_xio_sources})

target_link_libraries(dbpro_xio dbpro bxml)

