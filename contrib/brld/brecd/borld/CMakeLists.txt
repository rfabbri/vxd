# Brown Object Recognition Evaluation Library
# brld/recd/borld/CMakeLists.txt

include_directories(${VXL_GEL_INCLUDE_DIR})
include_directories(${VXL_BRL_INCLUDE_DIR})

set(borld_sources
  borld_category_info.cxx borld_category_info.h borld_category_info_sptr.h
  #   borld_evaluation.cxx borld_evaluation.h
  borld_exp_stat.cxx borld_exp_stat.h borld_exp_stat_sptr.h
  #   borld_image_bbox_description.cxx borld_image_bbox_description.h borld_image_bbox_description_sptr.h
  borld_image_data_description_base.cxx borld_image_data_description_base.h borld_image_data_description_base_sptr.h
  borld_image_description.cxx borld_image_description.h borld_image_description_sptr.h borld_description_base.h borld_description_base_sptr.h
  #   borld_image_description_utils.cxx borld_image_description_utils.h
  #   borld_image_mask_description.cxx borld_image_mask_description.h borld_image_mask_description_sptr.h
  #   borld_image_polygon_description.cxx borld_image_polygon_description.h borld_image_polygon_description_sptr.h
)

aux_source_directory(Templates borld_sources)

add_library(borld  ${borld_sources})

target_link_libraries(borld vsol vul vbl vsl)

# algorithms library
#add_subdirectory(algo)

# Processing Library
#add_subdirectory(pro)

# Visualization Library
#add_subdirectory(vis)

# if(BUILD_TESTING)
#   add_subdirectory(tests)
# endif(BUILD_TESTING)

# Executables
#add_subdirectory(bin)
