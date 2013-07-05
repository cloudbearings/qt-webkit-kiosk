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

#include "config.h"

#include <QtGui>
#include <QtNetwork>
#include <QtWebKit>
#include <QDebug>
#include "mainwindow.h"
#include "cachingnm.h"

MainWindow::MainWindow()
{
    progress = 0;

    cmdopts = new AnyOption();
    //cmdopts->setVerbose();

    cmdopts->addUsage("This is a simple web-browser working in fullscreen kiosk-mode.");
    cmdopts->addUsage("");
    cmdopts->addUsage("Usage: ");
    cmdopts->addUsage("");
    cmdopts->addUsage(" --help -h                       Print usage and exit");
    cmdopts->addUsage(" --version -V                    Print version and exit");
    cmdopts->addUsage(" --config options.ini            Configuration INI-file");
    cmdopts->addUsage(" --uri http://www.example.com/   Open this URI");
    cmdopts->addUsage(" --clear-cache -C                Clear cached request data");
    cmdopts->addUsage("");

    cmdopts->setFlag("help", 'h');
    cmdopts->setFlag("version", 'V');
    cmdopts->setFlag("clear-cache", 'C');

    cmdopts->setOption("config");
    cmdopts->setOption("uri");

    cmdopts->setVersion(VERSION);

    cmdopts->processCommandArgs( QCoreApplication::argc(), QCoreApplication::argv() );

    if (cmdopts->getFlag('h') || cmdopts->getFlag("help")) {
        qDebug() << ">> Help option in command prompt...";
        cmdopts->printUsage();
        eventExit = new QKeyEvent( QEvent::KeyPress, Qt::Key_Q, Qt::ControlModifier, "Exit", 0 );
        QCoreApplication::postEvent( this, eventExit );
        return;
    }

    if (cmdopts->getFlag('V') || cmdopts->getFlag("version")) {
        qDebug() << ">> Version option in command prompt...";
        cmdopts->printVersion();
        eventExit = new QKeyEvent( QEvent::KeyPress, Qt::Key_Q, Qt::ControlModifier, "Exit", 0 );
        QCoreApplication::postEvent( this, eventExit );
        return;
    }

    if (cmdopts->getValue("config")) {
        qDebug() << ">> Config option in command prompt...";
        loadSettings(QString(cmdopts->getValue("config")));
    } else {
        loadSettings(QString(""));
    }

    qDebug() << "Application icon: " << mainSettings->value("application/icon").toString();

    setWindowIcon(QIcon(
       mainSettings->value("application/icon").toString()
    ));

    if (cmdopts->getValue("uri")) {
        qDebug() << ">> Uri option in command prompt...";
        mainSettings->setValue("browser/homepage", cmdopts->getValue("uri"));
    }

    QCoreApplication::setOrganizationName(
            mainSettings->value("application/organization").toString()
            );
    QCoreApplication::setOrganizationDomain(
            mainSettings->value("application/organization-domain").toString()
            );
    QCoreApplication::setApplicationName(
            mainSettings->value("application/name").toString()
            );
    QCoreApplication::setApplicationVersion(
            mainSettings->value("application/version").toString()
            );

    // --- Network --- //

    if (mainSettings->value("proxy/enable").toBool()) {
        bool system = mainSettings->value("proxy/system").toBool();
        if (system) {
            QNetworkProxyFactory::setUseSystemConfiguration(system);
        } else {
            QNetworkProxy proxy;
            proxy.setType(QNetworkProxy::HttpProxy);
            proxy.setHostName(
                    mainSettings->value("proxy/host").toString()
            );
            proxy.setPort(mainSettings->value("proxy/port").toUInt());
            if (mainSettings->value("proxy/auth").toBool()) {
                proxy.setUser(mainSettings->value("proxy/username").toString());
                proxy.setPassword(mainSettings->value("proxy/password").toString());
            }
            QNetworkProxy::setApplicationProxy(proxy);
        }
    }

    if (!mainSettings->value("printing/show-printer-dialog").toBool()) {
        printer = new QPrinter(QPrinter::ScreenResolution);
        printer->setPrinterName(mainSettings->value("printing/printer").toString());
        printer->setPageMargins(
            mainSettings->value("printing/page_margin_left").toReal(),
            mainSettings->value("printing/page_margin_top").toReal(),
            mainSettings->value("printing/page_margin_right").toReal(),
            mainSettings->value("printing/page_margin_bottom").toReal(),
            QPrinter::Millimeter
        );
    }

    // --- Web View --- //

    view = new WebView(this);
    view->setSettings(mainSettings);
    view->setPage(new QWebPage(view));

    // --- Disk cache --- //
    if (mainSettings->value("cache/enable").toBool()) {
        diskCache = new QNetworkDiskCache(this);
        QString location = mainSettings->value("cache/location").toString();
        if (!location.length()) {
            location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
        }
        diskCache->setCacheDirectory(location);
        diskCache->setMaximumCacheSize(mainSettings->value("cache/size").toUInt());

        if (mainSettings->value("cache/clear-on-start").toBool()) {
            diskCache->clear();
        }
        if (cmdopts->getFlag('C') || cmdopts->getFlag("clear-cache")) {
            diskCache->clear();
        }

        CachingNetworkManager *nm = new CachingNetworkManager();
        nm->setCache(diskCache);
        view->page()->setNetworkAccessManager(nm);
    }

    view->settings()->setAttribute(QWebSettings::JavascriptEnabled,
        mainSettings->value("browser/javascript").toBool()
    );

    view->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows,
        mainSettings->value("browser/javascript_can_open_windows").toBool()
    );

    view->settings()->setAttribute(QWebSettings::JavascriptCanCloseWindows,
        mainSettings->value("browser/javascript_can_close_windows").toBool()
    );

    view->settings()->setAttribute(QWebSettings::WebGLEnabled,
        mainSettings->value("browser/webgl").toBool()
    );

    view->settings()->setAttribute(QWebSettings::JavaEnabled,
        mainSettings->value("browser/java").toBool()
    );

    view->settings()->setAttribute(QWebSettings::PluginsEnabled,
        mainSettings->value("browser/plugins").toBool()
    );

    if (mainSettings->value("inspector/enable").toBool()) {
        view->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

        inspector = new QWebInspector();
        inspector->setVisible(mainSettings->value("inspector/visible").toBool());
        inspector->setMinimumSize(800, 600);
        inspector->setWindowTitle(mainSettings->value("application/name").toString() + " - WebInspector");
        inspector->setWindowIcon(this->windowIcon());
        inspector->setPage(view->page());
    }

    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
    connect(view, SIGNAL(iconChanged()), SLOT(pageIconLoaded()));
    connect(view, SIGNAL(urlChanged(QUrl)), SLOT(urlChanged(QUrl)));

    connect(view->page(), SIGNAL(printRequested(QWebFrame*)), SLOT(printRequested(QWebFrame*)));

    connect(QApplication::desktop(), SIGNAL(resized(int)), SLOT(desktopResized(int)));

    show();

    setCentralWidget(view);

    view->page()->view()->setFocusPolicy(Qt::StrongFocus);
    view->setFocusPolicy(Qt::StrongFocus);
    this->setFocusPolicy(Qt::StrongFocus);

    if (mainSettings->value("view/fullscreen").toBool()) {
        showFullScreen();
    } else if (mainSettings->value("view/maximized").toBool()) {
        showMaximized();
    } else if (mainSettings->value("view/fixed-size").toBool()) {
        centerFixedSizeWindow();
    }

    QFileInfo finfo = QFileInfo();
    finfo.setFile(mainSettings->value("browser/homepage").toString());

    qDebug() << "Homepage: like file = " <<
                mainSettings->value("browser/homepage").toString() <<
                ", absolute path = " <<
                finfo.absoluteFilePath() <<
                ", local uri = " <<
                QUrl::fromLocalFile(
                    mainSettings->value("browser/homepage").toString()
                ).toString();

    if (finfo.isFile()) {
        qDebug() << "Homepage: Local FILE!";
        view->load(QUrl::fromLocalFile(
            finfo.absoluteFilePath()
        ));
    } else {
        qDebug() << "Homepage: Remote URI!";
        view->load(QUrl(
            mainSettings->value("browser/homepage").toString()
        ));
    }
}


void MainWindow::centerFixedSizeWindow()
{
    quint16 widowWidth = mainSettings->value("view/fixed-width").toUInt();
    quint16 widowHeight = mainSettings->value("view/fixed-height").toUInt();

    quint16 screenWidth = QApplication::desktop()->screenGeometry().width();
    quint16 screenHeight = QApplication::desktop()->screenGeometry().height();

    qDebug() << "Screen size: " << screenWidth << "x" << screenHeight;

    quint16 x = 0;
    quint16 y = 0;

    if (mainSettings->value("view/fixed-centered").toBool()) {
        x = (screenWidth - widowWidth) / 2;
        y = (screenHeight - widowHeight) / 2;
    } else {
        x = mainSettings->value("view/fixed-x").toUInt();
        y = mainSettings->value("view/fixed-y").toUInt();
    }

    qDebug() << "Move window to: (" << x << ";" << y << ")";

    move ( x, y );
    setFixedSize( widowWidth, widowHeight );

}


void MainWindow::clearCache()
{
    if (mainSettings->value("cache/enable").toBool()) {
        diskCache->clear();
    }
}

void MainWindow::clearCacheOnExit()
{
    if (mainSettings->value("cache/enable").toBool()) {
        if (mainSettings->value("cache/clear-on-exit").toBool()) {
            diskCache->clear();
        }
    }
}

void MainWindow::cleanupSlot()
{
    qDebug() << "Cleanup Slot (application exit)";
    clearCacheOnExit();
    QWebSettings::clearMemoryCaches();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        if (int(event->modifiers()) == Qt::CTRL) {
            QApplication::exit(0);
        }
        break;
    case Qt::Key_R:
        if (int(event->modifiers()) == Qt::CTRL) {
            clearCache();
            view->reload();
        }
        break;
    case Qt::Key_F5:
        view->reload();
        break;
    case Qt::Key_F12:
        if (mainSettings->value("inspector/enable").toBool()) {
            if (!inspector->isVisible()) {
                inspector->setVisible(true);
            } else {
                inspector->setVisible(false);
            }
        }
        break;
    case Qt::Key_F11:
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::loadSettings(QString ini_file)
{
    if (!ini_file.length()) {
        mainSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "QtWebkitKiosk", "config", this);
    } else {
        mainSettings = new QSettings(ini_file, QSettings::IniFormat, this);
    }
    qDebug() << "Ini file: " << mainSettings->fileName();

    if (!mainSettings->contains("application/organization")) {
        mainSettings->setValue("application/organization", "Organization" );
    }
    if (!mainSettings->contains("application/organization-domain")) {
        mainSettings->setValue("application/organization-domain", "www.example.com" );
    }
    if (!mainSettings->contains("application/name")) {
        mainSettings->setValue("application/name", "QtWebkitKiosk" );
    }
    if (!mainSettings->contains("application/version")) {
        mainSettings->setValue("application/version", VERSION );
    }
    if (!mainSettings->contains("application/icon")) {
        mainSettings->setValue("application/icon", ICON );
    }

    if (!mainSettings->contains("proxy/enable")) {
        mainSettings->setValue("proxy/enable", false);
    }
    if (!mainSettings->contains("proxy/system")) {
        mainSettings->setValue("proxy/system", true);
    }
    if (!mainSettings->contains("proxy/host")) {
        mainSettings->setValue("proxy/host", "proxy.example.com");
    }
    if (!mainSettings->contains("proxy/port")) {
        mainSettings->setValue("proxy/port", 3128);
    }
    if (!mainSettings->contains("proxy/auth")) {
        mainSettings->setValue("proxy/auth", false);
    }
    if (!mainSettings->contains("proxy/username")) {
        mainSettings->setValue("proxy/username", "username");
    }
    if (!mainSettings->contains("proxy/password")) {
        mainSettings->setValue("proxy/password", "password");
    }

    if (!mainSettings->contains("view/fullscreen")) {
        mainSettings->setValue("view/fullscreen", true);
    }
    if (!mainSettings->contains("view/maximized")) {
        mainSettings->setValue("view/maximized", false);
    }
    if (!mainSettings->contains("view/fixed-size")) {
        mainSettings->setValue("view/fixed-size", false);
    }
    if (!mainSettings->contains("view/fixed-width")) {
        mainSettings->setValue("view/fixed-width", 800);
    }
    if (!mainSettings->contains("view/fixed-height")) {
        mainSettings->setValue("view/fixed-height", 600);
    }
    if (!mainSettings->contains("view/fixed-centered")) {
        mainSettings->setValue("view/fixed-centered", true);
    }
    if (!mainSettings->contains("view/fixed-x")) {
        mainSettings->setValue("view/fixed-x", 0);
    }
    if (!mainSettings->contains("view/fixed-y")) {
        mainSettings->setValue("view/fixed-y", 0);
    }


    if (!mainSettings->contains("browser/homepage")) {
        mainSettings->setValue("browser/homepage", RESOURCES"default.html");
    }
    if (!mainSettings->contains("browser/javascript")) {
        mainSettings->setValue("browser/javascript", true);
    }
    if (!mainSettings->contains("browser/javascript_can_open_windows")) {
        mainSettings->setValue("browser/javascript_can_open_windows", false);
    }
    if (!mainSettings->contains("browser/javascript_can_close_windows")) {
        mainSettings->setValue("browser/javascript_can_close_windows", false);
    }
    if (!mainSettings->contains("browser/webgl")) {
        mainSettings->setValue("browser/webgl", false);
    }
    if (!mainSettings->contains("browser/java")) {
        mainSettings->setValue("browser/java", false);
    }
    if (!mainSettings->contains("browser/plugins")) {
        mainSettings->setValue("browser/plugins", true);
    }
    // Don't break on SSL errors
    if (!mainSettings->contains("browser/ignore_ssl_errors")) {
        mainSettings->setValue("browser/ignore_ssl_errors", true);
    }


    if (!mainSettings->contains("inspector/enable")) {
        mainSettings->setValue("inspector/enable", false);
    }
    if (!mainSettings->contains("inspector/visible")) {
        mainSettings->setValue("inspector/visible", false);
    }


    if (!mainSettings->contains("event-sounds/enable")) {
        mainSettings->setValue("event-sounds/enable", false);
    }
    if (!mainSettings->contains("event-sounds/window-clicked")) {
        mainSettings->setValue("event-sounds/window-clicked", RESOURCES"window-clicked.ogg");
    }
    if (!mainSettings->contains("event-sounds/link-clicked")) {
        mainSettings->setValue("event-sounds/link-clicked", RESOURCES"window-clicked.ogg");
    }

    if (!mainSettings->contains("cache/enable")) {
        mainSettings->setValue("cache/enable", false);
    }
    if (!mainSettings->contains("cache/location")) {
        QString location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
        QDir d = QDir(location);
        location += d.separator();
        location += mainSettings->value("application/name").toString();
        d.setPath(location);
        mainSettings->setValue("cache/location", d.absolutePath());
    }
    if (!mainSettings->contains("cache/size")) {
        mainSettings->setValue("cache/size", 100*1024*1024);
    }
    if (!mainSettings->contains("cache/clear-on-start")) {
        mainSettings->setValue("cache/clear-on-start", false);
    }
    if (!mainSettings->contains("cache/clear-on-exit")) {
        mainSettings->setValue("cache/clear-on-exit", false);
    }

    if (!mainSettings->contains("printing/show-printer-dialog")) {
        mainSettings->setValue("printing/show-printer-dialog", false);
    }
    if (!mainSettings->contains("printing/printer")) {
        mainSettings->setValue("printing/printer", "default");
    }
    if (!mainSettings->contains("printing/page_margin_left")) {
        mainSettings->setValue("printing/page_margin_left", 5);
    }
    if (!mainSettings->contains("printing/page_margin_top")) {
        mainSettings->setValue("printing/page_margin_top", 5);
    }
    if (!mainSettings->contains("printing/page_margin_right")) {
        mainSettings->setValue("printing/page_margin_right", 5);
    }
    if (!mainSettings->contains("printing/page_margin_bottom")) {
        mainSettings->setValue("printing/page_margin_bottom", 5);
    }

    if (!mainSettings->contains("attach/javascripts")) {
        mainSettings->setValue("attach/javascripts", "");
    }
    if (!mainSettings->contains("attach/styles")) {
        mainSettings->setValue("attach/styles", "");
    }

    mainSettings->sync();
}

void MainWindow::adjustTitle()
{
    if (progress <= 0 || progress >= 100) {
        setWindowTitle(view->title());
    } else {
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
    }
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}

void MainWindow::desktopResized(int p)
{
    qDebug() << "Desktop resized event: " << p;
    if (mainSettings->value("view/fullscreen").toBool()) {
        showFullScreen();
    } else if (mainSettings->value("view/maximized").toBool()) {
        showMaximized();
    } else if (mainSettings->value("view/fixed-size").toBool()) {
        centerFixedSizeWindow();
    }
}

void MainWindow::finishLoading(bool)
{
    progress = 100;
    adjustTitle();

    attachStyles();
    attachJavascripts();
}

void MainWindow::urlChanged(const QUrl &url)
{
    qDebug() << "URL changes: " << url.toString();
}

void MainWindow::attachJavascripts()
{
    if (!mainSettings->contains("attach/javascripts")) {
        return;
    }
    QStringList scripts = mainSettings->value("attach/javascripts").toStringList();
    if (!scripts.length()) {
        return;
    }
    QStringListIterator scriptsIterator(scripts);
    QFileInfo finfo = QFileInfo();
    QString file_name;
    quint32 countScripts = 0;
    while (scriptsIterator.hasNext()) {
        file_name = scriptsIterator.next();

        if (!file_name.length()) continue;

        qDebug() << "-- attach " << file_name;

        countScripts++;

        finfo.setFile(file_name);
        if (finfo.isFile()) {
            qDebug() << "-- it's local file";
            QFile f(file_name);
            QString content = "<script type=\"text/javascript\">";
            content += QString(f.readAll());
            content += "</script>\n";
            view->page()->mainFrame()->findFirstElement("body").appendInside(content);
            f.close();
        } else {
            qDebug() << "-- it's remote file";
            QString content = "<script type=\"text/javascript\" src=\"";
            content += file_name;
            content += "\"></script>\n";
            view->page()->mainFrame()->findFirstElement("body").appendInside(content);
        }
    }
    qDebug() << "Page loaded, found " << countScripts << " user javascript files...";
}

void MainWindow::attachStyles()
{
    if (!mainSettings->contains("attach/styles")) {
        return;
    }
    QStringList styles = mainSettings->value("attach/styles").toStringList();
    if (!styles.length()) {
        return;
    }
    QStringListIterator stylesIterator(styles);
    QString file_name;
    QFileInfo finfo = QFileInfo();
    quint32 countStyles = 0;
    while (stylesIterator.hasNext()) {
        file_name = stylesIterator.next();

        if (!file_name.length()) continue;

        qDebug() << "-- attach " << file_name;
        countStyles++;

        finfo.setFile(file_name);

        QString content;
        if (finfo.isFile()) {
            qDebug() << "-- it's local file";
            QFile f(file_name);
            content = "<style type=\"text/css\">\n";
            content += QString(f.readAll());
            content += "</style>\n";
            f.close();
        } else {
            qDebug() << "-- it's remote file";
            content = "<link type=\"text/css\" rel=\"stylesheet\" href=\"";
            content += file_name;
            content += "\"/>\n";
        }
        view->page()->mainFrame()->findFirstElement("head").appendInside(content);
    }
    qDebug() << "Page loaded, found " << countStyles << " user style files...";
}


void MainWindow::pageIconLoaded()
{
    setWindowIcon(view->icon());
}

void MainWindow::printRequested(QWebFrame *wf)
{
    if (!mainSettings->value("printing/show-printer-dialog").toBool()) {
        if (printer->printerState() != QPrinter::Error) {
            wf->print(printer);
        }
    }
}
