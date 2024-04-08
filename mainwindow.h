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
#include <QModelIndexList>
#include <QHeaderView>
#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLocale>
#include <QLabel>

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

private slots:
    void AddbuttonClick();
    void ViewbuttonClick();
    void diskPathIndexChange();
    void fileViewOpen(const QModelIndex index);
    void deleteButtonClick();


};

#endif // MAINWINDOW_H
