// Copyright 2014 Steve Robinson
//
// This file is part of RLM Log Reader.
//
// RLM Log Reader is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RLM Log Reader is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RLM Log Reader.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <chrono>
#include <vector>
#include <string>


void loadDataFromFile(const std::string& filePath, std::vector<std::string>& fileData);

void tokenizeString(const std::string& delimiter,
                    const std::string& rawEventData,
                    std::vector<std::string>& tokens);

void untokenizeString(const std::string& delimiter,
                    std::string& rawEventData,
                    const std::vector<std::string>& tokens);

void getUniqueItems(std::string& item, std::vector<std::string>& uniqueItems);

void write2DVectorToFile(const std::string filePath,
                         const std::vector<std::vector<std::string>>& data,
                         const std::string delimiter);

void findReplaceAll(const std::string oldPattern,
                    const std::string newPattern,
                    std::string& rawData);

void setVectorToZero(std::vector<size_t>& vector);
void setMatrixToZero(std::vector<std::vector<size_t>>& matrix);

std::string getFilenameFromFilepath(const std::string& filepath);

std::chrono::time_point<std::chrono::system_clock> stringToTime(const std::string& dateString, const std::string& timeString);

std::string durationToHHMMSS(std::chrono::nanoseconds duration);

template <typename T>
std::string toString(T& value)
{
    std::stringstream sstream;
    sstream << value;
    std::string str = sstream.str();

    return str;
}

void parseDataInto2DVector(const std::vector<std::string>& rawData,
                           std::vector< std::vector<std::string> >& allData);

void getFileListInDirectory(const std::string& directory, std::vector<std::string>& fileList);

bool fileExists(const std::string& filePath);
