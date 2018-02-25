#include "mesh.h"
#include<la.h>
#include <iostream>

Mesh::Mesh(GLWidget277 *context) : Drawable(context), joint_loaded(false), obj_loaded(false), verts(std::vector<Vertex*>()), halves(std::vector<HalfEdge*>()), faces(std::vector<Face*>())
{}

void Mesh::setJointLoaded(bool e) {
    joint_loaded = e;
}

//add to mesh data
void Mesh::addHE(HalfEdge* he)
{
    halves.push_back(he);
}

void Mesh::addV(Vertex* v)
{
    verts.push_back(v);
}

void Mesh::addF(Face* f)
{
    faces.push_back(f);
}

//planarity testing WIP
void Mesh::planarityTesting()
{
    std::vector<glm::vec4> faceNormals = std::vector<glm::vec4>();
    for(Face* face : faces) {
        HalfEdge* edge = face->getPointTo();
        do {
            glm::vec3 A = edge->getVert()->getPosition();
            glm::vec3 B = edge->getNext()->getVert()->getPosition();
            glm::vec3 C = edge->getNext()->getNext()->getVert()->getPosition();
            glm::vec3 BA = A - B;
            glm::vec3 BC = C - B;
            glm::vec3 normal = glm::normalize(glm::cross(BC,BA));
            faceNormals.push_back(glm::vec4(normal, 1.0f));
            edge = edge->getNext();
        } while(edge != face->getPointTo());
        for(int i = 0; i < 4; ++i)
        {
            if(faceNormals.at(i) != faceNormals.at(( i + 1 ) % 4));
        }
    }

}

void Mesh::createMeshData()
{
    //faces
    Face* front = new Face();
    front->setColor(glm::vec3(0,0,1));
    front->setID();
    Face* top = new Face();
    top->setColor(glm::vec3(0,0,1));
    top->setID();
    Face* back = new Face();
    back->setColor(glm::vec3(0,0,1));
    back->setID();
    Face* bottom = new Face();
    bottom->setColor(glm::vec3(0,0,1));
    bottom->setID();
    Face* right = new Face();
    right->setColor(glm::vec3(0,0,1));
    right->setID();
    Face* left = new Face();
    left->setColor(glm::vec3(0,0,1));
    left->setID();

    //verts
    Vertex* v1 = new Vertex();
    Vertex* v2 = new Vertex();
    Vertex* v3 = new Vertex();
    Vertex* v4 = new Vertex();
    Vertex* v5 = new Vertex();
    Vertex* v6 = new Vertex();
    Vertex* v7 = new Vertex();
    Vertex* v8 = new Vertex();

    v1->setPosition(glm::vec3(-0.5, 0.5, 0.5));
    v2->setPosition(glm::vec3(-0.5, -0.5, 0.5));
    v3->setPosition(glm::vec3(0.5, -0.5, 0.5));
    v4->setPosition(glm::vec3(0.5, 0.5, 0.5));
    v5->setPosition(glm::vec3(-0.5, 0.5, -0.5));
    v6->setPosition(glm::vec3(-0.5, -0.5, -0.5));
    v7->setPosition(glm::vec3(0.5, -0.5, -0.5));
    v8->setPosition(glm::vec3(0.5, 0.5, -0.5));

    HalfEdge* edges1[4] = {new HalfEdge(), new HalfEdge(), new HalfEdge(), new HalfEdge()};
    HalfEdge* edges2[4] = {new HalfEdge(), new HalfEdge(), new HalfEdge(), new HalfEdge()};
    HalfEdge* edges3[4] = {new HalfEdge(), new HalfEdge(), new HalfEdge(), new HalfEdge()};
    HalfEdge* edges4[4] = {new HalfEdge(), new HalfEdge(), new HalfEdge(), new HalfEdge()};
    HalfEdge* edges5[4] = {new HalfEdge(), new HalfEdge(), new HalfEdge(), new HalfEdge()};
    HalfEdge* edges6[4] = {new HalfEdge(), new HalfEdge(), new HalfEdge(), new HalfEdge()};

    //front
    //set up id and face pointers
    for(HalfEdge* edge : edges1){
        edge->setID();
        edge->setFace(front);
    }
    Vertex* vertices1[4] = {v1, v2, v3, v4};
    //set up verts and next pointers
    for(int i = 0; i < 4; ++i){
        edges1[i]->setVert(vertices1[(i + 1) % 4]);
        edges1[i]->setNext(edges1[(i + 1) % 4]);
    }
    //set sym
    edges1[0]->setSym(edges6[2]);
    edges1[1]->setSym(edges4[1]);
    edges1[2]->setSym(edges5[0]);
    edges1[3]->setSym(edges2[1]);
    front->setPointTo(edges1[0]);

    //TOP!!
    //top vertices
    //set up id and face pointers
    for(HalfEdge* edge : edges2){
        edge->setID();
        edge->setFace(top);
    }
    Vertex* vertices2[4] = {v5, v1, v4, v8};
    //set up verts and next pointers
    for(int i = 0; i < 4; ++i){
        edges2[i]->setVert(vertices2[(i + 1) % 4]);
        edges2[i]->setNext(edges2[(i + 1) % 4]);
    }
    //set sym
    edges2[0]->setSym(edges6[3]);
    edges2[1]->setSym(edges1[3]);
    edges2[2]->setSym(edges5[3]);
    edges2[3]->setSym(edges3[3]);
    top->setPointTo(edges2[0]);

    //BACK!
    //set up id and face pointers
    for(HalfEdge* edge : edges3){
        edge->setID();
        edge->setFace(back);
    }
    Vertex* vertices3[4] = {v8, v7, v6, v5};
    //set up verts and next pointers
    for(int i = 0; i < 4; ++i){
        edges3[i]->setVert(vertices3[(i + 1) % 4]);
        edges3[i]->setNext(edges3[(i + 1) % 4]);
    }

    //SYM
    edges3[0]->setSym(edges5[2]);
    edges3[1]->setSym(edges4[3]);
    edges3[2]->setSym(edges6[0]);
    edges3[3]->setSym(edges2[3]);

    back->setPointTo(edges3[0]);

    //bottom!
    //set up id and face pointers
    for(HalfEdge* edge : edges4){
        edge->setID();
        edge->setFace(bottom);
    }
    Vertex* vertices4[4] = {v7, v3, v2, v6};
    //set up verts and next pointers
    for(int i = 0; i < 4; ++i){
        edges4[i]->setVert(vertices4[(i + 1) % 4]);
        edges4[i]->setNext(edges4[(i + 1) % 4]);
    }
    //set sym
    edges4[0]->setSym(edges5[1]);
    edges4[1]->setSym(edges1[1]);
    edges4[2]->setSym(edges6[1]);
    edges4[3]->setSym(edges3[1]);
    bottom->setPointTo(edges4[0]);

    //Right!
    //set up id and face pointers
    for(HalfEdge* edge : edges5){
        edge->setID();
        edge->setFace(right);
    }
    Vertex* vertices5[4] = {v4, v3, v7, v8};
    //set up verts and next pointers
    for(int i = 0; i < 4; ++i){
        edges5[i]->setVert(vertices5[(i + 1) % 4]);
        edges5[i]->setNext(edges5[(i + 1) % 4]);
    }
    //set sym
    edges5[0]->setSym(edges1[2]);
    edges5[1]->setSym(edges4[0]);
    edges5[2]->setSym(edges3[0]);
    edges5[3]->setSym(edges2[2]);
    right->setPointTo(edges5[0]);

    //LEFT
    //set up id and face pointers
    for(HalfEdge* edge : edges6){
        edge->setID();
        edge->setFace(left);
    }
    Vertex* vertices6[4] = {v5, v6, v2, v1};
    //set up verts and next pointers
    for(int i = 0; i < 4; ++i){
        edges6[i]->setVert(vertices6[(i + 1) % 4]);
        edges6[i]->setNext(edges6[(i + 1) % 4]);
    }
    //set sym
    edges6[0]->setSym(edges3[2]);
    edges6[1]->setSym(edges4[2]);
    edges6[2]->setSym(edges1[0]);
    edges6[3]->setSym(edges2[0]);
    left->setPointTo(edges6[0]);

    //Face
    faces.push_back(front);
    faces.push_back(top);
    faces.push_back(back);
    faces.push_back(bottom);
    faces.push_back(right);
    faces.push_back(left);

    //V
    verts.push_back(v1);
    verts.push_back(v2);
    verts.push_back(v3);
    verts.push_back(v4);
    verts.push_back(v5);
    verts.push_back(v6);
    verts.push_back(v7);
    verts.push_back(v8);

    //HE
    for(HalfEdge* edge : edges1){
        halves.push_back(edge);
    }
    for(HalfEdge* edge : edges2){
        halves.push_back(edge);
    }
    for(HalfEdge* edge : edges3){
        halves.push_back(edge);
    }
    for(HalfEdge* edge : edges4){
        halves.push_back(edge);
    }
    for(HalfEdge* edge : edges5){
        halves.push_back(edge);
    }
    for(HalfEdge* edge : edges6){
        halves.push_back(edge);
    }
    //verts
    for(Vertex* vert : verts){
        vert->setID();
        for(HalfEdge* edge : halves){
            if(edge->getVert() == vert){
                vert->setPointTo(edge);
            }
        }
    }

}

void Mesh::create()
{
    positions = std::vector<glm::vec4>();
    indices = std::vector<GLuint>();
    normals = std::vector<glm::vec4>();
    colors = std::vector<glm::vec3>();
    uvs = std::vector<glm::vec2>();

    int start = 0;
    for(Face* face : faces) {
        int numFaceVerts = 0;
        HalfEdge* edge = face->getPointTo();
        do {
            glm::vec3 A = edge->getVert()->getPosition();
            glm::vec3 B = edge->getNext()->getVert()->getPosition();
            glm::vec3 C = edge->getNext()->getNext()->getVert()->getPosition();
            glm::vec3 BA = A - B;
            glm::vec3 BC = C - B;
            glm::vec3 normal = glm::normalize(glm::cross(BC,BA));
            normals.push_back(glm::vec4(normal, 1.0f));
            positions.push_back(glm::vec4(B, 1));
            colors.push_back(face->getColor());
            edge = edge->getNext();
            ++numFaceVerts;
        } while(edge != face->getPointTo());
        int ind = 0;
        do {
            indices.push_back(start);
            indices.push_back(ind + 1 + start);
            indices.push_back(ind + 2 + start);
            ++ind;
        } while(ind < numFaceVerts - 2);
        start = positions.size();
    }

    count = indices.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec4), normals.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), colors.data(), GL_STATIC_DRAW);


    if(joint_loaded == true) {
        std::vector<glm::vec2> two_IDs = std::vector<glm::vec2>();
        std::vector<glm::vec2> two_weights = std::vector<glm::vec2>();

        for(Face* face : faces) {
            HalfEdge* edge = face->getPointTo();
            Vertex* vert = edge->getVert();
            do {
                std::vector<float> vert_weights = vert->getWeight();
                std::vector<Joint*> joints = vert->getJoints();
                glm::ivec2 ID = glm::vec2(joints.at(0)->getID(), joints.at(1)->getID());
                glm::vec2 weights = glm::vec2(vert_weights.at(0), vert_weights.at(1));
                two_IDs.push_back(ID);
                two_weights.push_back(weights);
                vert = edge->getNext()->getVert();
            } while(vert != edge->getVert());
        }

        generateID();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufID);
        mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), two_IDs.data(), GL_STATIC_DRAW);

        generateWeight();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWeight);
        mp_context->glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), two_weights.data(), GL_STATIC_DRAW);

    }
}

