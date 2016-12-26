# breye/dbpro/CMakeLists.txt

project(dbpro)

include_directories( ${VXL_BRL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/basic )

find_package( Threads )

configure_file( ${dbpro_SOURCE_DIR}/dbpro_config.h.in
                ${dbpro_BINARY_DIR}/dbpro_config.h )


set(dbpro_sources
   dbpro_mutex.cxx                       dbpro_mutex.h 
   dbpro_parameters.cxx                  dbpro_parameters.h                  dbpro_parameters_sptr.h
   dbpro_parameters.hxx
   dbpro_process.cxx                     dbpro_process.h                     dbpro_process_sptr.h
                                         dbpro_basic_processes.h
                                         dbpro_vsl_processes.h
                                         dbpro_ios_processes.h
   dbpro_process_factory.cxx             dbpro_process_factory.h
                                         dbpro_storage.h                     dbpro_storage_sptr.h
                                         dbpro_observer.h                    dbpro_observer_sptr.h
   dbpro_connector.cxx                   dbpro_connector.h                   dbpro_connector_sptr.h
   dbpro_delay.cxx                       dbpro_delay.h
   dbpro_try_option.cxx                  dbpro_try_option.h
   dbpro_executive.cxx                   dbpro_executive.h
)



aux_source_directory(Templates dbpro_sources)

add_library(dbpro  ${dbpro_sources})


target_link_libraries(dbpro vul vbl)

if( CMAKE_USE_PTHREADS_INIT )
  set_target_properties(dbpro PROPERTIES COMPILE_FLAGS -pthread)
  target_link_libraries(dbpro pthread)
endif( CMAKE_USE_PTHREADS_INIT )


add_subdirectory( vis )

add_subdirectory( xio )

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
