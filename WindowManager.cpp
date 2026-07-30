#include "WindowManager.h"
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QtCore>
#include <QFile>
#include <QTextStream>
#include <QPalette>
#include <QColor>
#include <QStringList>
#include <iostream>

#include "Game.h"
#include "RouteEditorWindow.h"
#include "LoadWindow.h"
#include "CELoadWindow.h"
#include "ShapeViewerWindow.h"
#include "MapWindow.h"
#include "RouteEditorServer.h"
#include "RouteEditorClient.h"
#include "Undo.h"


// Helper method to position windows consistently using Qt 6 screen metrics
static void positionWindowOnScreen(QWidget* window) {
    if (!window) return;

    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    if (primaryScreen) {
        const QSize windowSize = window->size();
        const QRect primaryGeometry = primaryScreen->geometry();
        const QPoint centeredPos((primaryGeometry.width() - windowSize.width()) / 2,
                                 (primaryGeometry.height() - windowSize.height()) / 2);

        if (Game::debugOutput) {
            qDebug() << "Primary: " << primaryGeometry.width() << "/" << primaryGeometry.height();
            qDebug() << "Window: " << windowSize.width() << "/" << windowSize.height();
            qDebug() << "Window   Orig: " << window->pos();
        }

        // Ensure the window stays within the primary monitor bounds
        window->move(centeredPos.x() >= 0 ? centeredPos.x() : 0,
                     centeredPos.y() >= 0 ? centeredPos.y() : 0);

        if (Game::debugOutput) {
            qDebug() << "Window Center: " << window->pos();
        }
    }

    // Override with saved window position if present
    QStringList winPos = Game::mainPos.split(","); 
    if (winPos.count() > 1) {
        window->move(winPos[0].trimmed().toInt(), winPos[1].trimmed().toInt());
    }

    if (Game::debugOutput) {
        qDebug() << "Window  Final: " << window->pos();
    }
}

void WindowManager::LoadConEditor() {
    CELoadWindow* ceLoadWindow = new CELoadWindow();
    positionWindowOnScreen(ceLoadWindow);
    ceLoadWindow->show();
}

void WindowManager::LoadShapeViewer(QString arg) {
    ShapeViewerWindow* shapeWindow = new ShapeViewerWindow();
    if (arg.length() > 0) {
        shapeWindow->loadFile(arg);
    }

    positionWindowOnScreen(shapeWindow);
    shapeWindow->show();
}

void WindowManager::LoadRouteEditor() {
    if (Game::serverLogin.length() > 0) {
        Game::ServerMode = true;
    }
    
    if (Game::ServerMode) {
        Game::useQuadTree = true;
        Undo::UndoEnabled = false;
        // Create Server Client
        Game::serverClient = new RouteEditorClient();
    }
    
    RouteEditorWindow *window = new RouteEditorWindow();

    if (Game::fullscreen) {
        window->setWindowFlags(Qt::CustomizeWindowHint);
        window->setWindowState(Qt::WindowMaximized);
    } else {
        window->resize(1280, 800);
        positionWindowOnScreen(window);
    }
        
    if (!Game::ServerMode) {
        LoadWindow *loadWindow = new LoadWindow();

        // Qt 6 standard PMF (pointer-to-member-function) signal/slot connections
        QObject::connect(window, &RouteEditorWindow::exitNow, loadWindow, &LoadWindow::exitNow);
        QObject::connect(loadWindow, &LoadWindow::showMainWindow, window, &RouteEditorWindow::showRoute);

        if (Game::checkRoot(Game::root) && (Game::checkRoute(Game::route) || Game::createNewRoutes)) {
            window->showRoute();
        } else {
            // Position load window to saved position
            QStringList winPos = Game::mainPos.split(","); 
            if (winPos.count() > 1) {
                loadWindow->move(winPos[0].trimmed().toInt(), winPos[1].trimmed().toInt());
            }

            loadWindow->show();
        }
    } else {
        // Connect Qt 6 client signal to window slot
        QObject::connect(Game::serverClient, &RouteEditorClient::loadRoute, window, &RouteEditorWindow::showRoute);
        Game::serverClient->connectNow();
    }
}

