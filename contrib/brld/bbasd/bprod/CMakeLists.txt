# breye/bprod/CMakeLists.txt

project(bprod)

include_directories( ${VXL_BRL_INCLUDE_DIR} )
include_directories( ${lems_SOURCE_DIR}/basic )

find_package( Threads )

configure_file( ${bprod_SOURCE_DIR}/bprod_config.h.in
                ${bprod_BINARY_DIR}/bprod_config.h )


set(bprod_sources
   bprod_mutex.cxx                       bprod_mutex.h 
   bprod_parameters.cxx                  bprod_parameters.h                  bprod_parameters_sptr.h
   bprod_parameters.hxx
   bprod_process.cxx                     bprod_process.h                     bprod_process_sptr.h
                                         bprod_basic_processes.h
                                         bprod_vsl_processes.h
                                         bprod_ios_processes.h
   bprod_process_factory.cxx             bprod_process_factory.h
                                         bprod_storage.h                     bprod_storage_sptr.h
                                         bprod_observer.h                    bprod_observer_sptr.h
   bprod_connector.cxx                   bprod_connector.h                   bprod_connector_sptr.h
   bprod_delay.cxx                       bprod_delay.h
   bprod_try_option.cxx                  bprod_try_option.h
   bprod_executive.cxx                   bprod_executive.h
)



aux_source_directory(Templates bprod_sources)

add_library(bprod  ${bprod_sources})


target_link_libraries(bprod vul vbl)

if( CMAKE_USE_PTHREADS_INIT )
  set_target_properties(bprod PROPERTIES COMPILE_FLAGS -pthread)
  target_link_libraries(bprod pthread)
endif( CMAKE_USE_PTHREADS_INIT )


add_subdirectory( vis )

add_subdirectory( xio )

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)
