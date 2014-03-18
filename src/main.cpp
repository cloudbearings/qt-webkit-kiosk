/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "anyoption.h"

bool launch(AnyOption *cmdopts)
{
    cmdopts->addUsage("This is a simple web-browser working in fullscreen kiosk-mode.");
    cmdopts->addUsage("");
    cmdopts->addUsage("Usage: ");
    cmdopts->addUsage("");
    cmdopts->addUsage(" -h --help                           Print usage and exit");
    cmdopts->addUsage(" -v --version                        Print version and exit");
    cmdopts->addUsage(" -c --config options.ini             Configuration INI-file");
    cmdopts->addUsage(" -u --uri http://www.example.com/    Open this URI, home page");
    cmdopts->addUsage(" -C --clear-cache                    Clear cached request data");
    cmdopts->addUsage("");

    cmdopts->setFlag("help", 'h');
    cmdopts->setFlag("version", 'v');
    cmdopts->setFlag("clear-cache", 'C');

    cmdopts->setOption("config", 'c');
    cmdopts->setOption("uri", 'u');

    cmdopts->setVersion(VERSION);

    cmdopts->processCommandArgs( QCoreApplication::argc(), QCoreApplication::argv() );

    if (cmdopts->getFlag('h') || cmdopts->getFlag("help")) {
        qDebug(">> Help option in command prompt...");
        cmdopts->printUsage();
        return false;
    }

    if (cmdopts->getFlag('v') || cmdopts->getFlag("version")) {
        qDebug(">> Version option in command prompt...");
        cmdopts->printVersion();
        return false;
    }

    return true;
}

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    AnyOption *cmdopts = new AnyOption();
    if (!launch(cmdopts)) {
        return 0;
    }

    MainWindow *browser = new MainWindow();
    browser->init(cmdopts);

    // executes browser.cleanupSlot() when the Qt framework emits aboutToQuit() signal.
    QObject::connect(qApp, SIGNAL(aboutToQuit()),
                     browser, SLOT(cleanupSlot()));

    int ret = app.exec();
    qDebug() << "Application return:" << ret;
}
