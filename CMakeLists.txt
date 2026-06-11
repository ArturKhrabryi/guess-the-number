cmake_minimum_required(VERSION 3.20)
project(guess-the-number VERSION 1.0 LANGUAGES CXX)

file(GLOB SRCS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SRCS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(${PROJECT_NAME} PRIVATE
        -static-libstdc++
        -static-libgcc
    )
endif()
