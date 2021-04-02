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

#include "sfb.h"

class SFBGui : public QMainWindow {

    Q_OBJECT

public:
    SFBGui();

private:
    QVBoxLayout* vbox;
    QHBoxLayout* hbox[9];

    QPushButton* SaveButton;
    QLabel* PropLabel[9];
    QLineEdit* LineEdit[9];

    QAction *Actions[6];
    
    SFB sfb;

private slots:
  void OnActionOpen();
  void OnActionCreate();
  void OnActionSave();
  void OnActionSaveAs();
  void OnActionClose();
  void OnActionExit();

};

#endif
