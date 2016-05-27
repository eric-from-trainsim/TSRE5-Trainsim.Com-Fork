#ifndef CONLISTWIDGET_H
#define	CONLISTWIDGET_H

#include <QWidget>
#include <QtWidgets>

class EngLib;

class ConListWidget : public QWidget {
    Q_OBJECT
public:
    ConListWidget();
    virtual ~ConListWidget();
    void fillConList();
    void fillConList(QString n);
    void newConsist();
    void getUnsaed(std::vector<int> &unsavedConIds);
    EngLib* englib;
    
public slots:
    void itemsSelected(QListWidgetItem* it);
    void conFChan(QString n);
        
signals:
    void conListSelected(int id);
        
private:
    QListWidget items;
    QComboBox conType;
};
#endif	/* CONLISTWIDGET_H */
