# tbl/vepl1/CMakeLists.txt

## doxygen_add_library(contrib/tbl/vepl1
##   DEPENDS contrib/tbl/vipl core/vil1
##   PACKAGE contrib/tbl
##   DESCRIPTION "Easy Image Processing Library using vil1 (deprecated)"
## )

set(vepl1_sources
 vepl1_threshold.cxx            vepl1_threshold.h
 vepl1_add_random_noise.cxx     vepl1_add_random_noise.h
 vepl1_monadic.cxx              vepl1_monadic.h
 vepl1_dyadic.cxx               vepl1_dyadic.h
 vepl1_erode_disk.cxx           vepl1_erode_disk.h
 vepl1_dilate_disk.cxx          vepl1_dilate_disk.h
 vepl1_sobel.cxx                vepl1_sobel.h
 vepl1_gaussian_convolution.cxx vepl1_gaussian_convolution.h
 vepl1_x_gradient.cxx           vepl1_x_gradient.h
 vepl1_y_gradient.cxx           vepl1_y_gradient.h
 vepl1_gradient_mag.cxx         vepl1_gradient_mag.h
 vepl1_gradient_dir.cxx         vepl1_gradient_dir.h
 vepl1_median.cxx               vepl1_median.h
 vepl1_moment.cxx               vepl1_moment.h
 vepl1_histogram.cxx            vepl1_histogram.h

 vepl1_convert.txx              vepl1_convert.h
)

add_definitions(-DINSTANTIATE_TEMPLATES)
# [only when INSTANTIATE_TEMPLATES not defined:]
# aux_source_directory(Templates vepl1_sources)

if(CMAKE_COMPILER_IS_GNUCXX)
  set_source_files_properties(vepl1_threshold.cxx PROPERTIES COMPILE_FLAGS -O2)
  set_source_files_properties(vepl1_add_random_noise.cxx PROPERTIES COMPILE_FLAGS -O1)
  set_source_files_properties(vepl1_dyadic.cxx PROPERTIES COMPILE_FLAGS -O3)
  set_source_files_properties(vepl1_dilate_disk.cxx PROPERTIES COMPILE_FLAGS -O1)
  set_source_files_properties(vepl1_erode_disk.cxx PROPERTIES COMPILE_FLAGS -O1)
  set_source_files_properties(vepl1_median.cxx PROPERTIES COMPILE_FLAGS -O3)
  set_source_files_properties(vepl1_moment.cxx PROPERTIES COMPILE_FLAGS -O1)
  set_source_files_properties(vepl1_histogram.cxx PROPERTIES COMPILE_FLAGS -O0)
  set_source_files_properties(Templates/vepl1_convert+uchar-.cxx PROPERTIES COMPILE_FLAGS -O1)
  set_source_files_properties(Templates/vepl1_convert+short-.cxx PROPERTIES COMPILE_FLAGS -O1)
  set_source_files_properties(Templates/vepl1_convert+float-.cxx PROPERTIES COMPILE_FLAGS -O1)
endif()

add_library(vepl1 ${vepl1_sources})
# install_targets(/lib vepl1)
# INSTALL_NOBASE_HEADER_FILES(/include/vxl/contrib/tbl/vepl1 ${vepl1_sources})
target_link_libraries(vepl1 vipl vil1 ${CMAKE_THREAD_LIBS})

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif ()

if (BUILD_TESTING)
  add_subdirectory(tests)
endif ()
