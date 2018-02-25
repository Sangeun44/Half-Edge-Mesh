#include "joint.h"

int Joint::id = -1;

Joint::Joint(GLWidget277 *context): Drawable(context), thisID(), name(), parent(nullptr), children(std::vector<Joint*>()), position(), rotation(), joint_bindMatrix(), selected(false)
{
}

GLenum Joint::drawMode()
{
    return GL_LINES;
}

void Joint::setSelected(bool select) {
    selected = select;
}

int Joint::getID() const
{
    return this->thisID;
}

bool Joint::getSelected() const {
    return selected;
}

void Joint::setName(const QString name)
{
    setText(0, name);
    this->name = name;
}

void Joint::setPosition(const glm::vec3 pos)
{
    this->position = pos;
}

void Joint::setID()
{
    thisID = ++id;
}

void Joint::setParent(Joint* parent) {
    this->parent = parent;
}

void Joint::setChildren(Joint* child) {
    children.push_back(child);
    QTreeWidgetItem::addChild(child);
}

void Joint::setRotation(const glm::quat rotate) {
    this->rotation = rotate;
}

void Joint::setRotation(const glm::vec4 rot) {
    glm::quat quat_rot = glm::angleAxis(rot.x, glm::vec3(rot.y,rot.z,rot.w));
    this->rotation = quat_rot;
}

QString Joint::getName() const {
    return name;
}

glm::vec3 Joint::getPosition() const {
    return position;
}

Joint* Joint::getParent() const {
    return parent;
}

std::vector<Joint*> Joint::getChildren() const {
    return children;
}

glm::quat Joint::getRotation() const {
    return rotation;
}

glm::mat4 Joint::getJoint_BindMatrix() const {
    return joint_bindMatrix;
}

glm::mat4 Joint::getLocalTrans() const {
    glm::vec4 col1 = glm::vec4(1,0,0,0);
    glm::vec4 col2 = glm::vec4(0,1,0,0);
    glm::vec4 col3 = glm::vec4(0,0,1,0);
    glm::vec4 pos_col = glm::vec4(position.x, position.y, position.z, 1);
    glm::mat4 translation_mat = glm::mat4(col1, col2, col3, pos_col);

    glm::mat4 rotation_mat = glm::mat4_cast(rotation);

    glm::mat4 localTransMat = translation_mat * rotation_mat;

    return localTransMat;
}

glm::mat4 Joint::traverse(Joint* joint, glm::mat4 overall){
    if(joint->getParent() != nullptr) {
        Joint* parent = joint->getParent();
        overall = parent->getOverallTrans() * overall;
    }
    return overall;
}

glm::mat4 Joint::getOverallTrans() {
    glm::mat4 curr = this->getLocalTrans();
    return traverse(this, curr);
}

void Joint::setJoint_BindMatrix() {
    glm::mat4 overall = this->getOverallTrans();
    glm::mat4 bind = glm::inverse(overall);
    this->joint_bindMatrix = bind;
}

void Joint::create()
{
    //all vbo data
    std::vector<glm::vec3> col = std::vector<glm::vec3>();
    std::vector<glm::vec4> pos = std::vector<glm::vec4>();
    std::vector<GLuint> idx = std::vector<GLuint>();

    //joint connections
    glm::vec3 yellow = glm::vec3(1,1,0);
    glm::vec3 red = glm::vec3(1,0,0);

    //joint circles
    glm::vec3 red_wheel = glm::vec3(1,0,0);
    glm::vec3 blue_wheel = glm::vec3(0,0,1);
    glm::vec3 green_wheel = glm::vec3(0,1,0);

    //if this joint is selected
    if(this->getSelected() == true) {
        red_wheel = glm::vec3(1,1,1);
        blue_wheel = glm::vec3(1,1,1);
        green_wheel = glm::vec3(1,1,1);
    }

    int index = 0;

    const float divisions = 20;
    float angle = 360/divisions;

    int numVerts = 0;
    glm::vec4 origin = glm::vec4(0,0,0,1);
    glm::vec4 curr_pos = this->getOverallTrans() * origin;
    glm::vec4 x = glm::vec4(origin.x, origin.y, origin.z + 0.5, origin.w);
    do {
        pos.push_back(this->getOverallTrans() * x);
        col.push_back(red_wheel);
        x = glm::rotateX(x, glm::radians(angle));
        ++numVerts;
    } while(numVerts != divisions);

    do {
        idx.push_back(index);
        idx.push_back((++index) % numVerts);
    } while(index < numVerts);


    numVerts = 0;
    glm::vec4 y = glm::vec4(origin.x, origin.y, origin.z + 0.5, origin.w);
    do {
        pos.push_back(this->getOverallTrans() * y);
        col.push_back(blue_wheel);
        y = glm::rotateY(y, glm::radians(angle));
        ++numVerts;
    } while(numVerts != divisions);

    do {
        idx.push_back(index);
        idx.push_back((++index) % numVerts + divisions);
    } while(index < numVerts + divisions);



    numVerts = 0;
    glm::vec4 z = glm::vec4(origin.x + 0.5, origin.y, origin.z, origin.w);
    do {
        pos.push_back(this->getOverallTrans() * z);
        col.push_back(green_wheel);
        z = glm::rotateZ(z, glm::radians(angle));
        ++numVerts;
    } while(numVerts != divisions);

    do {
        idx.push_back(index);
        idx.push_back((++index) % numVerts + divisions + divisions);
    } while(index < numVerts + divisions + divisions);


    //create lines between joints. if this joint has a child
    if(this->getChildren().size() > 0) {
        std::vector<Joint*> curr_children = this->getChildren();

        for(Joint* child : curr_children) {
            glm::vec4 vertRed_pos = curr_pos;
            glm::vec4 child_pos = glm::vec4(child->getOverallTrans() * origin);
            glm::vec4 vertPointYellow_pos = child_pos;

            pos.push_back(vertPointYellow_pos);
            col.push_back(yellow);
            idx.push_back(index);

            pos.push_back(vertRed_pos);
            col.push_back(red);
            idx.push_back(++index);
        }
    }

    count = idx.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);
}


