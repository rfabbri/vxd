# Brown Object Recognition Evaluation Library
# brld/recd/borld/CMakeLists.txt

set(borld_sources
  borld_evaluation.cxx borld_evaluation.h
  borld_exp_stat.cxx borld_exp_stat.h borld_exp_stat_sptr.h
)

aux_source_directory(Templates borld_sources)

add_library(borld  ${borld_sources})

target_link_libraries(borld vsol vsl)

# algorithms library
#add_subdirectory(algo)

# Processing Library
#add_subdirectory(pro)

# Visualization Library
#add_subdirectory(vis)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)

# Executables
#add_subdirectory(bin)
