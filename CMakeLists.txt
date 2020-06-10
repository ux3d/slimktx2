cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# options
option(SLIMKTX2_USE_BASISU "use basis_universal to decode compressed ktx2 data" TRUE)

#this project
project(slimktx2)

#lib sources
set(slimktx2_sources
    source/DefaultAllocationCallback.cpp
    source/DefaultConsoleLogCallback.cpp
    source/DefaultFileIOCallback.cpp
    source/DefaultMemoryStreamCallback.cpp
    source/dfd.cpp
    source/format.cpp
    source/kvd.cpp
    source/slimktx2.cpp
    )

set(slimktx2_public_headers
    include/DefaultAllocationCallback.h
    include/DefaultConsoleLogCallback.h
    include/DefaultFileIOCallback.h
    include/DefaultMemoryStreamCallback.h
    include/basislz.h
    include/callbacks.h
    include/dfd.h
    include/format.h
    include/kvd.h
    include/slimktx2.h
    )

if(SLIMKTX2_USE_BASISU)
    set(slimktx2_sources ${slimktx2_sources}
        source/basistranscoder.cpp
        source/basistranscoder.h
        )
endif()


#lib project
add_library(${PROJECT_NAME} SHARED ${slimktx2_sources} ${slimktx2_public_headers})
target_include_directories(${PROJECT_NAME} PUBLIC "include" PRIVATE "source")


if(SLIMKTX2_USE_BASISU)
    add_definitions(-DSLIMKTX2_USE_BASISU)

    include(ExternalProject)
    ExternalProject_Add(basis_universal
      PREFIX thirdparty/basisu
      GIT_REPOSITORY    git@github.com:ux3d/basis_universal.git
      GIT_TAG           origin/build_library
      CMAKE_ARGS -DBUILD_AS_LIBRARY=TRUE -DSTATIC=TRUE -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    )
    ExternalProject_Get_Property(basis_universal install_dir)

    # provide an imported target for basisu
    add_library(basisu STATIC IMPORTED)
    if(UNIX)
        set_target_properties(basisu PROPERTIES IMPORTED_LOCATION ${install_dir}/lib/libbasisu.a)
    else()
        # DLL systems
        set_target_properties(basisu PROPERTIES IMPORTED_LOCATION ${install_dir}/lib/basisu.lib)
    endif()
    add_dependencies(basisu basis_universal)

    include_directories(${install_dir}/include)
    target_link_libraries(${PROJECT_NAME} basisu)
endif()

