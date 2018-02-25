#ifndef DRAWCOMPONENTS_H
#define DRAWCOMPONENTS_H

#include "components.h"
#include "joint.h"
#include "drawable.h"
#include <glm/gtx/rotate_vector.hpp>
#include <la.h>
#include <glm/gtx/vector_angle.hpp>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


class DrawVert : public Drawable
{
private:
    Vertex* V;
public:
    DrawVert(GLWidget277* mp_context);

    void setVert(Vertex* v);
    void create();
    virtual GLenum drawMode();

};

class DrawHE : public Drawable
{
private:
    HalfEdge* HE;
public:
    DrawHE(GLWidget277* mp_context);

    void setHE(HalfEdge* he);
    void create();
    virtual GLenum drawMode();
};

class DrawFace : public Drawable
{
private:
    Face* F;
public:
    DrawFace(GLWidget277* mp_context);

    void setFace(Face* f);
    void create();
    virtual GLenum drawMode();
};

#endif // DRAWCOMPONENTS_H
