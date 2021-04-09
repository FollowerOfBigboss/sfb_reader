#ifndef SFBGUI_H
#define SFBGUI_H

#include <iostream>

#include <QWidget>
#include <QApplication>
#include <QString>
#include <QMainWindow>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>

#include <QDropEvent>
#include <QMimeData>

#include "sfb.h"

class SFBGui : public QMainWindow {

    Q_OBJECT

public:
    SFBGui();
protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    QVBoxLayout* vbox;
    QHBoxLayout* hbox[9];

    QPushButton* SaveButton;
    QLabel* PropLabel[9];
    QLineEdit* LineEdit[9];

    QAction *Actions[6];
    
    SFB sfb;
    bool FileCreateMode = false;
    bool FileOpened = false;

    void* create_ptr = nullptr;
    void SetLineEdits();
    void GetFromLineEdits();

private slots:
  void OnActionOpen();
  void OnActionCreate();
  void OnActionSave();
  void OnActionSaveAs();
  void OnActionClose();
  void OnActionExit();

};

#endif
