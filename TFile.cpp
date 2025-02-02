/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TFile.h"
#include <QDebug>
#include <QFile>
#include "ReadFile.h"
#include <QDataStream>
#include "Game.h"

TFile::TFile() {
    loaded = false;
    used = false;
}

TFile::TFile(const TFile& orig) {
}

TFile::~TFile() {
}

void TFile::initNew(QString name, int samples, int sampleS, int patches){
    sampleEbuffer = new QString(name + "_e.raw");
    sampleNbuffer = new QString(name + "_n.raw");
    sampleYbuffer = new QString(name + "_y.raw");
    
    waterLevel = true;
    WSW = 0;
    WSE = 0;
    WNE = 0;
    WNW = 0;
    
    errthresholdScale = new float();
    *errthresholdScale = 1;
    alwaysselectMaxdist = new float();
    *alwaysselectMaxdist = 0;
    nsamples = new int(samples);
    sampleRotation = new float();
    *sampleRotation = 0;
    sampleSize = new float(sampleS);
    floor = -63.0;
    scale = 0.00195312;

    patchsetDistance = 0;
    patchsetNpatches = patches;
    flags = new int[patches*patches];
    tdata = new float[patches*patches*13];
    errorBias = new float[patches*patches];
    int tileSize = samples*sampleS;
    float patchSize = tileSize/patches;
    float patchPosZ = -0.5*patchSize;
    float patchPosX = 0.5*patchSize;
    for(int j = 0; j < patches; j++, patchPosZ -= patchSize){
        patchPosX = 0.5*patchSize;
        for(int i = 0; i < patches; i++, patchPosX += patchSize){
            flags[(j*patches+i)] = 0;
            tdata[(j*patches+i)*13+0] = patchPosX;
            tdata[(j*patches+i)*13+1] = 1;
            tdata[(j*patches+i)*13+2] = patchPosZ;
            tdata[(j*patches+i)*13+3] = 99.48125458;
            tdata[(j*patches+i)*13+4] = 0;
            tdata[(j*patches+i)*13+5] = 0.5*patchSize;
            tdata[(j*patches+i)*13+6] = 0;
            tdata[(j*patches+i)*13+7] = 0.0001;
            tdata[(j*patches+i)*13+8] = 0.0001;
            tdata[(j*patches+i)*13+9] = 0.0625;
            tdata[(j*patches+i)*13+10] = 0;
            tdata[(j*patches+i)*13+11] = 0;
            tdata[(j*patches+i)*13+12] = 0.0625;
            errorBias[(j*patches+i)] = 1;
        }
    }
        
    newMat();
    loaded = true;
}

void TFile::setBufferNames(QString name){
    sampleEbuffer = new QString(name + "_e.raw");
    sampleNbuffer = new QString(name + "_n.raw");
    sampleYbuffer = new QString(name + "_y.raw");
}

bool TFile::readT(QString fSfile) {
        fSfile.replace("//","/");
        if(Game::debugOutput) qDebug() << "T File "<< fSfile;
        QFile file(fSfile);
        if (!file.open(QIODevice::ReadOnly)){
            //qDebug() << "fail t file "<< fSfile;
            return false;
        }
        FileBuffer* data = ReadFile::read(&file);
        //qDebug() << "Date:" << data->length;
        load(data);
        delete data;
        return true;
}
void TFile::load(FileBuffer* data){
        data->off += 32;
        int pozycja, offset, akto;
        data->findToken(136);
        if(Game::extendedDebug)  qDebug() << "znaleziono sekcje 136 na " << data->off << " ";
        data->off += 5;
        for (int i = 0; i < 6; i++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            if(Game::extendedDebug) qDebug() << "znaleziono sekcje " << pozycja <<" na " << data->off << " " << offset;

            switch (pozycja) {
                case 137:
                    data->off++;
                    errthresholdScale = new float;
                    *errthresholdScale = data->getFloat();
                    break;
                case 138:
                    data->off++;
                    alwaysselectMaxdist = new float;
                    *alwaysselectMaxdist = data->getFloat();
                    break;
                case 139:
                    get139(data, akto + offset);
                    break;
                case 251:
                    get251(data);
                    break;
                case 151:
                    get151(data);
                    break;
                case 157:
                    get157(data);
                    break;
                default:
                    if(Game::debugOutput) qDebug() << "TFile - unknown token: "<< pozycja;
                    i--;
                    break;
            }
            //if(pozycja==157) qDebug() << " ok";
            data->off = akto + offset;
            if(data->off >= data->length) break;
        }
    }

void TFile::get139(FileBuffer* data, int length) {
        int pozycja, offset, akto;
        int slen;
        data->off++;
        for (int j = 0; j < 2;) {

            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            if(Game::extendedDebug) qDebug() << "139 znaleziono sekcje " << pozycja <<" na " << data->off << " " << offset;

            switch (pozycja) {
                case 140:
                    data->off++;
                    nsamples = new int;
                    *nsamples = data->getInt();
                    break;
                case 141:
                    data->off++;
                    sampleRotation = new float;
                    *sampleRotation = data->getFloat();
                    break;    
                case 142:
                    data->off++;
                    floor = data->getFloat();
                    break;
                case 143:
                    data->off++;
                    scale = data->getFloat();
                    break;
                case 144:
                    data->off++;
                    sampleSize = new float;
                    *sampleSize = data->getFloat();
                    break;
                case 145:
                    data->off++;
                    slen = data->getShort()*2;
                    sampleFbuffer = data->getString(data->off, data->off + slen);
                    break;
                case 146:
                    data->off++;
                    slen = data->getShort()*2;
                    sampleYbuffer = data->getString(data->off, data->off + slen);
                    break;
                case 147:
                    data->off++;
                    slen = data->getShort()*2;
                    sampleEbuffer = data->getString(data->off, data->off + slen);
                    break;
                case 148:
                    data->off++;
                    slen = data->getShort()*2;
                    sampleNbuffer = data->getString(data->off, data->off + slen);
                    break;
                case 281:
                    data->off++;
                    sampleASbuffer = new char[257*257/8+1];
                    for(int i = 0; i <= 257*257/8; i++)
                        sampleASbuffer[i] = data->get();
                    break;
                default:
                    if(Game::debugOutput) qDebug() << "TFile - unknown token: "<< pozycja;
                    break;
            }
            if(Game::extendedDebug) 
                qDebug() << 
                    "140: " << nsamples <<                     
                    "141: " << sampleRotation <<
                    "142: " << floor << 
                    "143: " << scale << 
                    "144: " << sampleSize <<
                    "145: " << sampleFbuffer << 
                    "146: " << sampleYbuffer << 
                    "147: " << sampleEbuffer << 
                    "148: " << sampleNbuffer << 
                    "281: " << sampleASbuffer ;
                    
            data->off = akto + offset;
            if(data->off >= length) break;
        }
    }

void TFile::get151(FileBuffer* data) {
        data->off++;
        int pozycja, offset, akto;
        int slen;
        
        int ttilosc = data->getInt();
        //materials = new mat[ttilosc];
        materialsCount = ttilosc/2;
        //System.out.println("ilosc " + ttilosc);
        Mat *tmat;
        //int mtilosc = ttilosc/2;
        for (int j = 0; j < ttilosc; j++) {
            
            if(j < materialsCount)
                tmat = &materials[j];
            else
                tmat = &amaterials[j-materialsCount];
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;

            int tttpozycja, tttoffset, tttakto;
            data->off++;

            slen = data->getShort()*2;
            tmat->name = data->getString(data->off, data->off + slen);
            //*tname = tname->trimmed();
            //qDebug() << *tmat->name;
            data->off += slen;
            
            for (int jj = 0; jj < 2; ) {

                tttpozycja = data->getInt();
                tttoffset = data->getInt();
                tttakto = data->off;

                switch (tttpozycja) {
                    case 153:
                        get153(data, tmat);
                        jj++;
                        break;
                    case 155:
                        get156(data, tmat);
                        jj++;
                        break;
                    default:
                        break;
                }
                data->off = tttakto + tttoffset;
            }

           data->off = akto + offset;
        }
    }

void TFile::get153(FileBuffer* data, TFile::Mat* m) {
        int pozycja, offset, akto;
        int slen;
        
        data->off++;
        int ilosc = data->getInt();
        m->count153 = ilosc;
        if(Game::extendedDebug) qDebug() << "i to " << ilosc;
        for (int j = 0; j < ilosc; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            data->off ++;

            //m->tex[j] = "";
            //char bbb;
            //while ((bbb = data->get()) > 31) {
            //    m->tex[j] += bbb;
            //    data->off++;
            //}
            //m->tex[j] = m->tex[j].trimmed();

            slen = data->getShort()*2;
            m->tex[j] = data->getString(data->off, data->off + slen);
            //*tname = tname->trimmed();
            //qDebug() << *m->tex[j];
            data->off += slen;
            
            
            //qDebug() << j << " "<< *m->tex[j];
            //data->off -= 3;
            m->atex[j][0] = data->getInt();
            m->atex[j][1] = data->getInt();
            //qDebug() << m->atex[j][0] << " " << m->atex[j][1];
            data->off = akto + offset;
        }
    }
    
void TFile::get156(FileBuffer* data, TFile::Mat* m) {
        int pozycja, offset, akto;
        data->off++;
        int ilosc = data->getInt();
        m->count155 = ilosc;
        //System.out.println("i to " + ilosc);
        for (int j = 0; j < ilosc; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            data->off++;
            
            m->itex[j][0] = data->getInt();//&0xff;
            m->itex[j][1] = data->getInt();//&0xff;
            m->itex[j][2] = data->getInt();//&0xff;
            m->itex[j][3] = data->getInt();//&0xff;
            if(Game::extendedDebug) qDebug() << "get156: " << j <<":" <<m->itex[j][0] << " " << m->itex[j][1] << " " << m->itex[j][2] << " " << m->itex[j][3];

            data->off = akto + offset;
        }
    }
    
void TFile::get157(FileBuffer* data) {
        data->off++;
        int pozycja, offset, akto;
        
        pozycja = data->getInt();
        offset = data->getInt();
        akto = data->off;
        data->off++;
        int ttilosc = data->getInt();
        //qDebug() << "ttilosc " << ttilosc;
        if(Game::extendedDebug) qDebug() << "ilosc " << ttilosc;
        int tttpozycja, tttoffset, tttakto;
        //
        
        for (int j = 0; j < ttilosc; j++) {
            tttpozycja = data->getInt();
            tttoffset = data->getInt();
            tttakto = data->off;
            data->off++;

            int ttpozycja, ttoffset, ttakto;

            for (int jj = 0; jj < 3;) {
                ttpozycja = data->getInt();
                ttoffset = data->getInt();
                ttakto = data->off;

                switch (ttpozycja) {
                    case 160:
                        data->off++;
                        patchsetDistance = data->getInt();
                        jj++;
                        break;
                    case 161:
                        data->off++;
                        patchsetNpatches = data->getInt();
                        jj++;
                        break;
                    case 163:
                        get163(data, patchsetNpatches);
                        jj++;
                        break;                        
                    default:
                        break;
                }
                data->off = ttakto + ttoffset;
            }
            data->off = tttakto + tttoffset;
        }
    }
    
void TFile::get163(FileBuffer* data, int n) {
        int pozycja, offset, akto;
        data->off++;
        //int ilosc = data.getInt();
        //qDebug() << "i to " << n;
        tdata = new float[n*n*13];
        errorBias = new float[n*n];
        flags = new int[n*n];
        for (int j = 0; j < n*n; j++) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            data->off++;
            
            flags[j] = data->getInt();//&0xFFFF;
            //data->off += 4*6;
            tdata[j*13+0] = data->getFloat();
            tdata[j*13+1] = data->getFloat();
            tdata[j*13+2] = data->getFloat();
            tdata[j*13+3] = data->getFloat();
            tdata[j*13+4] = data->getFloat();
            tdata[j*13+5] = data->getFloat();
            tdata[j*13+6] = (float)data->getInt();
            if(tdata[j*13+6] >= materialsCount)
                tdata[j*13+6] -= materialsCount;
            tdata[j*13+7] = data->getFloat();
            tdata[j*13+8] = data->getFloat();
            tdata[j*13+9] = data->getFloat();
            tdata[j*13+10] = data->getFloat();
            tdata[j*13+11] = data->getFloat();
            tdata[j*13+12] = data->getFloat();
            errorBias[j] = data->getFloat();
            if(Game::extendedDebug) qDebug() << "get163: " << tdata[j*7+1] << tdata[j*7+2] << tdata[j*7+3] << tdata[j*7+4] << tdata[j*7+5] << tdata[j*7+6];
                                
            //data->off = akto + offset;
        }
    }

void TFile::get251(FileBuffer* data) {
        data->off++;
        
        waterLevel = true;
        WSW = data->getFloat();
        WSE = data->getFloat();
        WNE = data->getFloat();
        WNW = data->getFloat();
        //tdata = new float[n*n][7];
        //flags = new int[n*n];
    }

int TFile::cloneMat(int id){
    QString* name = new QString();
    *name += *materials[id].name;
    materials[materialsCount].name = name;
    
    materials[materialsCount].count153 = materials[id].count153;
    for(int i = 0; i < materials[id].count153; i++){
        name = new QString();
        *name += *materials[id].tex[i];
        materials[materialsCount].tex[i] = name;
        materials[materialsCount].atex[i][0] = materials[id].atex[i][0];
        materials[materialsCount].atex[i][1] = materials[id].atex[i][1];
    }
    
    materials[materialsCount].count155 = materials[id].count155;
    for(int i = 0; i < materials[id].count155; i++){
        materials[materialsCount].itex[i][0] = materials[id].itex[i][0];
        materials[materialsCount].itex[i][1] = materials[id].itex[i][1];
        materials[materialsCount].itex[i][2] = materials[id].itex[i][2];
        materials[materialsCount].itex[i][3] = materials[id].itex[i][3];
    }
        
    cloneAMat(id);
    return this->materialsCount++;
}



int TFile::cloneAMat(int id){
    QString* name = new QString();
    *name += *amaterials[id].name;
    amaterials[materialsCount].name = name;
    
    amaterials[materialsCount].count153 = amaterials[id].count153;
    for(int i = 0; i < amaterials[id].count153; i++){
        name = new QString();
        *name += *amaterials[id].tex[i];
        amaterials[materialsCount].tex[i] = name;
        amaterials[materialsCount].atex[i][0] = amaterials[id].atex[i][0];
        amaterials[materialsCount].atex[i][1] = amaterials[id].atex[i][1];
    }
    
    amaterials[materialsCount].count155 = amaterials[id].count155;
    for(int i = 0; i < materials[id].count155; i++){
        amaterials[materialsCount].itex[i][0] = amaterials[id].itex[i][0];
        amaterials[materialsCount].itex[i][1] = amaterials[id].itex[i][1];
        amaterials[materialsCount].itex[i][2] = amaterials[id].itex[i][2];
        amaterials[materialsCount].itex[i][3] = amaterials[id].itex[i][3];
    }
        
    return this->materialsCount;
}

int TFile::newMat(){
    QString* name = new QString();
    *name += "DetailTerrain";
    materials[materialsCount].name = name;
    name = new QString();
    *name += "AlphaTerrain";
    amaterials[materialsCount].name = name;
    
    materials[materialsCount].count153 = 2;
    name = new QString();
    *name += "terrain.ace";
    materials[materialsCount].tex[0] = name;
    materials[materialsCount].atex[0][0] = 1;
    materials[materialsCount].atex[0][1] = 0;
    name = new QString();
    *name += "microtex.ace";
    materials[materialsCount].tex[1] = name;
    materials[materialsCount].atex[1][0] = 1;
    materials[materialsCount].atex[1][1] = 1;

    materials[materialsCount].count155 = 2;
    materials[materialsCount].itex[0][0] = 1;
    materials[materialsCount].itex[0][1] = 0;
    materials[materialsCount].itex[0][2] = 0;
    materials[materialsCount].itex[0][3] = 0;
    materials[materialsCount].itex[1][0] = 2;
    materials[materialsCount].itex[1][1] = 0;
    materials[materialsCount].itex[1][2] = 1;
    materials[materialsCount].itex[1][3] = 1107296256;
    
    amaterials[materialsCount].count153 = 1;
    name = new QString();
    *name += "terrain.ace";
    amaterials[materialsCount].tex[0] = name;
    amaterials[materialsCount].atex[0][0] = 1;
    amaterials[materialsCount].atex[0][1] = 0;
    
    amaterials[materialsCount].count155 = 1;
    amaterials[materialsCount].itex[0][0] = 1;
    amaterials[materialsCount].itex[0][1] = 0;
    amaterials[materialsCount].itex[0][2] = 0;
    amaterials[materialsCount].itex[0][3] = 0;
        
    return this->materialsCount++;
}

void TFile::removeMat(int id){
    if(id <= 0) 
        return;
    if(id > materialsCount)
        return;
    
    for(int j = 0; j < patchsetNpatches*patchsetNpatches; j++){
        if(tdata[j*13+0+6] == id){
            tdata[j*13+0+6] = 0;
            tdata[j*13 + 1 + 6] = 0.001;
            tdata[j*13 + 2 + 6] = 0.001;
            tdata[j*13 + 3 + 6] = 0.062375;
            tdata[j*13 + 4 + 6] = 0.0;
            tdata[j*13 + 5 + 6] = 0.0;
            tdata[j*13 + 6 + 6] = 0.062375;
        }
    }
    for(int j = 0; j < patchsetNpatches*patchsetNpatches; j++){
        if(tdata[j*13+0+6] == materialsCount-1)
            tdata[j*13+0+6] = id;
    }
    
    if(Game::debugOutput) qDebug() << *materials[id].name;
    materials[id] = materials[materialsCount-1];
    amaterials[id] = amaterials[materialsCount-1];
    if(Game::debugOutput) qDebug() << *materials[id].name;
    materialsCount--;
}

int TFile::getMatByTexture(QString tname){
    for(int j = 0; j < materialsCount; j++)
        for(int i = 0; i < materials[j].count153; i++){
            if(tname.toLower() == materials[j].tex[i]->toLower())
                return j;
        }
    return -1;
}

void TFile::save(QString name){
    name.replace("//", "/");
    QFile *file = new QFile(name);
    if(Game::debugOutput) qDebug() << "zapis .t "<<name;
    if (!file->open(QIODevice::WriteOnly))
        return;
    QDataStream write(file);
    write.setByteOrder(QDataStream::LittleEndian);
    write.setFloatingPointPrecision(QDataStream::SinglePrecision);
    save(write);
    write.unsetDevice();
    file->close();
}

void TFile::save(QDataStream &write){
    //calculate size
    
    int t137 = 0;
    if(errthresholdScale != NULL)
        t137 = 13;
   
    int t251 = 0;
    if(waterLevel)
        t251 = 25;
    
    int t138 = 0;
    if(alwaysselectMaxdist != NULL)
        t138 = 13;

    // 139
    int t139 = 1;
    // 140
    if(nsamples != NULL)
        t139+=13;
    // 141 
    if(sampleRotation != NULL)
        t139+=13;
    // 142
    t139+=13;
    // 143 
    t139+=13;
    // 144 
    if(sampleSize != NULL)
        t139+=13;
    // 281
    if(sampleASbuffer != NULL)
        t139+=257*257/8+2+8;
    // 145
    if(sampleFbuffer != NULL)
        t139+=sampleFbuffer->length()*2+3+8;
    // 146
    if(sampleYbuffer != NULL)
        t139+=sampleYbuffer->length()*2+3+8;
    // 147
    if(sampleEbuffer != NULL)
        t139+=sampleEbuffer->length()*2+3+8;
    // 148
    if(sampleNbuffer != NULL)
        t139+=sampleNbuffer->length()*2+3+8;
    
    // 151 
    int t151 = 0;
    t151+=5;
    int* t152 = new int[materialsCount*2];
    int* t153 = new int[materialsCount*2];
    Mat tmat;
    for(int j = 0; j < materialsCount*2; j++){
        t152[j] = 1;
        if(j < materialsCount)
            tmat = materials[j];
        else
            tmat = amaterials[j-materialsCount];
        t152[j] += tmat.name->length()*2+2;
        t152[j] += 13;
        t153[j] = 5;
        for(int i = 0; i < tmat.count153; i++){
            t152[j] += 11;
            t152[j] += tmat.tex[i]->length()*2;
            t152[j] += 8;
            t153[j] += 11;
            t153[j] += tmat.tex[i]->length()*2;
            t153[j] += 8;
        }
        t152[j] += 13;
        t152[j] += 25*tmat.count155;
        t151 += t152[j] + 8;
    }

    // 157
    int t157 = 0;
    t157 = 69*patchsetNpatches*patchsetNpatches+9+24+3+8+4+1+9;
    
    int t136 = 1 + t137 + t251 + t138 + t139 + 8 + t151 + 8 + t157 + 8;
    
    //write
    const char header[] = {
        0x53,0x49,0x4D,0x49,0x53,0x41,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
        0x4A,0x49,0x4E,0x58,0x30,0x74,0x36,0x62,0x5F,0x5F,0x5F,0x5F,0x5F,0x5F,0x0D,0x0A
    };
    write.writeRawData(header, 32);
    write << (qint32)136;
    write << (qint32)t136;
    write << (qint8)0;
    
    if(errthresholdScale != NULL){
        write << (qint32)137;
        write << (qint32)5;
        write << (qint8)0;
        write << *errthresholdScale;
    }
    
    if(waterLevel){
        write << (qint32)251;
        write << (qint32)17;
        write << (qint8)0;
        write << WSW;
        write << WSE;
        write << WNE;
        write << WNW;
    }
    
    if(alwaysselectMaxdist != NULL){
        write << (qint32)138;
        write << (qint32)5;
        write << (qint8)0;
        write << *alwaysselectMaxdist;
    }
    
    write << (qint32)139;
    write << (qint32)t139;
    write << (qint8)0;
    // 140
    if(nsamples != NULL){
        write << (qint32)140;
        write << (qint32)5;
        write << (qint8)0;
        write << *nsamples;
    }
    // 141 
    if(sampleRotation != NULL){
        write << (qint32)141;
        write << (qint32)5;
        write << (qint8)0;
        write << *sampleRotation;
    }
    // 142
    write << (qint32)142;
    write << (qint32)5;
    write << (qint8)0;
    write << floor;
    // 143 
    write << (qint32)143;
    write << (qint32)5;
    write << (qint8)0;
    write << scale;
    // 144 
    if(sampleSize != NULL){
        write << (qint32)144;
        write << (qint32)5;
        write << (qint8)0;
        write << *sampleSize;
    }
    // 281
    if(sampleASbuffer != NULL){
        write << (qint32)281;
        write << (qint32)257*257/8+2;
        write << (qint8)0;
        write.writeRawData(sampleASbuffer, 257*257/8+1);
    }
    // 145
    if(sampleFbuffer != NULL){
        write << (qint32)145;
        write << (qint32)sampleFbuffer->length()*2+3;
        write << (qint8)0;
        write << (qint16)sampleFbuffer->length();
        for(int i = 0; i < sampleFbuffer->length(); i++){
            write << sampleFbuffer->at(i).unicode();
        }
    }
    // 146
    if(sampleYbuffer != NULL){
        write << (qint32)146;
        write << (qint32)sampleYbuffer->length()*2+3;
        write << (qint8)0;
        write << (qint16)sampleYbuffer->length();
        for(int i = 0; i < sampleYbuffer->length(); i++){
            write << sampleYbuffer->at(i).unicode();
        }
    }
    // 147
    if(sampleEbuffer != NULL){
        write << (qint32)147;
        write << (qint32)sampleEbuffer->length()*2+3;
        write << (qint8)0;
        write << (qint16)sampleEbuffer->length();
        for(int i = 0; i < sampleEbuffer->length(); i++){
            write << sampleEbuffer->at(i).unicode();
        }
    }
    // 148
    if(sampleNbuffer != NULL){
        write << (qint32)148;
        write << (qint32)sampleNbuffer->length()*2+3;
        write << (qint8)0;
        write << (qint16)sampleNbuffer->length();
        for(int i = 0; i < sampleNbuffer->length(); i++){
            write << sampleNbuffer->at(i).unicode();
        }
    }

    // 151 
    write << (qint32)151;
    write << (qint32)t151;
    write << (qint8)0;
    write << (qint32)materialsCount*2;
    
    for(int j = 0; j < materialsCount*2; j++){
        if(j < materialsCount)
            tmat = materials[j];
        else
            tmat = amaterials[j-materialsCount];
        write << (qint32)152;
        write << (qint32)t152[j];
        write << (qint8)0;
        write << (qint16)tmat.name->length();
        for(int i = 0; i < tmat.name->length(); i++){
            write << tmat.name->at(i).unicode();
        }
        write << (qint32)153;
        write << (qint32)t153[j];
        write << (qint8)0;
        write << (qint32)tmat.count153;
        for(int i = 0; i < tmat.count153; i++){
            write << (qint32)154;
            write << (qint32)(tmat.tex[i]->length()*2+3+8);
            write << (qint8)0;
            write << (qint16)tmat.tex[i]->length();
            for(int ii = 0; ii < tmat.tex[i]->length(); ii++){
                write << tmat.tex[i]->at(ii).unicode();
            }
            write << (qint32)tmat.atex[i][0];
            write << (qint32)tmat.atex[i][1];
        }
        write << (qint32)155;
        write << (qint32)(25*tmat.count155+5);
        write << (qint8)0;
        write << (qint32)tmat.count155;
        for(int i = 0; i < tmat.count155; i++){
            write << (qint32)156;
            write << (qint32)17;
            write << (qint8)0;
            write << (qint32)tmat.itex[i][0];
            write << (qint32)tmat.itex[i][1];
            write << (qint32)tmat.itex[i][2];
            write << (qint32)tmat.itex[i][3];
        }
    }
    
    // 157
    write << (qint32)157;
    write << (qint32)t157;
    write << (qint8)0;
    
    write << (qint32)158;
    write << (qint32)69*patchsetNpatches*patchsetNpatches+9+24+3+8+4+1;
    write << (qint8)0;
    write << (qint32)1;
    
    write << (qint32)159;
    write << (qint32)69*patchsetNpatches*patchsetNpatches+9+24+3;
    write << (qint8)0;

    write << (qint32)160;
    write << (qint32)5;
    write << (qint8)0;
    write << (qint32)patchsetDistance;
    
    write << (qint32)161;
    write << (qint32)5;
    write << (qint8)0;
    write << (qint32)patchsetNpatches;
    
    write << (qint32)163;
    write << (qint32)69*patchsetNpatches*patchsetNpatches+1;
    write << (qint8)0;

    for(int j = 0; j < patchsetNpatches*patchsetNpatches; j++){
        write << (qint32)164;
        write << (qint32)61;
        write << (qint8)0;
        write << (qint32)flags[j];
        write << (float)tdata[j*13+0];
        write << (float)tdata[j*13+1];
        write << (float)tdata[j*13+2];
        write << (float)tdata[j*13+3];
        write << (float)tdata[j*13+4];
        write << (float)tdata[j*13+5];
        write << (qint32)tdata[j*13+6];
        write << (float)tdata[j*13+7];
        write << (float)tdata[j*13+8];
        write << (float)tdata[j*13+9];
        write << (float)tdata[j*13+10];
        write << (float)tdata[j*13+11];
        write << (float)tdata[j*13+12];
        write << (float)errorBias[j];
    }
}

void TFile::print(){
    if(Game::debugOutput) qDebug() << "Materials count " << (qint32)materialsCount;
    
    Mat tmat;
    for(int j = 0; j < materialsCount*2; j++){
        if(j < materialsCount)
            tmat = materials[j];
        else
            tmat = amaterials[j-materialsCount];
        if(Game::debugOutput) qDebug() << "Material " << j <<" "<< *tmat.name;

        if(Game::debugOutput) qDebug() << (qint32)tmat.count153;
        for(int i = 0; i < tmat.count153; i++){

            if(Game::debugOutput) qDebug() << "- "<< i << " " << *tmat.tex[i]
            << " " << (qint32)tmat.atex[i][0]
            << " " << (qint32)tmat.atex[i][1];
        }

        if(Game::debugOutput) qDebug() << (qint32)tmat.count155;
        for(int i = 0; i < tmat.count155; i++){
            if(Game::debugOutput) qDebug() << "+ "<< i << " " << (qint32)tmat.itex[i][0]
            << " " << (qint32)tmat.itex[i][1]
            << " " << (qint32)tmat.itex[i][2]
            << " " << (qint32)tmat.itex[i][3];
        }
    }
}