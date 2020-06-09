cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#this project
project(slimktx2)

macro(add_sources expression sources)
	file(GLOB source_files RELATIVE "${CMAKE_CURRENT_LIST_DIR}" "${expression}")
	set(${sources} "${${sources}};${source_files}")
endmacro()

#lib sources
add_sources("source/*.cpp" "slimktx2_sources")
add_sources("include/*.h" "slimktx2_sources")

#lib project
add_library(${PROJECT_NAME} SHARED ${slimktx2_sources})
target_include_directories(${PROJECT_NAME} PUBLIC "include")


# TODO: make optional
include(ExternalProject)
ExternalProject_Add(basis_universal
  PREFIX thirdparty
  GIT_REPOSITORY    git@github.com:ux3d/basis_universal.git
  GIT_TAG           origin/build_library
  CMAKE_ARGS -DBUILD_AS_LIBRARY=TRUE -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

SET(BASISU_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdpary/src/basis-universal)
SET(BASISU_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/lib)
message(STATUS ${BASISU_LIB_DIR})
add_library(basisu STATIC IMPORTED)
set_target_properties(basisu PROPERTIES IMPORTED_LOCATION ${BASISU_LIB_DIR}/libbasisu.a)
add_dependencies(basisu basis_universal)

target_link_libraries(${PROJECT_NAME} basisu)
add_dependencies(${PROJECT_NAME} basis_universal)

