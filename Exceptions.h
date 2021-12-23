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

#include <exception>
#include <sstream>
#include "Utilities.h"

using namespace std;


class EventDataException: public exception
{
public:
    EventDataException(size_t row)
    {
        string rowString = toString(row);
        m_error = "Missing data on line " + rowString;
    }
    ~EventDataException() throw() {}
    virtual const char* what() const throw()
    {
        return m_error.c_str();
    }
private:
    string m_error;
};


class INEventDetailException: public exception
{
public:
    INEventDetailException(size_t row)
    {
        string rowString = toString(row);
        m_error = "Unexpected IN event details on line " + rowString;
    }
    ~INEventDetailException() throw() {}
    virtual const char* what() const throw()
    {
        return m_error.c_str();
    }
private:
    string m_error;
};


class InvalidProductVersionException: public exception
{
public:
    InvalidProductVersionException(size_t row)
    {
        string rowString = toString(row);
        m_error = "Invalid product version formatting on line " + rowString;
    }
    ~InvalidProductVersionException() throw() {}
    virtual const char* what() const throw()
    {
        return m_error.c_str();
    }
private:
    string m_error;
};


class CannotOpenFileException: public exception
{
public:
    CannotOpenFileException(string filePath)
    {
        if (filePath.empty())
        {
            m_error = "No file selected";
        }
        else
        {
            m_error = "Unable to open file: " + filePath;
        }
    }
    ~CannotOpenFileException() throw() {}
    virtual const char* what() const throw()
    {
        return m_error.c_str();
    }
private:
    string m_error;
};


class CannotFindDirException: public exception
{
public:
    CannotFindDirException(string dirPath)
    {
        if (dirPath.empty())
        {
            m_error = "No directory selected";
        }
        else
        {
            m_error = "Unable to open directory: " + dirPath;
        }
    }
    ~CannotFindDirException() throw() {}
    virtual const char* what() const throw()
    {
        return m_error.c_str();
    }
private:
    string m_error;
};


class InvalidFileFormatException: public exception
{
public:
    InvalidFileFormatException()
    {
        m_error = "Log file format is invalid.  Only RLM report and ISV logs are supported.  "
            "Consult the README file for more information.";
    }
    ~InvalidFileFormatException() throw() {}
    virtual const char* what() const throw()
    {
        return m_error.c_str();
    }
private:
    string m_error;
};


class InvalidIndexException: public exception
{
public:
    InvalidIndexException(string indexName)
    {
        m_error = "No index to '" + indexName + "'";
    }
    ~InvalidIndexException() throw() {}
    virtual const char* what() const throw()
    {
        return m_error.c_str();
    }
private:
    string m_error;
};
