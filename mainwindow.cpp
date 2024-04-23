#include "mainwindow.h"
#include "./ui_mainwindow.h"

QFileInfoList fileList;  //Массив для хранения путей (используется, чтобы получить внутренние директории, *ремарка* не знаю, стоит ли делать его глобальным)
QString ArchiveName = ""; //Имя архива
QString ExtractPlace =""; //Куда будет извлекать архив (путь)

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
    QObject::connect(extractButton, &QToolButton::clicked, this, &MainWindow::ExtractButtonClick);

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

    //Окно извлечения архива
    extractW = new QDialog(this);
    extractW->setWindowTitle("Извлечь архив");
    extractW->setFixedSize(200, 60);
    GPExtract = new QGroupBox(extractW);
    GPExtract->setTitle("Введите путь");
    GPExtract->setGeometry(20, 40, 60, 40);
    GPExtract->setObjectName("AddGP");
    GPExtract->setStyleSheet("#AddGP {padding-top: 10px; border: 0.5px solid grey; border-radius: 3px; margin-top: 0px}");
    QRegularExpression rxExtact("^[A-Z]:\\\\[^\\\\/:*?.\"<>|]+(\\\\.[^.]+)*\\\\?$");
    QValidator *validExtract = new QRegularExpressionValidator(rxExtact, this);
    ExtractFolderEnter= new QLineEdit(GPExtract);
    ExtractFolderEnter->setValidator(validExtract);
    ExtractFolderEnter->setPlaceholderText("C:\\Users\\Username\\");
    ExtractFolderEnter->setGeometry(4, 15, 170, 20);
    ExtractFolderEnter->setStyleSheet(" border: 0px;");
    QVBoxLayout* ExtractLayout = new QVBoxLayout(extractW);
    ExtractLayout->addWidget(GPExtract);
    QObject::connect(ExtractFolderEnter, &QLineEdit::returnPressed, this, &MainWindow::extractArchivePlace);

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

std::vector <unsigned char> compresspath(const std::string& pathToFile) //сжатие пути до файла
{
    uLongf maxCompressedSize = compressBound(pathToFile.length());
    std::vector<unsigned char> compressed(maxCompressedSize); //сжатая строка
    uLongf compressedSize = maxCompressedSize;

    int compressionLevel = Z_DEFAULT_COMPRESSION;
    int result = compress2((Bytef*)compressed.data(), &compressedSize, (const Bytef*)pathToFile.data(), pathToFile.length(), compressionLevel);

    if (result != Z_OK) {
        throw std::runtime_error("Failed to compress string");
    }

    compressed.resize(compressedSize);
    return compressed;
}

std::vector<unsigned char> compressdata(const std::vector<unsigned char>& inputData)
{
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = inputData.size();
    stream.next_in = const_cast<Bytef*>(inputData.data());

    deflateInit(&stream, Z_DEFAULT_COMPRESSION);

    std::vector<unsigned char> output;
    unsigned char buffer[16384];
    do {
        stream.avail_out = sizeof(buffer);
        stream.next_out = buffer;

        int result = deflate(&stream, Z_FINISH);
        if (result == Z_STREAM_ERROR)
        {
            deflateEnd(&stream);
            throw std::runtime_error("Failed to compress data");
        }

        int have = sizeof(buffer) - stream.avail_out;
        output.insert(output.end(), buffer, buffer + have);
    } while (stream.avail_out == 0);

    deflateEnd(&stream);

    return output;
}

std::vector<unsigned char> decompressdata(const std::vector<unsigned char>& inputData) //Разархивирование данных
{
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in =  inputData.size();
    stream.next_in = const_cast<Bytef*>(inputData.data());

    if (inflateInit(&stream) != Z_OK)
    {
        throw std::runtime_error("Failed to initialize decompression stream");
    }

    std::vector<unsigned char> output;
    unsigned char buffer[16384];
    int ret;
    do {
        stream.avail_out = sizeof(buffer);
        stream.next_out = buffer;

        ret = inflate(&stream, Z_NO_FLUSH);

        if (ret < 0)
        {
            inflateEnd(&stream);
            throw std::runtime_error("Failed to decompress data");
        }

        int have = sizeof(buffer) - stream.avail_out;
        output.insert(output.end(), buffer, buffer + have);
    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);

    return output;
}
void create_file_with_directories(QString& path, std::vector<unsigned char>& dataApart) //Воссоздание директории после разархивации
{
    QDir dir;

    const QString finalPath (ExtractPlace + path);
    QFileInfo finf(finalPath);

    dir.mkpath(finf.path());
    std::string pathToCreateFile = finf.absoluteFilePath().toStdString();
    std::fstream fCreate(pathToCreateFile, std::ios::out);
    fCreate.write(reinterpret_cast<const char*>(dataApart.data()), dataApart.size());
}
void MainWindow::extractArchivePlace()
{
    ExtractPlace = ExtractFolderEnter->text();
    ExtractFolderEnter->clear();
    extractW->close();
}
void MainWindow::ExtractButtonClick() //Разархивация
{
    if (FileListAdd.empty())
    {
       QMessageBox::warning(this, "Предупреждение", "Выберете файл для разархивирования!");
        return;
    }
    extractW->exec();
    GPExtract->show();
    QString extension;
    for (QString str: FileListAdd) //Пробегаемся по каждому архиву, который выбрали
    {
        QFileInfo f(str);
        extension = f.suffix().toLower();
        if(extension.isEmpty() || extension!="estm") //Валидация расширения архива
        {
            QMessageBox::warning(this, "Предупреждение", "Вы выбрали папку или файл, который имеет расширение, отличное от поддерживаемого архиватором!");
            return;
        }
        else
        {
            std::fstream readArchive(QFile::encodeName(str).toStdString(), std::ios::binary | std::ios::in);
            if (!readArchive.is_open())
           {
               qDebug() << "File isn't opened";
           }
             std::vector<unsigned char> compressedData;
            while (!readArchive.eof())
            {
                std::string line;
                std::getline(readArchive, line);
                for (char c : line)
                {
                    compressedData.push_back(static_cast<unsigned char>(c));
                }


                std::vector<unsigned char> decompressData = decompressdata(compressedData);
                std::ofstream outFile("D:/testCourseWork/decoded_data.txt", std::ios::out | std::ios::binary);
                outFile.write(reinterpret_cast<const char*>(decompressData.data()), decompressData.size());
                outFile.close();
            }
        }
    }
    ExtractPlace.clear();

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
    QString buffer=""; //храним информацию
    QString pathToFile= "";
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
            infile.open(QFile::encodeName(AddPathFile).toStdString(), std::ios::in );
            if (!infile.is_open())
            {
                FileListAdd.clear();
                return;
            }
            else
            {
                std::vector <unsigned char> inputData((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
                infile.close();
                pathToFile+='\0';
                pathToFile = AddPathFile.remove(0, AddPathFile.indexOf('/')); //Получаем путь до файла без диска ( нужен для воссоздания исходной директории)
                std::vector<unsigned char> compressedPath = compresspath(QFile::encodeName(pathToFile).toStdString());
                std::vector  <unsigned char> compressedData =compressdata(inputData);
                std::ofstream writeToArchive(QFile::encodeName(ArchiveName).toStdString(), std::ios::binary | std::ios::out | std::ios::app);
                writeToArchive.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
                writeToArchive.write(reinterpret_cast<const char*>(compressedPath.data()), compressedPath.size());
                writeToArchive.close();
                pathToFile.clear();
        }
            ArchiveName.clear();
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
