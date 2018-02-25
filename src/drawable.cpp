#include "drawable.h"
#include <la.h>

Drawable::Drawable(GLWidget277* context)
    : bufIdx(), bufPos(), bufNor(), bufCol(), bufUV(),
      idxBound(false), posBound(false), norBound(false), colBound(false), uvBound(false),
      mp_context(context)
{}


void Drawable::destroy()
{
    mp_context->glDeleteBuffers(1, &bufIdx);
    mp_context->glDeleteBuffers(1, &bufPos);
    mp_context->glDeleteBuffers(1, &bufNor);
    mp_context->glDeleteBuffers(1, &bufCol);
    mp_context->glDeleteBuffers(1, &bufUV);
}

GLenum Drawable::drawMode()
{
    // Since we want every three indices in bufIdx to be
    // read to draw our Drawable, we tell that the draw mode
    // of this Drawable is GL_TRIANGLES

    // If we wanted to draw a wireframe, we would return GL_LINES

    return GL_TRIANGLES;
}

int Drawable::elemCount()
{
    return count;
}

void Drawable::generateIdx()
{
    idxBound = true;
    // Create a VBO on our GPU and store its handle in bufIdx
    mp_context->glGenBuffers(1, &bufIdx);
}

void Drawable::generatePos()
{
    posBound = true;
    // Create a VBO on our GPU and store its handle in bufPos
    mp_context->glGenBuffers(1, &bufPos);
}

void Drawable::generateNor()
{
    norBound = true;
    // Create a VBO on our GPU and store its handle in bufNor
    mp_context->glGenBuffers(1, &bufNor);
}

void Drawable::generateCol()
{
    colBound = true;
    // Create a VBO on our GPU and store its handle in bufCol
    mp_context->glGenBuffers(1, &bufCol);
}

void Drawable::generateUV()
{
    uvBound = true;
    // Create a VBO on our GPU and store its handle in bufUV
    mp_context->glGenBuffers(1, &bufUV);
}

void Drawable::generateID()
{
    IDBound = true;
    // Create a VBO on our GPU and store its handle in bufUV
    mp_context->glGenBuffers(1, &bufID);
}

void Drawable::generateWeight()
{
    weightBound = true;
    // Create a VBO on our GPU and store its handle in bufUV
    mp_context->glGenBuffers(1, &bufWeight);
}




bool Drawable::bindIdx()
{
    if(idxBound) {
        mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    }
    return idxBound;
}

bool Drawable::bindPos()
{
    if(posBound){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    }
    return posBound;
}

bool Drawable::bindNor()
{
    if(norBound){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    }
    return norBound;
}

bool Drawable::bindCol()
{
    if(colBound){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    }
    return colBound;
}

bool Drawable::bindUV()
{
    if(uvBound){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufUV);
    }
    return uvBound;
}

bool Drawable::bindID()
{
    if(IDBound){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufID);
    }
    return IDBound;
}

bool Drawable::bindWeight()
{
    if(weightBound){
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWeight);
    }
    return weightBound;
}
