#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QTreeView>
#include <QComboBox>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <QString>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>
#include <QtGlobal>
#include <QModelIndexList>
#include <QHeaderView>
#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLocale>
#include <QLabel>
#include <QList>
#include <QRegularExpression>
#include <QValidator>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <iostream>
#include <stack>
#include <zlib.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //Кнопки Hud'а
    Ui::MainWindow* ui;
    Ui::MainWindow* Addbutton;
    Ui::MainWindow* extractButton;
    Ui::MainWindow* viewButton;
    Ui::MainWindow* deleteButton;

    //Виджеты просмотру файлов
    QTreeView* fileView;
    QComboBox* diskPath;
    QFileSystemModel * systemFilesQbox;
    QFileSystemModel* systemFiles;

    //Виджеты к окну find
    QDialog* findW;
    QComboBox* FindDiskPath;
    QLineEdit* FileNameFind;
    QComboBox* ChooseDiskPath;
    QGroupBox* GPFind;
    QString NameOfFileString;

    //Виджеты к окну add
    QDialog* addW;
    QLineEdit* ArchiveNameEnter;
    QGroupBox* GPAdd;

    //Виджеты к окну extract
    QDialog* extractW;
    QLineEdit* ExtractFolderEnter;
    QGroupBox* GPExtract;

    //Список для хранения списка выделенных файлов для архивирования
    QList<QString> FileListAdd;



private slots:
    void AddbuttonClick();
    void ViewbuttonClick();
    void diskPathIndexChange();
    void fileViewOpen(const QModelIndex index);
    void deleteButtonClick();
    void addGetArchiveName();
    void ExtractButtonClick();
    void extractArchivePlace();
};
void ReadData(std::string& pathToFile, std::string& buffer); //Функция чтения данных

void makingHuffmanTree(std::string& pathToFile, std::string& buffer); //Создание и обработка дерева Хаффмана

bool isTextFile(const QString& pathToFile); //Проверка на тип файла (текстовый или отличный от него)

void GetAllFilesPath(const QString& index); //Функция получения путей выделенных файлов


#endif // MAINWINDOW_H
