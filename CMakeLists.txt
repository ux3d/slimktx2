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

# specify the public headers (will be copied to `include` in install step)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${slimktx2_public_headers}")


# optionally add basis universal
if(SLIMKTX2_USE_BASISU)
    # define the preprocessor definition SLIMKTX2_USE_BASISU
    add_definitions(-DSLIMKTX2_USE_BASISU)

    # construct the name of the basis_universal library (depending on the platform)
    if(UNIX)
        set(basisu_library_name libbasisu.a)
    else()
        # DLL systems
        set(basisu_library_name basisu.lib)
    endif()

    # use a custom branch of basisu, which allows it being built as library
    include(ExternalProject)
    ExternalProject_Add(basis_universal
      PREFIX thirdparty/basisu
      GIT_REPOSITORY    git@github.com:ux3d/basis_universal.git
      GIT_TAG           origin/build_library
      CMAKE_ARGS -DBUILD_AS_LIBRARY=TRUE -DSTATIC=TRUE -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      BUILD_BYPRODUCTS <INSTALL_DIR>/lib/${basisu_library_name}
      )
    ExternalProject_Get_Property(basis_universal install_dir)

    # provide an imported target for basisu
    add_library(basisu STATIC IMPORTED)
    set_target_properties(basisu PROPERTIES IMPORTED_LOCATION ${install_dir}/lib/${basisu_library_name})

    include_directories(${install_dir}/include)
    add_dependencies(basisu basis_universal)

    # link basisu into slimktx2
    target_link_libraries(${PROJECT_NAME} basisu)
endif()


# install target
install(TARGETS ${PROJECT_NAME})
