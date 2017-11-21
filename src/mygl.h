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
    Cylinder m_geomCylinder;// The instance of a unit cylinder we can use to render any cylinder
    Sphere m_geomSphere;// The instance of a unit sphere we can use to render any sphere
    Mesh m_mesh;
    DrawVert m_vert;
    DrawEdge m_edge;
    DrawFace m_face;
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

    Vertex* current_v;
    HalfEdge* current_he;
    Face* current_f;


public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void addFace(Face*);
    void addVertex(Vertex*);
    void addHE(HalfEdge*);

    void addR(double e);
    void addG(double e);
    void addB(double e);

private slots:
    void clickFace(QListWidgetItem* e);
    void clickHalfEdge(QListWidgetItem* e);
    void clickVertex(QListWidgetItem* e);

    void valueChangedR(double e);
    void valueChangedG(double e);
    void valueChangedB(double e);

    void valueChangedX(double e);
    void valueChangedY(double e);
    void valueChangedZ(double e);

    void pressedMidpoint();

    void pressedTriangulate();

};


#endif // MYGL_H
