cmake_minimum_required(VERSION 3.5)

project("cbe-tutorial" VERSION 0.1 LANGUAGES CXX C)

set(PROJECT_VERSION "")
set(PROJECT_VERSION_MAJOR "")
set(PROJECT_VERSION_MINOR "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILER ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set(CMAKE_THREAD_PREFER_PTREADF ON)
set(THREADS_PREFER_PTREAD_FLAG ON)
find_package(Threads REQUIRED)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
   "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Wextra \
                       -Wno-unused-parameter" )
endif()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
   "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Weffc++ \
                         -Wno-unused-parameter")
endif()

set(
  COMMON_SOURCES
  Main.cpp
)

add_executable(
  ${PROJECT_NAME}-3
  
  ${COMMON_SOURCES}
  Logic3.cpp
)
target_include_directories(${PROJECT_NAME}-3 PUBLIC "../../include")
target_link_directories(${PROJECT_NAME}-3 PUBLIC "../../lib/Linux_x86")

target_link_libraries(
  ${PROJECT_NAME}-3
  
  cb_sdk
)

add_custom_target(
  run-3
  
  COMMAND ${PROJECT_NAME}-3
) 
