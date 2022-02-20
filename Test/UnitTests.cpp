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
#include "LogData.h"
#include "Utilities.h"
#include "TestConfig.h"
#include "gtest/gtest.h"


void lineBreakTests(std::vector<std::string>& rawData)
{
    ASSERT_EQ(3, rawData.size());
    EXPECT_EQ("First line", rawData.at(0));
    EXPECT_EQ("Second line", rawData.at(1));
    EXPECT_EQ("Third line", rawData.at(2));
}
TEST(loadDataFromFile, WithWindowsLineBreaks)
{
    std::vector<std::string> rawData;
    loadDataFromFile(testInputDirectory + "/TestFileWindows.txt", rawData);
    lineBreakTests(rawData);
}
// Confirmed that the log files exported on the Mac use the UNIX-style line breaks
// So this test applies to Linux, UNIX, and Mac
TEST(loadDataFromFile, WithUNIXLineBreaks)
{
    std::vector<std::string> rawData;
    loadDataFromFile(testInputDirectory + "/TestFileUNIX.txt", rawData);
    lineBreakTests(rawData);
}

TEST(loadDataFromFile, EmptyFile)
{
    std::vector<std::string> rawData;
    loadDataFromFile(testInputDirectory + "/TestFileEmpty.txt", rawData);
    EXPECT_EQ("", rawData.at(0));
}

TEST(loadDataFromFile, CannotFindFile)
{
    std::vector<std::string> rawData;
    std::string inputFilePath = testInputDirectory + "/TestFileThatDoesNotExist.txt";
    std::string errorMessage;
    try
    {
        loadDataFromFile(inputFilePath, rawData);
    }
    catch (std::exception& e)
    {
        errorMessage = e.what();
    }
    EXPECT_EQ("Unable to open file: " + inputFilePath, errorMessage);
}


TEST(tokenizeString, Empty)
{
    std::vector<std::string> tokenVector;
    tokenizeString(" ", "" , tokenVector);
    ASSERT_EQ(0, tokenVector.size());
}

void delimiterTests(std::vector<std::string>& tokenVector)
{
    ASSERT_EQ(3, tokenVector.size());
    EXPECT_EQ("I", tokenVector.at(0));
    EXPECT_EQ("am", tokenVector.at(1));
    EXPECT_EQ("Sam", tokenVector.at(2));
}
TEST(tokenizeString, SingleConsecutiveDelimiter)
{
    std::vector<std::string> tokenVector;
    tokenizeString(" ", "I am Sam", tokenVector);
    delimiterTests(tokenVector);
}
TEST(tokenizeString, AlternateDelimiter)
{
    std::vector<std::string> tokenVector;
    tokenizeString("@", "I@am@Sam", tokenVector);
    delimiterTests(tokenVector);
}
TEST(tokenizeString, MultipleConsecutiveDelimiters)
{
    std::vector<std::string> tokenVector;
    tokenizeString(" ", " I  am   Sam    ", tokenVector);
    delimiterTests(tokenVector);
}
TEST(tokenizeString, IgnoreDelimitersBetweenQuotes)
{
    std::vector<std::string> tokenVector;
    tokenizeString(" ", "\"I am Sam\" \"Sam I am\"", tokenVector);
    ASSERT_EQ(2, tokenVector.size());
    EXPECT_EQ("I am Sam", tokenVector.at(0));
    EXPECT_EQ("Sam I am", tokenVector.at(1));
}


TEST(parseDataVector, Simple)
{
    std::vector<std::string> rawData;
    rawData.push_back("");
    rawData.push_back("I am Sam");
    rawData.push_back("");
    rawData.push_back("");
    rawData.push_back("W00t!");
    std::vector< std::vector<std::string> > allData;
    parseDataInto2DVector(rawData, allData);
    ASSERT_EQ(5,allData.size());
    EXPECT_EQ("I", allData[1][0]);
    EXPECT_EQ("am", allData[1][1]);
    EXPECT_EQ("Sam", allData[1][2]);
    EXPECT_EQ("W00t!", allData[4][0]);
}


TEST(getUniqueItems, FirstItem)
{
    std::vector<std::string> uniqueItems;
    std::string item = "spoon";
    getUniqueItems(item, uniqueItems);
    ASSERT_EQ(1, uniqueItems.size());
    EXPECT_EQ("spoon", uniqueItems[0]);
}


TEST(getUniqueItems, NewItemIsDuplicate)
{
    std::vector<std::string> uniqueItems;
    uniqueItems.push_back("spoon");
    std::string item = "spoon";
    getUniqueItems(item, uniqueItems);
    ASSERT_EQ(1, uniqueItems.size());
    EXPECT_EQ("spoon", uniqueItems[0]);
}

TEST(getUniqueItems, NewItemIsUnique)
{
    std::vector<std::string> uniqueItems;
    uniqueItems.push_back("spoon");
    std::string item = "fork";
    getUniqueItems(item, uniqueItems);
    ASSERT_EQ(2, uniqueItems.size());
    EXPECT_EQ("spoon", uniqueItems[0]);
    EXPECT_EQ("fork", uniqueItems[1]);
}


TEST(durationToHHMMSS, OneSecond)
{
    auto duration = std::chrono::seconds{1};

    EXPECT_EQ("00:00:01", durationToHHMMSS(duration));
}


TEST(durationToHHMMSS, OneMinute)
{
    auto duration = std::chrono::seconds{60};

    EXPECT_EQ("00:01:00", durationToHHMMSS(duration));
}


TEST(durationToHHMMSS, OneHour)
{
    auto duration = std::chrono::seconds{3600};

    EXPECT_EQ("01:00:00", durationToHHMMSS(duration));
}


TEST(stringToTime, HoursMinutesSeconds)
{
    std::string dateString = "05/11/2012";
    std::string timeString = "17:34:48";
    std::chrono::time_point<std::chrono::system_clock> datetime = stringToTime(dateString, timeString);

    std::ostringstream datetimestream;
    using namespace date;
    datetimestream << date::format("%F %T", date::floor<std::chrono::seconds>(datetime));

    EXPECT_EQ("2012-05-11 17:34:48", datetimestream.str());
}


TEST(stringToTime, HoursMinutes)
{
    std::string dateString = "05/11/2012";
    std::string timeString = "17:34";
    std::chrono::time_point<std::chrono::system_clock> datetime = stringToTime(dateString, timeString);

    std::ostringstream datetimestream;
    using namespace date;
    datetimestream << date::format("%F %T", date::floor<std::chrono::seconds>(datetime));

    EXPECT_EQ("2012-05-11 17:34:00", datetimestream.str());
}


TEST(getFileNamesInDirectory, works)
{
    std::vector<std::string> fileList;
    getFileListInDirectory(testInputDirectory, fileList);
    ASSERT_EQ(16, fileList.size());
}


TEST(getFilenameFromFilepath, SingleExtensionWithBackSlashes)
{
    std::string filepath = "C\\Users\\steve\\Documents\\file.log";
    EXPECT_EQ("file", getFilenameFromFilepath(filepath));
}

TEST(getFilenameFromFilepath, SingleExtensionWithforwardSlashes)
{
    std::string filepath = "/home/user/steve/Documents/file.log";
    EXPECT_EQ("file", getFilenameFromFilepath(filepath));
}

TEST(getFilenameFromFilepath, WithMultiplePeriodsInFile)
{
    std::string filepath = "/home/user/steve/Documents/file.log.txt";
    EXPECT_EQ("file.log", getFilenameFromFilepath(filepath));
}

TEST(getFilenameFromFilepath, NoSlashes)
{
    std::string filepath = "blah!.exe";
    EXPECT_EQ("blah!", getFilenameFromFilepath(filepath));
}

TEST(getFilenameFromFilepath, NoPeriods)
{
    std::string filepath = "/etc/blah!";
    EXPECT_EQ("blah!", getFilenameFromFilepath(filepath));
}

TEST(toString, ConvertSizet)
{
    size_t number = 0;
    EXPECT_EQ("0", toString(number));
}

TEST(fileExists, True)
{
    std::string filePath = testInputDirectory + "/TestFileWindows.txt";
    EXPECT_TRUE(fileExists(filePath));
}

TEST(fileExists, False)
{
    std::string filePath = testInputDirectory + "/TestFileThatDoesNotExist.txt";
    EXPECT_FALSE(fileExists(filePath));
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv); 
    int returnCode = RUN_ALL_TESTS();
    return returnCode;
}
