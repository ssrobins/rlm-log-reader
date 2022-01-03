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

add_executable(RLMLogReaderTest)

target_sources(RLMLogReaderTest PRIVATE
    UnitTests.cpp
    IntegrationTests.cpp
    TestConfig.h.in
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/TestConfig.h"
)

target_include_directories(RLMLogReaderTest PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(RLMLogReaderTest
    CONAN_PKG::boost
    CONAN_PKG::gtest
    Data
)

add_test(NAME RLMLogReaderTest COMMAND RLMLogReaderTest)

# Remove and remake the test results directory so old test results don't cause false positives
add_custom_command(TARGET RLMLogReaderTest
                   PRE_BUILD
                   COMMAND cmake -E remove_directory \"${CMAKE_CURRENT_BINARY_DIR}/TestResults\"
                   COMMAND cmake -E make_directory \"${CMAKE_CURRENT_BINARY_DIR}/TestResults\")

# Run unit tests after the build
add_custom_command(TARGET RLMLogReaderTest
                   POST_BUILD
                   COMMAND RLMLogReaderTest)
