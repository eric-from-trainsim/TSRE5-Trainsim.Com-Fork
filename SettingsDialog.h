#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QString>

class QFormLayout;
class QTabWidget;
class QWidget;
class QCheckBox;

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    void loadSettings();
    void save(const QString& filename);

private:
    void setupUi();
    void updateWidgetValue(const QString& key, const QString& val);
    QString getGameValue(const QString& key);
    
    QWidget* createScrollTab(QFormLayout*& layout, QTabWidget* tabs, const QString& title);
    
    // Updated signature with default parameters
    void addRow(QFormLayout* l, 
                const QString& key, 
                const QString& type, 
                const QString& label, 
                const QString& helpText = "", 
                bool dangerous = false);

    QMap<QString, QWidget*> valueWidgetMap;
    QMap<QString, QWidget*> subValueWidgetMap;
    QMap<QString, QCheckBox*> dangerousCheckboxMap; // The new map
};

#endif