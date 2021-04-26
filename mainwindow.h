#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void setLine();
    void setRectangle();
    void setEllipse();
    void setTriangle();

    void setPenColor();
    void setBrushColor();

    void makeGroup();
    void makeUnGroup();

    void deletingSelected();
private slots:
    void on_actionSave_as_triggered();

    void on_action_Open_triggered();

    void on_action_Save_triggered();

    void on_action_New_triggered();

private:
    QString cur_file;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
