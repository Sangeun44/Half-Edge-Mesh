#include "components.h"

int Face::id = -1;

Face::Face() : pointTo(), color(), thisID()
{
}

void Face::setPointTo(HalfEdge* pointTo)
{
    this->pointTo = pointTo;
}

void Face::setColor(glm::vec3 color)
{
    this->color = color;
}

void Face::setID()
{
    thisID = ++id;
    setText(QString::number(thisID));
}

glm::vec3 Face::getColor()
{
    return this->color;
}

HalfEdge* Face::getPointTo()
{
    return this->pointTo;
}

int Face::getID()
{
    return this->thisID;
}

int Vertex::id = -1;
Vertex::Vertex() : pos(), pointTo(), thisID()
{
}

void Vertex::setPointTo(HalfEdge *pointTo)
{
    this->pointTo = pointTo;
}

void Vertex::setID()
{
    this->thisID = ++id;
    setText(QString::number(thisID));
}

void Vertex::setPosition(glm::vec3 pos)
{
    this->pos = pos;
}

int Vertex::getID()
{
    return this->thisID;
}

HalfEdge* Vertex::getPointTo()
{
    return this->pointTo;
}

glm::vec3 Vertex::getPosition()
{
    return this->pos;
}

int HalfEdge::id = -1;
HalfEdge::HalfEdge(): next(), sym(), in(), vertex(), thisID()
{
}

void HalfEdge::setNext(HalfEdge* next)
{
    this->next = next;
}

void HalfEdge::setSym(HalfEdge* sym)
{
    this->sym = sym;
}

void HalfEdge::setFace(Face* in)
{
    this->in = in;
}

void HalfEdge::setVert(Vertex* vertex)
{
    this->vertex = vertex;
}

void HalfEdge::setID()
{
    this->thisID = ++id;
    setText(QString::number(thisID));

}

HalfEdge* HalfEdge::getNext()
{
    return this->next;
}

HalfEdge* HalfEdge::getSym()
{
    return this->sym;
}

Face* HalfEdge::getIn()
{
    return this->in;
}

Vertex* HalfEdge::getVert()
{
    return this->vertex;
}

int HalfEdge::getID()
{
    return this->thisID;
}

