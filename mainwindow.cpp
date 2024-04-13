#include "mainwindow.h"
#include "./ui_mainwindow.h"

QFileInfoList fileList;  //Массив для хранения путей (используется, чтобы получить внутренние директории, *ремарка* не знаю, стоит ли делать его глобальным)

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
    findW->setWindowTitle("Поиск файлов");
    findW->setFixedSize(200, 180);

    GPFind = new QGroupBox(findW);
    GPFind->setTitle("Что искать");
    GPFind->setGeometry(20, 10, 160, 150);

    QLabel* FileNameFindLabel = new QLabel("Введите имя файла:",GPFind);
    QLabel* FilePathFindLabel = new QLabel("Область поиска:", GPFind);

    ChooseDiskPath = new QComboBox(findW);
    FileNameFind = new QLineEdit(findW);


    QVBoxLayout* findLayout = new QVBoxLayout(GPFind);
    //Доделать GroupBox 02.04.24
    findLayout->addWidget(FileNameFindLabel);
    findLayout->addWidget(FileNameFind);
    findLayout->addWidget(FilePathFindLabel);
    findLayout->addWidget(ChooseDiskPath);
    GPFind->setLayout(findLayout);


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
    fileView->header()->setSectionsMovable(false); //Запрещаем пользователю двигать столбцы
    fileView->header()->setSectionResizeMode(QHeaderView::Fixed); //Запрещаем увеличивать размер столбцов
    fileView->setRootIndex(systemFiles->index("C://"));
    fileView->setGeometry(30,125,850,400);
    fileView->setSortingEnabled(true);
    fileView->setIndentation(20);                //Отступ
    fileView->setColumnWidth(0, 400); // Установить ширину колонки для отображения полных названий файлов
    fileView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Установить политику размеров для QTreeView
    fileView->setSelectionMode(QAbstractItemView::ExtendedSelection); // Задать режим выбора элементов
    fileView->setFocusPolicy(Qt::NoFocus);
    fileView->sortByColumn(1, Qt::AscendingOrder);
    QObject::connect(fileView->selectionModel(), &QItemSelectionModel::selectionChanged, [&](const QItemSelection &selected, const QItemSelection &deselected) //Реализуем лямбда функцию для получения выбранных элементов в QTreeView(fileView)
                     {
                         foreach(const QItemSelectionRange &range, selected)
                         {
                             QAbstractItemModel *model = fileView->model();
                             for(int i = range.top(); i <= range.bottom(); ++i)
                             {
                                 QModelIndex index = model->index(i, 0, range.parent()); // получаем индекс дочернего элемента
                                 QFileInfo fileInfo = systemFiles->fileInfo(index);
                                 FileListAdd.append(fileInfo.absoluteFilePath());
                             }
                         }
                     });

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

Node* createNode(int freq, char ch, Node* left, Node* right) //Создание узла дерева для кодирования
{
    Node* node = new Node();
    node->freq = freq;
    node->ch = ch;
    node->left = left;
    node->right;
    return node;
}

void encode (Node* root, std::string code, unordered_map<char, std::string>& HuffmanCode) // Алгоритм кодирования
{
    if (root == nullptr)
    {
        return;
    }

    if (!root->left && !root->right) //Ищем висячие узлы
    {
        HuffmanCode[root->ch] = code;
    }

    encode(root->left, code + "0", HuffmanCode,);
    encode(root->right, code + "1", Huffmancode);
}

void decode (Node* root, int& index, std::string code) //Расшифрока кода
{
    if (root == nullptr)
    {
        return;
    }

    index++;

    if (!root->left && !root->right) //Ищем висячие узлы
    {
        //тут типа запись в файл расшифрованной строки данных
    }
    if (code[index]== '0')
    {
        decode(root->left, index, code);
    }
    else
    {
        decode(root->right, index, code);
    }
}
bool isTextFile(const QString& pathToFile) //Проверка на то, является ли файл текстовым
{
    const int bufferSize = 1024;    //Суть в том, что мы считывает буффер размером 1024
    QFile f (pathToFile);
    if (!f.open(QIODevice::ReadOnly))
    {
     return false;
    }
    QByteArray fBuffer = f.read(bufferSize); // Считываем bufferSize байт
    f.close();
    if (fBuffer.isEmpty())
    {
        return false;
    }
    else
    {
        for (char c : fBuffer)  // Делаем простую проверку, если я могу считать символы с файла и они входят в таблицу ASCII, тогда возвращаем true - файл текстовый.
        {
            if (!QChar(c).isPrint())
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
}

void GetAllFilesPath(const QString& index) //Рекурсивная обработка всех  для последующей обработки
{
    QDir FolderPath(index);
    FolderPath.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //Фильтры для считывания всех элементов в папке, которую мы выбрали для архивации
    QFileInfoList folderContent = FolderPath.entryInfoList();
    for (const QFileInfo& fileinfo : folderContent)
    {
        if (fileinfo.isFile() && isTextFile(fileinfo.absoluteFilePath()))
        {
           fileList.append(fileinfo);//Считывание всех элементов, которые находятся в выбранной папке
        }
        else if (fileinfo.isDir())
        {
            GetAllFilesPath(fileinfo.absoluteFilePath());  //Рекурсивный обход внутренних папок
        }
    }
}
void MainWindow::AddbuttonClick() //Обработка добавления файлов в архив
{
    QString AddPathFile ="";
    if (FileListAdd.empty())
    {
        QMessageBox::warning(this, "Warning", "Выберете файл или папку для добавления!");
        return;
    }

        foreach (QString index, FileListAdd)
        {
            QFileInfo FileType(index); //Получаем инфу о том, что выбрано (файл или папка)
            if (FileType.isFile() && isTextFile(FileType.absoluteFilePath()))
            {
                fileList.append(FileType);
            }
            else
            {
                GetAllFilesPath(index);
            }
        }
        FileListAdd.clear();

        foreach(const QFileInfo i, fileList)  //Обработка каждого файла
        {
            QFile infile(i.absoluteFilePath());

            if (!infile.open(QIODevice::ReadOnly))
            {
                QMessageBox::warning(this, "Warning", "Не удалось открыть файл!");
                FileListAdd.clear();
                return;
            }
            else
            {
                QByteArray buffer = infile.readAll();
            }
            qDebug() << fileList;
        }

}


void MainWindow::ViewbuttonClick()
{
    findW->exec();
    GPFind->show();



        NameOfFileString = FileNameFind->text();
        //05.04.24 Доделать обработку события (нажатие Enter) для получения текста из QLineEdit


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
        QMessageBox::warning(this, "Warning", "Выберете файл или папку для удаления!");
    }
    else
    {
        foreach (const QModelIndex& index, fileViewindexList)  //Пробегаемся по списку выбранных файлов и папок
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
