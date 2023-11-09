#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene();
    int h = ui->graphicsView->height();
    int w = ui->graphicsView->width();
    scene->setSceneRect(-w / 2, -h / 2, w, h);
    ui->graphicsView->setScene(scene);
    call task;
    task.act = INIT;
    task_manager(task);
}

MainWindow::~MainWindow()
{
    call task;
    task.act = QUIT;
    task_manager(task);
    delete ui;
}

void MainWindow::draw()
{
    call task;
    task.act = DRAW;
    struct draw dr;
    dr.QS = ui->graphicsView->scene();
    dr.h = ui->graphicsView->height();
    dr.w = ui->graphicsView->width();
    task.dr = dr;
    int rc = show_message(task_manager(task));
    if (!rc)
        ui->graphicsView->repaint();
}

void MainWindow::on_pushButton_load_clicked()
{
    call task;
    task.act = LOAD_FILE;
    struct load_file lf;
    lf.file_name = ui->lineEdit_file->text().toStdString().c_str();
    task.lf = lf;
    int rc = show_message(task_manager(task));
    if (!rc)
        draw();
}

void MainWindow::on_pushButton_unload_clicked()
{
    ui->graphicsView->scene()->clear();
    ui->graphicsView->repaint();
}

void MainWindow::on_button_move_clicked()
{
    call task;
    task.act = MOVE;
    struct move mo;
    bool dx_ok = 1, dy_ok = 1, dz_ok = 1;
    mo.dx = ui->lineEdit_dx->text().toFloat(&dx_ok);
    mo.dy = ui->lineEdit_dy->text().toFloat(&dy_ok);
    mo.dz = ui->lineEdit_dz->text().toFloat(&dz_ok);
    if (!dx_ok || !dy_ok || !dz_ok)
    {
        show_message(INPUT_ERR);
        return;
    }
    task.mo = mo;
    int rc = show_message(task_manager(task));
    if (!rc)
        draw();
}

void MainWindow::on_button_scale_clicked()
{
    call task;
    task.act = SCALE;
    struct scale sc;
    bool kx_ok = 1, ky_ok = 1, kz_ok = 1;
    sc.kx = ui->lineEdit_KX->text().toFloat(&kx_ok);
    sc.ky = ui->lineEdit_KY->text().toFloat(&ky_ok);
    sc.kz = ui->lineEdit_KZ->text().toFloat(&kz_ok);

    if (!kx_ok || !ky_ok || !kz_ok)
    {
        show_message(INPUT_ERR);
        return;
    }

    task.sc = sc;
    int rc = show_message(task_manager(task));
    if (!rc)
        draw();
}

void MainWindow::on_button_rotate_clicked()
{
    call task;
    task.act = ROTATE;
    struct rotate ro;
    bool dgx_ok = 1, dgy_ok = 1, dgz_ok = 1;
    ro.dgx = ui->lineEdit_dgX->text().toInt(&dgx_ok);
    ro.dgy = ui->lineEdit_dgY->text().toInt(&dgy_ok);
    ro.dgz = ui->lineEdit_dgZ->text().toInt(&dgz_ok);

    if (!dgx_ok || !dgy_ok || !dgz_ok)
    {
        show_message(INPUT_ERR);
        return;
    }

    task.ro = ro;
    int rc = show_message(task_manager(task));
    if (!rc)
        draw();
}

int MainWindow::show_message(const int &rc)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    switch(rc)
    {
    case OK:
        break;
    case WRONG_FILE:
        msgBox.setText("Не удалось найти/открыть файл.");
        break;
    case FILE_ERR:
        msgBox.setText("Файл заполнен неверно.");
        break;
    case ALLOC_ERR:
        msgBox.setText("Возникла ошибка при выделении памяти.");
        break;
    case NODES_ERR:
        msgBox.setText("Кол-во точек должно быть натуральным числом.");
        break;
    case LINKS_ERR:
        msgBox.setText("Кол-во ребер должно быть неотрицательным числом.");
        break;
    case INPUT_ERR:
        msgBox.setText("Введённые переменные должны быть целого или вещественного типа.");
        break;
    case ARR_NULL:
        msgBox.setText("Массивы вершин и ребер не инициализированы.");
        break;
    case WRONG_LINKS:
        msgBox.setText("Файл заполнен неверно: ребра соединяют несуществующие вершины.");
        break;
    default:
        msgBox.setText("Неизвестная ошибка.");
        break;
    }
    if (rc)
        msgBox.exec();
    return rc;
}
