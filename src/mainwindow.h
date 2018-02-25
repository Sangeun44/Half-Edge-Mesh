#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <scene/components.h>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

private slots:
    void on_actionQuit_triggered();
    void on_actionCamera_Controls_triggered();

    //adding to the qtreelistwidget for mainwindow ui from gl
    void addFace(Face* e);
    void addVertex(Vertex* e);
    void addHE(HalfEdge* e);

    //add joints when json is clicked
    void addJoint(Joint* j);

   //update spinboxes slots for mainwindow ui from gl
    void updateColorR(double r);
    void updateColorB(double g);
    void updateColorG(double b);

    void updateX(double x);
    void updateY(double y);
    void updateZ(double z);

    //update rotation xyz
    void updateXrot(int x);
    void updateYrot(int y);
    void updateZrot(int z);

    void loadOBJclicked(bool);

private:
    Ui::MainWindow *ui;


};


#endif // MAINWINDOW_H
