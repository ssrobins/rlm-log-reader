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

set(app_title "RLM Log Reader")
set(project_name "RLMLogReader")
project(${project_name} VERSION 1.1.0)

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
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13) # Minimum version of macOS supported by Qt 5
    set(conan_settings -s os.version=${CMAKE_OSX_DEPLOYMENT_TARGET} -s compiler.version=${compiler_version} -s compiler.cppstd=${CMAKE_CXX_STANDARD})
elseif(UNIX)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(compiler clang)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(compiler gcc)
    else()
        message("Unhandled compiler, ${CMAKE_CXX_COMPILER_ID}, Conan install command may fail")
    endif()
    set(conan_settings -s compiler=${compiler} -s compiler.version=${compiler_version} -s compiler.libcxx=libstdc++11 -s compiler.cppstd=${CMAKE_CXX_STANDARD})
elseif(MSVC)
    set(conan_settings -s arch=x86 -s compiler.version=${compiler_version} -s compiler.runtime=MT -s compiler.cppstd=${CMAKE_CXX_STANDARD})
else()
    message("Unhandled platform, Conan install command may fail")
endif()

option(CONAN_BUILD_MISSING_PKGS "Build missing Conan packages" ON)
if(CONAN_BUILD_MISSING_PKGS)
    set(conan_build_option --build=missing)
endif()

set(conan_options
    -o boost:without_chrono=True
    -o boost:without_container=True
    -o boost:without_context=True
    -o boost:without_contract=True
    -o boost:without_coroutine=True
    -o boost:without_coroutine=True
    -o boost:without_exception=True
    -o boost:without_fiber=True
    -o boost:without_graph=True
    -o boost:without_iostreams=True
    -o boost:without_json=True
    -o boost:without_locale=True
    -o boost:without_log=True
    -o boost:without_math=True
    -o boost:without_nowide=True
    -o boost:without_program_options=True
    -o boost:without_random=True
    -o boost:without_regex=True
    -o boost:without_serialization=True
    -o boost:without_stacktrace=True
    -o boost:without_test=True
    -o boost:without_thread=True
    -o boost:without_timer=True
    -o boost:without_type_erasure=True
    -o boost:without_wave=True
    -o qt:qt3d=False
    -o qt:qtactiveqt=False
    -o qt:qtandroidextras=False
    -o qt:qtcharts=False
    -o qt:qtconnectivity=False
    -o qt:qtdatavis3d=False
    -o qt:qtdeclarative=False
    -o qt:qtdoc=False
    -o qt:qtgamepad=False
    -o qt:qtgraphicaleffects=False
    -o qt:qtimageformats=False
    -o qt:qtlocation=False
    -o qt:qtlottie=False
    -o qt:qtmacextras=False
    -o qt:qtmultimedia=False
    -o qt:qtnetworkauth=False
    -o qt:qtpurchasing=False
    -o qt:qtquick3d=False
    -o qt:qtquickcontrols=False
    -o qt:qtquickcontrols2=False
    -o qt:qtquicktimeline=False
    -o qt:qtremoteobjects=False
    -o qt:qtscript=False
    -o qt:qtscxml=False
    -o qt:qtsensors=False
    -o qt:qtserialbus=False
    -o qt:qtserialport=False
    -o qt:qtspeech=False
    -o qt:qtsvg=False
    -o qt:qttools=False
    -o qt:qttranslations=False
    -o qt:qtvirtualkeyboard=False
    -o qt:qtwayland=False
    -o qt:qtwebchannel=False
    -o qt:qtwebengine=False
    -o qt:qtwebglplugin=False
    -o qt:qtwebsockets=False
    -o qt:qtwebview=False
    -o qt:qtwinextras=False
    -o qt:qtx11extras=False
    -o qt:qtxmlpatterns=False
)

execute_process(
    COMMAND conan remote add conancenter https://center.conan.io --insert --force
    COMMAND conan install --install-folder ${CMAKE_BINARY_DIR} --update ${CMAKE_SOURCE_DIR} ${conan_build_option} ${conan_settings} ${conan_options}
    RESULT_VARIABLE conan_result
)
if(conan_result)
    message(FATAL_ERROR "Conan error")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/_install")
set(CPACK_PACKAGE_DIRECTORY "${PROJECT_BINARY_DIR}/_package")

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()

# Boost
find_package(Boost REQUIRED)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_library(Data STATIC)

target_sources(Data PRIVATE
    Exceptions.h
    LogData.cpp
    LogData.h
    Utilities.cpp
)

target_include_directories(Data PUBLIC
    .
)

target_link_libraries(Data
    CONAN_PKG::boost
)

add_executable(${project_name} WIN32 MACOSX_BUNDLE)

target_sources(${project_name} PRIVATE
    icon.rc
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindowConfig.h.in
)

# Configure a header file to pass the app name and version to the source code
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/MainWindowConfig.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MainWindowConfig.h"
)

target_include_directories(${project_name} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${project_name}
    CONAN_PKG::qt
    Data
)

add_subdirectory(Test)

if(APPLE)
    target_link_libraries(Qt5::QCocoaIntegrationPlugin
    INTERFACE
        cups
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5AccessibilitySupport${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5ClipboardSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5FontDatabaseSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5GraphicsSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5ThemeSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
    )

    target_link_libraries(${project_name}
        Qt5::QCocoaIntegrationPlugin
    )
elseif(WIN32)
    target_link_libraries(Qt5::QWindowsIntegrationPlugin
    INTERFACE
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5EventDispatcherSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5FontDatabaseSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5ThemeSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5WindowsUIAutomationSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
    )

    target_link_libraries(${project_name}
        Qt5::QWindowsIntegrationPlugin
    )
elseif(UNIX)
    add_library(Qt5::FontDatabaseSupport STATIC IMPORTED)
    set_target_properties(Qt5::FontDatabaseSupport PROPERTIES
        IMPORTED_LOCATION ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5FontDatabaseSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
    )
    target_link_libraries(Qt5::FontDatabaseSupport
    INTERFACE
        CONAN_PKG::fontconfig
        CONAN_PKG::freetype
    )

    add_library(Qt5::ServiceSupport STATIC IMPORTED)
    set_target_properties(Qt5::ServiceSupport PROPERTIES
        IMPORTED_LOCATION ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5ServiceSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
    )
    target_link_libraries(Qt5::ServiceSupport
    INTERFACE
        Qt5::Core
    )

    target_link_libraries(Qt5::QXcbIntegrationPlugin
    INTERFACE
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5EdidSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
        Qt5::FontDatabaseSupport
        Qt5::ServiceSupport
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5ThemeSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
        ${CONAN_QT_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}Qt5XkbCommonSupport${CMAKE_STATIC_LIBRARY_SUFFIX}
    )

    target_link_libraries(${project_name}
        Qt5::QXcbIntegrationPlugin
    )
endif()

enable_testing()

# Remove and remake the test results directory so old test results don't cause false positives
add_custom_command(TARGET ${project_name}
                   PRE_BUILD
                   COMMAND cmake -E remove_directory \"${CMAKE_INSTALL_PREFIX}\"
                   COMMAND cmake -E remove_directory \"${CPACK_PACKAGE_DIRECTORY}\")


if(APPLE)

    set (MACOSX_BUNDLE_ICON_FILE icon.icns)

    add_custom_target( MacResources
      COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${project_name}.app/Contents/Resources
      
      # Copies the icon file into the Mac OS X bundle
      COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/graphics/${MACOSX_BUNDLE_ICON_FILE}
            ${CMAKE_CURRENT_BINARY_DIR}/${project_name}.app/Contents/Resources/${MACOSX_BUNDLE_ICON_FILE}
    )

    add_dependencies(${project_name} MacResources)

endif()


# Set up installation

set(install_dir "${project_name}")

install(TARGETS ${project_name} DESTINATION ${install_dir})

# Install documentation and sample log files
install(FILES
    COPYING.html
    README.html
    Test/TestFiles/SampleLog_Report.log
    Test/TestFiles/SampleLog_ISV.log
    DESTINATION ${install_dir})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(bitness "64")
else()
    set(bitness "32")
endif()

if(WIN32)
    set(executable_name "${project_name}.exe")
    set(CPACK_GENERATOR "ZIP")
    set(platform "Windows")
    set(bitness "") # The build works on 32-bit and 64-bit so we can remove the "bitness" label for Windows
elseif(APPLE)
    set(executable_name "${project_name}.app")
    set(CPACK_GENERATOR "ZIP")
    set(platform "Mac")
else()
    set(executable_name "${project_name}")
    set(CPACK_GENERATOR "TGZ")
    set(platform "Linux")
endif()


# Settings for the installation package
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_PACKAGE_FILE_NAME "${project_name}_${PROJECT_VERSION}_${platform}${bitness}")

include(CPack)
