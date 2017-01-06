# This is brld/bbasd/CMakeLists.txt
# Basic Utility Libraries (that we are unsure will make to vxl/core)

# The Brown Basics (templates) Library (vxd development version)
add_subdirectory(bbld) 

# The Brown Geometry Library (vxd development version)
add_subdirectory(bgld) 

# The Brown Numerical Library (vxd development version)
add_subdirectory(bnld)

# The Brown Spatial Objects Library (vxd development version)
add_subdirectory(bsold) 

# The Brown Utility Library (vxd development version)
add_subdirectory(buld)

# The Brown Spatial Indexing Library (vxd development version)
add_subdirectory(bspid)

# The Brown Image Library (vxd development version)
add_subdirectory(bild) 

# The Brown Process Library (vxd deveolpment version)
add_subdirectory(bprod)

# The Brown Graph/Hypergraph Library (vxd development version)
add_subdirectory(bgrld)
