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
#include <string>
#include <vector>
#include <map>


enum fileFormat
{
    Invalid,
    ReportLog,
    ISVLog
};


class LogData
{
    public:
        LogData(const std::string& inputFilePath, const std::string& outputDirectory);
        ~LogData() {}
        void checkForExistingFiles(std::string& conflictedFiles);
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
        void loadEventIntoVector(const std::vector<std::string>& allDataRow,
                                 const size_t row,
                                 const std::vector<size_t>& indices);
        void getConcurrentUsage();
        int getCountOffset(const size_t& row);
        void gatherConcurrentUsageData(const size_t& row,
                                       const std::vector<std::string>& licenseUsageCount,
                                       const std::vector<size_t>& uniqueLicenseCountsByProduct,
                                       const std::vector<std::string>& maxLicenseUsageCount);
        void getUsageDuration();
        size_t getIndex(const std::string& name, const std::vector<std::string>& list);

        void writeSummaryData(const std::string& outputFilePath);
        void writeTotalDuration(const std::string& outputFilePath);

        // Methods that tweak the ISV log format to look more like the Report log format
        void reformatEventName(const size_t row,
                               const std::string newLabel);

        void reformatUserHost(const size_t row,
                              const std::vector<size_t>& indices);

        void reformatProductVersion(const size_t row,
                                    const size_t col,
                                    std::vector<std::string>& allDataRow);

        void reformatToken(std::vector<std::string>& allDataRow);

        void removeInDetails(const size_t row,
                              std::vector<std::string>& allDataRow);

        void removeNoGood(const size_t row,
                          std::vector<std::string>& allDataRow);

        void licenseCountAdjust(std::vector<size_t>& licenseCountNumbers,
                        std::vector<std::string>& licenseCountsByProduct);

        void checkForValidProductVersion(const size_t row,
                                         const size_t col,
                                         std::vector<std::string>& allDataRow);

        void checkForUnhandledINDetails(const size_t row);

        std::string m_inputFilePath;
        std::string m_inputFileName;
        std::string m_outputDirectory;
        enum fileFormat m_fileFormat;
        std::vector<std::string> m_outputPaths;
        std::vector<std::string> m_rawData;
        std::vector<std::vector<std::string>> m_allData;
        std::vector<std::vector<std::string>> m_eventData;
        std::vector<std::vector<std::string>> m_denialEvents;
        std::vector<std::vector<std::string>> m_shutdownEvents;
        std::vector<std::vector<std::string>> m_startEvents;
        std::vector<std::string> m_uniqueProducts;
        std::vector<std::string> m_uniqueUsers;

        std::string m_eventYear;
        std::string m_serverName;

        size_t m_eventIndex;
        std::vector<size_t> m_OUTindices;
        std::vector<size_t> m_INindices;
        std::vector<size_t> m_DENYindices;
        std::vector<size_t> m_STARTindices;
        std::vector<size_t> m_SHUTindices;
        std::vector<size_t> m_PRODUCTindices;

        std::vector<std::vector<std::string>> m_usage;
        std::vector<std::vector<std::string>> m_usageDuration;
        std::vector< std::vector<std::chrono::nanoseconds> > m_totalDuration;

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
