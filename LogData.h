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

#include <string>
#include <vector>
#include <map>

using namespace std;

enum fileFormat
{
    Invalid,
    ReportLog,
    ISVLog
};


class LogData
{
    public:
        LogData(const string& inputFilePath, const string& outputDirectory);
        ~LogData() {}
        void checkForExistingFiles(string& conflictedFiles);
        void publishResults();
        void publishEventDataResults();
        size_t fileFormat();
    private:
        void findFileFormat();
        void setOutputPaths();
        void extractEvents();
        void getEventIndices();
        void addYearToDate();
        void standardizeLogFormatting();
        void loadEventIntoVector(const vector<string>& allDataRow,
                                 const size_t row,
                                 const vector<size_t>& indices);
        void getConcurrentUsage();
        int getCountOffset(const size_t& row);
        void gatherConcurrentUsageData(const size_t& row,
                                       const vector<string>& licenseUsageCount,
                                       const vector<size_t>& uniqueLicenseCountsByProduct,
                                       const vector<string>& maxLicenseUsageCount);
        void getUsageDuration();
        size_t getIndex(const string& name, const vector<string>& list);

        void writeSummaryData(const string& outputFilePath);
        void writeTotalDuration(const string& outputFilePath);

        // Methods that tweak the ISV log format to look more like the Report log format
        void reformatEventName(const size_t row,
                               const string newLabel);

        void reformatUserHost(const size_t row,
                              const vector<size_t>& indices);

        void reformatProductVersion(const size_t row,
                                    const size_t col,
                                    vector<string>& allDataRow);

        void reformatToken(vector<string>& allDataRow);

        void removeInDetails(const size_t row,
                              vector<string>& allDataRow);

        void removeNoGood(const size_t row,
                          vector<string>& allDataRow);

        void licenseCountAdjust(vector<size_t>& licenseCountNumbers,
                        vector<string>& licenseCountsByProduct);

        void checkForValidProductVersion(const size_t row,
                                         const size_t col,
                                         vector<string>& allDataRow);

        void checkForUnhandledINDetails(const size_t row);

        string m_inputFilePath;
        string m_inputFileName;
        string m_outputDirectory;
        enum fileFormat m_fileFormat;
        vector<string> m_outputPaths;
        vector<string> m_rawData;
        vector< vector<string> > m_allData;
        vector< vector<string> > m_eventData;
        vector< vector<string> > m_denialEvents;
        vector< vector<string> > m_shutdownEvents;
        vector< vector<string> > m_startEvents;
        vector<string> m_uniqueProducts;
        vector<string> m_uniqueUsers;

        string m_eventYear;
        string m_serverName;

        size_t m_eventIndex;
        vector<size_t> m_OUTindices;
        vector<size_t> m_INindices;
        vector<size_t> m_DENYindices;
        vector<size_t> m_STARTindices;
        vector<size_t> m_SHUTindices;
        vector<size_t> m_PRODUCTindices;

        vector< vector<string> > m_usage;
        vector< vector<string> > m_usageDuration;
        vector< vector<std::chrono::nanoseconds> > m_totalDuration;

        size_t m_endTimeRow;
};


enum eventIndices
{
    IndexEvent,
    IndexDate,
    IndexTime,
    IndexProduct,
    IndexVersion,
    IndexUser,
    IndexHost,
    IndexCount,
    IndexHandle
};



enum RepOUTEventIndices
{
    RepOUTIndexDate = 16,
    RepOUTIndexTime = 17,
    RepOUTIndexProduct = 1,
    RepOUTIndexVersion = 2,
    RepOUTIndexUser = 4,
    RepOUTIndexHost = 5,
    RepOUTIndexCount = 8,
    RepOUTIndexHandle = 10
};
enum RepINEventIndices
{
    RepINIndexDate = 11,
    RepINIndexTime = 12,
    RepINIndexProduct = 2,
    RepINIndexVersion = 3,
    RepINIndexUser = 4,
    RepINIndexHost = 5,
    RepINIndexCount = 8,
    RepINIndexHandle = 10
};
enum RepDENYEventIndices
{
    RepDENYIndexDate = 9,
    RepDENYIndexTime = 10,
    RepDENYIndexProduct = 1,
    RepDENYIndexVersion = 2,
    RepDENYIndexUser = 3,
    RepDENYIndexHost = 4,
    RepDENYIndexCount = 6
};
enum RepSTARTEventIndices
{
    RepSTARTIndexDate = 2,
    RepSTARTIndexTime = 3,
    RepSTARTIndexServer = 1
};
enum RepSHUTEventIndices
{
    RepSHUTIndexDate = 3,
    RepSHUTIndexTime = 4
};
enum 
{
    RepPRODUCTIndexProduct = 1,
    RepPRODUCTIndexVersion = 2,
    RepPRODUCTIndexCount = 4
};


enum isvOUTEventIndices
{
    isvOUTIndexDate = 0,
    isvOUTIndexTime = 1,
    isvOUTIndexProduct = 4,
    isvOUTIndexVersion = 5,
    isvOUTIndexUser = 7,
    isvOUTIndexHost = 8,
    isvOUTTokenCount = 9
};
enum isvINEventIndices
{
    isvINIndexDate = 0,
    isvINIndexTime = 1,
    isvINIndexProduct = 4,
    isvINIndexVersion = 5,
    isvINIndexUser = 7,
    isvINIndexHost = 8,
    isvINTokenCount = 9
};
enum isvDENYEventIndices
{
    isvDENYIndexDate = 0,
    isvDENYIndexTime = 1,
    isvDENYIndexProduct = 5,
    isvDENYIndexVersion = 6,
    isvDENYIndexUser = 8,
    isvDENYIndexHost = 9
};
enum isvSTARTEventIndices
{
    isvSTARTIndexDate = 0,
    isvSTARTIndexTime = 1,
    isvSTARTIndexServer = 6
};
enum isvSHUTEventIndices
{
    isvSHUTIndexDate = 0,
    isvSHUTIndexTime = 1
};
