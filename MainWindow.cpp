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

#include <QtGui>
#include <QMessageBox>

#include "MainWindow.h"
#include "LogData.h"
#include "MainWindowConfig.h"
#include "Exceptions.h"

#include <iostream>
#include <string>

#include <QFileDialog>
#include <QString>
#include <QDebug>


MainWindow::MainWindow(QMainWindow* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    const std::string windowTitle = appTitle + " " + versionMajor + "." + versionMinor + "." + versionPatch;
    QWidget::setWindowTitle(windowTitle.c_str());
 
    connect( this->ui.openButton, SIGNAL( clicked() ), this, SLOT(openButtonClicked()) );
    connect( this->ui.saveButton, SIGNAL( clicked() ), this, SLOT(saveButtonClicked()) );
    connect( this->ui.generateButton, SIGNAL( clicked() ), this, SLOT(generateButtonClicked()) );

    QString settingsPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    m_settingsFile = settingsPath + "/" + appTitleNoSpaces.c_str() + ".ini";
    qDebug() << "Settings filepath: " << m_settingsFile;
}

void MainWindow::loadSettings()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    m_inputFilePath = settings.value("LastInputFilePath", "").toString();
    m_outputDirectory = settings.value("LastOutputDir", "").toString();
}

void MainWindow::saveSettings()
{
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    settings.setValue("LastInputFilePath", m_inputFilePath);
    settings.setValue("LastOutputDir", m_outputDirectory);
}
 
void MainWindow::openButtonClicked()
{
    loadSettings();

    // Get path to the log file
    QString path = QFileDialog::getOpenFileName(this,
        tr("Open RLM report or ISV log file"), m_inputFilePath, tr("Log Files (*.log *.txt);;All Files (*.*)"));
 
    if (! path.isNull())
    {
        path = QDir::toNativeSeparators(path);
        ui.inputTextField->setText(path);
        m_inputFilePath = path;
        saveSettings();
    }
}
 
void MainWindow::saveButtonClicked()
{
    loadSettings();

    // Get output path for the results
    QString path = QFileDialog::getExistingDirectory(this, tr("Set the directory to save the results"),
                                      m_outputDirectory,
                                      QFileDialog::ShowDirsOnly
                                      | QFileDialog::DontResolveSymlinks);

    if (! path.isNull())
    {
        path = QDir::toNativeSeparators(path);
        ui.outputTextField->setText(path);
        m_outputDirectory = path;
        saveSettings();
    }
}

void MainWindow::generateButtonClicked()
{
    m_inputFilePath = ui.inputTextField->displayText();
    m_outputDirectory = QDir::toNativeSeparators(ui.outputTextField->displayText());

    std::string inputFilePathString = m_inputFilePath.toStdString();
    std::string outputDirectoryString = m_outputDirectory.toStdString();

    bool filesPublished = false;
    
    this->setCursor(Qt::WaitCursor);
    try
    {
        QDir path = m_outputDirectory;

        if(!path.exists() || outputDirectoryString.empty())
        {
            CannotFindDirException cannotFindDirException(outputDirectoryString);
            throw cannotFindDirException;
        }

        LogData logData(inputFilePathString, outputDirectoryString);

        std::string conflictedFileList;
        logData.checkForExistingFiles(conflictedFileList);

        if (! conflictedFileList.empty())
        {
            std::string overwriteQuestion = "Do you wish to overwrite the following files?\n\n";
            overwriteQuestion.append(conflictedFileList);

            QMessageBox messageBox;
            messageBox.setWindowTitle("Overwrite?");
            messageBox.setText(QDir::toNativeSeparators(QString(overwriteQuestion.c_str())));
            messageBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            messageBox.setDefaultButton(QMessageBox::No);

            if (messageBox.exec() == QMessageBox::Yes)
            {
                logData.publishResults();
                filesPublished = true;
            }
        }
        else
        {
            logData.publishResults();
            filesPublished = true;
        }
        
        if (filesPublished)
        {
            QDesktopServices::openUrl(QUrl("file:///" + m_outputDirectory));
        }
    }
    catch (std::exception& e)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.setWindowTitle("Error");
        messageBox.setText(e.what());
        messageBox.exec();
    }
    this->setCursor(Qt::ArrowCursor);
}
