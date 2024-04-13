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
#include <fstream>
#include <sstream>
#include <unordered_map>

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

    //Список для хранения списка выделенных файлов для архивирования
    QList<QString> FileListAdd;



private slots:
    void AddbuttonClick();
    void ViewbuttonClick();
    void diskPathIndexChange();
    void fileViewOpen(const QModelIndex index);
    void deleteButtonClick();


};

class Node
{
private:
    int freq; //Частота появления символа
    char ch; //Сам символ
    Node* left; //Левый узел
    Node* right; //Правый узел
};

Node* createNode(int freq, char ch, Node* left, Node* right); //Функция создания узла дерева для кодирования

void encode (Node* root, std::string code, unordered_map<char, std::string>& HuffmanCode); // Функция для алгоритма кодирования

void decode (Node* root, int& index, std::string code); //Функция для алгоритма декодирования


#endif // MAINWINDOW_H
