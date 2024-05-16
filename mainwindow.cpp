#include "mainwindow.h"
#include "./ui_mainwindow.h"

QFileInfoList fileList;  //Массив для хранения путей (используется, чтобы получить внутренние директории, *ремарка* не знаю, стоит ли делать его глобальным)
QString ArchiveName = ""; //Имя архива
QString ExtractPlace =""; //Куда будет извлекать архив (путь)
QFileInfoList fileListView; //Храним список файлов для поиска
QString NameOfFileString; //Храним имя файла, который ищем

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
    Addbutton->setText("Create");
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
    FileNameFind->setPlaceholderText("123.txt");

    QRegularExpression regx ("^[A-Za-zа-ъА-Ъ0-9.]*$");
    QValidator* FindWValid = new QRegularExpressionValidator(regx, findW);
    FileNameFind->setValidator(FindWValid);
    QVBoxLayout* findLayout = new QVBoxLayout(GPFind);
    findLayout->addWidget(FileNameFindLabel);
    findLayout->addWidget(FileNameFind);
    findLayout->addWidget(FilePathFindLabel);
    findLayout->addWidget(ChooseDiskPath);
    GPFind->setLayout(findLayout);
    systemFilesBox = new QFileSystemModel(findW);
    systemFilesBox ->index("C://");
    systemFilesBox->setRootPath(QDir::currentPath());
    ChooseDiskPath->setModel(systemFilesBox);
    QObject::connect(FileNameFind, &QLineEdit::returnPressed, this, &MainWindow::ViewGetData);

    resultFoundW = new QDialog(this);
    resultFoundW->setFixedSize(800, 300);
    resultFoundW->setWindowTitle("Результаты поиска");
    FilesFound = new QTreeView(resultFoundW);
    FilesFoundModel = new QFileSystemModel(resultFoundW);
    FilesFound->setGeometry(20, 20, 760, 250);
    FilesFound->setColumnWidth(0, 200);
    FilesFound->header()->setSectionsMovable(false); //Запрещаем пользователю двигать столбцы
    FilesFound->header()->setSectionResizeMode(QHeaderView::Fixed); //Запрещаем увеличивать размер столбцов
    FilesFound->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Установить политику размеров для QTreeView
    FilesFound->setSelectionMode(QAbstractItemView::ExtendedSelection); // Задать режим выбора элементов
    FilesFound->setFocusPolicy(Qt::NoFocus);
    FilesFound->sortByColumn(1, Qt::AscendingOrder);
    QObject::connect(FilesFound, &QTreeView::doubleClicked, this, &MainWindow::fileFoundOpen);

    //Окно добавления архива
    addW = new QDialog(this);
    addW->setWindowTitle("Создать архив");
    addW->setFixedSize(200, 65);
    GPAdd = new QGroupBox(addW);
    GPAdd->setTitle("Введите путь");
    GPAdd->setGeometry(20, 40, 60, 55);
    GPAdd->setObjectName("AddGP");
    GPAdd->setStyleSheet("#AddGP {padding-top: 10px; border: 0.5px solid grey; border-radius: 3px; margin-top: 0px}");
    QRegularExpression rx("^[A-Z]:\\\\[^\\\\/:*?\"<>|]+(\\\\.+)*\\\\?$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ArchiveNameEnter = new QLineEdit(GPAdd);
    ArchiveNameEnter->setValidator(validator);
    ArchiveNameEnter->setPlaceholderText("C:\\Users\\Username\\ArchiveName.estm");
    ArchiveNameEnter->setGeometry(4, 20, 170, 20);
    ArchiveNameEnter->setStyleSheet(" border: 0px;");
    QVBoxLayout* addLayout = new QVBoxLayout(addW);
    addLayout->addWidget(GPAdd);
    QObject::connect(ArchiveNameEnter, &QLineEdit::returnPressed, this, &MainWindow::addGetArchiveName);

    //Окно извлечения архива
    extractW = new QDialog(this);
    extractW->setWindowTitle("Извлечь архив");
    extractW->setFixedSize(200, 65);
    GPExtract = new QGroupBox(extractW);
    GPExtract->setTitle("Введите путь");
    GPExtract->setGeometry(20, 40, 65, 55);
    GPExtract->setObjectName("AddGP");
    GPExtract->setStyleSheet("#AddGP {padding-top: 10px; border: 0.5px solid grey; border-radius: 3px; margin-top: 0px}");
    QRegularExpression rxExtact("^[A-Z]:\\\\[^\\\\/:*?.\"<>|]+(\\\\.[^.]+)*\\\\?$");
    QValidator *validExtract = new QRegularExpressionValidator(rxExtact, this);
    ExtractFolderEnter= new QLineEdit(GPExtract);
    ExtractFolderEnter->setValidator(validExtract);
    ExtractFolderEnter->setPlaceholderText("C:\\Folder\\");
    ExtractFolderEnter->setGeometry(4, 20, 170, 20);
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
    if (suffArchAddCh.suffix()!= "zip")
    {
        QMessageBox::warning(this, "Предупреждение", "Введите корректное расширение архива!");
        return;
    }
    ArchiveNameEnter->clear();
    addW->close();
}

void MainWindow::extractArchivePlace() //Получение места для разархивации архива
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
    //Проверка, что пользователь просто открыл окно для разархивирования, ничего не ввёл, и закрыл его
    try
    {
        if (extractW->close() && ExtractPlace.isEmpty())
        {
            throw 0 ;
        }

    }
    catch (int code)
    {
        if (code == 0)
        {
            extractW->close();
            ExtractFolderEnter->clear();
            return;
        }

    }

    QString extension;
    struct zip* archive;
    for (QString str: FileListAdd) //Пробегаемся по каждому архиву, который выбрали
    {
        QFileInfo f(str);
        extension = f.suffix().toLower();
        if(extension.isEmpty() || extension!="zip") //Валидация расширения архива
        {
            QMessageBox::warning(this, "Предупреждение", "Вы выбрали папку или файл, который имеет расширение, отличное от поддерживаемого архиватором!");
            return;
        }
        else
        {
            std::filesystem::path archivePathUnzip(str.toUtf8().constData());
            archive = zip_open(archivePathUnzip.string().c_str(), 0, NULL);
            if (!archive)
            {
                return;
            }
            struct zip_stat stat;
            for (int i = 0; i < zip_get_num_entries(archive, 0); ++i)
            {
                // Получить информацию о файле
                if (zip_stat_index(archive, i, 0, &stat) != 0)
                {
                    // Обработать ошибку
                    continue;
                }

                // Открыть файл в архиве
                struct zip_file *file = zip_fopen_index(archive, i, 0);
                if (!file)
                {
                    // Обработать ошибку
                    continue;
                }

                // Получить путь к файлу назначения
                QString DirectoryPath ="";
                QString FilePath = "";
                QFileInfo fileInfo(QString::fromUtf8(stat.name));
                QString FileinfoPath = fileInfo.path(); //Получаем имя файла
                if (FileinfoPath == '.')
                {
                    FileinfoPath =fileInfo.fileName();
                    DirectoryPath = ExtractPlace + '\\';
                    FilePath = ExtractPlace + '\\'+FileinfoPath;
                }
                else
                {
                    DirectoryPath = ExtractPlace + '\\';
                    FilePath =  ExtractPlace + '\\' + fileInfo.fileName();
                }

                // Заменить все разделители на разделитель, соответствующий текущей операционной системе
                FilePath.replace('/', QDir::separator());
                FilePath.replace('\\', QDir::separator());

                DirectoryPath.replace('/', QDir::separator());
                DirectoryPath.replace('\\', QDir::separator());
                // Создать каталоги, если необходимо

                    QDir dir;
                    dir.mkpath(DirectoryPath); //доделать проверку на то, существует ли директория
                // Открыть файл назначения
                QFile destFile(FilePath);
                if (!destFile.open(QIODevice::WriteOnly))
                {
                    qDebug() << destFile.errorString();
                    // Обработать ошибку
                    zip_fclose(file);
                    continue;
                }
                char buffer[4096];
                int bytesRead;
                while ((bytesRead = zip_fread(file, buffer, sizeof(buffer))) > 0)
                {
                    destFile.write(buffer, bytesRead);
                }
                // Закрыть файл назначения
                destFile.close();

                // Закрыть файл в архиве
                zip_fclose(file);
            }

            // Закрыть архив
            zip_close(archive);
        }
    }
    ExtractPlace.clear();

}

void GetAllFilesPath(const QString& index) //Рекурсивная обработка всех  для последующей обработки
{
    QDir FolderPath(index);
    FolderPath.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //Фильтры для считывания всех элементов в папке, которую мы выбрали для архивации
    QFileInfoList folderContent = FolderPath.entryInfoList();
    for (const QFileInfo& fileinfo : folderContent)
    {
        if (fileinfo.isFile())
        {
           fileList.append(fileinfo);//Считывание всех элементов, которые находятся в выбранной папке
        }
        else if (fileinfo.isDir())
        {
            GetAllFilesPath(fileinfo.absoluteFilePath());  //Рекурсивный обход внутренних папок
        }
    }
}
void MainWindow::AddbuttonClick() //Создание архива
{
    if (FileListAdd.empty())
    {
        QMessageBox::warning(this, "Предупреждение", "Выберете файл или папку для добавления!");
        return;
    }
    struct zip* archive;
    addW->exec();
    GPAdd->show();
    if (ArchiveName.isEmpty())
    {
        return;
    }

    std::string archivePathStr = ArchiveName.toUtf8().constData();
    std::filesystem::path archivePath(archivePathStr);

    foreach (QString index, FileListAdd)
    {
        QFileInfo FileType(index); //Получаем инфу о том, что выбрано (файл или папка)
        if (FileType.isFile() )
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
        archive = zip_open(archivePath.string().c_str(), ZIP_CREATE | ZIP_CHECKCONS, nullptr);
        QString AddPathFile = i.absoluteFilePath();
        QFile infile(AddPathFile);

        if (!infile.open(QIODevice::ReadOnly ))
        {
            FileListAdd.clear();
            zip_close(archive);
            return;
        }
        else
        {
            QByteArray byteArray = infile.readAll();
            std::string filePath = AddPathFile.toUtf8().constData();
            std::filesystem::path flsPath(filePath);
            std::vector<char> data(byteArray.begin(), byteArray.end());
            infile.close();

            zip_source_t *source = zip_source_buffer(archive, data.data(), data.size(), 0);
            if (source == nullptr)
            {
                QMessageBox::critical(this, "Ошибка", "Произошла ошибка записи данных в архив!");
                zip_close(archive);
                return;
            }

            zip_file_add(archive, flsPath.string().c_str(), source, ZIP_FL_ENC_GUESS | ZIP_FL_ENC_RAW);
            zip_close(archive);
        }
    }


    ArchiveName.clear();
}

void MainWindow::ViewGetData() //Получение имени файла для поиска
{
   //Проверка на то, что пользователь открыл окно поиска,  ничего не ввёл и закрыл его;
    try
    {
        NameOfFileString = FileNameFind->text();
        if (NameOfFileString.isEmpty())
        {
            throw 0;
        }
        QFileInfo inf(NameOfFileString);
        if (inf.suffix().isEmpty())
        {
            throw 1;
        }
    }
    catch(int code)
    {
        if (code == 0)
        {
            QMessageBox::warning(this, "Предупреждение", "Введите имя файла для поиска!");
            return;
        }
        if (code == 1)
        {
            QMessageBox::warning(this, "Предупреждение", "Введите расширение файла!");
            NameOfFileString.clear();
            return;
        }
    }

    FileNameFind->clear();
    findW->close();
}

void MainWindow::fileFoundOpen(const QModelIndex indx)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(FilesFoundModel->filePath(indx)));
}

auto compareFiles = [](const QFileInfo& a, const QFileInfo& b) //Компаратор
{
    return a.fileName() < b.fileName();
};

QList<QFileInfo>MainWindow::searchFiles(const QString &diskName, const QString &fileName) //Поиск файлов
{
    QList<QFileInfo> fileListView;
    QFileInfo ext(fileName);
    QDirIterator it(diskName, QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        QFileInfo fileInfo = it.fileInfo();
        if (fileInfo.isFile() && "*" + fileInfo.fileName() + "*" == "*" + ext.fileName() + "*")
        {
            fileListView.append(fileInfo);
        }
    }

    std::sort(fileListView.begin(), fileListView.end(), compareFiles);

    return fileListView;
}

void expandToRoot(const QModelIndex &index, QTreeView *treeView) //Открытие дерева после поиска файлов
{
    if (!index.isValid())
    {
        return;
    }

    expandToRoot(index.parent(), treeView);
    treeView->setExpanded(index, true);
}
void MainWindow::onSearchFinished() //Отображение результатов поиска
{
    QList<QFileInfo> fileListView = future.result();
    QFileSystemModel* mdl = new QFileSystemModel(resultFoundW); // Создаем новую модель
    if (fileListView.isEmpty())
    {

        FilesFound->setModel(mdl);
        FilesFound->setColumnWidth(0, 200);
        resultFoundW->exec();
        return;
    }
    // Очистка дерева
    delete FilesFoundModel;
    FilesFoundModel = new QFileSystemModel(resultFoundW);
    FilesFound->setModel(FilesFoundModel);
    for (const QFileInfo &fileInfo : fileListView)
    {
        if (fileInfo.isFile())
        {
            QString filePath = fileInfo.absoluteFilePath();
            QModelIndex index = FilesFoundModel->index(filePath);
            FilesFoundModel->fetchMore(index); // Загрузить дочерние элементы
            expandToRoot(index.parent(), FilesFound);
        }
    }

    // Очищаем результаты поиска
    fileListView.clear();

    FilesFound->setColumnWidth(0, 200);
    resultFoundW->exec();
}
void MainWindow::ViewbuttonClick() //Обработка нажатия на кнопку поиска
{
    findW->exec();
    GPFind->show();
    try
    {
        if (findW->close() && NameOfFileString.isEmpty())
        {
            throw 1;
        }
    }
    catch(int code)
    {
        if (code == 1)
        {
            findW->close();
            return;
        }
    }

    FilesFound->setModel(FilesFoundModel);
    int index = ChooseDiskPath->currentIndex(); //выбранный индекс в QComboBox
    QModelIndex sysModelIndex = ChooseDiskPath->model()->index(index, 0); //Получаем индекс в модели QComboBox (QSystemModel)
    QVariant DiskInfo = ChooseDiskPath->model()->data(sysModelIndex, QFileSystemModel::FilePathRole); //Получаем информацию о файле по пути
    DiskName = DiskInfo.toString(); //Получаем путь до файла

     futureWatcher = new  QFutureWatcher<QList<QFileInfo>>(findW);
     future = QtConcurrent::run(&MainWindow::searchFiles, this, DiskName, NameOfFileString);
     futureWatcher->setFuture(future);
     connect(futureWatcher, &QFutureWatcher<QList<QFileInfo>>::finished, this, &MainWindow::onSearchFinished);
}


void MainWindow::diskPathIndexChange()  //Динамическое изменение списка файлов в зависимости от выбранного диска
{
    QList <QFileInfo> mainDrives = QDir::drives();
    fileView->setRootIndex(systemFiles->index(mainDrives.at(diskPath->currentIndex()).absoluteFilePath()));
}
//Открытие файла в окне просмотра
void MainWindow::fileViewOpen(const QModelIndex index)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(systemFiles->filePath(index)));
}

void MainWindow::deleteButtonClick() //Удаление файла
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
        QMessageBox::information(this, "Успешно", "Файлы успешно удалены!");
    }

}


