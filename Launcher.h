#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QMessageBox>
#include <QPushButton>
#include <QString>

class Launcher {
public:
    // Returns the char code for the selected app, or 'z' for exit
    static QString getSelection();
};

#endif // LAUNCHER_H