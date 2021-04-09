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
        hbox[i] = new QHBoxLayout;

        PropLabel[i] = new QLabel(PropList[i], this);
        PropLabel[i]->setMinimumSize(QSize(150,0));
        LineEdit[i] = new QLineEdit(this);

        hbox[i]->addWidget(PropLabel[i]);
        hbox[i]->addWidget(LineEdit[i]);
        vbox->addStretch(1);
        vbox->addLayout(hbox[i]);
    }

    SaveButton = new QPushButton("Save", this);
    connect(SaveButton, &QPushButton::clicked, this, &SFBGui::OnActionSave);

    QHBoxLayout* savelayout = new QHBoxLayout;
    savelayout->addWidget(SaveButton, 0, Qt::AlignRight);

    vbox->addStretch(1);
    vbox->addLayout(savelayout);

    widget->setLayout(vbox);
    this->setWindowTitle("SFB Editor");
    setAcceptDrops(true);
}

void SFBGui::OnActionOpen()
{
    if (FileOpened == true)
    {
        sfb.close();
        FileOpened = false;
    }

    if (create_ptr != nullptr)
    {
        sfb.close_as(create_ptr);
        FileCreateMode = false;
    }

    QString filename = QFileDialog::getOpenFileName(this, "Open a sfb file", "", tr("sfb files(*.sfb *.SFB);; All Files(*.*)"));
    if (filename.size() > 0)
    {
        if (sfb.open(filename.toStdString()) != true)
        {
            QMessageBox::critical(this, "Error", QString("%1 couldn't open successfully").arg(filename));
            FileOpened = false;
            return;
        }
        
        if (sfb.read() != true)
        {
            QMessageBox::critical(this, "Error", QString("Error occurred while reading"));
            FileOpened = false;
            return;
        }

        SetLineEdits();
        FileOpened = true;
        this->setWindowTitle(QString("SFB Editor - %1").arg(filename));
    }
    return;
}
void SFBGui::OnActionCreate()
{
    if (FileOpened == true)
    {
        sfb.close();
        FileOpened = false;
    }

    sfb.set_defaults();
    SetLineEdits();
    this->setWindowTitle("SFB Editor - New Project");

    FileCreateMode = true;
    return;
}
void SFBGui::OnActionSave()
{
    if (FileCreateMode == true)
    {
        if (create_ptr == nullptr)
        {
            QString filename = QFileDialog::getSaveFileName(this, "Save file", "", "sfb files (*.SFB);; All Files(*.*)");
            if (filename.size() > 0)
            {
                create_ptr = sfb.create_as(filename.toStdString());
                GetFromLineEdits();
                this->setWindowTitle(QString("SFB Editor - Create Mode -  %1").arg(filename));
            }
        }

        GetFromLineEdits();
        sfb.write_as(create_ptr);
        return;
    }
    
    if (FileOpened == true)
    {
        GetFromLineEdits();
        sfb.write();
        return;
    }
}
void SFBGui::OnActionSaveAs()
{
    if (FileCreateMode == true || FileOpened == true)
    {
        QString filename = QFileDialog::getSaveFileName(this, "Save file", "", "sfb files (*.SFB);; All Files(*.*)");
        if (filename.size() > 0)
        {
            void* saveas = sfb.create_as(filename.toStdString());
            
            GetFromLineEdits();
            sfb.write_as(saveas);
            sfb.close_as(saveas);
        }
    }
}
void SFBGui::OnActionClose()
{
    if (FileCreateMode == true)
    {
        if (create_ptr != nullptr)
        {
            sfb.close_as(create_ptr);
            create_ptr = nullptr;
        }
        FileCreateMode = false;
    }    

    if (FileOpened == true)
    {
        if (sfb.close() != true)
        {
            QMessageBox::critical(this, "Error", QString("Error occurred while closing file"));
        }
        FileOpened = false;
    }

    for (int i = 0; i < 9; i++)
    {
        LineEdit[i]->setText("");
    }
    
    this->setWindowTitle("SFB Editor");
    return;
}
void SFBGui::OnActionExit()
{
    if (FileOpened == true)
    {
        sfb.close();
    }

    if (create_ptr != nullptr)
    {
        sfb.close_as(create_ptr);
    }

    QApplication::exit();
}

void SFBGui::SetLineEdits()
{
    LineEdit[0]->setText(QString::asprintf("0x%.8X", REV(sfb.version)));
    LineEdit[1]->setText(sfb.hybrid_flag);
    LineEdit[2]->setText(QString::asprintf("0x%.8X", REV(sfb.disc_content_data_offset)));
    LineEdit[3]->setText(QString::asprintf("0x%.8X", REV(sfb.disc_content_data_lenght)));
    LineEdit[4]->setText(sfb.disc_title_name);
    LineEdit[5]->setText(QString::asprintf("0x%.8X", REV(sfb.disc_title_data_offset)));
    LineEdit[6]->setText(QString::asprintf("0x%.8X", REV(sfb.disc_title_data_lenght)));
    LineEdit[7]->setText(sfb.disc_content);
    LineEdit[8]->setText(sfb.disc_title);
}
void SFBGui::GetFromLineEdits()
{
    // Badly implemented checks
    const int expectedLenghts[9] = { 10, 11, 10, 10, 8,  10, 10, 32, 16 };
    for (int i = 0; i < 9; i++)
    {
        if (LineEdit[i]->text().length() > expectedLenghts[i])
        {
            QMessageBox::warning(this, "Warning", QString("%1' s lenght can not bigger than %2 ").arg(PropList[i]).arg(expectedLenghts[i]) );
            return;
        }


        if (i == 0 || i == 2 || i == 3 || i == 5 || i == 6)
        {
            if (LineEdit[i]->text().startsWith("0x") == false)
            {
                QMessageBox::warning(this, "Warning", QString("%1 should be startswith 0x").arg(PropList[i]));
                return;
            }
        }
    }

    sfb.version = REV(((uint32_t)LineEdit[0]->text().toInt(nullptr, 16)));
    strcpy(sfb.hybrid_flag, LineEdit[1]->text().toStdString().c_str());
    sfb.disc_content_data_offset = REV(((uint32_t)LineEdit[2]->text().toInt(nullptr, 16)));
    sfb.disc_content_data_lenght = REV(((uint32_t)LineEdit[3]->text().toInt(nullptr, 16)));
    strcpy(sfb.disc_title_name, LineEdit[4]->text().toStdString().c_str());
    sfb.disc_title_data_offset = REV(((uint32_t)LineEdit[5]->text().toInt(nullptr, 16)));
    sfb.disc_title_data_lenght = REV(((uint32_t)LineEdit[6]->text().toInt(nullptr, 16)));
    strcpy(sfb.disc_content, LineEdit[7]->text().toStdString().c_str());
    strcpy(sfb.disc_title, LineEdit[8]->text().toStdString().c_str());
}

void SFBGui::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

void SFBGui::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void SFBGui::dropEvent(QDropEvent* event)
{
    event->acceptProposedAction();

    if (FileOpened == true)
    {
        sfb.close();
    }

    if (create_ptr != nullptr)
    {
        sfb.close_as(create_ptr);
        FileCreateMode = false;
    }

    QString filename = event->mimeData()->urls()[0].toLocalFile();
    
    if (sfb.open(filename.toStdString()) != true)
    {
        QMessageBox::critical(this, "Error", QString("%1 couldn't open successfully").arg(filename));
        FileOpened = false;
        return;
    }

    if (sfb.read() != true)
    {
        QMessageBox::critical(this, "Error", QString("Error occurred while reading"));
        FileOpened = false;
        return;
    }
    SetLineEdits();
    FileOpened = true;
    this->setWindowTitle(QString("SFB Editor - %1").arg(filename));
}