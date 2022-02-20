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

#include "LogData.h"
#include "gtest/gtest.h"
#include "TestConfig.h"
#include "Utilities.h"


void integrationTest(const string& logFileName,
                     vector<string>& usage,
                     vector<string>& event,
                     vector<string>& summary)
{
    string inputFilePath = testInputDirectory + "/" + logFileName;
    LogData logData(inputFilePath, testOutputDirectory);
    logData.publishResults();
    logData.publishEventDataResults();
    string inputFileName = getFilenameFromFilepath(logFileName);
    loadDataFromFile(testOutputDirectory + "/" + inputFileName + "_AllEventData.txt", event);
    loadDataFromFile(testOutputDirectory + "/" + inputFileName + "_UsageOverTime.csv", usage);
    loadDataFromFile(testOutputDirectory + "/" + inputFileName + "_Summary.txt", summary);
}

void parseReportLog(const string& logFileName,
                    vector<string>& usage,
                    vector<string>& event,
                    vector<string>& summary,
                    vector<string>& duration,
                    vector<string>& totalDuration)
{
    integrationTest(logFileName, usage, event, summary);
    string inputFileName = getFilenameFromFilepath(logFileName);
    loadDataFromFile(testOutputDirectory + "/" + inputFileName + "_UsageDuration.csv", duration);
    loadDataFromFile(testOutputDirectory + "/" + inputFileName + "_TotalDuration.csv", totalDuration);
}


TEST(IntegrationTest, ReportLog)
{
    string logFileName = "SampleLog_Report.log";
    vector<string> usage;
    vector<string> event;
    vector<string> summary;
    vector<string> duration;
    vector<string> totalDuration;
    parseReportLog(logFileName, usage, event, summary, duration, totalDuration);

    // All events from log data
    ASSERT_EQ(21, event.size());
    EXPECT_EQ("START 05/11/2013 15:16:49 Win2008", event.at(0));
    EXPECT_EQ("PRODUCT simulator 3.12 1", event.at(1));
    EXPECT_EQ("PRODUCT analytics 3.12 2", event.at(2));
    EXPECT_EQ("OUT 05/11/2013 15:17:14 analytics 2.09 cecil win2008 1 41", event.at(3));
    EXPECT_EQ("OUT 05/11/2013 15:19:08 simulator 2.12 terra ubuntu 1 81", event.at(4));
    EXPECT_EQ("DENY 05/11/2013 15:20 simulator 2.1 cecil win2008 1", event.at(5));
    EXPECT_EQ("IN 05/11/2013 15:22:13 simulator 2.12 terra ubuntu 0 81", event.at(6));
    EXPECT_EQ("SHUTDOWN 05/11/2013 15:23:15", event.at(7));
    EXPECT_EQ("START 05/11/2013 15:24:28 Win2008", event.at(8));
    EXPECT_EQ("PRODUCT datavis 3.12 10", event.at(9));
    EXPECT_EQ("PRODUCT analytics 3.12 3", event.at(10));
    EXPECT_EQ("PRODUCT simulator 3.12 50", event.at(11));
    EXPECT_EQ("OUT 05/11/2013 15:25:00 datavis 2.04 cecil win2008 1 41", event.at(12));
    EXPECT_EQ("OUT 05/11/2013 15:26:36 analytics 3.03 terra ubuntu 1 81", event.at(13));
    EXPECT_EQ("OUT 05/11/2013 15:28:06 datavis 3.01 cecil redhat 2 c1", event.at(14));
    EXPECT_EQ("IN 05/11/2013 16:07:39 analytics 3.03 terra ubuntu 0 81", event.at(15));
    EXPECT_EQ("PRODUCT datavis 3.12 10", event.at(16));
    EXPECT_EQ("PRODUCT analytics 3.12 3", event.at(17));
    EXPECT_EQ("PRODUCT simulator 3.12 50", event.at(18));
    EXPECT_EQ("IN 05/12/2013 01:32:28 datavis 3.01 terra ubuntu 1 c1", event.at(19));
    EXPECT_EQ("", event.at(20));

    // Summary of log data
    ASSERT_EQ(24, summary.size());
    EXPECT_EQ("Log Data Summary For:", summary.at(0));
    EXPECT_EQ(testInputDirectory + "/" + logFileName, summary.at(1));
    EXPECT_EQ("", summary.at(2));
    EXPECT_EQ("Server Name: Win2008", summary.at(3));
    EXPECT_EQ("", summary.at(4));
    EXPECT_EQ("Server Start(s): (2 Total)", summary.at(5));
    EXPECT_EQ("05/11/2013 15:16:49 Win2008 ", summary.at(6));
    EXPECT_EQ("05/11/2013 15:24:28 Win2008 ", summary.at(7));
    EXPECT_EQ("", summary.at(8));
    EXPECT_EQ("Server Shutdown(s): (1 Total)", summary.at(9));
    EXPECT_EQ("05/11/2013 15:23:15 ", summary.at(10));
    EXPECT_EQ("", summary.at(11));
    EXPECT_EQ("Product(s): (3 Total)", summary.at(12));
    EXPECT_EQ("simulator", summary.at(13));
    EXPECT_EQ("analytics", summary.at(14));
    EXPECT_EQ("datavis", summary.at(15));
    EXPECT_EQ("", summary.at(16));
    EXPECT_EQ("Users(s): (2 Total)", summary.at(17));
    EXPECT_EQ("cecil", summary.at(18));
    EXPECT_EQ("terra", summary.at(19));
    EXPECT_EQ("", summary.at(20));
    EXPECT_EQ("Denials(s): (1 Total)", summary.at(21));
    EXPECT_EQ("05/11/2013 15:20 simulator 2.1 cecil ", summary.at(22));
    EXPECT_EQ("", summary.at(23));

    // Usage over time from log data
    ASSERT_EQ(11, usage.size());
    EXPECT_EQ("Date/Time,simulator Licenses in use,simulator Unique user count,simulator Total licenses,analytics Licenses in use,analytics Unique user count,analytics Total licenses,datavis Licenses in use,datavis Unique user count,datavis Total licenses", usage.at(0));
    EXPECT_EQ("05/11/2013 15:17:14,0,0,1,1,1,2,0,0,0", usage.at(1));
    EXPECT_EQ("05/11/2013 15:19:08,1,1,1,1,1,2,0,0,0", usage.at(2));
    EXPECT_EQ("05/11/2013 15:22:13,0,0,1,1,1,2,0,0,0", usage.at(3));
    EXPECT_EQ("05/11/2013 15:23:15,0,0,1,0,0,2,0,0,0", usage.at(4));
    EXPECT_EQ("05/11/2013 15:25:00,0,0,50,0,0,3,1,1,10", usage.at(5));
    EXPECT_EQ("05/11/2013 15:26:36,0,0,50,1,1,3,1,1,10", usage.at(6));
    EXPECT_EQ("05/11/2013 15:28:06,0,0,50,1,1,3,2,1,10", usage.at(7));
    EXPECT_EQ("05/11/2013 16:07:39,0,0,50,0,0,3,2,1,10", usage.at(8));
    EXPECT_EQ("05/12/2013 01:32:28,0,0,50,0,0,3,1,1,10", usage.at(9));
    EXPECT_EQ("", usage.at(10));

    // Usage duration of each session
    ASSERT_EQ(7, duration.size());
    EXPECT_EQ("Checkout Date/Time,Checkin Date/Time,Product,Version,User,Duration (HH:MM:SS)", duration.at(0));
    EXPECT_EQ("05/11/2013 15:17:14,05/11/2013 15:23:15,analytics,2.09,cecil,00:06:01", duration.at(1));
    EXPECT_EQ("05/11/2013 15:19:08,05/11/2013 15:22:13,simulator,2.12,terra,00:03:05", duration.at(2));
    EXPECT_EQ("05/11/2013 15:25:00,(Still checked out),datavis,2.04,cecil,10:07:28", duration.at(3));
    EXPECT_EQ("05/11/2013 15:26:36,05/11/2013 16:07:39,analytics,3.03,terra,00:41:03", duration.at(4));
    EXPECT_EQ("05/11/2013 15:28:06,05/12/2013 01:32:28,datavis,3.01,cecil,10:04:22", duration.at(5));
    EXPECT_EQ("", duration.at(6));

    // Total usage duration by user and product
    ASSERT_EQ(4, totalDuration.size());
    EXPECT_EQ("User,simulator Duration (HH:MM:SS),analytics Duration (HH:MM:SS),datavis Duration (HH:MM:SS)", totalDuration.at(0));
    EXPECT_EQ("cecil,00:00:00,00:06:01,20:11:50", totalDuration.at(1));
    EXPECT_EQ("terra,00:03:05,00:41:03,00:00:00", totalDuration.at(2));
    EXPECT_EQ("", totalDuration.at(3));
}

TEST(IntegrationTest, ISVLog)
{
    string logFileName = "SampleLog_ISV.log";
    vector<string> usage;
    vector<string> event;
    vector<string> summary;
    integrationTest(logFileName, usage, event, summary);

    // All events from log data
    ASSERT_EQ(13, event.size());
    EXPECT_EQ("START 05/11 15:16 Win2008", event.at(0));
    EXPECT_EQ("OUT 05/11 15:17 analytics 2.09 cecil win2008 1", event.at(1));
    EXPECT_EQ("OUT 05/11 15:19 simulator 2.12 terra ubuntu 1", event.at(2));
    EXPECT_EQ("DENY 05/11 15:20 simulator 2.1 cecil win2008", event.at(3));
    EXPECT_EQ("IN 05/11 15:22 simulator 2.12 terra ubuntu 1", event.at(4));
    EXPECT_EQ("SHUTDOWN 05/11 15:23", event.at(5));
    EXPECT_EQ("START 05/11 15:24 Win2008", event.at(6));
    EXPECT_EQ("OUT 05/11 15:25 datavis 2.04 cecil win2008 1", event.at(7));
    EXPECT_EQ("OUT 05/11 15:26 analytics 3.03 terra ubuntu 1", event.at(8));
    EXPECT_EQ("OUT 05/11 15:28 datavis 3.01 cecil redhat 1", event.at(9));
    EXPECT_EQ("IN 05/11 16:07 analytics 3.03 terra ubuntu 1", event.at(10));
    EXPECT_EQ("IN 05/12 01:32 datavis 3.01 terra ubuntu 1", event.at(11));
    EXPECT_EQ("", event.at(12));

    // Summary of log data
    ASSERT_EQ(24, summary.size());
    EXPECT_EQ("Log Data Summary For:", summary.at(0));
    EXPECT_EQ(testInputDirectory + "/" + logFileName, summary.at(1));
    EXPECT_EQ("", summary.at(2));
    EXPECT_EQ("Server Name: Win2008", summary.at(3));
    EXPECT_EQ("", summary.at(4));
    EXPECT_EQ("Server Start(s): (2 Total)", summary.at(5));
    EXPECT_EQ("05/11 15:16 Win2008 ", summary.at(6));
    EXPECT_EQ("05/11 15:24 Win2008 ", summary.at(7));
    EXPECT_EQ("", summary.at(8));
    EXPECT_EQ("Server Shutdown(s): (1 Total)", summary.at(9));
    EXPECT_EQ("05/11 15:23 ", summary.at(10));
    EXPECT_EQ("", summary.at(11));
    EXPECT_EQ("Product(s): (3 Total)", summary.at(12));
    EXPECT_EQ("analytics", summary.at(13));
    EXPECT_EQ("simulator", summary.at(14));
    EXPECT_EQ("datavis", summary.at(15));
    EXPECT_EQ("", summary.at(16));
    EXPECT_EQ("Users(s): (2 Total)", summary.at(17));
    EXPECT_EQ("cecil", summary.at(18));
    EXPECT_EQ("terra", summary.at(19));
    EXPECT_EQ("", summary.at(20));
    EXPECT_EQ("Denials(s): (1 Total)", summary.at(21));
    EXPECT_EQ("05/11 15:20 simulator 2.1 cecil ", summary.at(22));
    EXPECT_EQ("", summary.at(23));

    // Usage over time from log data
    ASSERT_EQ(11, usage.size());
    EXPECT_EQ("Date/Time,analytics Licenses in use,analytics Unique user count,simulator Licenses in use,simulator Unique user count,datavis Licenses in use,datavis Unique user count", usage.at(0));
    EXPECT_EQ("05/11 15:17,1,1,0,0,0,0", usage.at(1));
    EXPECT_EQ("05/11 15:19,1,1,1,1,0,0", usage.at(2));
    EXPECT_EQ("05/11 15:22,1,1,0,0,0,0", usage.at(3));
    EXPECT_EQ("05/11 15:23,0,0,0,0,0,0", usage.at(4));
    EXPECT_EQ("05/11 15:25,0,0,0,0,1,1", usage.at(5));
    EXPECT_EQ("05/11 15:26,1,1,0,0,1,1", usage.at(6));
    EXPECT_EQ("05/11 15:28,1,1,0,0,2,1", usage.at(7));
    EXPECT_EQ("05/11 16:07,0,0,0,0,2,1", usage.at(8));
    EXPECT_EQ("05/12 01:32,0,0,0,0,1,1", usage.at(9));
    EXPECT_EQ("", usage.at(10));
}


TEST(IntegrationTest, ReportLogTokens)
{
    string logFileName = "SampleLog_Report_Tokens.log";
    vector<string> usage;
    vector<string> event;
    vector<string> summary;
    integrationTest(logFileName, usage, event, summary);

    // Usage over time from log data
    ASSERT_EQ(4, usage.size());
    EXPECT_EQ("Date/Time,analytics Licenses in use,analytics Unique user count,analytics Total licenses", usage.at(0));
    EXPECT_EQ("01/07/2014 10:15:12,56,1,400", usage.at(1));
    EXPECT_EQ("01/07/2014 10:15:12,42,1,400", usage.at(2));
    EXPECT_EQ("", usage.at(3));
}


TEST(IntegrationTest, ISVLogTokens)
{
    string logFileName = "SampleLog_ISV_Tokens.log";
    vector<string> usage;
    vector<string> event;
    vector<string> summary;
    integrationTest(logFileName, usage, event, summary);

    // Usage over time from log data
    ASSERT_EQ(4, usage.size());
    EXPECT_EQ("Date/Time,analytics Licenses in use,analytics Unique user count", usage.at(0));
    EXPECT_EQ("06/13 08:52,14,1", usage.at(1));
    EXPECT_EQ("06/13 08:55,0,0", usage.at(2));
    EXPECT_EQ("", usage.at(3));
}


TEST(IntegrationTest, Exception)
{
    string logFileName = "EventDataException.log";
    vector<string> usage;
    vector<string> event;
    vector<string> summary;
    string errorMessage;
    try
    {
        integrationTest(logFileName, usage, event, summary);
    }
    catch (exception& e)
    {
        errorMessage = e.what();
    }
    EXPECT_EQ("Missing data on line 15", errorMessage);
}


TEST(findFileFormat, DetectsReportLogFormat)
{
    string inputFilePath = testInputDirectory + "/TestFileFormatReport.txt";
    LogData logData(inputFilePath, testOutputDirectory);
    EXPECT_EQ(1, logData.fileFormat());
}

TEST(findFileFormat, DetectsISVLogFormat)
{
    string inputFilePath = testInputDirectory + "/TestFileFormatISV.txt";
    LogData logData(inputFilePath, testOutputDirectory);
    EXPECT_EQ(2, logData.fileFormat());
}

void detectInvalidFileTest(string& inputFilePath)
{
    string errorMessage;
    try
    {
        LogData logData(inputFilePath, testOutputDirectory);
    }
    catch (exception& e)
    {
        errorMessage = e.what();
    }
    EXPECT_EQ("Log file format is invalid.  Only RLM report and ISV logs are supported.  Consult the README file for more information.", errorMessage);
}
TEST(findFileFormat, DetectsNoFormatInEmptyFile)
{
    string inputFilePath = testInputDirectory + "/TestFileEmpty.txt";
    detectInvalidFileTest(inputFilePath);
}
TEST(findFileFormat, DetectsNoFormatInInvalidFiles)
{
    string inputFilePath = testInputDirectory + "/TestFileFormatRLM.txt";
    detectInvalidFileTest(inputFilePath);

    inputFilePath = testInputDirectory + "/TestFileFormatInvalid.txt";
    detectInvalidFileTest(inputFilePath);
}


TEST(IntegrationTest, ReportLogTecplotNewYear)
{
    string logFilePath = "NewYear.log";
    vector<string> usage;
    vector<string> event;
    vector<string> summary;
    integrationTest(logFilePath, usage, event, summary);
    ASSERT_EQ(5, usage.size());
    EXPECT_EQ("12/31/2012 17:55:19,6,1,50", usage.at(1));
    EXPECT_EQ("01/01/2013 00:00:05,5,1,50", usage.at(2));
    EXPECT_EQ("01/01/2013 01:03:47,6,1,50", usage.at(3));
    EXPECT_EQ("", usage.at(4));
}


TEST(removeInDetails, EventParsedCorrectly)
{
    string logFileName = "ISVInDetails.log";
    vector<string> usage;
    vector<string> event;
    vector<string> summary;
    integrationTest(logFileName, usage, event, summary);
    ASSERT_EQ(9, event.size());
    EXPECT_EQ("IN 03/19 08:52 analytics 3.03 james macpro 1", event.at(4));
    EXPECT_EQ("IN 03/19 09:15 datavis 3.1 cecil winxp 1", event.at(5));
    EXPECT_EQ("IN 03/19 14:09 datavis 3.02 cid macbook 1", event.at(6));
    EXPECT_EQ("IN 01/02 10:48 datavis 2.04 cecil win7_xeon 1", event.at(7));
}


TEST(removeDenyDetails, EventParsedCorrectly)
{
    string logFileName = "ISVDenyDetails.log";
    vector<string> usage;
    vector<string> event;
    vector<string> summary;
    integrationTest(logFileName, usage, event, summary);
    ASSERT_EQ(3, event.size());
    EXPECT_EQ("DENY 03/18 19:19 1 3.03 edgar vaio", event.at(1));
}


TEST(uniqueUsers, SetToOneIfSeatsAlreadyCheckedOutWhenLogStarts)
{
    string logFileName = "UniqueUsers.log";
    vector<string> usage;
    vector<string> event;
    vector<string> summary;
    integrationTest(logFileName, usage, event, summary);
    ASSERT_EQ(8, usage.size());
    EXPECT_EQ("09/12/2012 15:52:41,9,1,10", usage.at(1));
    EXPECT_EQ("09/12/2012 21:27:33,10,1,10", usage.at(2));
    EXPECT_EQ("09/13/2012 15:46:50,11,2,10", usage.at(3));
    EXPECT_EQ("09/13/2012 15:52:41,10,1,10", usage.at(4));
    EXPECT_EQ("09/13/2012 22:13:09,9,1,10", usage.at(5));
    EXPECT_EQ("09/15/2012 22:51:19,8,1,10", usage.at(6));
}


TEST(IntegrationTest, ExtraFiles)
{
    string filePath;
    vector<string> fileList;
    getFileListInDirectory(extraTestDirectory, fileList);
    size_t numberOfFiles = fileList.size();

    if (numberOfFiles == 0)
    {
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        cout << "    To test your own log files, copy them here:" << endl;
        cout << "    " << extraTestDirectory << endl;
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    }
    else
    {
        for (size_t file=0; file < numberOfFiles; ++file)
        {
            filePath = fileList.at(file);
            cout << filePath << endl;
            LogData logData(filePath, testOutputDirectory);
            logData.publishResults();
        }
    }
}
