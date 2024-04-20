#include "mainwindow.h"
#include "./ui_mainwindow.h"

QFileInfoList fileList;  //Массив для хранения путей (используется, чтобы получить внутренние директории, *ремарка* не знаю, стоит ли делать его глобальным)
QString ArchiveName = ""; //Имя архива
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    MainWindow::setWindowTitle("ESTM");
    MainWindow::setFixedSize(930, 570);
    MainWindow::setWindowFlags(MainWindow::windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

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
    viewButton->setText("Find");
    viewButton->setStyleSheet( "QToolButton { border: none; }" "QToolButton:pressed { background-color: #cce6ff; border: 0.5px solid #66b3ff; padding: 0px }" ); //Стили на кнопку
    QObject::connect(viewButton, &QToolButton::clicked, this, &MainWindow::ViewbuttonClick); //Обработка нажатия на кнопку просмотра файлов

    //Окно поиска
    findW = new QDialog(this);
    findW->setWindowTitle("Поиск файлов");
    findW->setFixedSize(250, 180);
    GPFind = new QGroupBox(findW);
    GPFind->setTitle("Что искать");
    GPFind->setGeometry(20, 10, 200, 150);
    GPFind->setObjectName("GPFind");
    GPFind->setStyleSheet("#GPFind {border: 0.5px solid grey; border-radius: 3px;}");
    QLabel* FileNameFindLabel = new QLabel("Введите имя файла:",GPFind);
    QLabel* FilePathFindLabel = new QLabel("Область поиска", GPFind);
    ChooseDiskPath = new QComboBox(findW);
    FileNameFind = new QLineEdit(findW);
    QRegularExpression regx ("^[A-Z a-z]+*?$");
    QVBoxLayout* findLayout = new QVBoxLayout(GPFind);
    findLayout->addWidget(FileNameFindLabel);
    findLayout->addWidget(FileNameFind);
    findLayout->addWidget(FilePathFindLabel);
    findLayout->addWidget(ChooseDiskPath);
    GPFind->setLayout(findLayout);
    QFileSystemModel* systemFilesBox = new QFileSystemModel(findW);
    systemFilesBox ->index("C://");
    systemFilesBox->setRootPath(QDir::currentPath());
    ChooseDiskPath->setModel(systemFilesBox);

    //Окно добавления архива
    addW = new QDialog(this);
    addW->setWindowTitle("Создать архив");
    addW->setFixedSize(200, 60);
    GPAdd = new QGroupBox(addW);
    GPAdd->setTitle("Введите путь");
    GPAdd->setGeometry(20, 40, 60, 40);
    GPAdd->setObjectName("AddGP");
    GPAdd->setStyleSheet("#AddGP {padding-top: 10px; border: 0.5px solid grey; border-radius: 3px; margin-top: 0px}");
    QRegularExpression rx("^[A-Z]:\\\\[^\\\\/:*?\"<>|]+(\\\\.+)*\\\\?$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ArchiveNameEnter = new QLineEdit(GPAdd);
    ArchiveNameEnter->setValidator(validator);
    ArchiveNameEnter->setPlaceholderText("C:\\Users\\Username\\ArchiveName.estm");
    ArchiveNameEnter->setGeometry(4, 15, 170, 20);
    ArchiveNameEnter->setStyleSheet(" border: 0px;");
    QVBoxLayout* addLayout = new QVBoxLayout(addW);
    addLayout->addWidget(GPAdd);
    QObject::connect(ArchiveNameEnter, &QLineEdit::returnPressed, this, &MainWindow::addGetArchiveName);


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
                         if (!deselected.empty())
                             {
                                FileListAdd.clear();
                             }
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

Node* createNode(char ch, int freq, Node* left, Node* right) //Создание узла дерева для кодирования
{
    Node* node = new Node();
    node->freq = freq;
    node->ch = ch;
    node->left = left;
    node->right = right;
    return node;
}

void deleteHuffmanTree(Node* root)
{
    if (root == nullptr)
    {
        return;
    }
    deleteHuffmanTree(root->left);
    deleteHuffmanTree(root->right);
    delete root;
}

void encode (Node* root, std::string code, std::unordered_map<char, std::string>& HuffmanCode) // Алгоритм кодирования
{
    if (root == nullptr)
    {
        return;
    }

    if (!root->left && !root->right) //Ищем висячие узлы
    {
        HuffmanCode[root->ch] = code;   //15.04.24 Проверить шифровку данных
    }

    encode(root->left, code + "0", HuffmanCode);
    encode(root->right, code + "1", HuffmanCode);
}

void decode(Node* root, int &index, std::string encodedStr, QTextStream& outfile)
{
    if (root == nullptr)
    {
        return;
    }


    if (!root->left && !root->right)
    {
        root->ch;
        return;
    }

    index++;

    if (encodedStr[index] =='0')
        decode(root->left, index, encodedStr, outfile);
    else
        decode(root->right, index, encodedStr, outfile);
}

struct compare //Компаратор для очереди с приоритетом (выбираем узел в наименьшей частотой - высший приоритет)
{
    bool operator()(Node* l, Node* r)
    {
        return l->freq > r->freq;
    }
};

void MainWindow::addGetArchiveName() //Получаем имя архива от пользователя
{
    ArchiveName = ArchiveNameEnter->text();
    QFileInfo suffArchAddCh(ArchiveName);
    if (suffArchAddCh.suffix()!= "estm")
    {
        QMessageBox::warning(this, "Предупреждение", "Введите корректное расширение архива!");
        return;
    }
    ArchiveNameEnter->clear();
    addW->close();
}

void makingHuffmanTree(std::string& buffer) //Функция создания дерева Хаффмана
{
    std::unordered_map <char, int> freq; //хэш-таблица для хранения частоты повторения символов

    //Определеяем частоты символолов
    for (char ch: buffer)
    {
        freq[ch]++;
    }

    std::priority_queue <Node*, std::vector <Node*> , compare> pQueue;

    //Создаём висячие вершины для каждой буквы и помещаем их очередь с приоритетом
    for (auto pair: freq)
    {
        pQueue.push(createNode(pair.first, pair.second, nullptr, nullptr));
    }

    //Собираем наши висячие вершины, объединяя их в более крупные узлы (собираем структуру дерева)
    while (pQueue.size() != 1)
    {
        Node* left = pQueue.top();
        pQueue.pop();
        Node* right= pQueue.top();
         pQueue.pop();

        int sum = left->freq + right->freq;
        pQueue.push(createNode('\0', sum, left, right));
    }

    Node* root = pQueue.top(); //Собраное дерево

    std::unordered_map<char, std::string> HuffmanCode;
    encode(root, "", HuffmanCode); //Вызываем функцию кодирования

    std::string encodedStr; //Сохраняем закодированное содержимое файла

    for (char ch: buffer)
    {
        encodedStr+=HuffmanCode[ch];
    }

    int index = -1;

    QFile fOut(ArchiveName);
    if(fOut.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream outfile(&fOut);
       // while (index < (int)encodedStr.size()-2)
       //{
          //  decode(root, index, encodedStr, outfile);
       // }

        qDebug ()<< encodedStr.size();
        encodedStr+="\n";
        qDebug() << encodedStr;

        fOut.write(reinterpret_cast<const char*>(&encodedStr), sizeof(encodedStr));
        fOut.close();
    }
    else
    {
        qDebug() << "File wasn't opened!";
    }
    deleteHuffmanTree(root);

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

    if (FileListAdd.empty())
    {
        QMessageBox::warning(this, "Предупреждение", "Выберете файл или папку для добавления!");
        return;
    }
    QString buffer="";
    char ch;
    addW->exec();
    GPAdd->show();
    if (ArchiveName.isEmpty())
    {
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
        foreach(const QFileInfo i, fileList)  //Обработка каждого файла
        {
            QString AddPathFile =i.absoluteFilePath();
            std::ifstream infile;
            infile.open(QFile::encodeName(AddPathFile).toStdString(), std::ios::in);
            if (!infile.is_open())
            {
                QMessageBox::warning(this, "Warning", "Не удалось открыть файл!");
                FileListAdd.clear();
                return;
            }
            else
            {
                buffer = AddPathFile.remove(0, AddPathFile.indexOf('/')); //Получаем путь до файла без диска ( нужен для воссоздания исходной директории)
                buffer += "\n";
                while (infile.get(ch))
                {
                      buffer+=ch;
                }
            }
            buffer += "\r\n";
            std::string tempStr = buffer.toStdString();
            makingHuffmanTree(tempStr);
            buffer.clear();
        }
        ArchiveName.clear();
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
        QMessageBox::warning(this, "Предупреждение", "Выберете файл или папку для удаления!");
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
