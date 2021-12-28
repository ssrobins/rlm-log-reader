# Copyright 2014 Steve Robinson
#
# This file is part of RLM Log Reader.
#
# RLM Log Reader is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RLM Log Reader is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with RLM Log Reader.  If not, see <http:#www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.21)

set (app_title "RLM Log Reader")
set (project_name "RLMLogReader")
project (${project_name})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    string(REGEX MATCH "[0-9]+\.[0-9]+" compiler_version ${CMAKE_CXX_COMPILER_VERSION})
elseif(MSVC)
    string(REGEX MATCH "[0-9]+" compiler_version ${CMAKE_GENERATOR})
else()
    string(REGEX MATCH "[0-9]+" compiler_version ${CMAKE_CXX_COMPILER_VERSION})
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND ${compiler_version} LESS 8)
        string(REGEX MATCH "[0-9]+\.[0-9]+" compiler_version ${CMAKE_CXX_COMPILER_VERSION})
    endif()
endif()

if(APPLE)
    #set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13) # Minimum version of macOS supported by Qt 5
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.14) # Minimum version of macOS supported by Qt 6
    set(conan_args -s os.version=${CMAKE_OSX_DEPLOYMENT_TARGET} -s compiler.version=${compiler_version} -s compiler.cppstd=${CMAKE_CXX_STANDARD})
elseif(UNIX)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(compiler clang)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(compiler gcc)
    else()
        message("Unhandled compiler, ${CMAKE_CXX_COMPILER_ID}, Conan install command may fail")
    endif()
    set(conan_args -s compiler=${compiler} -s compiler.version=${compiler_version} -s compiler.libcxx=libstdc++11 -s compiler.cppstd=${CMAKE_CXX_STANDARD})
elseif(MSVC)
    set(conan_args -s compiler.version=${compiler_version} -s compiler.runtime=MT -s compiler.cppstd=${CMAKE_CXX_STANDARD})
else()
    message("Unhandled platform, Conan install command may fail")
endif()

option(CONAN_BUILD_MISSING_PKGS "Build missing Conan packages" ON)
if(CONAN_BUILD_MISSING_PKGS)
    set(conan_build_option --build=missing)
endif()

execute_process(
    COMMAND conan remote add conancenter https://center.conan.io --insert --force
    COMMAND conan install --install-folder ${CMAKE_BINARY_DIR} --update ${CMAKE_SOURCE_DIR} ${conan_build_option} ${conan_args}
    RESULT_VARIABLE conan_result
)
if(conan_result)
    message(FATAL_ERROR "Conan error")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 1)
set (VERSION_PATCH 0)

set (CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/_install")
set (CPACK_PACKAGE_DIRECTORY "${PROJECT_BINARY_DIR}/_package")

# Configure a header file to pass the app name and version to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/MainWindowConfig.h.in"
  "${PROJECT_BINARY_DIR}/MainWindowConfig.h"
  )

# Boost
find_package(Boost REQUIRED)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Source files
set (SourceFiles main.cpp icon.rc MainWindow.cpp LogData.cpp Utilities.cpp)

# Header files
set (HeaderFiles LogData.h MainWindow.h MainWindowConfig.h.in Exceptions.h Utilities.h)

# Any header file with Qt's Q_OBJECT in it must be wrapped
#QT4_WRAP_CPP(QtMockSources MainWindow.h)

# Qt UI files
#QT4_WRAP_UI(QtUIFiles MainWindow.ui)

# Qt resources
#QT4_ADD_RESOURCES(QtResources graphics.qrc)

# Include the current binary directory so code that uses auto-generated ui headers can be built
# Include the project binary directory so the MainWindowConfig.h file can be found
include_directories(${CMAKE_CURRENT_BINARY_DIR} "${PROJECT_BINARY_DIR}")

# GTest
#set (gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
#add_subdirectory (gtest-1.6.0)
add_subdirectory (Test)
#include_directories(gtest-1.6.0/include)


# Combination of WIN32 in add_executable and ${QT_QTMAIN_LIBRARY} in target_link_libraries allows the application to run on Windows without displaying the command prompt
# Addition of MACOSX_BUNDLE in add_executable wraps the program in a .app package, which the user can double-click on to launch the program
add_executable (${project_name} WIN32 MACOSX_BUNDLE ${SourceFiles} ${HeaderFiles} ${QtResources} ${QtMockSources} ${QtUIFiles})
target_link_libraries(${project_name}
    CONAN_PKG::boost
    CONAN_PKG::qt
    ${QT_LIBRARIES}
    ${QT_QTMAIN_LIBRARY}
)


enable_testing ()

# Remove and remake the test results directory so old test results don't cause false positives
add_custom_command (TARGET ${project_name}
                    PRE_BUILD
                    COMMAND cmake -E remove_directory \"${CMAKE_INSTALL_PREFIX}\"
                    COMMAND cmake -E remove_directory \"${CPACK_PACKAGE_DIRECTORY}\")


if (APPLE)

    set (MACOSX_BUNDLE_ICON_FILE icon.icns)

    add_custom_target( MacResources
      COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${project_name}.app/Contents/Resources
      
      # Copies the icon file into the Mac OS X bundle
      COMMAND cp ${PROJECT_SOURCE_DIR}/graphics/${MACOSX_BUNDLE_ICON_FILE}
            ${CMAKE_CURRENT_BINARY_DIR}/${project_name}.app/Contents/Resources/${MACOSX_BUNDLE_ICON_FILE}
      
      # Copies the qt.conf file into the Mac OS X bundle to stop the application from crashing at startup
      COMMAND cp ${PROJECT_SOURCE_DIR}/qt.conf
            ${CMAKE_CURRENT_BINARY_DIR}/${project_name}.app/Contents/Resources/qt.conf
    )

    add_dependencies(${project_name} MacResources)

endif (APPLE)


# Set up installation

set (install_dir "${project_name}")

# Install the C++ runtime libraries
set (CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${install_dir})
include (InstallRequiredSystemLibraries)

install (TARGETS ${project_name} DESTINATION ${install_dir})

# Install documentation and sample log files
install (FILES
    COPYING.html
    README.html
    Test/TestFiles/SampleLog_Report.log
    Test/TestFiles/SampleLog_ISV.log
    DESTINATION ${install_dir})

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set (bitness "64")
else ()
    set (bitness "32")
endif()

if (WIN32)
    set (executable_name "${project_name}.exe")
    set (CPACK_GENERATOR "ZIP")
    set (platform "Windows")
    set (bitness "") # The build works on 32-bit and 64-bit so we can remove the "bitness" label for Windows
elseif (APPLE)
    set (executable_name "${project_name}.app")
    set (CPACK_GENERATOR "ZIP")
    set (platform "Mac")
else ()
    set (executable_name "${project_name}")
    set (CPACK_GENERATOR "TGZ")
    set (platform "Linux")
endif ()

# Leading backslash is required, which keeps ${CMAKE_INSTALL_PREFIX} from being converted to its variable value.
# Otherwise, the Qt libraries don't get copied into the package in the fixup_bundle command.
# No idea why!
set (APPS "\${CMAKE_INSTALL_PREFIX}/${install_dir}/${executable_name}")  # paths to executables
set (DIRS ${QT_LIBRARY_DIR})   # directories to search for prerequisites
install (CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   " COMPONENT Runtime)


# Settings for the installation package
set (CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "0")
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set (CPACK_PACKAGE_FILE_NAME "${project_name}_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}_${platform}${bitness}")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "${project_name}_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}_source")
set (CPACK_SOURCE_IGNORE_FILES "/.git/;.gitignore;/ExtraTestFiles/")

# Make a custom target for building the source package so it can easily be run on Windows
add_custom_target(PACKAGE_SOURCE
                  COMMAND cpack --config ${PROJECT_BINARY_DIR}/CPackSourceConfig.cmake
                  )
include (CPack)