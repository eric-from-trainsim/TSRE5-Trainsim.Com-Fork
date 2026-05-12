#include "SettingsDialog.h"
#include "Game.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QFormLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QFileDialog>
#include <QColorDialog>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QLabel>
#include <QIntValidator>
#include <QCheckBox>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("TSRE5 Settings Editor");
    resize(1100, 850);
    setupUi();
}

QWidget* SettingsDialog::createScrollTab(QFormLayout*& layout, QTabWidget* tabs, const QString& title) {
    QScrollArea* sa = new QScrollArea(tabs);
    sa->setWidgetResizable(true);
    QWidget* container = new QWidget();
    QVBoxLayout* vBox = new QVBoxLayout(container);
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(11, 5, 11, 5);
    headerLayout->setSpacing(10);
    QLabel* lblToken = new QLabel("<b>Token</b>");
    lblToken->setFixedWidth(200);
    lblToken->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QLabel* lblValue = new QLabel("<b>Value / Description</b>");
    lblValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(lblToken);
    headerLayout->addWidget(lblValue);
    headerLayout->addStretch(1);
    vBox->addLayout(headerLayout);
    QWidget* formContent = new QWidget();
    layout = new QFormLayout(formContent);
    layout->setLabelAlignment(Qt::AlignRight);
    layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    formContent->setStyleSheet("QLabel { min-width: 200px; max-width: 200px; qproperty-alignment: 'AlignTop | AlignRight'; }");
    vBox->addWidget(formContent);
    vBox->addStretch(1);
    sa->setWidget(container);
    tabs->addTab(sa, title);
    return container;
}

void SettingsDialog::addRow(QFormLayout* l, const QString& key, const QString& type, const QString& label, const QString& helpText, bool dangerous) {
    QString lowKey = key.toLower();
    QHBoxLayout* rowLayout = new QHBoxLayout();
    rowLayout->setSpacing(15);
    rowLayout->setContentsMargins(0, 0, 0, 0);

    if (dangerous) {
        QCheckBox* cb = new QCheckBox();
        cb->setToolTip("Enable this setting");
        dangerousCheckboxMap[lowKey] = cb;
        rowLayout->addWidget(cb);
    }

    QWidget* valueContainer = new QWidget();
    valueContainer->setFixedWidth(550);
    QHBoxLayout* valueLayout = new QHBoxLayout(valueContainer);
    valueLayout->setContentsMargins(0, 0, 0, 0);
    valueLayout->setSpacing(5);
    valueLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QWidget* inputPart = nullptr;

    if (type == "bool") {
        QWidget* boolCont = new QWidget();
        QHBoxLayout* h = new QHBoxLayout(boolCont);
        h->setContentsMargins(0, 0, 0, 0);
        h->setSpacing(10);
        QRadioButton* rbTrue = new QRadioButton("True");
        QRadioButton* rbFalse = new QRadioButton("False");
        rbTrue->setProperty("buddy", QVariant::fromValue(rbFalse));
        rbFalse->setProperty("buddy", QVariant::fromValue(rbTrue));
        h->addWidget(rbTrue);
        h->addWidget(rbFalse);
        valueWidgetMap[lowKey] = rbTrue;
        inputPart = boolCont;
    } else if (type == "dir" || type == "color") {
        QWidget* dirCont = new QWidget();
        QHBoxLayout* h = new QHBoxLayout(dirCont);
        h->setContentsMargins(0, 0, 0, 0);
        QLineEdit* le = new QLineEdit();
        le->setFixedWidth(450);
        QPushButton* btn = new QPushButton(type == "dir" ? "..." : "Color");
        btn->setFixedWidth(type == "dir" ? 30 : 60);
        if (type == "dir") {
            connect(btn, &QPushButton::clicked, [this, le]() {
                QString d = QFileDialog::getExistingDirectory(this, "Select Directory", le->text());
                if (!d.isEmpty()) le->setText(d);
            });
        } else {
            connect(btn, &QPushButton::clicked, [this, le]() {
                QColor c = QColorDialog::getColor(QColor(le->text()), this);
                if (c.isValid()) le->setText(c.name().toUpper());
            });
        }
        h->addWidget(le); h->addWidget(btn);
        valueWidgetMap[lowKey] = le;
        inputPart = dirCont;
    } else if (type == "twonumber") {
        QWidget* numCont = new QWidget();
        QHBoxLayout* h = new QHBoxLayout(numCont);
        h->setContentsMargins(0, 0, 0, 0);
        QLineEdit* n1 = new QLineEdit();
        QLineEdit* n2 = new QLineEdit();
        n1->setFixedWidth(60);
        n2->setFixedWidth(60);
        h->addWidget(n1); h->addWidget(n2);
        valueWidgetMap[lowKey] = n1;
        subValueWidgetMap[lowKey] = n2;
        inputPart = numCont;
    } else if (type == "textbox") {
        QTextEdit* te = new QTextEdit();
        te->setMaximumHeight(50);
        te->setFixedWidth(500);
        valueWidgetMap[lowKey] = te;
        inputPart = te;
    } else {
        QLineEdit* le = new QLineEdit();
        le->setFixedWidth(500);
        if (type == "number" || type == "int") {
            le->setFixedWidth(60);
            le->setValidator(new QIntValidator(le));
        }
        valueWidgetMap[lowKey] = le;
        inputPart = le;
    }

    if (dangerous) {
        inputPart->setEnabled(false);
        connect(dangerousCheckboxMap[lowKey], &QCheckBox::toggled, inputPart, &QWidget::setEnabled);
    }

    valueLayout->addWidget(inputPart);
    rowLayout->addWidget(valueContainer, 0, Qt::AlignTop);
    QLabel* helpLabel = new QLabel(helpText);
    helpLabel->setWordWrap(true);
    helpLabel->setStyleSheet("font-size: 11px;");
    rowLayout->addWidget(helpLabel, 1, Qt::AlignTop);
    l->addRow(label, rowLayout);
}

void SettingsDialog::setupUi() {
    QVBoxLayout* main = new QVBoxLayout(this);
    QTabWidget* tabs = new QTabWidget(this);
    main->addWidget(tabs);
    QFormLayout* l = nullptr;

    createScrollTab(l, tabs, "System");
    addRow(l, "consoleOutput", "bool", "Console Output", "Displays log output in realtime in command window");
    addRow(l, "debugOutput", "bool", "Debug Output", "Enables extended logging detail");
    addRow(l, "fullscreen", "bool", "Fullscreen", "Prevents screen from being maximized");
    addRow(l, "imageSubstitution", "bool", "Image Substitution", "Allow for ACE or DDS to be shown if missing DDS or ACE");
    addRow(l, "imageUpgrade", "bool", "Image Upgrade", "Show DDS if available");
    addRow(l, "logfiledays", "number", "Log File Days", "Delete files older than X days");
    addRow(l, "logfilemax", "number", "Log File Max", "Keep only X logs");
    addRow(l, "mainWindowLayout", "string", "Window Layout", "P = Properties, T = Tools W = World S = Status");
    addRow(l, "maxObjLag", "int", "Max Obj Lag", "");
    addRow(l, "mouseSpeed", "int", "Mouse Speed", "");
    addRow(l, "shadowLowMapSize", "number", "Shadow Low Map", "");
    addRow(l, "shadowMapSize", "number", "Shadow Map", "");
    addRow(l, "shadowsEnabled", "bool", "Shadows Enabled", "Can affect performance");
    addRow(l, "soundEnabled", "bool", "Sound Enabled", "");
    addRow(l, "startapp", "string", "Start App", "R=Route Edit, C=Consist Edit, S=Shapeviewer");
    addRow(l, "systemTheme", "bool", "System Theme", "True uses your Windows theme");
    addRow(l, "useImperial", "bool", "Use Imperial", "Show Imperial measures");
    addRow(l, "usenNumPad", "bool", "Use NumPad", "");
    addRow(l, "useWorkingDir", "bool", "Use Working Dir", "False saves logs to TSRE app folder");
    addRow(l, "warningBox", "bool", "Warning Box", "Warn before exiting without save");
    addRow(l, "appRepository", "string", "appRepository", "Location of data not included in executable");

    createScrollTab(l, tabs, "Route Editor");
    addRow(l, "gameRoot", "dir", "Game Root", "Your ORTS Content drive/folder");
    addRow(l, "routeName", "string", "Route Name", "Add route name to skip route selection menu");
    addRow(l, "createNewIfNotExist", "bool", "Create New Route", "Create route if not present");
    addRow(l, "geoPath", "dir", "Geo Path", "Folder housing HGT files");
    addRow(l, "loadActivities", "bool", "Load Activities", "");
    addRow(l, "loadAllWFiles", "bool", "Load All W Files", "");
    addRow(l, "season", "string", "Season", "Winter/Autumn/Spring/Summer/Night");
    addRow(l, "seasonalEditing", "bool", "Seasonal Editing", "");
    addRow(l, "autoFix", "bool", "Auto Fix", "Repair TDB anomalies");
    addRow(l, "deepunderground", "number", "Deep Underground", "");
    addRow(l, "deleteTrWatermarks", "bool", "Delete Tr Watermarks", "");
    addRow(l, "deleteViewDbSpheres", "bool", "Delete VDB Spheres", "");
    addRow(l, "legacySupport", "bool", "Legacy Support", "Retention of ViewDBSphere and VDBID");
    addRow(l, "listfiles", "bool", "List Files", "Create lists of files used/unused on exit");
    addRow(l, "objectsToRemove", "textbox", "Objects To Remove", "Comma separated list of shapes");
    addRow(l, "sortTileObjects", "bool", "Sort Tile Objects", "Orders items by detail level on save");
    addRow(l, "defaultElevationBox", "number", "Default Elevation", "");
    addRow(l, "defaultMoveStep", "number", "Default Move Step", "");
    addRow(l, "ignoreMissingGlobalShapes", "bool", "Ignore Global Shapes", "");
    addRow(l, "leaveTrackShapeAfterDelete", "bool", "Leave Track Shape", "");
    addRow(l, "maxAutoPlacement", "number", "Max Auto Placement", "");
    addRow(l, "mapImageResolution", "number", "Map Resolution", "");
    addRow(l, "numRecentItems", "number", "Recent Items List", "");
    addRow(l, "preloadTextures", "textbox", "Preload Textures", "TERRTEX: ace, bmp, dds, png");
    addRow(l, "sigOffset", "number", "Signal Offset", "");
    addRow(l, "snapableRadius", "number", "Snapable Radius", "");
    addRow(l, "snapableOnlyRot", "number", "Snapable Only Rot", "");
    addRow(l, "terrainBrushColor", "color", "Terrain Brush Color", "");

    addRow(l, "terrainBrushIntensity", "number", "Brush Intensity", "");
    addRow(l, "terrainBrushSize", "number", "Brush Size", "");
    addRow(l, "terrainCut", "number", "Terrain Cut", "");
    addRow(l, "terrainEmbankment", "number", "Terrain Embankment", "");
    addRow(l, "terrainRadius", "number", "Terrain Radius", "");
    addRow(l, "terrainSize", "number", "Terrain Size", "");



    addRow(l, "textureQuality", "number", "Texture Quality", "");
    addRow(l, "trackElevationMaxPm", "number", "Max Grade Permille", "");
    addRow(l, "useOnlyPositiveQuaternions", "bool", "Positive Quaternions", "");
    addRow(l, "useQuadTree", "bool", "Use QuadTree", "");
    addRow(l, "useTdbEmptyItems", "bool", "Use TDB Empty Items", "");
    addRow(l, "writeEnabled", "bool", "Write Enabled", "");
    addRow(l, "writeTDB", "bool", "Write TDB", "");
    addRow(l, "AASamples", "number", "AA Samples", "");
    addRow(l, "allowObjLag", "number", "Allow Obj Lag", "");
    addRow(l, "cameraFov", "number", "Camera FOV", "");
    addRow(l, "cameraSpeedMax", "number", "Camera Speed Max (Shift)", "");
    addRow(l, "cameraSpeedMin", "number", "Camera Speed Min (Ctrl)", "");
    addRow(l, "cameraSpeedStd", "number", "Camera Speed Normal", "");
    addRow(l, "cameraStickToTerrain", "bool", "Stick to Terrain", "Toggle with / key");
    addRow(l, "hudEnabled", "bool", "HUD Enabled", "");
    addRow(l, "hudScale", "number", "HUD Scale", "");
    addRow(l, "lockCamera", "bool", "Lock Camera", "Toggle with . key");
    addRow(l, "markerHeight", "number", "Marker Height", "");
    addRow(l, "markerLines", "bool", "Marker Lines", "");
    addRow(l, "markerText", "number", "Marker Text Size", "");
    addRow(l, "MSTSshadows", "bool", "MSTS Shadows", "");
    addRow(l, "naviWindow", "twonumber", "Navi Window", "X, Y position");
    addRow(l, "newSymbols", "bool", "New Symbols", "False uses old pyramids");
    addRow(l, "objectLod", "number", "Object LOD", "2000 is plenty");
    addRow(l, "oglDefaultLineWidth", "number", "OGL Line Width", "");
    addRow(l, "railProfile", "twonumber", "Rail Profile", "Rail inner/outer edges for rendering dynamic track");
    addRow(l, "renderTrItems", "bool", "Render Tr Items", "");
    addRow(l, "sectionLineHeight", "number", "Section Line Height", "");
    addRow(l, "selectedColor", "color", "Selected Color", "");
    addRow(l, "selectedWidth", "number", "Selected Width", "");
    addRow(l, "selectedTerrColor", "color", "Selected Terrain Color", "");
    addRow(l, "selectedTerrWidth", "number", "Selected Terrain Width", "");
    addRow(l, "statusWindow", "twonumber", "Status Window", "X, Y position");
    addRow(l, "tileLod", "number", "Tile LOD", "");
    addRow(l, "toolsHidden", "bool", "Tools Hidden", "");
    addRow(l, "useSuperelevation", "bool", "Use Superelevation", "");
    addRow(l, "viewCompass", "bool", "View Compass", "");
    addRow(l, "viewMarkers", "bool", "View Markers", "");
    addRow(l, "viewTRLabels", "bool", "View TR Labels", "");
    addRow(l, "wireLineHeight", "number", "Wire Line Height", "");

    createScrollTab(l, tabs, "Maps");
    addRow(l, "mapengine", "string", "Map Engine", "");
    addRow(l, "MapboxImageMapURL", "string", "Mapbox URL", "");
    addRow(l, "MapboxImageMapsZoomOffset", "number", "Mapbox Zoom Offset", "");
    addRow(l, "MapboxMapAPIKey", "string", "Mapbox API Key", "");
    addRow(l, "GoogleImageMapURL", "string", "Google URL", "");
    addRow(l, "GoogleMapAPIKey", "string", "Google API Key", "");
    addRow(l, "GoogleImageMapsZoomOffset", "number", "Google Zoom Offset", "Not presently needed");    
    addRow(l, "imageMapsURL", "string", "Image Maps URL", "");
    addRow(l, "MapAPIKey", "string", "General Map API Key", "");

    createScrollTab(l, tabs, "Consist Editor");
    addRow(l, "ceWindowLayout", "string", "CE Layout", "C-Consists, 1-Main List, 2-Second List");
    addRow(l, "colorConView", "color", "Consist View Color", "");
    addRow(l, "colorShapeView", "color", "Shape View Color", "");
    addRow(l, "includeFolder", "string", "Include Folder", "");
    addRow(l, "loadConsists", "bool", "Load Consists", "");
    addRow(l, "ortsEngEnable", "bool", "ORTS Eng Enable", "");

    createScrollTab(l, tabs, "Multi-User");
    addRow(l, "fpsLimit", "number", "FPS Limit", "");
    addRow(l, "playerMode", "bool", "Player Mode", "");
    addRow(l, "proceduralTracks", "bool", "Procedural Tracks", "");
    addRow(l, "serverAuth", "bool", "Server Auth", "");
    addRow(l, "serverLogin", "string", "Server Login", "");
    addRow(l, "useNetworkEng", "bool", "Use Network Engine", "");

    createScrollTab(l, tabs, "Somewhat Dangerous");
    addRow(l, "startTileX", "number", "Start Tile X", "Must be valid tile within route or editor may crash", true);
    addRow(l, "startTileY", "number", "Start Tile Y", "Must be valid tile within route or editor may crash", true);
    addRow(l, "unsafemode", "bool", "Unsafe Mode", "Only for risky features", true);
    addRow(l, "routeRebuildTDB", "bool", "Route Rebuild TDB", "", true);
    addRow(l, "routeMergeString", "string", "Route Merge String", "e.g. IRM:0:0:0", true);
    addRow(l, "routeMergeTDB", "bool", "Route Merge TDB", "", true);
    addRow(l, "routeMergeTerrain", "bool", "Route Merge Terrain", "", true);
    addRow(l, "routeMergeTerrtex", "bool", "Route Merge Terrtex", "", true);

    
    QLabel* instrLabel = new QLabel("Settings can be updated but will not be applied while application is running. Save your settings, and relaunch the application to apply your changes.", this);
    instrLabel->setAlignment(Qt::AlignCenter); // Optional: Centers text across the width
    main->addWidget(instrLabel);    
    
// 1. Define the filename for the backup version
    QString setFileBase = "settings.txt";

    QPushButton* checkSaveBtn = new QPushButton("Save and Backup Existing Settings.txt", this);
    main->addWidget(checkSaveBtn);

    // Added checkSaveBtn to the capture list so clearFocus() works
    connect(checkSaveBtn, &QPushButton::clicked, this, [this, setFileBase, checkSaveBtn]() {
        QFile file(setFileBase);
        if (file.exists()) {
            QString timestamp = QDateTime::currentDateTime().toString("HHmm.ddMMM");
            QString backupName = QString("%1.%2").arg(setFileBase).arg(timestamp);
            file.rename(backupName); 
        }
        save(setFileBase);        
        checkSaveBtn->clearFocus();
    });    
    
    // 2. Define the filename for the standard version
    QString setFileNew = "settings.txt.new";
    
    QPushButton* saveBtn = new QPushButton(QString("Save %1").arg(setFileNew));
    main->addWidget(saveBtn);

    connect(saveBtn, &QPushButton::clicked, this, [this, setFileNew, saveBtn]() { 
        save(setFileNew); 
        saveBtn->clearFocus(); 
    });
    loadSettings();
}

void SettingsDialog::save(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << "# TSRE5 " << Game::AppVersion << " Settings File\n\n";
    QMapIterator<QString, QWidget*> i(valueWidgetMap);
    while (i.hasNext()) {
        i.next();
        QString key = i.key();
        QWidget* w = i.value();
        QString valStr;
        if (subValueWidgetMap.contains(key)) {
            QLineEdit* primary = qobject_cast<QLineEdit*>(w);
            QLineEdit* secondary = qobject_cast<QLineEdit*>(subValueWidgetMap[key]);

            if (primary && secondary) {
                valStr = primary->text().trimmed() + ", " + secondary->text().trimmed();
            }
        } else if (QRadioButton* rb = qobject_cast<QRadioButton*>(w)) {
            valStr = rb->isChecked() ? "true" : "false";
        } else if (QLineEdit* le = qobject_cast<QLineEdit*>(w)) {
            valStr = le->text().trimmed();
        } else if (QTextEdit* te = qobject_cast<QTextEdit*>(w)) {
            valStr = te->toPlainText().replace("\n", ",").trimmed();
        }
        QString finalKey = key;
        if (dangerousCheckboxMap.contains(key) && !dangerousCheckboxMap[key]->isChecked()) {
            finalKey = "## " + key;
        }
        out << finalKey << " = " << valStr << "\n";
    }
    file.close();
    /// accept();   /// leave the dialog open
}





void SettingsDialog::loadSettings() {
    QMapIterator<QString, QWidget*> it(valueWidgetMap);
    while (it.hasNext()) {
        it.next();
        // The key in valueWidgetMap is already lowercase from addRow
        QString memVal = getGameValue(it.key());
        if (!memVal.isEmpty()) updateWidgetValue(it.key(), memVal);
    }

    QFile file("settings.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QString cleanLine = line;
        bool wasCommented = false;

        // Use Qt::CaseInsensitive for startsWith checks
        if (line.startsWith("##", Qt::CaseInsensitive)) {
            cleanLine = line.mid(2).trimmed();
            wasCommented = true;
        } else if (line.startsWith("#", Qt::CaseInsensitive)) {
            cleanLine = line.mid(1).trimmed();
        } else if (line.startsWith("//", Qt::CaseInsensitive)) {
            cleanLine = line.mid(2).trimmed();
        }

        int eqPos = cleanLine.indexOf('=');
        if (eqPos == -1) continue;

        // Normalize the key to lower case immediately
        QString key = cleanLine.left(eqPos).trimmed().toLower();
        QString val = cleanLine.mid(eqPos + 1).trimmed();

        // Check for comments trailing the value - case insensitive
        int cp = val.indexOf(" #", 0, Qt::CaseInsensitive);
        if (cp == -1) cp = val.indexOf(" //", 0, Qt::CaseInsensitive);
        
        if (cp != -1) val = val.left(cp).trimmed();
        val.remove("\"");

        // map.contains() lookup will now work perfectly since 'key' is lowercased
        if (valueWidgetMap.contains(key)) {
            updateWidgetValue(key, val);
            
            if (wasCommented && dangerousCheckboxMap.contains(key)) {
                dangerousCheckboxMap[key]->setChecked(false);
            } else if (dangerousCheckboxMap.contains(key)) {
                dangerousCheckboxMap[key]->setChecked(true);
            }
        }
    }
    file.close();
}


void SettingsDialog::updateWidgetValue(const QString& key, const QString& val) {
    QWidget* w = valueWidgetMap[key];
    if (!w) return;
    if (subValueWidgetMap.contains(key)) {
        QStringList vals = val.split(",");
        qobject_cast<QLineEdit*>(w)->setText(vals.value(0).trimmed());

        if (vals.size() > 1) {
            // Cast the sub-widget here
            QLineEdit* subEdit = qobject_cast<QLineEdit*>(subValueWidgetMap[key]);
            if (subEdit) {
                subEdit->setText(vals.value(1).trimmed());
            }
        }
    } else if (QRadioButton* rb = qobject_cast<QRadioButton*>(w)) {
        bool isTrue = (val.toLower() == "true" || val.toLower() == "on" || val == "1");
        rb->setChecked(isTrue);
        QRadioButton* rbFalse = rb->property("buddy").value<QRadioButton*>();
        if (rbFalse) rbFalse->setChecked(!isTrue);
    } else if (QLineEdit* le = qobject_cast<QLineEdit*>(w)) {
        le->setText(val);
    } else if (QTextEdit* te = qobject_cast<QTextEdit*>(w)) {
        te->setPlainText(QString(val).replace(",", "\n"));
    }
}

QString SettingsDialog::getGameValue(const QString& key) {

        // Key is always passed in lowercase here
        if (key == "aasamples") return QString::number(Game::AASamples);
        if (key == "allowobjlag") return QString::number(Game::allowObjLag);
        if (key == "apprepository") return Game::AppRepository;
        if (key == "autofix") return Game::autoFix ? "true" : "false";
        if (key == "camerafov") return QString::number(Game::cameraFov);
        if (key == "cameraspeedmax") return QString::number(Game::cameraSpeedMax);
        if (key == "cameraspeedmin") return QString::number(Game::cameraSpeedMin);
        if (key == "cameraspeedstd") return QString::number(Game::cameraSpeedStd);
        if (key == "camerasticktoterrain") return Game::cameraStickToTerrain ? "true" : "false";
        if (key == "cewindowlayout") return Game::ceWindowLayout;

        if (key == "colorconview" && Game::colorConView) return Game::colorConView->name();                
        if (key == "colorshapeview" && Game::colorShapeView) return Game::colorShapeView->name();                

        
        if (key == "consoleoutput") return Game::consoleOutput ? "true" : "false";
        if (key == "createnewroutes") return Game::createNewRoutes ? "true" : "false";
        if (key == "debugoutput") return Game::debugOutput ? "true" : "false";
        if (key == "deepunderground") return QString::number(Game::deepUnderground);
        if (key == "defaultelevationbox") return QString::number(Game::DefaultElevationBox);
        if (key == "defaultmovestep") return QString::number(Game::DefaultMoveStep);
        if (key == "deletetrwatermarks") return Game::deleteTrWatermarks ? "true" : "false";
        if (key == "deleteviewdbspheres") return Game::deleteViewDbSpheres ? "true" : "false";
        if (key == "fpslimit") return QString::number(Game::fpsLimit);
        if (key == "fullscreen") return Game::fullscreen ? "true" : "false";
        
        
        if (key == "googleimagemapurl") return Game::GoogleImageMapURL;
        if (key == "googleapikey") return Game::GoogleAPIKey;                
        if (key == "googleimagemapszoomoffset") return QString::number(Game::GoogleImageMapsZoomOffset);

        if (key == "hudenabled") return Game::hudEnabled ? "true" : "false";
        if (key == "hudscale") return QString::number(Game::hudScale);
        if (key == "ignoremissingglobalshapes") return Game::ignoreMissingGlobalShapes ? "true" : "false";
        if (key == "imagemapsURL") return QString::number(Game::imageMapsZoomOffset);        
        if (key == "imagemapszoomoffset") return QString::number(Game::imageMapsZoomOffset);
        if (key == "imagesubstitution") return Game::imageSubstitution ? "true" : "false";
        if (key == "imageupgrade") return Game::imageUpgrade ? "true" : "false";
        if (key == "includefolder") return Game::includeFolder;
        if (key == "leavetrackshapeafterdelete") return Game::leaveTrackShapeAfterDelete ? "true" : "false";
        if (key == "legacysupport") return Game::legacySupport ? "true" : "false";
        if (key == "listfiles") return Game::listFiles ? "true" : "false";
        if (key == "loadactivities") return Game::loadActivities ? "true" : "false";
        if (key == "loadallwfiles") return Game::loadAllWFiles ? "true" : "false";
        if (key == "loadconsists") return Game::loadConsists ? "true" : "false";
        if (key == "localtsectiononly") return Game::LocalTSectionOnly ? "true" : "false";
        if (key == "lockcamera") return Game::lockCamera ? "true" : "false";
        if (key == "logfiledays") return QString::number(Game::logfileDays);
        if (key == "logfilemax") return QString::number(Game::logfileMax);
        if (key == "mainwindowlayout") return Game::mainWindowLayout;
        if (key == "mainwindow") return Game::mainPos;        

        if (key == "mapapikey") return Game::MapAPIKey;                
        
        if (key == "mapboximagemapurl") return Game::MapBoxImageMapURL;
        if (key == "mapboxapikey") return Game::MapBoxAPIKey;                
        if (key == "mapboximagemapszoomoffset") return QString::number(Game::MapBoxImageMapsZoomOffset);

        if (key == "mapengine") return Game::mapEngine;
        if (key == "mapimageresolution") return QString::number(Game::mapImageResolution);
        if (key == "markerheight") return QString::number(Game::markerHeight);
        if (key == "markerlines") return Game::markerLines ? "true" : "false";
        if (key == "markertext") return QString::number(Game::markerText);
        if (key == "maxautoplacement") return QString::number(Game::maxAutoPlacement);
        if (key == "maxobjlag") return QString::number(Game::maxObjLag);
        if (key == "mousespeed") return QString::number(Game::mouseSpeed);
        if (key == "mstsshadows") return Game::mstsShadows ? "true" : "false";        
        if (key == "naviwindow") return QString(Game::naviPos);        
        if (key == "newsymbols") return Game::newSymbols ? "true" : "false";
        if (key == "numrecentitems") return QString::number(Game::numRecentItems);
        if (key == "objectlod") return QString::number(Game::objectLod);
        if (key == "ogldefaultlinewidth") return QString::number(Game::oglDefaultLineWidth);
        if (key == "ortsengenable") return Game::ortsEngEnable ? "true" : "false";
        if (key == "playermode") return Game::playerMode ? "true" : "false";
        if (key == "proceduraltracks") return Game::proceduralTracks ? "true" : "false";
        if (key == "railprofile") return QString("%1, %2").arg(Game::railProfile[0]).arg(Game::railProfile[1]);
        if (key == "rendertritems") return Game::renderTrItems ? "true" : "false";
        if (key == "routemergestring") return Game::routeMergeString;
        if (key == "routemergetdb") return Game::routeMergeTDB ? "true" : "false";
        if (key == "routemergeterrain") return Game::routeMergeTerrain ? "true" : "false";
        if (key == "routemergeterrtex") return Game::routeMergeTerrtex ? "true" : "false";
        if (key == "routerebuildtdb") return Game::routeRebuildTDB ? "true" : "false";
        if (key == "seasonalediting") return Game::seasonalEditing ? "true" : "false";
        if (key == "sectionlineheight") return QString::number(Game::sectionLineHeight);
        if (key == "selectedcolor" && Game::selectedColor) return Game::selectedColor->name();
        if (key == "selectedterrcolor" && Game::selectedTerrColor) return Game::selectedTerrColor->name();
        if (key == "selectedterrwidth") return QString::number(Game::selectedTerrWidth);
        if (key == "selectedwidth") return QString::number(Game::selectedWidth);
        if (key == "serverauth") return !Game::serverAuth.isEmpty() ? "true" : "false";
        if (key == "serverlogin") return Game::serverLogin;
        if (key == "servermode") return Game::ServerMode ? "true" : "false";
        if (key == "shadowlowmapsize") return QString::number(Game::shadowLowMapSize);
        if (key == "shadowmapsize") return QString::number(Game::shadowMapSize);
        if (key == "shadowsenabled") return QString::number(Game::shadowsEnabled);
        if (key == "sigoffset") return QString::number(Game::sigOffset);
        if (key == "snapableonlyrot") return Game::snapableOnlyRot ? "true" : "false";
        if (key == "snapableradius") return QString::number(Game::snapableRadius);
        if (key == "sorttileobjects") return Game::sortTileObjects ? "true" : "false";
        if (key == "soundenabled") return Game::soundEnabled ? "true" : "false";
        if (key == "startapp") return Game::startapp;
        if (key == "starttilex") return QString::number(Game::startTileX);
        if (key == "starttiley") return QString::number(Game::startTileY);
        if (key == "statuswindow") return QString(Game::statusPos);        
        if (key == "systemtheme") return Game::systemTheme ? "true" : "false";
        if (key == "terrainbrushcolor" && Game::terrBrushColor) return Game::terrBrushColor->name();
        if (key == "terrainsize") return QString::number(Game::terrainTools[0]);
        if (key == "terrainembankment") return QString::number(Game::terrainTools[1]);
        if (key == "terraincut") return QString::number(Game::terrainTools[2]);
        if (key == "terrainradius") return QString::number(Game::terrainTools[3]);
        if (key == "terrainbrushsize") return QString::number(Game::terrainTools[4]);
        if (key == "terrainbrushintensity") return QString::number(Game::terrainTools[5]);        
        if (key == "texturequality") return QString::number(Game::textureQuality);
        if (key == "tilelod") return QString::number(Game::tileLod);
        if (key == "toolshidden") return Game::toolsHidden ? "true" : "false";
        if (key == "trackelevationmaxpm") return QString::number(Game::trackElevationMaxPm);
        if (key == "unsafemode") return Game::UnsafeMode ? "true" : "false";
        if (key == "usenetworkeng") return Game::useNetworkEng ? "true" : "false";
        if (key == "usennumpad") return Game::usenNumPad ? "true" : "false";
        if (key == "useonlypositivequaternions") return Game::useOnlyPositiveQuaternions ? "true" : "false";
        if (key == "usequadtree") return Game::useQuadTree ? "true" : "false";
        if (key == "usesuperelevation") return Game::useSuperelevation ? "true" : "false";
        if (key == "usetdbemptyitems") return Game::useTdbEmptyItems ? "true" : "false";
        if (key == "useworkingdir") return Game::UseWorkingDir ? "true" : "false";
        if (key == "viewcompass") return Game::viewCompass ? "true" : "false";
        if (key == "viewmarkers") return Game::viewMarkers ? "true" : "false";
        if (key == "viewtrlabels") return Game::viewTRLabels ? "true" : "false";
        if (key == "warningbox") return Game::warningBox ? "true" : "false";
        if (key == "wirelineheight") return QString::number(Game::wireLineHeight);
        if (key == "writeenabled") return Game::writeEnabled ? "true" : "false";
        if (key == "writetdb") return Game::writeTDB ? "true" : "false";

    return "";
}