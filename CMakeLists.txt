cmake_minimum_required(VERSION 3.10)
project(librnd)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(NOT EXISTS ${LIBPRIM_DIR})
  message(FATAL_ERROR "${LIBPRIM_DIR} does not exist")
endif()
if(NOT EXISTS ${LIBPRIM_DIR}/include)
  message(FATAL_ERROR "${LIBPRIM_DIR}/include does not exist")
endif()
if(NOT EXISTS ${LIBPRIM_DIR}/lib)
  message(FATAL_ERROR "${LIBPRIM_DIR}/lib does not exist")
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
  ${LIBPRIM_DIR}/include
  )
target_link_libraries(
  rnd
  ${LIBPRIM_DIR}/lib/libprim.a
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
