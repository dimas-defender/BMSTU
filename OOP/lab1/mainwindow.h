#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "task_manager.h"
#include "input.h"
#include "actions.h"
#include "object.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_load_clicked();

    void draw();

    void on_pushButton_unload_clicked();

    void on_button_move_clicked();

    void on_button_scale_clicked();

    void on_button_rotate_clicked();

    int show_message(const int &rc);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
