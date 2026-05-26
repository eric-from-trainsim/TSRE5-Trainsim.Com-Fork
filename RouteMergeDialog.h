// RouteMergeDialog.h
#ifndef ROUTEMERGEDIALOG_H
#define ROUTEMERGEDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>


class RouteMergeDialog : public QDialog {
    Q_OBJECT
public:
    explicit RouteMergeDialog(QWidget *parent = nullptr);
private:
    // ALL UI ELEMENTS MUST BE POINTERS
    QLineEdit *rmdlabel1; 
    QCheckBox *rmdcheckBox0, *rmdcheckBox1, *rmdcheckBox2, *rmdcheckBox3, *rmdcheckBox4;
    QPushButton *rmdokButton, *rmdcancelButton;
};

#endif // ROUTEMERGEDIALOG_H