/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESABSTRACT_H
#define	PROPERTIESABSTRACT_H

#include <QtWidgets>
#include "Undo.h"

class GameObj;
class WorldObj;

class PropertiesAbstract : public QWidget {
    Q_OBJECT
public:
    PropertiesAbstract();
    virtual ~PropertiesAbstract();
    virtual bool support(GameObj* obj);
    virtual void showObj(GameObj* obj);
    virtual void updateObj(GameObj* obj);
    
public slots:
    void copyFileNameEnabled();
    void copyREnabled();
    void pasteREnabled();
    void copyPEnabled();
    void pastePEnabled();
    void copyPREnabled();
    void pastePREnabled();
    void copyFEnabled();
    void pasteFEnabled();
    void rotYEnabled();
    void transformEnabled();
    void rtransformEnabled();
    void resetRotEnabled();
    void msg(QString name, QString val);
    
signals:
    void enableTool(QString);
    
protected:
    static QString ElevTypeName;
    
    QLabel *infoLabel;    
    QLineEdit flags;
    QLineEdit fileName;
    QLineEdit posX;
    QLineEdit posY;
    QLineEdit posZ;
    QLineEdit quat;
    QLineEdit tX;
    QLineEdit tY;
    QLineEdit uid;
    QLineEdit defaultDetailLevel;
    QLineEdit customDetailLevel;
    QComboBox eTemplate;
    QCheckBox enableCustomDetailLevel;
    WorldObj *worldObj;
    QMap<QString, QPushButton*> buttonTools;
private:

};

#endif	/* PROPERTIESABSTRACT_H */

