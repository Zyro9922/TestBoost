cmake_minimum_required(VERSION 3.10)

project(Boost_test_run)

find_package(Boost 1.36.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(astronomy_library /Users/alihasan/Documents/GSOC/astronomy)
    include_directories(${astronomy_library}/include)
    add_executable( Executable horizon_to_equatorial.cpp)
    TARGET_LINK_LIBRARIES( Executable LINK_PUBLIC ${Boost_LIBRARIES} )
endif()