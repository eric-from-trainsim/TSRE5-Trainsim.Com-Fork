/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SOUNDSOURCEOBJ_H
#define	SOUNDSOURCEOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class PoleObj;

class SoundSourceObj : public WorldObj {
public:
    SoundSourceObj();
    SoundSourceObj(const SoundSourceObj& o);
    WorldObj* clone();
    virtual ~SoundSourceObj();
    bool allowNew();
    bool isSoundItem();
    void load(int x, int y);
    void set(QString sh, long long int val);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    int getDefaultDetailLevel();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);
private:
    PoleObj* pointer3d = NULL;
    PoleObj* pointer3dSelected = NULL;
    
    int soundDefinitionId;
    int soundSourceId;
};

#endif	/* SOUNDSOURCEOBJ_H */

