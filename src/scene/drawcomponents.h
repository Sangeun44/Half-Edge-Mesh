#ifndef DRAWCOMPONENTS_H
#define DRAWCOMPONENTS_H

#include "components.h"
#include "drawable.h"
#include <la.h>

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

class DrawEdge : public Drawable
{
private:
    HalfEdge* HE;
public:
    DrawEdge(GLWidget277* mp_context);

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
