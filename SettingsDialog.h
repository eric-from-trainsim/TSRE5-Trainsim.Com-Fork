#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QString>

// Forward declarations to keep compile times fast
class QFormLayout;
class QTabWidget;
class QWidget;
class QCheckBox;
class QLineEdit;
class QTextEdit;
class QLabel;

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    virtual ~SettingsDialog() = default;
    void loadSettings();
    void save(const QString& filename = "settings.txt.new");
    
private:
    // --- UI Setup ---
    void setupUi();
    QWidget* createScrollTab(QFormLayout*& layout, QTabWidget* tabs, const QString& title);
 // SettingsDialog.h
    void addRow(QFormLayout* l, 
                const QString& key, 
                const QString& type, 
                const QString& label, 
                const QString& helpText = ""); // Add = "" here

    // --- Data Loading & Mapping ---
    void updateWidgetValue(const QString& key, const QString& val);
    QString getGameValue(const QString& key);

    // --- State Storage ---
    // Maps the token key (e.g., "camerafov") to the specific UI widget
    QMap<QString, QCheckBox*> enabledMap;      // The "Enabled" column checkboxes
    QMap<QString, QWidget*> valueWidgetMap;    // Primary input (QLineEdit, QCheckBox, or QTextEdit)
    QMap<QString, QLineEdit*> subValueWidgetMap; // Secondary input for "twonumber" types
};

#endif // SETTINGSDIALOG_H