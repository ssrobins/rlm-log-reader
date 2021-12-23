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

cmake_minimum_required (VERSION 2.6)

set (project_name "RLMLogReaderTest")
project (${project_name})

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/TestConfig.h.in"
  "${PROJECT_BINARY_DIR}/TestConfig.h"
  )


include_directories(../gtest-1.6.0/include "${PROJECT_BINARY_DIR}")

add_executable(${project_name} UnitTests.cpp IntegrationTests.cpp ../LogData.cpp ../Utilities.cpp TestConfig.h.in)
target_link_libraries (${project_name} ${Boost_LIBRARIES} gtest gtest_main)

add_test(NAME ${project_name} COMMAND ${project_name})

# Remove and remake the test results directory so old test results don't cause false positives
add_custom_command (TARGET ${project_name}
                    PRE_BUILD
                    COMMAND cmake -E remove_directory \"${PROJECT_BINARY_DIR}/TestResults\"
                    COMMAND cmake -E make_directory \"${PROJECT_BINARY_DIR}/TestResults\")

# Run unit tests after the build
add_custom_command (TARGET ${project_name}
                    POST_BUILD
                    COMMAND ${project_name})