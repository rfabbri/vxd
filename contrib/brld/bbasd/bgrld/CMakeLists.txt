# This is lemsvxlsrc/basic/dbgrl/CMakeLists.txt
# Development Brown Graph/Hypergraph Library

set(dbgrl_sources
         
dbgrl_vertex.h             dbgrl_vertex.hxx    
dbgrl_edge.h               dbgrl_edge.hxx
dbgrl_graph.h              dbgrl_graph.hxx            

dbgrl_hg_vertex.cxx        dbgrl_hg_vertex.h
dbgrl_hg_edge.cxx          dbgrl_hg_edge.h
dbgrl_hg_hyperedge.cxx     dbgrl_hg_hyperedge.h

dbgrl_hg_graph.cxx         dbgrl_hg_graph.h
dbgrl_hg_hypergraph.cxx    dbgrl_hg_hypergraph.h

)

add_library(dbgrl ${dbgrl_sources})

target_link_libraries( dbgrl )  

#Algorithms on graphs
add_subdirectory(algo)

# Processing Library
#add_subdirectory(pro)

# Visualization Library
#add_subdirectory(vis)

if(BUILD_TESTING)
add_subdirectory(tests)
endif(BUILD_TESTING)
