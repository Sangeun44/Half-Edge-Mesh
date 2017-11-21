#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "drawable.h"
#include <la.h>
#include <QListWidgetItem>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class HalfEdge;
class Vertex;

class Face : public QListWidgetItem
{
public:
    Face();
    void setPointTo(HalfEdge* pointTo);
    void setColor(glm::vec3 color);
    void setID();

    HalfEdge* getPointTo();
    glm::vec3 getColor();
    int getID();

    static int id;

private:
    HalfEdge* pointTo;
    glm::vec3 color;
    int thisID;

};

class HalfEdge : public QListWidgetItem
{
public:
    HalfEdge();
    void setNext(HalfEdge* next);
    void setSym(HalfEdge* sym);
    void setFace(Face* in);
    void setVert(Vertex* vertex);
    void setID();

    HalfEdge* getNext();
    HalfEdge* getSym();
    Face* getIn();
    Vertex* getVert();
    int getID();

    static int id;

private:
    HalfEdge* next;
    HalfEdge* sym;
    Face* in;
    Vertex* vertex;
    int thisID;

};

class Vertex : public QListWidgetItem
{
public:
    Vertex();
    void setPointTo(HalfEdge* pointTo);
    void setPosition(glm::vec3 pos);
    void setID();

    HalfEdge* getPointTo();
    glm::vec3 getPosition();
    int getID();

    static int id;


private:
    glm::vec3 pos;
    HalfEdge* pointTo;
    int thisID;
};


#endif // COMPONENTS_H
