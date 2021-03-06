#-----------------------------*-cmake-*----------------------------------------#
# file   mesh/CMakeLists.txt
# author Ryan Wollaeger <wollaeger@lanl.gov>
# date   Thursday, Jun 07, 2018, 15:37 pm
# brief  Generate build project files for mesh.
# note   Copyright (C) 2018, Triad National Security, LLC.
#        All rights reserved.
#------------------------------------------------------------------------------#
cmake_minimum_required(VERSION 3.9.0)
project( mesh CXX )

# ---------------------------------------------------------------------------- #
# Source files
# ---------------------------------------------------------------------------- #

file( GLOB sources *.cc )
file( GLOB explicit_instantiations *_pt.cc )
file( GLOB headers *.hh )

# ---------------------------------------------------------------------------- #
# Build package library
# ---------------------------------------------------------------------------- #

add_component_library(
   TARGET       Lib_mesh
   TARGET_DEPS  "Lib_RTT_Format_Reader;Lib_parser"
   LIBRARY_NAME ${PROJECT_NAME}
   SOURCES      "${sources}"
   HEADERS      "${headers}"
)

# ---------------------------------------------------------------------------- #
# Installation instructions
# ---------------------------------------------------------------------------- #

install( TARGETS Lib_mesh EXPORT draco-targets DESTINATION
  ${DBSCFGDIR}lib )
install( FILES ${headers} DESTINATION ${DBSCFGDIR}include/mesh )
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  install(FILES $<TARGET_PDB_FILE:Lib_mesh> DESTINATION ${DBSCFGDIR}lib
    OPTIONAL)
endif()

# ---------------------------------------------------------------------------- #
# Unit tests
# ---------------------------------------------------------------------------- #

if( BUILD_TESTING )
   add_subdirectory( test )
endif()

# ---------------------------------------------------------------------------- #
# Autodoc
# ---------------------------------------------------------------------------- #

process_autodoc_pages()

# ---------------------------------------------------------------------------- #
# End mesh/CMakeLists.txt
# ---------------------------------------------------------------------------- #
