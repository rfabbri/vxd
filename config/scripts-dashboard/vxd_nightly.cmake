# VXL Example Dashboard Script

set(CTEST_DASHBOARD_ROOT "/projects/dashboards/vxd")
set(CTEST_SCRIPT_DIRECTORY "/projects/dashboards/vxd/scripts")
set(CTEST_SOURCE_DIRECTORY "/projects/dashboards/vxd/src")
set(CTEST_BINARY_DIRECTORY "/projects/dashboards/vxd/bin-nightly")

# remove build directory because this is not a continuous build
#set(CTEST_START_WITH_EMPTY_BINARY_DIRECTORY TRUE)
set(CTEST_SITE "github.com")
set(CTEST_BUILD_NAME "LINUX - githubserver")
set(CTEST_BUILD_FLAGS "-j4") # parallel build for makefiles
set(CTEST_BUILD_CONFIGURATION Release)
set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
set(CTEST_UPDATE_COMMAND /usr/bin/svn)

set(dashboard_url "https://githubserver.com/svn/vxd/trunk")
set(dashboard_model Nightly)

set(dashboard_cache "
CMAKE_GENERATOR:INTERNAL=Unix Makefiles
CMAKE_BUILD_TYPE:STRING=Release
BUILD_FOR_VXL_DASHBOARD:BOOL=YES
CMAKE_C_FLAGS:STRING=-Wall -fPIC
CMAKE_CXX_FLAGS:STRING=-Wall -fPIC
VXL_DIR:FILEPATH=/projects/dashboards/vxl/bin
")

include(${CTEST_SCRIPT_DIRECTORY}/vxd_common.cmake)
