#include "drawcomponents.h"
#include "components.h"
#include <QTextStream>



DrawVert::DrawVert(GLWidget277 *context): Drawable(context), V()
{}

void DrawVert::setVert(Vertex* v)
{
    V = v;
}

GLenum DrawVert::drawMode()
{
    return GL_POINTS;
}

void DrawVert::create()
{

    glm::vec3 color = glm::vec3(1,1,1);
    glm::vec4 position = glm::vec4(V->getPosition(), 1);

    std::vector<glm::vec3> col {color};
    std::vector<glm::vec4> pos {position};
    std::vector<GLuint> idx {0};
    count = 1;

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

}

DrawEdge::DrawEdge(GLWidget277 *context): Drawable(context), HE()
{}

void DrawEdge::setHE(HalfEdge* he)
{
    HE = he;
}

GLenum DrawEdge::drawMode()
{
    return GL_LINES;
}

void DrawEdge::create()
{
    glm::vec4 vertPointYellow_pos = glm::vec4(HE->getVert()->getPosition(),1);
    glm::vec4 vertRed_pos = glm::vec4(HE->getSym()->getVert()->getPosition(),1);

    glm::vec3 yellow = glm::vec3(1,1,0);
    glm::vec3 red = glm::vec3(1,0,0);

    std::vector<glm::vec3> col {yellow, red};
    std::vector<glm::vec4> pos {vertPointYellow_pos, vertRed_pos};
    std::vector<GLuint> idx {0,1};
    count = 2;

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

}


DrawFace::DrawFace(GLWidget277 *context) : Drawable(context), F()
{}

void DrawFace::setFace(Face* f)
{
    F = f;
}

GLenum DrawFace::drawMode()
{
    return GL_LINES;
}

void DrawFace::create()
{
    std::vector<glm::vec4> positions = std::vector<glm::vec4>();
    std::vector<GLuint> indices = std::vector<GLuint>();
    std::vector<glm::vec3> colors = std::vector<glm::vec3>();

    int numVerts = 0;
    HalfEdge* edge = F->getPointTo();
    do {
        positions.push_back(glm::vec4(edge->getVert()->getPosition(),1));
        glm::vec3 face_color = F->getColor();
        glm::vec3 vertex_color = glm::vec3(glm::abs(1-face_color.r), glm::abs(1-face_color.g), glm::abs(1-face_color.b));
        colors.push_back(vertex_color);
        edge = edge->getNext();
        ++numVerts;
    } while(edge != F->getPointTo());
    int ind = 0;
    do {
        indices.push_back(ind);
        indices.push_back((ind + 1) % numVerts);
        ++ind;
    } while(ind < (int)positions.size());



    count = indices.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

}
