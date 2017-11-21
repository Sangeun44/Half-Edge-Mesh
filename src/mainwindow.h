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

    void addFace(Face* e);
    void addVertex(Vertex* e);
    void addHE(HalfEdge* e);

private:
    Ui::MainWindow *ui;


};


#endif // MAINWINDOW_H
