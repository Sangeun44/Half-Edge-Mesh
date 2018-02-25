#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    //obj pressed clear lists
    connect(ui->mygl, SIGNAL(sendOBJ(bool)), this, SLOT(loadOBJclicked(bool)));

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

    //set initial rgb
    connect(ui->mygl, SIGNAL(initialR(double)), this, SLOT(updateColorR(double)));
    connect(ui->mygl, SIGNAL(initialG(double)), this, SLOT(updateColorG(double)));
    connect(ui->mygl, SIGNAL(initialB(double)), this, SLOT(updateColorB(double)));

    //set initial vertex
    connect(ui->mygl, SIGNAL(initialX(double)), this, SLOT(updateX(double)));
    connect(ui->mygl, SIGNAL(initialY(double)), this, SLOT(updateY(double)));
    connect(ui->mygl, SIGNAL(initialZ(double)), this, SLOT(updateZ(double)));

    //midpoint adding
    connect(ui->Addmidpoint, SIGNAL(pressed()), ui->mygl, SLOT(pressedMidpoint()));

    //triangulate
    connect(ui->triangulate, SIGNAL(pressed()), ui->mygl, SLOT(pressedTriangulate()));

    //Catmull-Clark Subdivision
    connect(ui->subdivide, SIGNAL(pressed()), ui->mygl, SLOT(pressedSubdivide()));

    //Extrude
    connect(ui->extrude, SIGNAL(pressed()), ui->mygl, SLOT(pressedExtrude()));

    //load obj
    connect(ui->loadobj, SIGNAL(pressed()), ui->mygl, SLOT(pressedOBJ()));

    //load JSON
    connect(ui->json, SIGNAL(pressed()), ui->mygl, SLOT(pressedJson()));

    //adding joints to QTreeWidget
    connect(ui->mygl, SIGNAL(addJoint(Joint*)), this, SLOT(addJoint(Joint*)));

    //select joint
    connect(ui->joint_list, SIGNAL(itemClicked(QTreeWidgetItem*, int)), ui->mygl, SLOT(clickJoint(QTreeWidgetItem*, int)));

    //change joint rotation xyz value displayed on the GUI
    connect(ui->mygl, SIGNAL(sendRotateXCurr(int)), this, SLOT(updateXrot(int)));
    connect(ui->mygl, SIGNAL(sendRotateYCurr(int)), this, SLOT(updateYrot(int)));
    connect(ui->mygl, SIGNAL(sendRotateZCurr(int)), this, SLOT(updateZrot(int)));

    //change joint rotation
    connect(ui->rotatex, SIGNAL(pressed()), ui->mygl, SLOT(rotateX()));
    connect(ui->rotatey, SIGNAL(pressed()), ui->mygl, SLOT(rotateY()));
    connect(ui->rotatez, SIGNAL(pressed()), ui->mygl, SLOT(rotateZ()));

    //skin mesh button
    connect(ui->skinMesh, SIGNAL(pressed()), ui->mygl, SLOT(skin()));

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

//OBJ clicked
void MainWindow::loadOBJclicked(bool)
{
    ui->Vertex_list->clear();
    ui->HalfEdge_list->clear();
    ui->Face_list->clear();
}

//add face to Qlistwidget
void MainWindow::addFace(Face* e)
{
    ui->Face_list->addItem(e);
}

//add HE to Qlistwidget
void MainWindow::addHE(HalfEdge* e)
{
    ui->HalfEdge_list->addItem(e);
}

//add vert to Qlistwidget
void MainWindow::addVertex(Vertex* e)
{
    ui->Vertex_list->addItem(e);
}

//add HE to QTreeList
void MainWindow::addJoint(Joint* e)
{
    ui->joint_list->addTopLevelItem(e);
}

//update color spinboxes
void MainWindow::updateColorR(double r) {
    ui->face_color_r->setValue(r);
}

void MainWindow::updateColorG(double g) {
    ui->face_color_g->setValue(g);
}

void MainWindow::updateColorB(double b) {
    ui->face_color_b->setValue(b);
}

//update spinboxes xyz
void MainWindow::updateX(double x) {
    ui->vertex_x->setValue(x);
}
void MainWindow::updateY(double y) {
    ui->vertex_y->setValue(y);
}
void MainWindow::updateZ(double z) {
    ui->vertex_z->setValue(z);
}

//update rotation xyz
void MainWindow::updateXrot(int x) {
    ui->currRotateX->setValue(x);
}

void MainWindow::updateYrot(int y) {
    ui->currRotateY->setValue(y);
}

void MainWindow::updateZrot(int z) {
    ui->currRotateZ->setValue(z);
}
