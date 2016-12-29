# Kanatani BMVC08 optimal triangulation code

set(triangulation_kanatani_sources
extensions.cpp extensions.h
matrix.cpp matrix.h
eigen_okumura.cpp
matutil.cpp
twoview_triangulation.cpp twoview_triangulation.h libmatrix.h
vector.cpp vector.h
)

add_library(triangulation_kanatani ${triangulation_kanatani_sources})

