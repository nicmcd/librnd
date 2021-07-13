cmake_minimum_required(VERSION 3.20)
project(librnd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FindPkgConfig)

# libprim
pkg_check_modules(libprim REQUIRED IMPORTED_TARGET libprim)
  get_target_property(
  LIBPRIM_INC
  PkgConfig::libprim
  INTERFACE_INCLUDE_DIRECTORIES
)

add_library(
  rnd
  SHARED
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
  POSITION_INDEPENDENT_CODE
  ON
  )

target_include_directories(
  rnd
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src
  ${LIBPRIM_INC}
  )

target_link_libraries(
  rnd
  PkgConfig::libprim
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

configure_file(
  ${PROJECT_SOURCE_DIR}/librnd.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/librnd.pc
  @ONLY
  )
install(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/librnd.pc
  DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/pkgconfig/
  )
