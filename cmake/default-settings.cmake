# Top level Variables

if(NOT CCS_ARCH)
    set (CCS_ARCH "i86" CACHE STRING "Set CCS build architecture")
    message (STATUS "Variable CCS_ARCH unset. Setting to default value.")
endif()
set_property(CACHE CCS_ARCH PROPERTY STRINGS "i86" "x64")
message(STATUS "ARCH is: ${CCS_ARCH}")

# For FindRTIConnextDDS to work
set(CONNEXTDDS_DIR "/opt/rti_connext_dds-5.3.1"
    CACHE PATH "RTI DDS root directory")
set(CONNEXTDDS_DISABLE_VERSION_CHECK TRUE 
    CACHE BOOL "skip RTI DDS version checks")
# RTI will not link to their shared libs unless this is true
set(BUILD_SHARED_LIBS TRUE 
    CACHE BOOL "Globally build shared libs by default")
message(STATUS "Setting BUILD_SHARED_LIBS to 'TRUE' by default.")

if(CCS_ARCH STREQUAL "x64")
    set (CONNEXTDDS_ARCH x64Linux3gcc4.8.2 
            CACHE STRING "Set RTI DDS architecture")
  LIST (APPEND CMAKE_PROGRAM_PATH  "/usr/lib64/qt4/bin/") # For FindQt to work
else()
    set (CONNEXTDDS_ARCH i86Linux3gcc4.8.2 
            CACHE STRING "Set RTI DDS architecture")
  LIST (APPEND CMAKE_PROGRAM_PATH  "/usr/lib/qt4/bin/")
endif()

# function to set the correct architure flags on a target
function(set_target_architecture TARGET)
  if (CCS_ARCH STREQUAL "i86")
    set_target_properties(${TARGET}
        PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
  endif()
endfunction()

# set(CMAKE_INSTALL_RPATH $ORIGIN)
# set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message (STATUS "Setting build type to 'Release' as none was specified.")
  set (CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(
      CACHE CMAKE_BUILD_TYPE
      PROPERTY STRINGS
      "Debug"
      "Release"
      "MinSizeRel"
      "RelWithDebInfo")
endif()

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED YES)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# This function tries to prevent in-source builds
function(AssureOutOfSourceBuilds)
    # make sure the user doesn't play dirty with symlinks
    get_filename_component(srcdir "${PROJECT_SOURCE_DIR}" REALPATH)
    get_filename_component(bindir "${PROJECT_BINARY_DIR}" REALPATH)

    # disallow in-source builds
    if("${srcdir}" STREQUAL "${bindir}")
        message("######################################################")
        message("Warning: in-source builds are disabled")
        message("Please create a separate build directory and run `cmake ..` from there")
        message("NOTE: cmake has created CMakeCache.txt and CMakeFiles/*.")
        message("You must delete them, or cmake will refuse to work.")
        message("######################################################")
        message(FATAL_ERROR "Quitting configuration")
    endif()
endfunction()

assureoutofsourcebuilds()
