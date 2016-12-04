# basic/CMakeLists.txt
# Brown Basic Libraries.

project(basic)

include_directories( ${lems_SOURCE_DIR}/basic )
include_directories( ${lems_SOURCE_DIR}/db3p )
include_directories( ${basic_BINARY_DIR} )

# The Development Brown Process Library
add_subdirectory( dbpro )

# The Development Brown Numerical Library
add_subdirectory( dbnl )

# The Development Brown Geometry Library
add_subdirectory( dbgl ) 

# The Development Brown Graph/Hypergraph Library
add_subdirectory( dbgrl ) 

# The Development Brown Spatial Objects Library
add_subdirectory( dbsol ) 

# The Development Brown Spatial Indexing Library
add_subdirectory( dbspi )

# The Development Brown Image Library
add_subdirectory( dbil ) 

# The Development Brown Image3d Library
add_subdirectory( dbil3d ) 

# The Development Brown Video Library
add_subdirectory( dbvidl )

# The Development Brown Video Streaming Library
add_subdirectory( dbvidl2 ) 

# Development Brown GUI (stuff that could move to /brl/bbas/bgui)
add_subdirectory( dbgui )

# Development XML Parser using Xerces
#add_subdirectory( dbxml-xerces )

# Development XML Parser - augmenting bxml
add_subdirectory( dbxml )

# Development XML Parser using expat
# NOTE: moved to vxl/contrib/brl/bbas/bxml
##add_subdirectory( dbxml2 )

# The Development Brown Statistics Library
add_subdirectory( dbsta )

# The Development Brown Utility Library
add_subdirectory( dbul )

# The Development Brown Dynamic Programming Library
add_subdirectory( dbdp ) 

# The Development Brown Basic (templates) Library
add_subdirectory( dbbl ) 

# The Development Brown Testing Library
add_subdirectory( dbtest ) 

