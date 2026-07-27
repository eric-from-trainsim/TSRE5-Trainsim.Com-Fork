/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "StatusWindow.h"
#include "GeoCoordinates.h"
#include <QtWidgets>
#include <QDebug>
#include "Game.h"
#include "Route.h"
#include "RouteEditorGLWidget.h"
#include "ShapeLib.h"

StatusWindow::StatusWindow(QWidget* parent) : QWidget(parent) {
    this->setWindowFlags(Qt::WindowType::Tool);
    //this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setFixedWidth(300);
    this->setFixedHeight(180);
    this->setWindowTitle(tr("Status Window"));
    QStringList winPos = Game::statusPos.split(","); 
    if(winPos.count() > 1) this->move( winPos[0].trimmed().toInt(), winPos[1].trimmed().toInt());
   
    
    /// EFO New
    

      
   
    QVBoxLayout *v = new QVBoxLayout;
    v->setSpacing(2);
    v->setContentsMargins(0,1,1,1);
       
    QGridLayout *vbox = new QGridLayout;


/// EFO    
    vbox->setSpacing(2);
    vbox->setContentsMargins(3,0,1,0);    
    //int row = 0;
    vbox->addWidget(&status0,0,0);
    vbox->addWidget(&status1,1,0);
    vbox->addWidget(&status2,2,0);
    vbox->addWidget(&status3,3,0);
    vbox->addWidget(&status4,4,0);    

    vbox->addWidget(&status5,0,1);
    vbox->addWidget(&status6,1,1);
    vbox->addWidget(&status7,2,1);
    vbox->addWidget(&status8,3,1);
    vbox->addWidget(&status9,4,1);    

    vbox->addWidget(&status10,5,0);        
    vbox->addWidget(&status11,5,1);        
    
    v->addItem(vbox);    

    
    /// EFO end

    this->setLayout(v);

    if(Game::systemTheme == false)
    {
     statG = "background-color: green; color: white;";
     statY = "background-color: yellow; color: black;";
     statS = "background-color: black; color: white;";
     statR = "background-color: red; color: black;";
    }
    else    
    {
     statG = "background-color: #9BFF9B;";
     statY = "background-color: #FFFF9B";
     statS = "";     
     statR = "background-color: #FF9B9B";
    }
    
}


StatusWindow::~StatusWindow() {
}

void StatusWindow::hideEvent(QHideEvent *e){
    emit windowClosed();
}


void StatusWindow::cameraButtonAction(bool val){
    qDebug() << "Camera Clicked: " << val;
    if(val)
        emit enableTool("selectTool");
    else
        emit enableTool("selectTool");
}


void StatusWindow::recStatus(QString statName, QString statVal ){   
    // These get emitted from REGLW triggers and update here
    if(statName.contains("camera"))    { status0.setText(statVal); if(statVal.contains("ON")) status0.setStyleSheet(statG); else status0.setStyleSheet(statS);  }
    if(statName.contains("autotdb"))   { status1.setText(statVal); if(statVal.contains("ON")) status1.setStyleSheet(statG); else status1.setStyleSheet(statY);  }
    if(statName.contains("brushdir"))  { status2.setText(statVal); } 
    if(statName.contains("resize"))    { status3.setText(statVal); if(statVal.contains("ON")) status3.setStyleSheet(statY); else status3.setStyleSheet(statS);  }    
    if(statName.contains("select"))    { status4.setText(statVal); if(statVal.contains("ON")) status4.setStyleSheet(statG); else status4.setStyleSheet(statS);  } 
    
    if(statName.contains("camterr"))   { status5.setText(statVal); if(statVal.contains("Locked")) status5.setStyleSheet(statG); else status5.setStyleSheet(statY);  }    
    if(statName.contains("stickterr")) { status6.setText(statVal); if(statVal.contains("ON")) status6.setStyleSheet(statG); else status6.setStyleSheet(statY);  }
    if(statName.contains("rotate"))    { status7.setText(statVal); if(statVal.contains("ON")) status7.setStyleSheet(statY); else status7.setStyleSheet(statS);  }    
    if(statName.contains("translate")) { status8.setText(statVal); if(statVal.contains("ON")) status8.setStyleSheet(statY); else status8.setStyleSheet(statS);  }    
    if(statName.contains("place"))     { status9.setText(statVal); if(statVal.contains("ON")) status9.setStyleSheet(statG); else status9.setStyleSheet(statS);  }             
    if(statName.contains("timer"))     { status10.setText(statVal + "m elapsed without Save"); if(statVal.toInt() > 10) status10.setStyleSheet(statY); else status10.setStyleSheet(statS);  }
    if(statName.contains("object"))    { if(statVal.size() > 0) {status11.setText(statVal + " Object Selected"); status11.setStyleSheet(statY); } else {status11.setText(statVal + ""); status11.setStyleSheet(statS);}  }
}
    
