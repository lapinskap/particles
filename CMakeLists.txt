cmake_minimum_required(VERSION 3.12)
project(Molecule)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE molecule_src ./Molecule CONFIGURE_DEPENDS "*.cpp")
add_executable(Molecule WIN32 ${molecule_src})
set_target_properties(Molecule PROPERTIES LINKER_LANGUAGE CXX)