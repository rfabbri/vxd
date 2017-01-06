# This is lemsvxlsrc/bbasd/bgrld/CMakeLists.txt
# Development Brown Graph/Hypergraph Library

set(bgrld_sources
         
bgrld_vertex.h             bgrld_vertex.hxx    
bgrld_edge.h               bgrld_edge.hxx
bgrld_graph.h              bgrld_graph.hxx            

bgrld_hg_vertex.cxx        bgrld_hg_vertex.h
bgrld_hg_edge.cxx          bgrld_hg_edge.h
bgrld_hg_hyperedge.cxx     bgrld_hg_hyperedge.h

bgrld_hg_graph.cxx         bgrld_hg_graph.h
bgrld_hg_hypergraph.cxx    bgrld_hg_hypergraph.h

)

add_library(bgrld ${bgrld_sources})

target_link_libraries( bgrld )  

#Algorithms on graphs
add_subdirectory(algo)

# Processing Library
#add_subdirectory(pro)

# Visualization Library
#add_subdirectory(vis)

if(BUILD_TESTING)
add_subdirectory(tests)
endif(BUILD_TESTING)
