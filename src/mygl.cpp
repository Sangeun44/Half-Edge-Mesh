#include "mygl.h"
#include <la.h>
#include <gl.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <iterator>

int MyGL::x = 0;
int MyGL::y = 0;
int MyGL::z = 0;

bool MyGL::skinned = false;

MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      //m_geomCylinder(this), m_geomSphere(this),
      skeleton(), m_mesh(this), m_vert(this), m_edge(this), m_face(this),
      m_progLambert(this), m_progFlat(this), m_prog_skeleton(this),
      m_glCamera(), current_v(nullptr), current_he(nullptr), current_f(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    //m_geomCylinder.destroy();
    //m_geomSphere.destroy();
    m_mesh.destroy();
    m_face.destroy();
    m_edge.destroy();
    m_vert.destroy();
}

//clicked Joint
void MyGL::clickJoint(QTreeWidgetItem* e , int col)
{
    if(e == nullptr){
        return;
    }
    if(current_joint != nullptr) {
        current_joint->setSelected(false);
        current_joint->destroy();
        current_joint->create();
    }

    current_joint = (Joint*) e;
    current_joint->setSelected(true);
    current_joint->destroy();
    current_joint->create();
    update();
}

//clicked vertex
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

//clicked half edge
void MyGL::clickHalfEdge(QListWidgetItem* e)
{
    if(e == nullptr){
        return;
    }
    current_he = (HalfEdge*) e;
    m_edge.setHE(current_he);
    m_edge.destroy();
    m_edge.create();
    if(current_v != nullptr){
        m_vert.create();
    }
    update();
}

//clicked face
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

//changed spinbox r
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

//changed spinbox g
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

//changed spinbox b
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

//changed spinbox x
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
    if(current_f != nullptr) {
        m_face.create();
    }
    if(current_he != nullptr) {
        m_edge.create();
    }
    update();
}

//changed spinbox y
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
    if(current_f != nullptr) {
        m_face.create();
    }
    if(current_he != nullptr) {
        m_edge.create();
    }
    update();
}

//changed spinbox z
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
    if(current_f != nullptr) {
        m_face.create();
    }
    if(current_he != nullptr) {
        m_edge.create();
    }
    update();
}

void MyGL::rotateX() {
    if(current_joint == nullptr) {
        return;
    }
    ++x;
    glm::quat quat_rot = current_joint->getRotation();
    quat_rot = glm::rotate(quat_rot, 5.f, glm::vec3(1,0,0));
    current_joint->setRotation(quat_rot);
    int change = x * 5;
    emit sendRotateXCurr(change);
    update();
}

void MyGL::rotateY() {
    if(current_joint == nullptr) {
        return;
    }
    ++y;
    glm::quat quat_rot = current_joint->getRotation();
    quat_rot = glm::rotate(quat_rot, 5.f, glm::vec3(0,1,0));
    current_joint->setRotation(quat_rot);
    emit sendRotateYCurr(y * 5);
    update();
}

void MyGL::rotateZ() {
    if(current_joint == nullptr) {
        return;
    }
    ++z;
    glm::quat quat_rot = current_joint->getRotation();
    quat_rot = glm::rotate(quat_rot, 5.f, glm::vec3(0,0,1));
    current_joint->setRotation(quat_rot);
    emit sendRotateZCurr(z * 5);
    update();
}

void MyGL::skin() {
    skinned = true;
    if(skinned == true) {
        m_mesh.setJointLoaded(true);
    }
    if(skeleton.size() != 0) {
        update();
    }
}

//creates midpoint on half edge
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

//when pressed, will triangulate a face
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

//splits the half edges on the polygon mesh by calculating the smoothed position of the midpoint using
//using centroids and vertices
std::vector<Vertex*> MyGL::splitSmoothed(std::vector<HalfEdge*> pairs, std::map<Face*, Vertex*> centroids)
{
    //for midpoints
    std::vector<Vertex*> smooth_mid = std::vector<Vertex*>();

    for(HalfEdge* HE : pairs) {
        HalfEdge* HE1 = HE;
        HalfEdge* HE2 = HE->getSym();

        Vertex* HE1_VERT = HE1->getVert();
        Vertex* HE2_VERT = HE2->getVert();

        Face* f1 = HE1->getIn();
        Face* f2 = HE2->getIn();

        Vertex* cen1 = centroids[f1];
        Vertex* cen2 = centroids[f2];

        glm::vec3 mid_pos = (HE1_VERT->getPosition() + HE2_VERT->getPosition() + cen1->getPosition() + cen2->getPosition()) / glm::vec3(4);

        Vertex* mid_point = new Vertex();
        mid_point->setID();
        mid_point->setPosition(mid_pos);

        //create two new half edges
        HalfEdge* HE1B = new HalfEdge();
        HalfEdge* HE2B = new HalfEdge();
        HE1B->setVert(HE1_VERT);
        HE1_VERT->setPointTo(HE1B);
        HE1B->setFace(HE1->getIn());
        HE1B->setSym(HE2);
        HE1B->setID();
        HE1B->setNext(HE1->getNext());
        HE1B->setVert(HE1_VERT);

        HE2B->setVert(HE2_VERT);
        HE2_VERT->setPointTo(HE2B);
        HE2B->setFace(HE2->getIn());
        HE2B->setSym(HE1);
        HE2B->setID();
        HE2B->setNext(HE2->getNext());
        HE2B->setVert(HE2_VERT);

        HE1->setVert(mid_point);
        mid_point->setPointTo(HE1);
        HE1->setNext(HE1B);
        HE1->setSym(HE2B);

        HE2->setVert(mid_point);
        HE2->setNext(HE2B);
        HE2->setSym(HE1B);

        smooth_mid.push_back(mid_point);
        m_mesh.verts.push_back(mid_point);
        m_mesh.halves.push_back(HE1B);
        m_mesh.halves.push_back(HE2B);
    }
    return smooth_mid;
}

//creates quadrangles on the original faces
void MyGL::quadrangulate(std::vector<HalfEdge*> HalfE, Vertex* centroid) {

    std::map<HalfEdge*, std::pair<Vertex*, Vertex*>> sym_pairs =  std::map<HalfEdge*, std::pair<Vertex*, Vertex*>>();
    std::vector<HalfEdge*> HE_needs_sym = std::vector<HalfEdge*>();

    for(HalfEdge* HE: HalfE) {
        HalfEdge* HE1 = HE;
        HalfEdge* HE2 = HE->getNext();
        HalfEdge* HE3 = new HalfEdge();
        HE3->setID();
        HalfEdge* HE4 = new HalfEdge();
        HE4->setID();
        Vertex* other_mid = HE1->getSym()->getVert();

        HE1->setNext(HE2);
        HE2->setNext(HE3);
        HE3->setVert(centroid);
        centroid->setPointTo(HE3);
        HE3->setNext(HE4);
        HE4->setVert(other_mid);
        HE4->setNext(HE1);

        HE_needs_sym.push_back(HE3);
        HE_needs_sym.push_back(HE4);

        std::pair<Vertex*, Vertex*> vert_pair1 = std::pair<Vertex*, Vertex*>();
        vert_pair1.first = HE2->getVert();
        vert_pair1.second = centroid;
        sym_pairs[HE3] = vert_pair1;

        std::pair<Vertex*, Vertex*> vert_pair2 = std::pair<Vertex*, Vertex*>();
        vert_pair2.first = centroid;
        vert_pair2.second = other_mid;
        sym_pairs[HE4] = vert_pair2;

        float r1 = ((double) rand() / (RAND_MAX));
        float r2 = ((double) rand() / (RAND_MAX));
        float r3 = ((double) rand() / (RAND_MAX));

        Face* new_f = new Face();
        new_f->setPointTo(HE1);
        new_f->setID();
        new_f->setColor(glm::vec3(r1,r2,r3));
        m_mesh.faces.push_back(new_f);

        HE1->setFace(new_f);
        HE2->setFace(new_f);
        HE3->setFace(new_f);
        HE4->setFace(new_f);

        m_mesh.halves.push_back(HE3);
        m_mesh.halves.push_back(HE4);
    }


    for(HalfEdge* EH1 : HE_needs_sym) {
        std::pair<Vertex*, Vertex*> pair1 = sym_pairs[EH1];
        for(HalfEdge* EH2 : HE_needs_sym) {
            if(EH1 != EH2 && EH1->getSym() == nullptr) {
                std::pair<Vertex*, Vertex*> pair2 = sym_pairs[EH2];
                if(pair1.first == pair2.first && pair1.second == pair2.second) {
                    EH1->setSym(EH2);
                    EH2->setSym(EH1);
                }
                else if(pair1.second == pair2.first && pair1.first == pair2.second) {
                    EH1->setSym(EH2);
                    EH2->setSym(EH1);
                }
                else if(pair2.second == pair1.first && pair2.first == pair1.second) {
                    EH1->setSym(EH2);
                    EH2->setSym(EH1);
                }
            }
        }
    }
}

//will subdivide the whole polygon
void MyGL::pressedSubdivide() {
    m_edge.destroy();
    m_face.destroy();
    m_vert.destroy();
    m_mesh.destroy();
    update();

    std::map<Face*, Vertex*> centroids = std::map<Face*,Vertex*>();
    std::vector<Vertex*> midpoints = std::vector<Vertex*>();
    std::vector<HalfEdge*> HE_pairs = std::vector<HalfEdge*>();

    std::vector<Vertex*> original_Verts = std::vector<Vertex*>();
    original_Verts = m_mesh.verts;
    std::vector<Face*> original_Face = std::vector<Face*>();
    original_Face = m_mesh.faces;
    std::vector<HalfEdge*> original_HE = std::vector<HalfEdge*>();
    original_HE = m_mesh.halves;
    std::map<Face*, int> num_sides = std::map<Face*, int>();

    //for centroid finding
    for(Face* face : original_Face) {
        glm::vec3 verts_pos_sum; //sum of all vertices

        int verts = 0;

        HalfEdge* curr = face->getPointTo(); //current halfedge, starting from face pointer
        //get each vert, vert#, add pos, get HE_pairs,
        do{
            std::vector<HalfEdge*>::iterator it;
            it = std::find(HE_pairs.begin(), HE_pairs.end(), curr->getSym());
            if (it == HE_pairs.end()) {
                HE_pairs.push_back(curr);
            }
            verts_pos_sum += curr->getVert()->getPosition();
            curr = curr->getNext();
            verts++;
        } while(curr != face->getPointTo());
        num_sides[face] = verts;

        Vertex* centroid = new Vertex(); //centroid of each face
        centroid->setPosition(verts_pos_sum/glm::vec3(verts));
        centroid->setID();
        m_mesh.verts.push_back(centroid);
        centroids[face] = centroid;
    }

    //create midpoints
    midpoints = this->splitSmoothed(HE_pairs, centroids);

    for(Vertex* vert : original_Verts) {
        std::vector<Vertex*> all_adj_mid = std::vector<Vertex*>();
        std::vector<Face*> all_adj_face = std::vector<Face*>();

        HalfEdge* vert_HE = vert->getPointTo();

        do{
            HalfEdge* HE_next = vert_HE->getNext();
            Vertex* mid_point = HE_next->getVert();
            Face* face = HE_next->getIn();
            all_adj_face.push_back(face);
            all_adj_mid.push_back(mid_point);
            vert_HE = HE_next->getSym();
        } while(vert_HE != vert->getPointTo());

        glm::vec3 sum_E; //sum of adj midpoints
        glm::vec3 sum_F; //sum of adj centroids
        for(int i = 0; i < all_adj_face.size(); ++i) {
            Face* face = all_adj_face.at(i);
            Vertex* cent = centroids[face];
            sum_E += all_adj_mid.at(i)->getPosition();
            sum_F += cent->getPosition();
        }

        glm::vec3 v = vert->getPosition();
        int n = all_adj_mid.size();
        glm::vec3 new_point = ((glm::vec3(n - 2) * v) / glm::vec3(n)) + (sum_E / glm::vec3(n * n)) + (sum_F / glm::vec3(n * n));

        vert->setPosition(new_point);
    }


    m_mesh.faces = std::vector<Face*>();

    for(Face* face : original_Face) {
        std::vector<HalfEdge*> face_HE_pointers_to_original_verts = std::vector<HalfEdge*>();
        HalfEdge* curr_1 = face->getPointTo();
        do {
            Vertex* candidate_vert = curr_1->getVert();
            std::vector<Vertex*>::iterator it;
            it = std::find(original_Verts.begin(), original_Verts.end(), candidate_vert);
            if (it != original_Verts.end()) {
                face_HE_pointers_to_original_verts.push_back(curr_1);
            }
            curr_1 = curr_1->getNext();
        } while(curr_1 != face->getPointTo());

        Vertex* cen = centroids[face];
        quadrangulate(face_HE_pointers_to_original_verts, cen);
    }

    if(current_he != nullptr){
        m_edge.create();
    }

    if(current_v != nullptr){
        m_vert.create();
    }

    if(current_f != nullptr){
        m_face.create();
    }

    m_mesh.create();
    update();

}

//will extrude face when face is selected and button is pressed
void MyGL::pressedExtrude()
{
    if(current_f == nullptr) {
        return;
    }

    //calculate normal vector
    HalfEdge* edge = current_f->getPointTo();
    glm::vec3 A = edge->getVert()->getPosition();
    glm::vec3 B = edge->getNext()->getVert()->getPosition();
    glm::vec3 C = edge->getNext()->getNext()->getVert()->getPosition();
    glm::vec3 BA = A - B;
    glm::vec3 BC = C - B;
    glm::vec3 normal = glm::normalize(glm::cross(BC,BA));

    std::vector<HalfEdge*> columns_HE = std::vector<HalfEdge*>();
    std::vector<Vertex*> new_face_vert = std::vector<Vertex*>();
    std::vector<Vertex*> or_face_vert = std::vector<Vertex*>();
    std::map<HalfEdge*, std::pair<Vertex*, Vertex*>> sym_pairs =  std::map<HalfEdge*, std::pair<Vertex*, Vertex*>>();


    //new vertices
    HalfEdge* curr = current_f->getPointTo();
    do {
        Vertex* curr_v = curr->getVert();
        or_face_vert.push_back(curr_v);
        Vertex* new_v = new Vertex();
        new_v->setID();
        glm::vec3 pos = curr_v->getPosition();
        new_v->setPosition(glm::vec3(pos + (normal * glm::vec3(.5))));
        new_face_vert.push_back(new_v);
        m_mesh.verts.push_back(new_v);
        curr = curr->getNext();
    } while(curr != current_f->getPointTo());

    int index1 = 0;
    int index2 = 0;
    HalfEdge* HE1 = current_f->getPointTo()->getNext();
    do {
        HalfEdge* HE2 = HE1->getSym();
        Vertex* v1 = or_face_vert.at((index1) % or_face_vert.size());
        Vertex* v2 = or_face_vert.at((++index1) % or_face_vert.size());
        Vertex* v3 = new_face_vert.at((index2) % new_face_vert.size());
        Vertex* v4 = new_face_vert.at((++index2) % new_face_vert.size());

        HE1->setVert(v4);
        v4->setPointTo(HE1);

        HalfEdge* prev1 = new HalfEdge();

        HalfEdge* curr = HE1;
        do {
            prev1 = curr;
            curr = curr->getNext();
        } while(curr != HE1);
        prev1->setVert(v3);
        prev1->setNext(HE1);
        v3->setPointTo(prev1);

        HalfEdge* HE1B = new HalfEdge();
        HE1B->setID();
        HalfEdge* HE2B = new HalfEdge();
        HE2B->setID();

        HE1->setSym(HE1B);
        HE2->setSym(HE2B);
        HE1B->setSym(HE1);
        HE2B->setSym(HE2);

        HE1B->setVert(v3);
        v3->setPointTo(HE1B);
        HE2B->setVert(v2);
        v2->setPointTo(HE2B);

        Face* new_face = new Face();
        new_face->setID();
        new_face->setPointTo(HE2B);
        float r1 = ((double) rand() / (RAND_MAX));
        float r2 = ((double) rand() / (RAND_MAX));
        float r3 = ((double) rand() / (RAND_MAX));

        new_face->setColor(glm::vec3(r1,r2,r3));
        m_mesh.faces.push_back(new_face);

        HalfEdge* HE3 = new HalfEdge();
        HE3->setID();

        std::pair<Vertex*, Vertex*> vert_pair1 = std::pair<Vertex*, Vertex*>();
        vert_pair1.first = v2;
        vert_pair1.second = v4;
        sym_pairs[HE3] = vert_pair1;

        HalfEdge* HE4 = new HalfEdge();
        HE4->setID();

        std::pair<Vertex*, Vertex*> vert_pair2 = std::pair<Vertex*, Vertex*>();
        vert_pair2.first = v1;
        vert_pair2.second = v3;
        sym_pairs[HE4] = vert_pair2;

        HE1B->setFace(new_face);
        HE2B->setFace(new_face);
        HE3->setFace(new_face);
        HE4->setFace(new_face);

        HE3->setVert(v4);
        v4->setPointTo(HE3);
        HE4->setVert(v1);
        v1->setPointTo(HE4);

        HE1B->setNext(HE4);
        HE2B->setNext(HE3);
        HE3->setNext(HE1B);
        HE4->setNext(HE2B);

        columns_HE.push_back(HE3);
        columns_HE.push_back(HE4);
        m_mesh.halves.push_back(HE3);
        m_mesh.halves.push_back(HE4);

        HE1 = HE1->getNext();
    } while(HE1 != current_f->getPointTo()->getNext());

    for(HalfEdge* EH1 : columns_HE) {
        std::pair<Vertex*, Vertex*> pair1 = sym_pairs[EH1];
        for(HalfEdge* EH2 : columns_HE) {
            if(EH1 != EH2 && EH1->getSym() == nullptr) {
                std::pair<Vertex*, Vertex*> pair2 = sym_pairs[EH2];
                if(pair1.first == pair2.first && pair1.second == pair2.second) {
                    EH1->setSym(EH2);
                    EH2->setSym(EH1);
                }
                else if(pair1.second == pair2.first && pair1.first == pair2.second) {
                    EH1->setSym(EH2);
                    EH2->setSym(EH1);
                }
                else if(pair2.second == pair1.first && pair2.first == pair1.second) {
                    EH1->setSym(EH2);
                    EH2->setSym(EH1);
                }
            }
        }
    }
    m_mesh.destroy();
    m_mesh.create();
    m_face.create();
    update();
}

//when pressed, it will load obj file
void MyGL::pressedOBJ()
{
    std::map<HalfEdge*, std::pair<Vertex*, Vertex*>> sym_pairs =  std::map<HalfEdge*, std::pair<Vertex*, Vertex*>>();

    QString filename = QFileDialog::getOpenFileName(0, QString("load"), QString("../../"), tr("*.obj"));

    QFile file(filename);

    QList<Vertex*> v;
    QList<glm::vec2> vt;
    QList<glm::vec4> vn;
    int index;

    //if file exists, and opened
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //clear out pre-existing data
        m_mesh.obj_loaded = true;
        //clear qlistwidget
        emit sendOBJ(true);
        m_mesh.destroy();
        Vertex::resetID();
        HalfEdge::resetID();
        Face::resetID();
        m_mesh.halves = std::vector<HalfEdge*>(); //HE
        m_mesh.faces = std::vector<Face*>(); //F
        m_mesh.verts = std::vector<Vertex*>(); //V

        m_mesh.normals = std::vector<glm::vec4>(); //n
        m_mesh.positions = std::vector<glm::vec4>(); //p
        m_mesh.colors = std::vector<glm::vec3>(); //c
        m_mesh.indices = std::vector<GLuint>(); //indx

        QTextStream in(&file);
        QString line = in.readLine();

        while(!line.isNull()){
            char name[3];
            float x, y, z;

            //vertex position
            if(line.indexOf(QString("v")) == 0 && line.indexOf(QString(" ")) == 1) {
                QTextStream stream(&line);
                stream >> name[0] >> x >> y >> z;
                Vertex* vert = new Vertex();
                vert->setPosition(glm::vec3(x,y,z));
                vert->setID();
                v.push_back(vert);
                m_mesh.verts.push_back(vert);
            }

            //vertex texture
            if(line.indexOf(QString("v")) == 0 && line.indexOf(QString("t")) == 1) {
                QTextStream stream(&line);
                stream >> name[1] >> name[2] >> x >> y;
                vt.push_back(glm::vec2(x,y));
            }

            //vertex normal
            if(line.indexOf(QString("v")) == 0 && line.indexOf(QString("n")) == 1) {
                QTextStream stream(&line);
                stream >> name[1] >> name[2] >> x >> y >> z;
                vn.push_back(glm::vec4(x, y, z, 1));
            }


            //face
            if(line.indexOf(QString("f ")) == 0) {
                QTextStream stream(&line);
                QString prefix;
                stream >> prefix;
                std::vector<QString> corners;
                while (!stream.atEnd()) {
                    QString curr;
                    stream >> curr;
                    corners.push_back(curr);
                }
                std::vector<Vertex*> face_verts = std::vector<Vertex*>();
                std::vector<HalfEdge*> face_HE = std::vector<HalfEdge*>();
                for(QString corner : corners) {
                    QStringList list = corner.split("/", QString::SkipEmptyParts);
                    Vertex* vrt = v.at(list.at(0).toInt() - 1);
                    face_verts.push_back(vrt);
                }
                Face* face = new Face();
                float r1 = ((double) rand() / (RAND_MAX));
                float r2 = ((double) rand() / (RAND_MAX));
                float r3 = ((double) rand() / (RAND_MAX));
                face->setID();
                face->setColor(glm::vec3(r1,r2,r3));
                m_mesh.faces.push_back(face);
                for(Vertex* vert : face_verts) {
                    HalfEdge* new_HE = new HalfEdge();
                    m_mesh.halves.push_back(new_HE);
                    new_HE->setID();
                    new_HE->setVert(vert);
                    new_HE->setFace(face);
                    vert->setPointTo(new_HE);
                    face->setPointTo(new_HE);
                    face_HE.push_back(new_HE);
                }
                for(int i = 0; i < face_HE.size(); i) {
                    HalfEdge* new1 = face_HE.at(i % face_HE.size());
                    HalfEdge* new2 = face_HE.at((++i) % face_HE.size());
                    new1->setNext(new2);
                }
                for(HalfEdge* HE : face_HE) {
                    HalfEdge* curr = HE->getNext();
                    HalfEdge* prev_HE = new HalfEdge();
                    while (curr != HE){
                        prev_HE = curr;
                        curr = curr->getNext();
                    }
                    Vertex* prev = prev_HE->getVert();
                    std::pair<Vertex*, Vertex*> vert_pair1 = std::pair<Vertex*, Vertex*>();
                    vert_pair1.first = prev;
                    vert_pair1.second = HE->getVert();
                    sym_pairs[HE] = vert_pair1;
                }
            }
            line = in.readLine();
        }
    }
    for(HalfEdge* EH1 : m_mesh.halves) {
        std::pair<Vertex*, Vertex*> pair1 = sym_pairs[EH1];
        for(HalfEdge* EH2 : m_mesh.halves) {
            if(EH1 != EH2 && EH1->getSym() == nullptr) {
                std::pair<Vertex*, Vertex*> pair2 = sym_pairs[EH2];
                if(pair1.first == pair2.first && pair1.second == pair2.second) {
                    EH1->setSym(EH2);
                    EH2->setSym(EH1);
                }
                else if(pair1.second == pair2.first && pair1.first == pair2.second) {
                    EH1->setSym(EH2);
                    EH2->setSym(EH1);
                }
                else if(pair2.second == pair1.first && pair2.first == pair1.second) {
                    EH1->setSym(EH2);
                    EH2->setSym(EH1);
                }
            }
        }
    }
    m_mesh.create();
    update();
}

//help set the children and parent of each joint node
void MyGL::traverse(QJsonObject joint, Joint* parent) {
    QJsonValue value_name = joint["name"];
    QJsonValue value_pos = joint["pos"];
    QJsonArray array_pos = value_pos.toArray();
    QJsonValue value_rot = joint["rot"];
    QJsonArray array_rot = value_rot.toArray();
    QJsonValue array = joint["children"];
    QJsonArray array_children = joint["children"].toArray();

    Joint* joint_node = new Joint(this);
    skeleton.push_back(joint_node);
    joint_node->setID();
    joint_node->setName(value_name.toString());
    joint_node->setParent(parent);
    if(parent != nullptr) {
        parent->setChildren(joint_node);
    }
    joint_node->setPosition(glm::vec3(array_pos.at(0).toDouble(), array_pos.at(1).toDouble(), array_pos.at(3).toDouble()));
    joint_node->setRotation(glm::vec4(array_rot.at(0).toDouble(), array_rot.at(1).toDouble(), array_rot.at(2).toDouble(), array_rot.at(3).toDouble()));

    for(int j = 0; j < array_children.size(); ++j) {
        traverse(array_children.at(j).toObject(), joint_node);
    }
}

//Json file upload/create when pressed from mainwindow ui
void MyGL::pressedJson()
{
    //read json file
    QString filename = QFileDialog::getOpenFileName(0, QString("load"), QString("../../"), tr("*.json"));
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray byte_array = file.readAll();
         file.close(); //close

        QJsonDocument doc = QJsonDocument::fromJson(byte_array); //create Json doc

         QJsonObject object_total = doc.object();  //create json object
         QJsonValue first_value = object_total.value(QString("root"));

        QJsonObject root_object = first_value.toObject();

        traverse(root_object, nullptr);

         emit addJoint(skeleton.at(0));

         for(Joint* joint : skeleton) {
                joint->create();
         }
        update();
    }
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

    if(current_f != nullptr) {
        emit initialR(current_f->getColor().r);
        emit initialG(current_f->getColor().g);
        emit initialB(current_f->getColor().b);
    }
    if(current_v != nullptr) {
        emit initialX(current_v->getPosition().x);
        emit initialY(current_v->getPosition().y);
        emit initialZ(current_v->getPosition().z);
    }
    for(int i = 0; i < m_mesh.faces.size(); ++i) {
        emit addFace(m_mesh.faces.at(i));
    }
    for(int i = 0; i < m_mesh.halves.size(); ++i) {
        emit addHE(m_mesh.halves.at(i));
    }
    for(int i = 0; i < m_mesh.verts.size(); ++i) {
        emit addVertex(m_mesh.verts.at(i));
    }

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    //Create and set up the skeleton shader
    m_prog_skeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");

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
    m_prog_skeleton.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
//DO NOT CONSTRUCT YOUR SCENE GRAPH IN THIS FUNCTION!

void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(skinned == true) {
        m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
        m_prog_skeleton.setViewProjMatrix(m_glCamera.getViewProj());

        std::vector<glm::mat4> bindMatArray = std::vector<glm::mat4>();

        for(int i = 0; i < skeleton.size(); ++i) {
            skeleton.at(i)->setJoint_BindMatrix();
            glm::mat4 bind = skeleton.at(i)->getJoint_BindMatrix();
            bindMatArray.push_back(bind);
        }

        std::vector<glm::mat4> overallArray = std::vector<glm::mat4>();
        for(int i = 0; i < skeleton.size(); ++i) {
            glm::mat4 bind = skeleton.at(i)->getOverallTrans();
            overallArray.push_back(bind);
        }

        m_prog_skeleton.setBindMats(bindMatArray);
        m_prog_skeleton.setOverallMats(overallArray);

        for(int i = 0; i < m_mesh.faces.size(); ++i){
            emit addFace(m_mesh.faces.at(i));
        }
        for(int i = 0; i < m_mesh.halves.size(); ++i){
            emit addHE(m_mesh.halves.at(i));
        }
        for(int i = 0; i < m_mesh.verts.size(); ++i){
            emit addVertex(m_mesh.verts.at(i));
        }

        if(current_f != nullptr) {
            emit initialR(current_f->getColor().r);
            emit initialG(current_f->getColor().g);
            emit initialB(current_f->getColor().b);
        }
        if(current_v != nullptr) {
            emit initialX(current_v->getPosition().x);
            emit initialY(current_v->getPosition().y);
            emit initialZ(current_v->getPosition().z);
        }

        for(Joint* joint : skeleton) {
            joint->create();
        }

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
        model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(10.0f), glm::radians(15.0f), glm::vec3(6,6,6));

        m_prog_skeleton.setModelMatrix(model);
        m_prog_skeleton.draw(m_mesh);

        glDisable(GL_DEPTH_TEST);
        m_prog_skeleton.draw(m_vert);
        m_prog_skeleton.draw(m_face);
        m_prog_skeleton.draw(m_edge);
        for(Joint* joint : skeleton) {
            m_progFlat.draw(*joint);
        }
        glEnable(GL_DEPTH_TEST);
    }

    else {
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

        if(current_f != nullptr) {
            emit initialR(current_f->getColor().r);
            emit initialG(current_f->getColor().g);
            emit initialB(current_f->getColor().b);
        }
        if(current_v != nullptr) {
            emit initialX(current_v->getPosition().x);
            emit initialY(current_v->getPosition().y);
            emit initialZ(current_v->getPosition().z);
        }

        for(Joint* joint : skeleton) {
            joint->create();
        }

        //#define NOPE
#ifndef NOPE
        //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
        //Note that we have to transpose the model matrix before passing it to the shader
        //This is because OpenGL expects column-major matrices, but you've
        //implemented row-major matrices.
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
        //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>

        //Send the geometry's transformation matrix to the shader
        m_progLambert.setModelMatrix(model);
        m_progLambert.draw(m_mesh);

        glDisable(GL_DEPTH_TEST);
        m_progFlat.setModelMatrix(model);
        m_progFlat.draw(m_vert);
        m_progFlat.draw(m_face);
        m_progFlat.draw(m_edge);
        for(Joint* joint : skeleton) {
            m_progFlat.draw(*joint);
        }
        glEnable(GL_DEPTH_TEST);
    }
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
        m_glCamera.RotatePhi(-amount * 3.14/180);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotatePhi(amount* 3.14/180);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateTheta(-amount* 3.14/180);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateTheta(amount* 3.14/180);
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

