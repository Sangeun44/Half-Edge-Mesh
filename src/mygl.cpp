#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      m_geomCylinder(this), m_geomSphere(this), m_mesh(this),
      m_vert(this), m_edge(this), m_face(this),
      m_progLambert(this), m_progFlat(this),
      m_glCamera(), current_v(nullptr), current_he(nullptr), current_f(nullptr)
{

    setFocusPolicy(Qt::StrongFocus);

}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomCylinder.destroy();
    m_geomSphere.destroy();
    m_mesh.destroy();
    m_face.destroy();
    m_edge.destroy();
    m_vert.destroy();
}

void MyGL::clickVertex(QListWidgetItem* e)
{
    if(e == nullptr){
        return;
    }
    current_v = (Vertex*) e;
    m_vert.setVert(current_v);
    m_vert.destroy();
    m_vert.create();
    update();
}

void MyGL::clickHalfEdge(QListWidgetItem* e)
{
    if(e == nullptr){
        return;
    }
    current_he = (HalfEdge*) e;
    m_edge.setHE(current_he);
    m_edge.destroy();
    m_edge.create();
    update();
}

void MyGL::clickFace(QListWidgetItem* e)
{
    if(e == nullptr){
        return;
    }
    current_f = (Face*) e;
        m_face.setFace(current_f);
        m_face.destroy();
        m_face.create();
    update();
}

void MyGL::valueChangedR(double e)
{
    m_mesh.destroy();
    m_face.create();
    update();
    glm::vec3 current_color = current_f->getColor();
    glm::vec3 new_color = glm::vec3(e, current_color.g, current_color.b);
    current_f->setColor(new_color);
    m_mesh.create();
    update();
}

void MyGL::valueChangedG(double e)
{
    m_mesh.destroy();
    m_face.create();
    update();
    glm::vec3 current_color = current_f->getColor();
    glm::vec3 new_color = glm::vec3(current_color.r, e, current_color.b);
    current_f->setColor(new_color);
    m_mesh.create();
    update();
}

void MyGL::valueChangedB(double e)
{
    m_mesh.destroy();
    m_face.create();
    update();
    glm::vec3 current_color = current_f->getColor();
    glm::vec3 new_color = glm::vec3(current_color.r, current_color.g, e);
    current_f->setColor(new_color);
    m_mesh.create();
    update();
}

void MyGL::valueChangedX(double e)
{
    m_mesh.destroy();
    m_vert.destroy();
    update();
    glm::vec3 current_pos = current_v->getPosition();
    glm::vec3 new_pos = glm::vec3(e, current_pos.y, current_pos.z);
    current_v->setPosition(new_pos);
    m_mesh.create();
    m_vert.create();
    update();
}

void MyGL::valueChangedY(double e)
{
    m_mesh.destroy();
    m_vert.destroy();
    update();
    glm::vec3 current_pos = current_v->getPosition();
    glm::vec3 new_pos = glm::vec3(current_pos.x, e, current_pos.z);
    current_v->setPosition(new_pos);
    m_mesh.create();
    m_vert.create();
    update();
}

void MyGL::valueChangedZ(double e)
{
    m_mesh.destroy();
    m_vert.destroy();
    update();
    glm::vec3 current_pos = current_v->getPosition();
    glm::vec3 new_pos = glm::vec3(current_pos.x, current_pos.y, e);
    current_v->setPosition(new_pos);
    m_mesh.create();
    m_vert.create();
    update();
}

void MyGL::pressedMidpoint()
{
    if(current_he == nullptr){
        return;
    }
    //HE
    HalfEdge* HE1 = current_he;
    HalfEdge* HE2 = current_he->getSym();
    //verts
    Vertex* HE1_VERT = HE1->getVert();
    Vertex* HE2_VERT = HE2->getVert();
    //get new position
    glm::vec3 v1 = HE1_VERT->getPosition();
    glm::vec3 v2 = HE2_VERT->getPosition();
    glm::vec3 dist = (v2 + v1)/2.f;
    glm::vec3 midpoint = dist;

    //create midpoint
    Vertex* v3 = new Vertex();
        v3->setID();
    v3->setPosition(midpoint);
    //create two new half edges
    HalfEdge* HE1B = new HalfEdge();
    HalfEdge* HE2B = new HalfEdge();
        HE1B->setVert(HE1_VERT);
        HE1B->setFace(HE1->getIn());
        HE1B->setSym(HE2);
        HE1B->setID();
        HE1B->setNext(HE1->getNext());
        HE1B->setVert(HE1_VERT);

        HE2B->setVert(HE2_VERT);
        HE2B->setFace(HE2->getIn());
        HE2B->setSym(HE1);
        HE2B->setID();
        HE2B->setNext(HE2->getNext());
        HE2B->setVert(HE2_VERT);

        HE1->setVert(v3);
        HE1->setNext(HE1B);
        HE1->setSym(HE2B);

        HE2->setVert(v3);
        HE2->setNext(HE2B);
        HE2->setSym(HE1B);



    m_mesh.verts.push_back(v3);
    m_mesh.halves.push_back(HE1B);
    m_mesh.halves.push_back(HE2B);
    m_edge.create();

    update();

}

void MyGL::pressedTriangulate()
{

    if(current_f == nullptr)
    {
        return;
    }

    HalfEdge* curr_HE = current_f->getPointTo();

    HalfEdge* HE_A = new HalfEdge();
    HE_A->setID();
    HalfEdge* HE_B = new HalfEdge();
    HE_B->setID();
    Face* FACE2 = new Face();
    FACE2->setID();

    HE_A->setVert(curr_HE->getVert());
    HE_A->setSym(HE_B);
    HE_A->setFace(FACE2);

    HE_B->setVert(curr_HE->getNext()->getNext()->getVert());
    HE_B->setSym(HE_A);
    HE_B->setFace(current_f);

    curr_HE->getNext()->setFace(FACE2);
    curr_HE->getNext()->getNext()->setFace(FACE2);

    FACE2->setPointTo(HE_A);
    FACE2->setColor(current_f->getColor());
    current_f->setPointTo(curr_HE);
    HE_B->setNext(curr_HE->getNext()->getNext()->getNext());
    curr_HE->getNext()->getNext()->setNext(HE_A);
    HE_A->setNext(curr_HE->getNext());
    curr_HE->setNext(HE_B);
    m_mesh.destroy();
    update();

    m_mesh.halves.push_back(HE_A);
    m_mesh.halves.push_back(HE_B);
    m_mesh.faces.push_back(FACE2);

    m_face.create();
    m_mesh.create();
    update();

}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
    m_mesh.createMeshData();
    m_mesh.create();

    for(int i = 0; i < m_mesh.faces.size(); ++i){
        emit addFace(m_mesh.faces.at(i));
    }
    for(int i = 0; i < m_mesh.halves.size(); ++i){
        emit addHE(m_mesh.halves.at(i));
    }
    for(int i = 0; i < m_mesh.verts.size(); ++i){
        emit addVertex(m_mesh.verts.at(i));
    }
    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    m_progLambert.setGeometryColor(glm::vec4(0,1,0,1));


    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
//DO NOT CONSTRUCT YOUR SCENE GRAPH IN THIS FUNCTION!

void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());

    for(int i = 0; i < m_mesh.faces.size(); ++i){
        emit addFace(m_mesh.faces.at(i));
    }
    for(int i = 0; i < m_mesh.halves.size(); ++i){
        emit addHE(m_mesh.halves.at(i));
    }
    for(int i = 0; i < m_mesh.verts.size(); ++i){
        emit addVertex(m_mesh.verts.at(i));
    }
//#define NOPE
#ifndef NOPE
    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
//    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(10.0f), glm::radians(15.0f), glm::vec3(6,6,6));
    //Send the geometry's transformation matrix to the shader
    m_progLambert.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
    m_progLambert.draw(m_mesh);

    glDisable(GL_DEPTH_TEST);
    m_progFlat.setModelMatrix(model);
    m_progFlat.draw(m_vert);
    m_progFlat.draw(m_face);
    m_progFlat.draw(m_edge);
    glEnable(GL_DEPTH_TEST);
#endif

}


void MyGL::keyPressEvent(QKeyEvent *e)
{

    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    } else if (e->key() == Qt::Key_N) {
        if(current_he == nullptr)
        {
            return;
        }
        current_he = current_he->getNext();
        m_edge.setHE(current_he);
        m_edge.create();
    } else if (e->key() == Qt::Key_M) {
        if(current_he == nullptr)
        {
            return;
        }
        current_he = current_he->getSym();
        m_edge.setHE(current_he);
        m_edge.create();
    } else if (e->key() == Qt::Key_F) {
        if(current_he == nullptr)
        {
            return;
        }
        current_f = current_he->getIn();
        m_edge.destroy();
        m_face.setFace(current_f);
        m_face.create();
    } else if (e->key() == Qt::Key_V) {
        if(current_he == nullptr)
        {
            return;
        }
        current_v = current_he->getVert();
        m_edge.destroy();
        m_vert.setVert(current_v);
        m_vert.create();
    } else if (e->key() == Qt::Key_H) {
        if(current_v == nullptr)
        {
            return;
        }
        current_he = current_v->getPointTo();
        m_vert.destroy();
        m_edge.setHE(current_v->getPointTo());
        m_edge.create();
    } else if (e->modifiers() == Qt::ShiftModifier) {
        if(current_f == nullptr)
        {
            return;
        }
        current_he = current_f->getPointTo();
        m_face.destroy();
        m_edge.setHE(current_he);
        m_edge.create();
    }
    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}
