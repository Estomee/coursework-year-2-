#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    //Кнопка добавления Архива
    QToolButton *Addbutton = new QToolButton(this);
    QPixmap pixAdd(":/images/Images/Add_Icon.png"); //Иконка
    Addbutton->setGeometry(30,20,70,70);
    Addbutton->setIcon(pixAdd);
    Addbutton->setIconSize(QSize (32,32));
    Addbutton->setIcon(pixAdd.scaled(QSize (32,32),Qt::KeepAspectRatio,Qt::SmoothTransformation)); //Scale иконки
    Addbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);  //Текст под иконкой
    Addbutton->setText("Add");
    Addbutton->setStyleSheet( "QToolButton { border: none; }" "QToolButton:pressed { background-color: #cce6ff; border: 0.5px solid #66b3ff; padding: 0px }" ); //Стили на кнопку
    QObject::connect(Addbutton, &QToolButton::clicked, this, &MainWindow::AddbuttonClick); //Обработка нажатия на кнопку добавления

    //Кнопка распаковки архива
    QToolButton* extractButton = new QToolButton(this);
    QPixmap pixExtr (":/images/Images/Extract_Icon.png");
    extractButton->setGeometry(100,20,70,70);
    extractButton->setIcon(pixExtr);
    extractButton->setIconSize(QSize (32,32));
    extractButton->setIcon(pixExtr.scaled(QSize (32,32),Qt::KeepAspectRatio,Qt::SmoothTransformation)); //Scale иконки
    extractButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);  //Текст под иконкой
    extractButton->setText("Extract");
    extractButton->setStyleSheet( "QToolButton { border: none; }" "QToolButton:pressed { background-color: #cce6ff; border: 0.5px solid #66b3ff; padding: 0px }" ); //Стили на кнопку

    //Кнопка просмотра файлов директории
    QToolButton* viewButton = new QToolButton(this);
    QPixmap pixView (":/images/Images/View_Icon.png");
    viewButton->setGeometry(170,20,70,70);
    viewButton->setIcon(pixView);
    viewButton->setIconSize(QSize (32,32));
    viewButton->setIcon(pixView.scaled(QSize (32,32),Qt::KeepAspectRatio,Qt::SmoothTransformation)); //Scale иконки
    viewButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);  //Текст под иконкой
    viewButton->setText("View");
    viewButton->setStyleSheet( "QToolButton { border: none; }" "QToolButton:pressed { background-color: #cce6ff; border: 0.5px solid #66b3ff; padding: 0px }" ); //Стили на кнопку
    QObject::connect(viewButton, &QToolButton::clicked, this, &MainWindow::ViewbuttonClick); //Обработка нажатия на кнопку просмотра файлов

    //Кнопка удаления файлов
    QToolButton* deleteButton = new QToolButton(this);
    QPixmap pixDelete (":/images/Images/Delete_Icon.png");
    deleteButton->setGeometry(240,20,70,70);
    deleteButton->setIcon(pixDelete);
    deleteButton->setIconSize(QSize (32,32));
    deleteButton->setIcon(pixDelete.scaled(QSize (32,32),Qt::KeepAspectRatio,Qt::SmoothTransformation)); //Scale иконки
    deleteButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);  //Текст под иконкой
    deleteButton->setText("Delete");
    deleteButton->setStyleSheet( "QToolButton { border: none; }" "QToolButton:pressed { background-color: #cce6ff; border: 0.5px solid #66b3ff; padding: 0px }" ); //Стили на кнопку





}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddbuttonClick()
{   QFileDialog FileSelect;
     QFileDialog::getOpenFileName(this,"Выберете файл","C://");
}

void MainWindow::ViewbuttonClick()
{     QTreeView*  fileView = new QTreeView(this);
    // Очистить QTreeView перед загрузкой новых файлов
    fileView->setModel(nullptr);
    fileView->reset();
    //Виджеты для просмотра списка файлов
    QFileSystemModel* systemFiles = new QFileSystemModel(this);
    // Установить корень модели файловой системы в соответствии с текущим диском

    systemFiles->setRootPath(QDir::rootPath());

    fileView->setModel(systemFiles);
    fileView->setGeometry(30,125,700,400);
    fileView->setSortingEnabled(true);
    fileView->setIndentation(20);                //Отступ
    fileView->setColumnWidth(0, 400); // Установить ширину колонки для отображения полных названий файлов
    fileView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Установить политику размеров для QTreeView
    fileView->setSelectionMode(QAbstractItemView::ExtendedSelection); // Задать режим выбора элементов

    fileView->show();

    //Поле для вывода пути
    QFileSystemModel* systemFilesQbox = new QFileSystemModel(this);
    systemFilesQbox->setRootPath(QDir::currentPath());

    QComboBox* diskPath = new QComboBox(this);
    diskPath->setGeometry(30,100,700,20);
    diskPath->setModel(systemFilesQbox);
    connect(diskPath, &QComboBox::currentIndexChanged, this, &MainWindow::diskPath_Index_Change);



    diskPath->show();

}


void MainWindow:: diskPath_Index_Change() //Доделать слот изменения содержания QTreeView от QComboBox
{   QFileSystemModel abc;
    abc.setRootPath(QDir(diskPath->currentText()).absolutePath());
    fileView->setModel(&abc);
    fileView->setRootIndex(abc.index(QDir(diskPath->currentText()).absolutePath()));));

}
