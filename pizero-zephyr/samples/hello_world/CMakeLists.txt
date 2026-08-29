# CMake documentation: https://cmake.org/cmake/help/latest/index.html
# Specify the minimum required CMake version
cmake_minimum_required(VERSION 3.20.0)

# Find the Zephyr package
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Set the project name
project(hello_world
        VERSION 1.0.0
        DESCRIPTION "Hello World Application"
        HOMEPAGE_URL "https://github.com/pizero-zephyr/pizero-zephyr"
)

# Add the source files
target_sources(app PRIVATE src/main.c)

# Example
# Add a library
# add_library(my_library STATIC src/my_library.c)

# Set the include directories for the library. PUBLIC adds the directory
# to the search path for any targets that link to this library.
# target_include_directories(my_library PUBLIC include)
# target_include_directories(my_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# target_link_libraries(my_library PUBLIC other_library)
