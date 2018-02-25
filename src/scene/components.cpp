#include "components.h"

int Face::id = -1;

Face::Face() : pointTo(), color(), thisID()
{
}

void Face::setPointTo(HalfEdge* pointTo)
{
    this->pointTo = pointTo;
}

void Face::setColor(const glm::vec3 color)
{
    this->color = color;
}

void Face::setID()
{
    thisID = ++id;
    setText(QString::number(thisID));
}

void Face::resetID()
{
    id = -1;
}


glm::vec3 Face::getColor() const
{
    return this->color;
}

HalfEdge* Face::getPointTo() const
{
    return this->pointTo;
}

int Face::getID() const
{
    return this->thisID;
}

int Vertex::id = -1;
Vertex::Vertex() : pos(), pointTo(), thisID(), joints(), weight1(), weight2()
{
}

void Vertex::setJoints(std::vector<Joint*> joints) {
    float min_dist = 100;
    Joint* closest;
    for(Joint* joint : joints) {
        glm::vec3 joint_pos = joint->getPosition();
        float dist = glm::distance(joint_pos,this->pos);
        if(min_dist > dist) {
            min_dist = dist;
            closest = joint;
            joints.push_back(closest);
        }
    }
    Joint* second_closest;
    float min_dist_2 = 100;
    for(Joint* joint : joints) {
        if(joint != closest) {
            glm::vec3 joint_pos = joint->getPosition();
            float dist = glm::distance(joint_pos,this->pos);
            if(min_dist_2 > dist) {
                min_dist_2 = dist;
                second_closest = joint;
                joints.push_back(second_closest);
            }
        }
    }
}

void Vertex::calculateWeight() {
    Joint* joint1 = joints.at(0);
    Joint* joint2 = joints.at(1);
    glm::vec3 joint_pos1 = joint1->getPosition();
    glm::vec3 joint_pos2 = joint2->getPosition();
    float dist1 = glm::distance(joint_pos1, this->getPosition());
    float dist2 = glm::distance(joint_pos2, this->getPosition());
    weight1 = 1 - (dist1/(dist1+dist2));
    weight2 = 1 - (dist2/(dist1+dist2));
}

std::vector<Joint*> Vertex::getJoints() const {
    return joints;
}

std::vector<float> Vertex::getWeight() const {
    std::vector<float> weights = std::vector<float>();
    weights.push_back(weight1);
    weights.push_back(weight2);
    return weights;
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

void Vertex::resetID()
{
    id = -1;
}

void Vertex::setPosition(const glm::vec3 pos)
{
    this->pos = pos;
}

int Vertex::getID() const
{
    return this->thisID;
}

HalfEdge* Vertex::getPointTo() const
{
    return this->pointTo;
}

glm::vec3 Vertex::getPosition() const
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
void HalfEdge::resetID()
{
    id = -1;
}

HalfEdge* HalfEdge::getNext() const
{
    return this->next;
}

HalfEdge* HalfEdge::getSym() const
{
    return this->sym;
}

Face* HalfEdge::getIn() const
{
    return this->in;
}

Vertex* HalfEdge::getVert() const
{
    return this->vertex;
}

int HalfEdge::getID() const
{
    return this->thisID;
}

