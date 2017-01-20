# bmvgd/CMakeLists.txt
# Brown Multi-View Geometry Libraries (vxd development version).

# Brown Multiview Differential Geometry Library (vxd development version)
add_subdirectory(bdifd)

# Brown Multiview Curve Stereo Library 
# Basis for the 3D curve sketch system (vxd development version)
add_subdirectory(bmcsd)

# Brown Epipolar Curve Library (vxd development version)
add_subdirectory(becld)

# Photogrametry Library (VXD version of VXL/core/vpgl additions).
add_subdirectory(vpgld)

# Multiview Curve Photogrammetry (VXD development version)
add_subdirectory(vcpld)
