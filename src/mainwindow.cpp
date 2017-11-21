#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    //adding faces to QLISTWIDGET
    connect(ui->mygl, SIGNAL(addFace(Face*)), this, SLOT(addFace(Face*)));
    connect(ui->mygl, SIGNAL(addHE(HalfEdge*)), this, SLOT(addHE(HalfEdge*)));
    connect(ui->mygl, SIGNAL(addVertex(Vertex*)), this, SLOT(addVertex(Vertex*)));

    //select component
    connect(ui->Face_list, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(clickFace(QListWidgetItem*)));
    connect(ui->HalfEdge_list, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(clickHalfEdge(QListWidgetItem*)));
    connect(ui->Vertex_list, SIGNAL(itemClicked(QListWidgetItem*)), ui->mygl, SLOT(clickVertex(QListWidgetItem*)));

    //change face color
    connect(ui->face_color_r, SIGNAL(valueChanged(double)), ui->mygl, SLOT(valueChangedR(double)));
    connect(ui->face_color_g, SIGNAL(valueChanged(double)), ui->mygl, SLOT(valueChangedG(double)));
    connect(ui->face_color_b, SIGNAL(valueChanged(double)), ui->mygl, SLOT(valueChangedB(double)));

    //change vertex position
    connect(ui->vertex_x, SIGNAL(valueChanged(double)), ui->mygl, SLOT(valueChangedX(double)));
    connect(ui->vertex_y, SIGNAL(valueChanged(double)), ui->mygl, SLOT(valueChangedY(double)));
    connect(ui->vertex_z, SIGNAL(valueChanged(double)), ui->mygl, SLOT(valueChangedZ(double)));

    //midpoint adding
    connect(ui->Addmidpoint, SIGNAL(pressed()), ui->mygl, SLOT(pressedMidpoint()));

    //triangulate
    connect(ui->triangulate, SIGNAL(pressed()), ui->mygl, SLOT(pressedTriangulate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::addFace(Face* e)
{
    ui->Face_list->addItem(e);
}

void MainWindow::addHE(HalfEdge* e)
{
    ui->HalfEdge_list->addItem(e);
}

void MainWindow::addVertex(Vertex* e)
{
    ui->Vertex_list->addItem(e);
}
