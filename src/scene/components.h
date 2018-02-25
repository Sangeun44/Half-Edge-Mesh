#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "drawable.h"
#include "joint.h"
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
    void setColor(const glm::vec3 color);
    void setID();
    static void resetID();

    HalfEdge* getPointTo() const;
    glm::vec3 getColor() const;
    int getID() const;

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
    static void resetID();

    HalfEdge* getNext() const;
    HalfEdge* getSym() const;
    Face* getIn() const;
    Vertex* getVert() const;
    int getID() const;

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
    void setPosition(const glm::vec3 pos);
    void setJoints(std::vector<Joint*> joint);
    void setID();

    static void resetID();

    HalfEdge* getPointTo() const;
    glm::vec3 getPosition() const;
    int getID() const;

    void calculateWeight();
    std::vector<float> getWeight()const;
    std::vector<Joint*> getJoints() const;

    static int id;


private:
    glm::vec3 pos;
    HalfEdge* pointTo;
    int thisID;

    std::vector<Joint*> joints;
    float weight1;
    float weight2;

};


#endif // COMPONENTS_H
