#include "routemergedialog.h"
#include "Game.h"
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

RouteMergeDialog::RouteMergeDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Route Merge Options");
    setFixedSize(500, 200);
    
    
    // 1. Initialize UI elements
    rmdlabel1 = new QLineEdit(this);
    rmdcheckBox0 = new QCheckBox("Merge Scenery", this);
    rmdcheckBox1 = new QCheckBox("Merge TDB", this);
    rmdcheckBox2 = new QCheckBox("Merge RDB", this);    
    rmdcheckBox3 = new QCheckBox("Merge Terrain", this);
    rmdcheckBox4 = new QCheckBox("Merge Terrain Textures", this);
    rmdokButton = new QPushButton("Proceed with Merge", this);
    rmdcancelButton = new QPushButton("Cancel Merge", this);

    // 2. Safe data parsing
    QStringList args = Game::routeMergeString.split(":");
    QString mergeRouteName = (!args.isEmpty()) ? args[0].remove('"').remove(';') : "Unknown";
    rmdlabel1->setText("Merging " + mergeRouteName + " into " + Game::route);
    
    // 3. Configure State
    rmdcheckBox0->setChecked(true);
    rmdcheckBox0->setEnabled(false);
    
    if(Game::routeMergeTDB) rmdcheckBox1->setChecked(true);
    if(Game::routeMergeRDB) rmdcheckBox2->setChecked(true);
    if(Game::routeMergeTerrain) rmdcheckBox3->setChecked(true);
    if(Game::routeMergeTerrtex) rmdcheckBox4->setChecked(true);

    rmdcheckBox1->setEnabled(false);
    rmdcheckBox2->setEnabled(false);
    rmdcheckBox3->setEnabled(false);
    rmdcheckBox4->setEnabled(false);
    rmdlabel1->setEnabled(false);
    
    // 4. Layout management
    QVBoxLayout *rmdcheckBoxLayout = new QVBoxLayout;
    rmdcheckBoxLayout->addWidget(rmdlabel1);
    rmdcheckBoxLayout->addWidget(rmdcheckBox0);    
    rmdcheckBoxLayout->addWidget(rmdcheckBox1);
    rmdcheckBoxLayout->addWidget(rmdcheckBox2);
    rmdcheckBoxLayout->addWidget(rmdcheckBox3);
    rmdcheckBoxLayout->addWidget(rmdcheckBox4);

    QHBoxLayout *rmdbuttonLayout = new QHBoxLayout;
    rmdbuttonLayout->addWidget(rmdcancelButton);
    rmdbuttonLayout->addWidget(rmdokButton);

    // Set main layout with 'this' as parent to ensure proper memory management
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(rmdcheckBoxLayout);
    mainLayout->addLayout(rmdbuttonLayout);

    // 5. Signal connections
    connect(rmdokButton, &QPushButton::clicked, this, &RouteMergeDialog::accept);
    connect(rmdcancelButton, &QPushButton::clicked, this, &RouteMergeDialog::reject);
 
 
}