cmake_minimum_required(VERSION 3.24)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(pybind11)
include_directories(include pybind11/include)
#pybind11_add_module()
#add_library(untitled SHARED library.cpp shtmItem.h shtmTreeItem.h utils.h shtmTreeItem.cpp SimpleHtmlDocTree.cpp
#        SimpleHtmlDocTree.h simdjson.cpp simdjson.h)
add_executable(library_test library.cpp shtmItem.h shtmTreeItem.h utils.h shtmTreeItem.cpp SimpleHtmlDocTree.cpp
        SimpleHtmlDocTree.h errors.h)
#pybind11_add_module(mylibrary library.cpp shtmItem.h shtmTreeItem.h utils.h shtmTreeItem.cpp SimpleHtmlDocTree.cpp
#        SimpleHtmlDocTree.h errors.h reading_bind.cpp)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

pybind11_add_module(example example.cpp library.cpp shtmItem.h shtmTreeItem.h utils.h shtmTreeItem.cpp SimpleHtmlDocTree.cpp
        SimpleHtmlDocTree.h errors.h)
#add_library(myLib SHARED ${SOURCE_FILES})
#set(CMAKE_EXE_LINKER_FLAGS " -static")
#target_link_libraries(example -static-libgcc -static-libstdc++)
