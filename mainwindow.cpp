#include <QLayout>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paintarea.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionLine,&QAction::triggered, this, &setLine);
    connect(ui->actionRectangle,&QAction::triggered, this, &setRectangle);
    connect(ui->actionEllipse,&QAction::triggered, this, &setEllipse);
    connect(ui->actionTriangle,&QAction::triggered, this, &setTriangle);

    connect(ui->actionPen, &QAction::triggered, this, &setPenColor);
    connect(ui->actionBrush, &QAction::triggered, this, &setBrushColor);


    connect(ui->actionGroup, &QAction::triggered, this, &makeGroup);
    connect(ui->actionUngroup, &QAction::triggered, this, &makeUnGroup);

    connect(ui->actionDelete, &QAction::triggered, this, &deletingSelected);

    ui->label->addObserver(ui->treeWidget);
    ui->treeWidget->addObserver(ui->label);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeGroup()
{
    ui->label->group();
}
void MainWindow::makeUnGroup()
{
    ui->label->ungroup();
}

void MainWindow::deletingSelected()
{
    ui->label->deletingSelected();
}
void MainWindow::setLine()
{
    ui->label->setShapeToDraw(new CLine(QPoint(),QPoint(),Qt::black,Qt::black));
}

void MainWindow::setRectangle()
{
    ui->label->setShapeToDraw(new CRectangle(0,0,1,1,Qt::black,Qt::black));
}

void MainWindow::setEllipse()
{
    ui->label->setShapeToDraw(new CEllipse(0,0,1,1,Qt::black,Qt::black));
}

void MainWindow::setTriangle()
{
    ui->label->setShapeToDraw(new CTreangle(0,0,1,1,Qt::black,Qt::black));
}

void MainWindow::setPenColor()
{
    ui->label->setPenColor(QColorDialog::getColor(Qt::black,this,"Choose pen color",QColorDialog::ShowAlphaChannel));
}

void MainWindow::setBrushColor()
{
    ui->label->setBrushColor(QColorDialog::getColor(Qt::black,this,"Choose brush color",QColorDialog::ShowAlphaChannel));
}


void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
          "Save as", "", "*.myimg");

    ui->label->save(fileName);
}

void MainWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          "Open", "", "*.myimg");

    if( ! fileName.isEmpty())
    {
        ui->label->open(fileName);
        cur_file = fileName;
    }
}

void MainWindow::on_action_Save_triggered()
{
    if( cur_file.isEmpty() )
        on_actionSave_as_triggered();
    else{
        ui->label->save(cur_file);
    }
}

void MainWindow::on_action_New_triggered()
{
    QMessageBox::StandardButton b = QMessageBox::information(this,"Сохранение","Хотите ли вы сохранить текущий файл?", QMessageBox::Yes, QMessageBox::No);
    if(b == QMessageBox::Yes)
    {
        on_action_Save_triggered();
        ui->label->clear();
        cur_file.clear();
    }
    else if(b == QMessageBox::No)
    {
        ui->label->clear();
        cur_file.clear();
    }
}
