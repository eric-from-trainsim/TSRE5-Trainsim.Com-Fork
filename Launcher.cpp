#include "Launcher.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>

QString Launcher::getSelection() {
    // 1. Switch to a standard QDialog
    QDialog diary; 
    diary.setWindowTitle("TSRE 8.006 Launcher");
    
    // 2. Main vertical layout for the whole window
    QVBoxLayout *mainLayout = new QVBoxLayout(&diary);

    // 3. Image at the top
    QPixmap pixmap("tsre_appdata/load.png");
    if (!pixmap.isNull()) {
        QLabel *imgLabel = new QLabel();
        imgLabel->setPixmap(pixmap);
        imgLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(imgLabel);
    }

    // 4. Instructions
    QLabel *txtLabel = new QLabel("Please select an application to start:");
    txtLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(txtLabel);

    // 5. Create stacked buttons
    QPushButton *btnRE = new QPushButton("Route Editor");
    
    QPushButton *btnCE = new QPushButton("Consist Editor");
    
    QPushButton *btnSV = new QPushButton("Shape Viewer");
    
    QPushButton *btnSE = new QPushButton("Settings Editor");
    
    QPushButton *btnExit = new QPushButton("Exit Application");


    // Make buttons a uniform size
    btnRE->setMinimumHeight(30);

    mainLayout->addWidget(btnRE);
    mainLayout->addWidget(btnCE);
    mainLayout->addWidget(btnSV);
    mainLayout->addWidget(btnSE);
    mainLayout->addSpacing(10); // Gap before exit
    mainLayout->addWidget(btnExit);

    // 6. Logic to return values
    QString choice = "z"; // Default to exit
    QObject::connect(btnCE, &QPushButton::clicked, [&]() { choice = "c"; diary.accept(); });
    QObject::connect(btnRE, &QPushButton::clicked, [&]() { choice = "r"; diary.accept(); });
    QObject::connect(btnSV, &QPushButton::clicked, [&]() { choice = "s"; diary.accept(); });
    QObject::connect(btnSE, &QPushButton::clicked, [&]() { choice = "x"; diary.accept(); });
    QObject::connect(btnExit, &QPushButton::clicked, [&]() { choice = "z"; diary.reject(); });

    diary.exec();

    return choice;
}