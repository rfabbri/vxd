set (HAS_BOIL "NO")
if (BUILD_BOIL)

  # Find Coin3D - Open Inventor
  include( ${LEMS_CMAKE_DIR}/Modules/FindCoin.cmake )
  
  if (COIN_FOUND)
    include_directories( ${COIN_INCLUDE_DIR} )
    set( HAS_BOIL "YES" )
    add_definitions( -DHAS_BOIL )
    link_libraries( ${COIN_LIBRARY} )
  endif (COIN_FOUND)

endif (BUILD_BOIL)