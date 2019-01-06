cmake_minimum_required(VERSION 2.8)

project(Spinnaker)
# The version number.
set (Spinnaker_VERSION_MAJOR 0)
set (Spinnaker_VERSION_MINOR 1)

# find spinnaker libs
unset(SPINNAKER_FOUND)
unset(SPINNAKER_INCLUDE_DIRS)
unset(SPINNAKER_LIB)
find_path(SPINNAKER_INCLUDE_DIRS NAMES
  Spinnaker.h
  HINTS
  /usr/include/spinnaker/
  /usr/local/include/spinnaker/)
find_library(SPINNAKER_LIB NAMES Spinnaker
    HINTS
    /usr/lib
    /usr/local/lib)
if (SPINNAKER_INCLUDE_DIRS AND SPINNAKER_LIB)
  set(SPINNAKER_FOUND 1)
endif (SPINNAKER_INCLUDE_DIRS AND SPINNAKER_LIB)
if (SPINNAKER_FOUND)
    include_directories(${SPINNAKER_INCLUDE_DIRS})
    message(STATUS ${SPINNAKER_INCLUDE_DIRS} ", " ${SPINNAKER_LIB} " found")
endif(SPINNAKER_FOUND)
set (EXTRA_LIBS ${EXTRA_LIBS} ${SPINNAKER_LIB})

# find opencv sources and libs
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
set (EXTRA_LIBS ${EXTRA_LIBS} ${OpenCV_LIBS})

include_directories(src/)
set( project_sources
   main.cpp
   src/grabber.cpp
   src/ocv_spinnaker.cpp
   src/imfilib.cpp
)

add_executable(${PROJECT_NAME} ${project_sources})
target_link_libraries (${PROJECT_NAME}  ${EXTRA_LIBS})


