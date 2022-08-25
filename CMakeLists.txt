################################################################
# CMakeLists.txt
cmake_minimum_required(VERSION 3.8.2)
project( OpenScenarioClientTester )
message( STATUS "\n${PROJECT_NAME}" )

################################################################
# Preprocessor settings
option(SUPPORT_OSC_1_0 "Build the artifacts supporting OSC standard version 1.0" OFF)
option(SUPPORT_OSC_1_1 "Build the artifacts supporting OSC standard version 1.1" OFF)
option(SUPPORT_OSC_1_2 "Build the artifacts supporting OSC standard version 1.2" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)


################################################################
# Add a module path for cmake modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
include(CMakeHelpers)
# Set common, C++, and build params
RAC_SET_COMMON_PARAM()
RAC_SET_CPP_PARAM()
RAC_SET_BUILD_PARAM()


################################################################
# Build as Debug or Release, for Linux or Windows
if(UNIX)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Debug, Release" FORCE)
    endif(NOT CMAKE_BUILD_TYPE)
    set(BUILD_TARGET_PARAM ${CMAKE_BUILD_TYPE})
endif(UNIX)
          
if(NOT PLATFORM_PARAM)
    set(PLATFORM_PARAM "${CMAKE_SYSTEM_NAME}" CACHE STRING "Linux, Windows, etc" FORCE)
endif(NOT PLATFORM_PARAM)


################################################################
# Preprocessor settings
if( WIN32 )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
else( WIN32 )
    add_definitions( -Wall -fPIC -Wno-unused-variable )
endif( WIN32 )


################################################################
# Include folders
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )


################################################################
# Fetch the osc lib project

Include(FetchContent)
FetchContent_Declare(
    oscLib
    GIT_REPOSITORY  https://github.com/RA-Consulting-GmbH/openscenario.api.test.git
    GIT_TAG         dev
	SOURCE_SUBDIR   cpp
)
	  
FetchContent_MakeAvailable(oscLib)

################################################################
# Header files
set( HEADERS
	  ${HEADERS}
	  "src/TestFilesV1_2.h"
	  "src/TestBseV1_2.h"
)

# Source files
set( SOURCES
    "src/OpenScenarioTester.cpp"
	"src/TestBaseV1_2.cpp"
    "src/TestFilesV1_2.cpp"
)    


# Resource files
if( MSVC )
    set( RESOURCE_FILES
        "rc/${PROJECT_NAME}.rc"
        "rc/resource.h"
    )
    set( VS_RESOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/rc/${PROJECT_NAME}.rc )
endif()


################################################################
# Create groups for VS
if( MSVC )
    # Groups for source files
    source_group( Sources FILES ${SOURCES})
    # Groups for header files
    source_group( Headers FILES ${HEADERS} )
    # Groups for resource files
    source_group( Resources FILES ${RESOURCE_FILES} )
endif()


################################################################
# Generate executable
add_executable( ${PROJECT_NAME}
                ${SOURCES} 				
                ${RESOURCE_FILES} )


if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if( ${BUILD_SHARED_LIBS} )
        target_compile_options(${PROJECT_NAME} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE "/MT$<$<CONFIG:Debug>:d>")
    endif()
endif()

target_link_libraries( ${PROJECT_NAME} OpenScenarioLib )

POST_BUILD_COPY_DEPENDENT_LIB(OpenScenarioLib ${PROJECT_NAME})
POST_BUILD_COPY_DEPENDENT_LIB(ExpressionsLib ${PROJECT_NAME})
POST_BUILD_ADD_COPY_TEST_RESOURCES(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/res)

################################################################
# Use the target property of OpenScenarioLib to copy the antlr 
# runtime shared lib
if (BUILD_SHARED_LIBS)
get_target_property(ANTLR4_RUNTIME_LIBRARIES OpenScenarioLib "ANTLR4_RUNTIME_LIBRARIES")

add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND}
                           -E copy ${ANTLR4_RUNTIME_LIBRARIES}
						   "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif()

################################################################
# Visual Studio solution settings
if( MSVC )
    set_target_properties( ${PROJECT_NAME} PROPERTIES FOLDER Apps )
	set_target_properties( ${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>" )
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "OpenScenarioClientTester")
endif()
