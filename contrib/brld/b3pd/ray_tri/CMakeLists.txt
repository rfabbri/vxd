# This is /b3pd/ray_tri/CMakeLists.txt
# Code for testing if a ray intersects a triangle in 3D or not

set ( ray_tri_sources
  ray_tri.cxx    ray_tri.h
)
 
add_library(ray_tri ${ray_tri_sources})
