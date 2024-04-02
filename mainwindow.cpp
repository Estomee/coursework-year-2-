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

    //Окно поиска
    findW = new QDialog(this);
    findW->setWindowTitle("Search file");
    findW->setFixedSize(400,300);

    //Выбор коренной директории поиска

    ChooseDiskPath = new QComboBox(findW);
    ChooseDiskPath->setGeometry(10, 15, 100, 25);

    FileNameFind = new QLineEdit(findW);
    FileNameFind->setGeometry(115, 15, 100, 25);
    QVBoxLayout findLayout;
    //Доделать GroupBox 02.04.24






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
    QObject::connect(deleteButton, &QToolButton::clicked, this, &MainWindow::deleteButtonClick);
    fileView = new QTreeView(this);

    //Виджеты для просмотра списка файлов
    systemFiles = new QFileSystemModel(this);
    // Установить корень модели файловой системы в соответствии с текущим диском
    systemFiles->setRootPath(QDir::rootPath());
    fileView->setModel(systemFiles);
    fileView->setRootIndex(systemFiles->index("C://"));
    fileView->setGeometry(30,125,850,400);
    fileView->setSortingEnabled(true);
    fileView->header()->setSectionsMovable(false); //Запрещаем пользователю двигать столбцы
    fileView->setIndentation(20);                //Отступ
    fileView->setColumnWidth(0, 400); // Установить ширину колонки для отображения полных названий файлов
    fileView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Установить политику размеров для QTreeView
    fileView->setSelectionMode(QAbstractItemView::ExtendedSelection); // Задать режим выбора элементов
    fileView->setFocusPolicy(Qt::NoFocus);
    fileView->sortByColumn(1, Qt::AscendingOrder);


    //Вид поля с путями
    systemFilesQbox = new QFileSystemModel(this);
    systemFilesQbox->index("C://");
    systemFilesQbox->setRootPath(QDir::currentPath());

    //Поле для вывода пути
    diskPath = new QComboBox(this);
    diskPath->setGeometry(30,100,850,20);
    diskPath->setModel(systemFilesQbox);
    connect(diskPath, &QComboBox::currentIndexChanged, this, &MainWindow::diskPathIndexChange); //Соедниение события - при изменениее индекса (диска) меняется содержимое (папки и файлы)

    //Соединения события - открытие окна
    connect(fileView, &QTreeView::doubleClicked, this, &MainWindow::fileViewOpen);

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
{
    findW->exec();


}
//Динамическое изменение списка файлов в зависимости от выбранного диска
void MainWindow::diskPathIndexChange()
{
    QList <QFileInfo> mainDrives = QDir::drives();
    fileView->setRootIndex(systemFiles->index(mainDrives.at(diskPath->currentIndex()).absoluteFilePath()));
}
//Открытие файла в окне просмотра
void MainWindow::fileViewOpen(const QModelIndex index)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(systemFiles->filePath(index)));
}

void MainWindow::deleteButtonClick()
{
    QString filesPath="";

    QModelIndexList fileViewindexList =  fileView->selectionModel()->selectedIndexes(); //Образуем список выбранных индексов(папок) в fileView
    QFileSystemModel *fileModel = qobject_cast<QFileSystemModel*>(fileView->model()); //Создаём файловую модель QTreeView для обработки индексов (папок и файлов)

    if (fileViewindexList.empty())  //Проверка выделены ли файлы в списке
    {
        QMessageBox::warning(this,"Warning","Выберете файл или папку для удаления!");
    }
    else
    {
        foreach (const QModelIndex &index, fileViewindexList)  //Пробегаемся по списку выбранных файлов и папок
            {
                filesPath = fileModel->filePath(index);
                QFileInfo fileInfo(filesPath);

                if (fileInfo.isFile())
                {
                    QFile::remove(filesPath);
                }
                if (fileInfo.isDir())
                {
                    QDir dir(filesPath);
                    dir.removeRecursively();
                }
            }
        QMessageBox::information(this, "Success", "Файлы успешно удалены!");
    }

}

/* 24.03.24 - Заметка
 * Cделать локаль - в меню вывода списка файлово пофиксить байтов и тп, на английские названия
 * Поработать с восходящим путём по диску (файл с названием "..")
 *
 *
 *
 *
*/
