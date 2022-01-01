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

#include "MainWindow.h"
#include <iostream>
#include <string>
#include <vector>
#include <QtPlugin>
#include <QApplication>


using namespace std;

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif defined(Q_OS_MAC)
    Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#elif defined(Q_OS_LINUX)
#endif

    QApplication app(argc, argv);

    QIcon windowIcon;
    windowIcon.addFile(":/graphics/icon_016x016.png");
    windowIcon.addFile(":/graphics/icon_032x032.png");
    QApplication::setWindowIcon(windowIcon);

    MainWindow window;
    window.show();
    return app.exec();
}
