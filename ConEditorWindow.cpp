#include "ConEditorWindow.h"
#include <QDebug>
#include "EngLib.h"
#include "ConLib.h"
#include "Eng.h"
#include "Consist.h"
#include "Game.h"
#include "EngListWidget.h"
#include "ConListWidget.h"
#include "glShapeWidget.h"
#include "CameraFree.h"
#include "CameraConsist.h"
#include "CameraRot.h"
#include "GuiFunct.h"
#include "ConUnitsWidget.h"
#include "AboutWindow.h"
#include "OverwriteDialog.h"
#include "UnsavedDialog.h"

ConEditorWindow::ConEditorWindow() : QMainWindow() {
    aboutWindow = new AboutWindow();
    englib = new EngLib();
    //conEngLib = new EngLib();
    englib->loadAll(Game::root);
    Game::currentEngLib = englib;
    ConLib::loadAll(Game::root);
    glShapeWidget = new GlShapeWidget(this);
    //glShapeWidget->currentEngLib = englib;
    glConWidget = new GlShapeWidget(this);
    glConWidget->currentEngLib = englib;
    conCamera = new CameraConsist();
    conCamera->setPos(-100,2.5,42);
    conCamera->setPlayerRot(M_PI/2.0,0);
    engCamera = new CameraRot();
    engCamera->setPos(0,2.5,0);
    engCamera->setPlayerRot(M_PI/2.0,0);
    
    cDurability.setDecimals(2);
    cDurability.setMinimum(0);
    cDurability.setMaximum(1);
    cDurability.setSingleStep(0.05);
    
    glConWidget->setCamera(conCamera);
    glShapeWidget->setCamera(engCamera);
    glShapeWidget->setMode("rot");
    //qDebug()<<"aaa";
    eng1 = new EngListWidget();
    eng1->englib = englib;
    eng1->fillEngList();
    eng2 = new EngListWidget();
    eng2->englib = englib;
    eng2->fillEngList();
    units = new ConUnitsWidget();
    units->englib = englib;
    con1 = new ConListWidget();
    con1->englib = englib;
    con1->fillConList();
    //qDebug()<<"aaa";
    conSlider = new QScrollBar(Qt::Horizontal);
    conSlider->setMaximum(0);
    conSlider->setMinimum(0);
    
    QWidget* main = new QWidget();
    
    QVBoxLayout *mbox = new QVBoxLayout;
    mbox->setSpacing(2);
    mbox->setContentsMargins(1,1,1,1);
    QGridLayout *vbox = new QGridLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    vbox->addWidget(con1,0,0);
    vbox->addWidget(units,0,1);
    vbox->addWidget(eng1,0,2);
    vbox->addWidget(eng2,0,3);
    engInfo = new QWidget(this);
    QVBoxLayout *engInfoLayout = new QVBoxLayout;
    engInfoLayout->addWidget(glShapeWidget);
    glShapeWidget->setMinimumSize(100, 100);
    QGridLayout *engInfoForm = new QGridLayout;
    engInfoForm->setSpacing(2);
    engInfoForm->setContentsMargins(1,1,1,1);    
    engInfoForm->addWidget(new QLabel("File Name:"),0,0);
    engInfoForm->addWidget(new QLabel("Dir Nam:"),0,2);
    engInfoForm->addWidget(new QLabel("Name:"),1,0);
    engInfoForm->addWidget(new QLabel("Type:"),1,2);
    engInfoForm->addWidget(new QLabel("Mass:"),2,0);
    engInfoForm->addWidget(new QLabel("Max. Speed:"),2,2);
    engInfoForm->addWidget(new QLabel("Brakes:"),3,0);
    engInfoForm->addWidget(new QLabel("Couplings:"),4,0);
    engInfoForm->addWidget(new QLabel("Shape:"),3,2);
    engInfoForm->addWidget(new QLabel("Size:"),4,2);
    engInfoForm->addWidget(&eFileName,0,1);
    engInfoForm->addWidget(&eDirName,0,3);
    engInfoForm->addWidget(&eName,1,1);
    engInfoForm->addWidget(&eType,1,3);
    engInfoForm->addWidget(&eMass,2,1);
    engInfoForm->addWidget(&eMaxSpeed,2,3);
    engInfoForm->addWidget(&eBrakes,3,1);
    engInfoForm->addWidget(&eCouplings,4,1);
    engInfoForm->addWidget(&eShape,3,3);
    engInfoForm->addWidget(&eSize,4,3);
    engInfoLayout->addItem(engInfoForm);
    engInfoLayout->setSpacing(0);
    engInfoLayout->setContentsMargins(0,0,0,0);
    engInfo->setLayout(engInfoLayout);
    vbox->addWidget(engInfo,0,4);
    //vbox->addStretch(1);
    mbox->addItem(vbox);
    conInfo = new QWidget(this);
    QGridLayout *conInfoForm = new QGridLayout;
    conInfoForm->setSpacing(2);
    conInfoForm->setContentsMargins(1,1,1,1);    
    conInfoForm->addWidget(new QLabel("File Name:"),0,0);
    conInfoForm->addWidget(new QLabel("Display Name:"),1,0);
    conInfoForm->addWidget(new QLabel("Total Mass:"),0,2);
    conInfoForm->addWidget(new QLabel("Length:"),1,2);
    conInfoForm->addWidget(new QLabel("Eng Mass:"),0,4);
    conInfoForm->addWidget(new QLabel("Wag Mass:"),1,4);
    conInfoForm->addWidget(new QLabel("Units:"),0,6);
    conInfoForm->addWidget(new QLabel("Durability:"),1,6);
    conInfoForm->addWidget(&cFileName,0,1);
    conInfoForm->addWidget(&cDisplayName,1,1);
    conInfoForm->addWidget(&cMass,0,3);
    conInfoForm->addWidget(&cLength,1,3);
    conInfoForm->addWidget(&cEmass,0,5);
    conInfoForm->addWidget(&cWmass,1,5);
    conInfoForm->addWidget(&cUnits,0,7);
    conInfoForm->addWidget(&cDurability,1,7);
    cMass.setFixedWidth(100);
    cEmass.setFixedWidth(100);
    cWmass.setFixedWidth(100);
    cLength.setFixedWidth(100);
    cUnits.setFixedWidth(100);
    cDurability.setFixedWidth(100);
    conInfo->setLayout(conInfoForm);
    mbox->addWidget(conInfo);
    mbox->addWidget(glConWidget);
    mbox->addWidget(conSlider);
    //glConWidget->setFixedHeight(150);
    glConWidget->setMinimumSize(1000, 100);
    QSizePolicy policy(glConWidget->sizePolicy());
    policy.setHeightForWidth(true);
    glConWidget->setSizePolicy(policy);
    
    main->setLayout(mbox);
    this->setCentralWidget(main);
    
    setWindowTitle(tr("TSRE5 v0.612 Consist Editor"));
    fileMenu = menuBar()->addMenu(tr("&File"));
    fNew = new QAction(tr("&New"), this); 
    fileMenu->addAction(fNew);
    QObject::connect(fNew, SIGNAL(triggered(bool)), this, SLOT(newConsist()));
    fSave = new QAction(tr("&Save"), this); 
    fileMenu->addAction(fSave);
    QObject::connect(fSave, SIGNAL(triggered(bool)), this, SLOT(saveCurrentConsist()));
    fExit = new QAction(tr("&Exit"), this); 
    fileMenu->addAction(fExit);
    QObject::connect(fExit, SIGNAL(triggered(bool)), this, SLOT(close()));
    consistMenu = menuBar()->addMenu(tr("&Consist"));
    cReverse = new QAction(tr("&Reverse"), this); 
    consistMenu->addAction(cReverse);
    QObject::connect(cReverse, SIGNAL(triggered(bool)), this, SLOT(cReverseSelected()));
    menuBar()->addMenu(tr("&Edit"));
    viewMenu = menuBar()->addMenu(tr("&View"));
    vConList = GuiFunct::newMenuCheckAction(tr("&Consist List"), this); 
    viewMenu->addAction(vConList);
    QObject::connect(vConList, SIGNAL(triggered(bool)), this, SLOT(viewConList(bool)));
    vEngList1 = GuiFunct::newMenuCheckAction(tr("&Eng List 1"), this); 
    viewMenu->addAction(vEngList1);
    QObject::connect(vEngList1, SIGNAL(triggered(bool)), this, SLOT(viewEngList1(bool)));
    vEngList2 = GuiFunct::newMenuCheckAction(tr("&Eng List 2"), this); 
    viewMenu->addAction(vEngList2);
    QObject::connect(vEngList2, SIGNAL(triggered(bool)), this, SLOT(viewEngList2(bool)));
    vConUnits = GuiFunct::newMenuCheckAction(tr("&Consist Units"), this); 
    viewMenu->addAction(vConUnits);
    QObject::connect(vConUnits, SIGNAL(triggered(bool)), this, SLOT(viewConUnits(bool)));
    vEngView = GuiFunct::newMenuCheckAction(tr("&Eng View"), this); 
    viewMenu->addAction(vEngView);
    QObject::connect(vEngView, SIGNAL(triggered(bool)), this, SLOT(viewEngView(bool)));
    vConView = GuiFunct::newMenuCheckAction(tr("&Con View"), this); 
    viewMenu->addAction(vConView);
    QObject::connect(vConView, SIGNAL(triggered(bool)), this, SLOT(viewConView(bool)));
    vResetShapeView = new QAction(tr("&Reset Shape View"), this); 
    viewMenu->addAction(vResetShapeView);
    QObject::connect(vResetShapeView, SIGNAL(triggered()), this, SLOT(vResetShapeViewSelected()));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("&About"), this);
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    helpMenu->addAction(aboutAction);
    
    QObject::connect(eng1, SIGNAL(engListSelected(int)),
                      this, SLOT(engListSelected(int)));
    QObject::connect(eng2, SIGNAL(engListSelected(int)),
                      this, SLOT(engListSelected(int)));
    
    QObject::connect(eng1, SIGNAL(addToConSelected(int, int)),
                      this, SLOT(addToConSelected(int, int)));
    QObject::connect(eng2, SIGNAL(addToConSelected(int, int)),
                      this, SLOT(addToConSelected(int, int)));
    
    
    QObject::connect(con1, SIGNAL(conListSelected(int)),
                      this, SLOT(conListSelected(int)));
    
    QObject::connect(this, SIGNAL(showEng(QString, QString)),
                      glShapeWidget, SLOT(showEng(QString, QString))); 
    
    QObject::connect(this, SIGNAL(showCon(int)),
                      glConWidget, SLOT(showCon(int))); 
    
    QObject::connect(conSlider, SIGNAL(valueChanged(int)),
                      this, SLOT(conSliderValueChanged(int))); 
    
    QObject::connect(units, SIGNAL(selected(int)),
                      this, SLOT(conUnitSelected(int))); 
    
    QObject::connect(glConWidget, SIGNAL(selected(int)),
                      this, SLOT(conUnitSelected(int))); 
    
    QObject::connect(glConWidget, SIGNAL(refreshItem()),
                      this, SLOT(refreshCurrentCon())); 
    
    QObject::connect(units, SIGNAL(refreshItem()),
                      this, SLOT(refreshCurrentCon())); 
    
    QObject::connect(&cFileName, SIGNAL(textEdited(QString)),
                      this, SLOT(cFileNameSelected(QString))); 
    
    QObject::connect(&cDisplayName, SIGNAL(textEdited(QString)),
                      this, SLOT(cDisplayNameSelected(QString))); 
    
    QObject::connect(&cDurability, SIGNAL(editingFinished()),
                      this, SLOT(cDurabilitySelected())); 

    vEngList2->trigger();
    vConUnits->trigger();
}

ConEditorWindow::~ConEditorWindow() {
}

void ConEditorWindow::vResetShapeViewSelected(){
    if(currentEng == NULL) return;
    float pos = -currentEng->sizez-1;
    if(pos > -15) pos = -15;
    engCamera->setPos(pos,2.5,0);
    engCamera->setPlayerRot(M_PI/2.0,0);
    glShapeWidget->resetRot();
}

void ConEditorWindow::saveCurrentConsist(){
    if(currentCon == NULL) return;
    if(currentCon->isNewConsist()){
        OverwriteDialog owerwriteDialog;
        owerwriteDialog.setWindowTitle("Overwrite \""+currentCon->conName+"\" ?");
        //owerwriteDialog.name.setText(currentCon->conName);
        QString spath;
        do {
            spath = currentCon->path + "/" + currentCon->name;
            spath.replace("//", "/");
            qDebug() << spath;
            QFile file(spath);
            if(!file.exists())
                break;
            owerwriteDialog.exec();
            if(owerwriteDialog.changed == 0)
                return;
            if(owerwriteDialog.changed == 1)
                break;
            //currentCon->conName = owerwriteDialog.name.text();
            //currentCon->name = currentCon->conName + ".con";
            //cFileName.setText(currentCon->conName);
        } while(owerwriteDialog.changed == 2);
    }
    Game::currentEngLib = englib;
    currentCon->save();
}

void ConEditorWindow::newConsist(){
    Game::currentEngLib = englib;
    con1->newConsist();
}

void ConEditorWindow::about(){
    aboutWindow->show();
}

void ConEditorWindow::cDurabilitySelected(){
    if(currentCon == NULL) return;
    currentCon->setDurability(cDurability.value());
}

void ConEditorWindow::cFileNameSelected(QString n){
    if(currentCon == NULL) return;
    if(!currentCon->isNewConsist()){
        cFileName.setText(currentCon->conName);
        return;
    }
    currentCon->conName = n;
    currentCon->name = n+".con";
}

void ConEditorWindow::cDisplayNameSelected(QString n){
    if(currentCon == NULL) return;
    currentCon->setDisplayName(n);
}

void ConEditorWindow::cReverseSelected(){
    if(currentCon == NULL) return;
    Game::currentEngLib = englib;
    currentCon->reverse();
    refreshCurrentCon();
}

void ConEditorWindow::conUnitSelected(int uid){
    if(currentCon == NULL) return;
    currentCon->select(uid);
    setCurrentEng(currentCon->engItems[uid].eng);
}

void ConEditorWindow::viewConList(bool show){
    if(show) con1->show();
    else con1->hide();
}
void ConEditorWindow::viewEngList1(bool show){
    if(show) eng1->show();
    else eng1->hide();
}
void ConEditorWindow::viewEngList2(bool show){
    if(show) eng2->show();
    else eng2->hide();
}
void ConEditorWindow::viewConUnits(bool show){
    if(show) units->show();
    else units->hide();
}
void ConEditorWindow::viewEngView(bool show){
    if(show) engInfo->show();
    else engInfo->hide();
}
void ConEditorWindow::viewConView(bool show){
    if(show) glConWidget->show();
    else glConWidget->hide();
    if(show) conInfo->show();
    else conInfo->hide();
    if(show) conSlider->show();
    else conSlider->hide();
}

void ConEditorWindow::setCurrentEng(int id){
    currentEng = englib->eng[id];
    qDebug() << currentEng->engName;
    float pos = -currentEng->sizez-1;
    if(pos > -15) pos = -15;
    engCamera->setPos(pos,2.5,0);
    engCamera->setPlayerRot(M_PI/2.0,0);
    emit showEng(englib->eng[id]->path, englib->eng[id]->name);
    
    eName.setText(currentEng->displayName);
    eFileName.setText(currentEng->name);
    eDirName.setText(currentEng->path.split("/").last());
    QString ttype = currentEng->type;
    if(currentEng->engType.length() > 1)
        ttype += " ( "+currentEng->engType+" )";
    eType.setText(ttype);
    //eBrakes;
    //eCouplings;
    eMass.setText(QString::number(currentEng->mass) + " t");
    eMaxSpeed.setText(QString::number((int)currentEng->maxSpeed) + " km/h");
    eShape.setText(currentEng->sNames[0]);
    eSize.setText(QString::number(currentEng->sizex)+" "+QString::number(currentEng->sizey)+" "+QString::number(currentEng->sizez)+" ");
    eCouplings.setText(currentEng->getCouplingsName());
    eBrakes.setText(currentEng->brakeSystemType);
}

void ConEditorWindow::engListSelected(int id){
    setCurrentEng(id);
    //currentEng = englib->eng[id];
    qDebug() << currentEng->engName;
    //float pos = -currentEng->sizez-1;
    //if(pos > -15) pos = -15;
    //engCamera->setPos(pos,2.5,0);
    //engCamera->setPlayerRot(M_PI/2.0,0);

    //emit showEng(englib->eng[id]->path, englib->eng[id]->name);

}

void ConEditorWindow::addToConSelected(int id, int count){
    if(currentCon == NULL) return;
    Game::currentEngLib = englib;
    for(int i = 0; i < count; i++)
        currentCon->appendEngItem(id);
    refreshCurrentCon();
}

void ConEditorWindow::conListSelected(int id){
    currentCon = ConLib::con[id];
    qDebug() << currentCon->conName;
    refreshCurrentCon();
    emit showCon(id);
}

void ConEditorWindow::refreshCurrentCon(){
    units->setCon(currentCon);
    conSlider->setMinimum(0);
    conSlider->setMaximum(currentCon->engItems.size());
    cFileName.setText(currentCon->conName);
    cDisplayName.setText(currentCon->displayName);
    cMass.setText(QString::number(currentCon->mass) + " t");
    cEmass.setText(QString::number(currentCon->emass) + " t");
    cWmass.setText(QString::number(currentCon->mass - currentCon->emass) + " t");
    cLength.setText(QString::number(currentCon->conLength) + " m");
    cUnits.setText(QString::number(currentCon->engItems.size()));
    cDurability.setValue(currentCon->durability);
}

void ConEditorWindow::conSliderValueChanged(int val){
    if(currentCon == NULL) return;
    if(currentCon->engItems.size() < 1) return;
    if(val > currentCon->engItems.size() - 1)
        val = currentCon->engItems.size() - 1;
    float len = currentCon->engItems[val].conLength;
    conCamera->setPos(-100,2.5,42 + len);
}

void ConEditorWindow::closeEvent( QCloseEvent *event )
{
    std::vector<int> unsavedConIds;
    con1->getUnsaed(unsavedConIds);
    if(unsavedConIds.size() == 0){
        qDebug() << "nic do zapisania";
        event->accept();
        return;
    }
    
    UnsavedDialog unsavedDialog;
    unsavedDialog.setWindowTitle("Save changes?");
    for(int i = 0; i < unsavedConIds.size(); i++){
        if(ConLib::con[unsavedConIds[i]] == NULL) continue;
        unsavedDialog.items.addItem(ConLib::con[unsavedConIds[i]]->showName);
    }
    unsavedDialog.exec();
    if(unsavedDialog.changed == 0){
        event->ignore();
        return;
    }
    if(unsavedDialog.changed == 2){
        event->accept();
        return;
    }
    
    for(int i = 0; i < unsavedConIds.size(); i++){
        currentCon = ConLib::con[unsavedConIds[i]];
        if(currentCon == NULL) continue;
        if(currentCon->isUnSaved())
            this->saveCurrentConsist();
    }

    //qDebug() << "aaa2";
    event->accept();
}