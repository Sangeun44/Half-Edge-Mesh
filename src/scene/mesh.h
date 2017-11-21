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

    void createMeshData();

    void addHE(HalfEdge* he);
    void addV(Vertex* v);
    void addF(Face* f);
    void planarityTesting();

    std::vector<Vertex*> verts;
    std::vector<HalfEdge*> halves;
    std::vector<Face*> faces;

    std::vector<GLuint> indices;
    std::vector<glm::vec4> positions;
    std::vector<glm::vec4> normals;
    std::vector<glm::vec3> colors;


};

#endif // MESH_H
