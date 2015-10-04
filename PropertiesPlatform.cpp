#include "PropertiesPlatform.h"
#include "WorldObj.h"

PropertiesPlatform::PropertiesPlatform() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Platform:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    
    // names
    QLabel * label = new QLabel("Station Name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->nameStation);
    label = new QLabel("Platform Name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->namePlatform);
    // side
    label = new QLabel("Side:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    leftSide.setText("Left");
    rightSide.setText("Right");
    vbox->addWidget(&leftSide);
    vbox->addWidget(&rightSide);
    // wait
    label = new QLabel("Platform Wait:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Minutes:",&this->waitMin);
    vlist->addRow("Seconds:",&this->waitSec);
    vlist->addRow("Passengers:",&this->waitPas);
    vbox->addItem(vlist);
    // misc
    label = new QLabel("Misc:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    disablePlatform.setText("Disable Platform");
    vbox->addWidget(&disablePlatform);
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesPlatform::~PropertiesPlatform() {
}

void PropertiesPlatform::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    
    this->infoLabel->setText("Object: "+obj->type);
    /*this->fileName.setText(obj->fileName);
    
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    this->posX.setText(QString::number(obj->position[0], 'G', 4));
    this->posY.setText(QString::number(obj->position[1], 'G', 4));
    this->posZ.setText(QString::number(obj->position[2], 'G', 4));
    this->quat.setText(
            QString::number(obj->qDirection[0], 'G', 4) + " " +
            QString::number(obj->qDirection[1], 'G', 4) + " " +
            QString::number(obj->qDirection[2], 'G', 4) + " " +
            QString::number(obj->qDirection[3], 'G', 4)
            );
    
    
    int l;
    QString flags;
    flags = QString::number(obj->staticFlags, 16);
    l = flags.length();
    for(int i=0; i<8-l; i++)
        flags = "0"+flags;
    this->flags.setText(flags);*/
}

bool PropertiesPlatform::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "platform")
        return true;
    return false;
}