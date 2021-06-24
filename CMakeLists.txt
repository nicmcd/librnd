cmake_minimum_required(VERSION 3.10)
project(librnd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# libprim
find_library(LIBPRIM_A NAMES libprim.a PATHS ${CMAKE_INSTALL_RPATH})
if(LIBPRIM_A MATCHES "NOTFOUND")
  message(FATAL_ERROR "libprim.a not found")
endif()
string(REPLACE lib/libprim.a include/ LIBPRIM_INC ${LIBPRIM_A})
if(NOT EXISTS ${LIBPRIM_INC})
  message(FATAL_ERROR "libprim include dir doesn't exist")
endif()

add_library(
  rnd
  STATIC
  ${PROJECT_SOURCE_DIR}/src/rnd/Random.cc
  ${PROJECT_SOURCE_DIR}/src/rnd/Queue.h
  ${PROJECT_SOURCE_DIR}/src/rnd/Random.h
  ${PROJECT_SOURCE_DIR}/src/rnd/Queue.tcc
  ${PROJECT_SOURCE_DIR}/src/rnd/Random.tcc
  )
set_target_properties(
  rnd
  PROPERTIES
  LINKER_LANGUAGE
  CXX
  )
target_include_directories(
  rnd
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  ${LIBPRIM_INC}
  )
target_link_libraries(
  rnd
  ${LIBPRIM_A}
  )
include(GNUInstallDirs)
install(
  FILES
  ${PROJECT_SOURCE_DIR}/src/rnd/Queue.h
  DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR}/rnd/
  )
install(
  FILES
  ${PROJECT_SOURCE_DIR}/src/rnd/Random.h
  DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR}/rnd/
  )
install(
  FILES
  ${PROJECT_SOURCE_DIR}/src/rnd/Queue.tcc
  DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR}/rnd/
  )
install(
  FILES
  ${PROJECT_SOURCE_DIR}/src/rnd/Random.tcc
  DESTINATION
  ${CMAKE_INSTALL_INCLUDEDIR}/rnd/
  )
install(
  TARGETS
  rnd
  )
