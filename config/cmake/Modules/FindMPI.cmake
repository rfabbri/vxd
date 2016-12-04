# (c) 2005 Bio-Tree Systems, Inc.  All rights reserved.
# No part of this software may be used, reproduced or transmitted for
# any purpose, in any form or by any means, except as expressly
# permitted in writing by Bio-Tree Systems, Inc.

# Add conditional for MPI C++ bindings
set (MPI_CCFLAGS)
if (MPI_CPP_BINDING)
    set (MPI_CCFLAGS -DMPI_CPP_BINDING)
endif (MPI_CPP_BINDING)

# Find MPI related paths and set up includes, libraries, and other defintions based
# on platform and selections.
if (WIN32)
    # On windows platform default paths can be found in registry (assuming MPICH2 was installed)
    find_path (MPI_INCLUDE_DIR mpi.h 
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MPICH2;Path]/include" 
# added the following just to compile ok on the local machine
      "C:/Program Files (x86)/MPICH/SDK/Include/"              
      DOC "What is the path to the MPI include files?")
    find_path (MPI_LIBRARY_DIR mpi.lib 
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MPICH2;Path]/lib" 
      DOC "What is the path to the MPI libraries?")
    set (MPI_LIBRARIES mpi.lib)
    # If C++ bindings, add library
    if (MPI_CPP_BINDING)
        # C++ bindings need this library
        set (MPI_LIBRARIES ${MPI_LIBRARIES} cxx.lib)
    endif (MPI_CPP_BINDING)
    # The following link flag is required but generates many unresolved symbols "___delayLoadHelper2@8"
    # It must be manually specified in the "Delay loaded DLLs" Linker input dialog
    # Without this option, executables get a DLL initialization error during launch.
    # set (MPI_LINKFLAGS /DELAYLOAD:"MPICH2MPI.DLL")
    set (MPI_LINKFLAGS)
else (WIN32)
    # Linux platform "cross-builds" for CCV cluster by default.
    option (BUILD_CLUSTERING_FOR_CCV "Build cluster support for CCV" YES)
    if (BUILD_CLUSTERING_FOR_CCV)
        # By default we use the Myrinet includes/libraries.
        find_path (MPI_INCLUDE_DIR mpi.h /opt/mpich/myrinet/gnu/include DOC "What is the path to the MPI include files?")
        find_path (MPI_LIBRARY_DIR libmpich.a /opt/mpich/myrinet/gnu/lib DOC "What is the path to the MPI libraries?")
        set (MPI_LIBRARIES mpich)
        option (BUILD_CLUSTERING_FOR_MYRINET "Build cluster support to use CCV Myrinet" YES)
        if (BUILD_CLUSTERING_FOR_MYRINET)
            # Myrinet requires the gm libraries
            find_path (GM_LIBRARY_DIR libgm.a /opt/gm/lib DOC "What is the path to the GM libraries?")
            set (MPI_LIBRARIES ${MPI_LIBRARIES} gm)
            set (MPI_LIBRARY_DIR ${MPI_LIBRARY_DIR} ${GM_LIBRARY_DIR})
        endif (BUILD_CLUSTERING_FOR_MYRINET)
        # These flags are used by the mpiCC/mpicc commands
    else (BUILD_CLUSTERING_FOR_CCV)
        # When not cross-building, we assume that version 1.2.7 is being used.
        find_path (MPI_INCLUDE_DIR mpi.h 
          "/opt/mpich-1.2.7/include" 
          "/opt/mpich/gnu/include"
          DOC "What is the path to the MPI include files?")
        find_path (MPI_LIBRARY_DIR libmpich.a 
          "/opt/mpich-1.2.7/lib"
          "/opt/mpich/gnu/lib"
          DOC "What is the path to the MPI libraries?")
        set (MPI_LIBRARIES mpich pthread rt)
    endif (BUILD_CLUSTERING_FOR_CCV)
    set (MPI_LINKFLAGS)
    set (MPI_CCFLAGS ${MPI_CCFLAGS} -DUSE_STDARG -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_UNISTD_H=1 -DHAVE_STDARG_H=1 -DUSE_STDARG=1 -DMALLOC_RET_VOID=1)
    if (MPI_CPP_BINDING)
        # C++ bindings require additional includes, libraries, and switches
        set (MPI_INCLUDE_DIR ${MPI_INCLUDE_DIR} ${MPI_INCLUDE_DIR}/mpi2c++)
        set (MPI_LIBRARIES pmpich++ ${MPI_LIBRARIES})
        set (MPI_CCFLAGS ${MPI_CCFLAGS} -DHAVE_MPI_CPP)
    endif (MPI_CPP_BINDING)
endif (WIN32)