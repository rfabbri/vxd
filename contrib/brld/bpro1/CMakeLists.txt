# breye1/bpro1/CMakeLists.txt


set(bpro1_sources
   bpro1_parameters.cxx                  bpro1_parameters.h                  bpro1_parameters_sptr.h
   bpro1_parameters.hxx
   bpro1_process.cxx                     bpro1_process.h                     bpro1_process_sptr.h
   bpro1_storage.cxx                     bpro1_storage.h                     bpro1_storage_sptr.h
)


aux_source_directory(Templates bpro1_sources)

add_library(bpro1  ${bpro1_sources})

target_link_libraries(bpro1 vsl vbl)

