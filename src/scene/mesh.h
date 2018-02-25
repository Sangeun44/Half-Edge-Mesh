#ifndef MESH_H
#define MESH_H

#include "components.h"
#include "drawable.h"
#include <QListWidgetItem>

#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Mesh : public Drawable
{
public:
    Mesh(GLWidget277* mp_context);
    virtual void create();

    //create halfedge
    void createMeshData();

    //add new halfedge data to mesh
    void addHE(HalfEdge* he);
    void addV(Vertex* v);
    void addF(Face* f);

    void setJointLoaded(bool e);

    //for triangulating different planes
    void planarityTesting();

    //halfedge mesh data attr
    bool obj_loaded;
    std::vector<Vertex*> verts;
    std::vector<HalfEdge*> halves;
    std::vector<Face*> faces;

    //vbo attr
    std::vector<GLuint> indices;
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec2> uvs;

    bool joint_loaded;

};

#endif // MESH_H
