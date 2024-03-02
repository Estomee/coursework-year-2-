#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Кнопка добавления Архива
    QToolButton *button = new QToolButton(this);
    QPixmap pixmap(":/images/Images/Add_Icon.png"); //Иконка
    button->setGeometry(30,20,70,70);
    button->setIcon(pixmap);
    button->setIconSize(QSize (32,32));
    button->setIcon(pixmap.scaled(QSize (32,32),Qt::KeepAspectRatio,Qt::SmoothTransformation)); //Scale иконки
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);  //Текст под иконкой
    button->setText("Добавить \nархив");
    button->setStyleSheet( "QToolButton { border: none; }" "QToolButton:pressed { background-color: #cce6ff; border: 0.5px solid #66b3ff; padding: 1px }" ); //Стили на кнопку
    QObject::connect(button, &QToolButton::clicked, this, &MainWindow::AddbuttonClick); //Обработка нажатия на кнопку Добавления
}

MainWindow::~MainWindow()
{
    delete ui;
    delete button;
}

void MainWindow::AddbuttonClick()
{   QFileDialog FileSelect;


     QFileDialog::getOpenFileName(this,"Выберете файл","C://");

}

