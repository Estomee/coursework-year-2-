#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QTreeView>
#include <QComboBox>
#include <QSortFilterProxyModel>
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

    //Виджеты к кнопке View
    QTreeView* fileView;
    Ui::MainWindow* systemFilesProxy;
    QComboBox* diskPath;
    Ui::MainWindow* systemFilesQbox;



private slots:
    void AddbuttonClick();
    void ViewbuttonClick();
    void on_driveComboBox_currentIndexChanged(const QString &drive);

};

#endif // MAINWINDOW_H
