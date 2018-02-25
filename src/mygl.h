#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include <scene/components.h>
#include <scene/drawcomponents.h>
#include <scene/mesh.h>
#include "camera.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:
    static int x;
    static int y;
    static int z;

    static bool skinned;

    //Cylinder m_geomCylinder;// The instance of a unit cylinder we can use to render any cylinder
    //Sphere m_geomSphere;// The instance of a unit sphere we can use to render any sphere
    Mesh m_mesh;
    std::vector<Joint*> skeleton;
    DrawVert m_vert;
    DrawHE m_edge;
    DrawFace m_face;
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_prog_skeleton; //A shader program for skeleton deformation

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

    Vertex* current_v;
    HalfEdge* current_he;
    Face* current_f;
    Joint* current_joint;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    //for splitting edges to smooth edges
    std::vector<Vertex*> splitSmoothed(std::vector<HalfEdge*> pairs, std::map<Face*, Vertex*> map);

    //quad
    void quadrangulate(std::vector<HalfEdge*> HalfE, Vertex* centroid);

    //traverse for the json file
    void traverse(QJsonObject joint, Joint* parent);

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    //add mesh comp to the lists
    void addFace(Face*);
    void addVertex(Vertex*);
    void addHE(HalfEdge*);

    //add joints to the treewidget
    void addJoint(Joint*);

    //add newly changed rgb
    void addR(double e);
    void addG(double e);
    void addB(double e);

    //add initial clicked face rgb
    void initialR(double e);
    void initialG(double e);
    void initialB(double e);

    //add initial clicked vertex
    void initialX(double e);
    void initialY(double e);
    void initialZ(double e);

    //add obj
    void sendOBJ(bool);

    //send the changed value
    void sendRotateXCurr(int e);
    void sendRotateYCurr(int e);
    void sendRotateZCurr(int e);

private slots:
    //for receiving clicks from the mainwindow ui
    void clickFace(QListWidgetItem* e);
    void clickHalfEdge(QListWidgetItem* e);
    void clickVertex(QListWidgetItem* e);

    void clickJoint(QTreeWidgetItem* e, int col);

    //for receiving value changes from the mainwindow ui
    void valueChangedR(double e);
    void valueChangedG(double e);
    void valueChangedB(double e);

    //for receiving value changes from the mainwindow ui
    void valueChangedX(double e);
    void valueChangedY(double e);
    void valueChangedZ(double e);

    //when rotate button is pressed on the ui
    void rotateX();
    void rotateY();
    void rotateZ();

    //pressed midpoint from the mainwindow ui
    void pressedMidpoint();

    //pressed triangulate from the mainwindow ui
    void pressedTriangulate();

    //pressed subdivide from the mainwindow ui
    void pressedSubdivide();

    //pressed extrude face from the mainwindow ui
    void pressedExtrude();

    //pressed load obj from the mainwindow ui
    void pressedOBJ();

    //pressed load obj from the mainwindow ui
    void pressedJson();

    //pressed skin Mesh on ui
    void skin();

};


#endif // MYGL_H
