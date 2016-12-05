# brld/bpro1d/CMakeLists.txt


set(bpro1d_sources
   bpro1d_parameters.cxx                  bpro1d_parameters.h                  bpro1d_parameters_sptr.h
   bpro1d_parameters.hxx
   bpro1d_process.cxx                     bpro1d_process.h                     bpro1d_process_sptr.h
   bpro1d_storage.cxx                     bpro1d_storage.h                     bpro1d_storage_sptr.h
)


aux_source_directory(Templates bpro1d_sources)

add_library(bpro1d  ${bpro1d_sources})

target_link_libraries(bpro1d vsl vbl)

