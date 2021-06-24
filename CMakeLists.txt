cmake_minimum_required(VERSION 3.18)
project(librnd)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#if(NOT EXISTS ${LIBPRIM_DIR})
#  message(FATAL_ERROR "${LIBPRIM_DIR} does not exist")
#endif()
#if(NOT EXISTS ${LIBPRIM_DIR}/include)
#  message(FATAL_ERROR "${LIBPRIM_DIR}/include does not exist")
#endif()
#if(NOT EXISTS ${LIBPRIM_DIR}/lib)
#  message(FATAL_ERROR "${LIBPRIM_DIR}/lib does not exist")
#endif()
find_library(LIBPRIM_A NAMES libprim.a PATHS ${CMAKE_INSTALL_RPATH})
if(LIBPRIM_A MATCHES "NOTFOUND")
  message(FATAL_ERROR "Bad: ${LIBPRIM_A}")
endif()
string(REPLACE lib/libprim.a include/ LIBPRIM_INC ${LIBPRIM_A})
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
