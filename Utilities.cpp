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

#include "date/date.h"
#include "Utilities.h"
#include "Exceptions.h"
#include "qdir.h"
#include <fstream>
#include <string>
#include <vector>


void loadDataFromFile(const std::string& filePath, std::vector<std::string>& fileData)
{
    std::string line;
    std::ifstream myfile (filePath.c_str());
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
            getline (myfile,line);

            // Remove extra line break, if present
            findReplaceAll("\r","", line);

            fileData.push_back(line);
        }
        myfile.close();
    }
    else
    {
        CannotOpenFileException cannotOpenFileException(filePath);
        throw cannotOpenFileException;
    }
}

void tokenizeString(const std::string& delimiter,
                    const std::string& str,
                    std::vector<std::string>& tokens)
{
    size_t startPos = 0;
    size_t endPos = 0;
    tokens.clear();
    bool withinQuotes = false;
    std::string localDelimiter = delimiter;

    do
    {
        startPos = str.find_first_not_of(localDelimiter, startPos);
        endPos = str.find_first_of(localDelimiter, startPos);

        if (startPos != std::string::npos)
        {
            if (str.at(startPos) == '"')
            {
                withinQuotes = true;
                startPos = startPos + 1;
                localDelimiter = "\"";
                endPos = str.find_first_of(localDelimiter, startPos);
                if (endPos != std::string::npos)
                    localDelimiter = delimiter;
            }

            tokens.push_back(str.substr(startPos, endPos - startPos));

            if (withinQuotes)
                endPos = endPos + 1;

            withinQuotes = false;
        }

        startPos = endPos;
    }
    while (endPos != std::string::npos);
}

void untokenizeString(const std::string& delimiter,
                      std::string& str,
                      const std::vector<std::string>& tokens)
{
    str.clear();
    for (size_t row = 0; row < tokens.size(); ++row)
    {
        str.append(tokens.at(row));
        if (row <= tokens.size())
        {
            str.append(delimiter);
        }
    }
}

void getUniqueItems(std::string& itemName, std::vector<std::string>& uniqueItems)
{
    bool duplicate = false;
    for (size_t item=0; item < uniqueItems.size(); ++item)
    {
        if (itemName == uniqueItems.at(item))
        {
            duplicate = true;
            break;
        }
    }

    if (! duplicate)
    {
        uniqueItems.push_back(itemName);
    }
}

void write2DVectorToFile(const std::string filePath,
                         const std::vector<std::vector<std::string>>& data,
                         const std::string delimiter)
{
    std::ofstream myfile;
    myfile.open (filePath.c_str());
    if (myfile.is_open())
    {
        for (size_t row = 0; row<data.size(); ++row)
        {
            size_t columnSize = data.at(row).size();
            for (size_t col = 0; col<columnSize; ++col)
            {
                myfile << data.at(row).at(col);
                if (col != columnSize-1)
                {
                    myfile << delimiter;
                }
            }
            myfile << "\n";
        }
        myfile.close();
    }
    else
    {
        CannotOpenFileException cannotOpenFileException(filePath);
        throw cannotOpenFileException;
    }
}


void findReplaceAll(const std::string oldPattern,
                    const std::string newPattern,
                    std::string& data)
{
    size_t found = 0;
    size_t oldPatternSize = oldPattern.size();

    while(true)
    {
        found = data.find(oldPattern, found);
        if (found != std::string::npos)
        {
            data.erase(found, oldPatternSize);
            data.insert(found, newPattern);
        }
        else
        {
            break;
        }
        ++found;
    }
}

void setVectorToZero(std::vector<size_t>& vector)
{
    for (size_t index=0; index<vector.size(); ++index)
    {
        vector.at(index) = 0;
    }
}

void setMatrixToZero(std::vector<std::vector<size_t>>& matrix)
{
    for (size_t row=0; row < matrix.size(); ++row)
    {
        for (size_t col=0; col < matrix.at(row).size(); ++col)
        {
            matrix.at(row).at(col) = 0;
        }
    }
}

std::string getFilenameFromFilepath(const std::string& filepath)
{
    std::string filename = filepath;

    size_t found = filename.find_last_of("/\\");
    filename = filename.substr(found+1);

    found = filename.find_last_of(".");
    filename = filename.substr(0, found);

    return filename;
}

std::chrono::time_point<std::chrono::system_clock> stringToTime(const std::string& dateString, const std::string& timeString)
{
    std::vector<std::string> timeVector;
    tokenizeString(":", timeString, timeVector);
    std::string hours = timeVector.at(0).c_str();
    std::string minutes = timeVector.at(1).c_str();
    std::string seconds;

    if (timeVector.size() < 3)
    {
        seconds = "00";
    }
    else
    {
        seconds = timeVector.at(2).c_str();
    }

    const std::string datetime = dateString + " " + hours + ":" + minutes + ":" + seconds;
    std::stringstream in{datetime};
    std::chrono::time_point<std::chrono::system_clock> tp;
    using namespace date;
    in >> date::parse("%m/%d/%Y %T", tp);

    return tp;
}

std::string durationToHHMMSS(std::chrono::nanoseconds duration)
{
    auto durationFloorSeconds = date::floor<std::chrono::seconds>(duration);
    using namespace date;
    std::ostringstream durationStream;
    durationStream << date::format("%T", durationFloorSeconds);

    return durationStream.str();
}

void parseDataInto2DVector(const std::vector<std::string>& rowData,
                           std::vector< std::vector<std::string> >& parsedData)
{
    std::string delimiter = " ";
    std::vector<std::string> eventLine;
    std::string rawLine;

    for (size_t line=0; line<rowData.size(); ++line)
    {
        tokenizeString(delimiter, rowData.at(line), eventLine);
        parsedData.push_back(eventLine);
    }
}


void getFileListInDirectory(const std::string& directory, std::vector<std::string>& fileList)
{
    if (QDir(directory.c_str()).exists())
    {
        std::string filePath;

        QStringList entries = QDir(directory.c_str()).entryList();
        for(QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry)
        {
            if (!QDir(*entry).exists())
            {
                filePath = QDir((*entry)).absolutePath().toLocal8Bit().constData();
                fileList.push_back(filePath);
            }
        }
    }
}


bool fileExists(const std::string& filePath)
{
    std::ifstream ifile(filePath.c_str());
    return (ifile.is_open());
}
