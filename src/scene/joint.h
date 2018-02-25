#ifndef JOINT_H
#define JOINT_H

#include "drawable.h"
#include <QListWidgetItem>

#include <la.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QTreeWidgetItem>


class Joint : public QTreeWidgetItem, public Drawable
{
public:
    //constructor
    Joint(GLWidget277* mp_context);
    //drawable
    virtual GLenum drawMode();

    static int id;

    //matrix getter
    glm::mat4 getLocalTrans() const;
    glm::mat4 getOverallTrans();

    //set member variables
    void setID();
    void setName(QString name);
    void setPosition(const glm::vec3 pos);
    void setParent(Joint* parent);
    void setChildren(Joint* child);
    void setRotation(const glm::vec4 rot);
    void setRotation(const glm::quat rotate);
    void setJoint_BindMatrix();
    void setSelected(bool sel);
    glm::mat4 traverse(Joint* joint, glm::mat4 mat);

    //get member variables
    int getID() const;
    QString getName() const;
    glm::vec3 getPosition() const;
    Joint* getParent() const;
    std::vector<Joint*> getChildren() const;
    glm::quat getRotation() const;
    glm::mat4 getJoint_BindMatrix() const;
    bool getSelected() const;

    void create();

private:
    QString name;
    Joint* parent;
    std::vector<Joint*> children;
    glm::vec3 position;
    glm::quat rotation;
    glm::mat4 joint_bindMatrix;
    int thisID;
    bool selected;

};

#endif // JOINT_H
