cmake_minimum_required(VERSION 3.4)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

project(arangodb-php-driver)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#########################################################################################
# Configuration
option(ARANGODB_TESTS    "Build Tests" OFF)
option(ARANGODB_EXAMPLES "Build EXAMPLES" OFF)

#########################################################################################
# Dependencies
set(VELOCYPACK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/velocypack)

if(PHPCPP_SOURCE_DIR)
    add_subdirectory(${PHPCPP_SOURCE_DIR} ./build)
    set(PHPCPP_LIBRARIES phpcpp)
else()
    include(FindPhpcpp)
    find_package(Phpcpp)

    if (NOT ${PHPCPP_FOUND})
        message(FATAL_ERROR "Phpcpp not found. Clone from https://github.com/arangodb/phpcpp and install it")
    endif()
endif()

option(BuildVelocyPackExamples "Build examples" OFF)
option(BuildTools "Build support programs and tools" OFF)

#include_directories(/usr/local/include/php /usr/local/include/php/TSRM /usr/local/include/php/main /usr/local/include/php/Zend)

add_subdirectory(deps/fuerte)
#add_subdirectory(deps/phpcpp)

## arangodb-php-driver
add_library(arangodb SHARED
    src/connection.cpp
    src/main.cpp
    src/request.cpp
    src/response.cpp
    src/vpack.cpp
)

# Gives our library file a .node extension without any "lib" prefix
#set_target_properties(arangodb PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon,
# You should add this line in every CMake.js based project
#target_include_directories(arangodb PRIVATE ${CMAKE_JS_INC})

target_include_directories(arangodb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Essential library files to link to a node addon
# You should add this line in every CMake.js based project
#target_link_libraries(arangodb ${CMAKE_JS_LIB} fuerte)

target_link_libraries(arangodb
    ${PHPCPP_LIBRARIES}
    fuerte
)

#########################################################################################
# Clang Completion
#copy compile commands to source dir
if( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    message(STATUS "copy compile_commands.json")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()

execute_process(COMMAND php-config --extension-dir OUTPUT_VARIABLE EXTENSION_DIR)
# remove new lines
string(REGEX REPLACE "\n$" "" EXTENSION_DIR "${EXTENSION_DIR}")

message("Installing ArangoDB PHP Driver to ${EXTENSION_DIR}")

# Install
install(TARGETS arangodb LIBRARY DESTINATION ${EXTENSION_DIR})
