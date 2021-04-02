#include "sfbgui.h"

const QStringList PropList = {
    "Version",
    "Hybrid Flag",
    "Disc Content Data Offset",
    "Disc Content Data Lenght",
    "Disc Title Name",
    "Disc Title Data Offset",
    "Disc Title Data Lenght",
    "Disc Content",
    "Disc Title"
};

SFBGui::SFBGui()
{
    QMenu* menu = menuBar()->addMenu(tr("&File"));
    Actions[0] = menu->addAction("Open");
    Actions[1] = menu->addAction("Create");
    Actions[2] = menu->addAction("Save");
    Actions[3] = menu->addAction("Save as");
    Actions[4] = menu->addAction("Close");
    menu->addSeparator();
    Actions[5] = menu->addAction("Exit");

    connect(Actions[0], &QAction::triggered, this, &SFBGui::OnActionOpen);
    connect(Actions[1], &QAction::triggered, this, &SFBGui::OnActionCreate);
    connect(Actions[2], &QAction::triggered, this, &SFBGui::OnActionSave);
    connect(Actions[3], &QAction::triggered, this, &SFBGui::OnActionSaveAs);
    connect(Actions[4], &QAction::triggered, this, &SFBGui::OnActionClose);
    connect(Actions[5], &QAction::triggered, this, &SFBGui::OnActionExit);


    QWidget *widget = new QWidget;
    setCentralWidget(widget);
    vbox = new QVBoxLayout;

    for (int i = 0; i < 9; i++)
    {
        hbox[i] = new QHBoxLayout();

        PropLabel[i] = new QLabel(PropList[i], this);
        PropLabel[i]->setMinimumSize(QSize(130,0));
        LineEdit[i] = new QLineEdit(this);

        hbox[i]->addWidget(PropLabel[i]);
        hbox[i]->addWidget(LineEdit[i]);
        vbox->addStretch(1);
        vbox->addLayout(hbox[i]);
    }

    SaveButton = new QPushButton("Save", this);

    QHBoxLayout* savelayout = new QHBoxLayout;
    savelayout->addWidget(SaveButton, 0, Qt::AlignRight);

    vbox->addStretch(1);
    vbox->addLayout(savelayout);

    widget->setLayout(vbox);

    this->setWindowTitle("Test");
}

void SFBGui::OnActionOpen()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open a sfb file", "", tr("sfb files(*.sfb *.SFB);; All Files(*.*)"));
    qDebug(filename.toUtf8());
}
void SFBGui::OnActionCreate()
{
    return;
}

void SFBGui::OnActionSave()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", "sfb files (*.SFB);; All Files(*.*)");
    qDebug(filename.toUtf8());
}

void SFBGui::OnActionSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", "sfb files (*.SFB);; All Files(*.*)");
    qDebug(filename.toUtf8());
}


void SFBGui::OnActionClose()
{
    return;
}

void SFBGui::OnActionExit()
{
    QApplication::exit();
}
