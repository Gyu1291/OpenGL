
#ifndef QUAD_H
#define QUAD_H

#include "shader.h"

class Quadpyra {
public:

    // vertex position array
    GLfloat quadVertices[54] = { 
        .0f, .5f, .0f,  -.5f, -.5f, .5f,  .5f,-.5f, .5f,   // v0,v1,v2,v3 (front)
        .0f, .5f, .0f,   .5f,-.5f, .5f,   .5f,-.5f,-.5f,   // v0,v3,v4,v5 (right)
        .0f, .5f, .0f,   .5f, -.5f,-.5f,  -.5f, -.5f, -.5f, // v0,v5,v6,v1 (back)
        .0f, .5f, .0f,  -.5f, -.5f,-.5f,  -.5f,-.5f, .5f, // v1,v6,v7,v2 (left)
        -.5f,-.5f, .5f,   .5f,-.5f, .5f,   -.5f,-.5f, -.5f,  // v7,v4,v3,v2 (bottom1)
        .5f,-.5f, .5f,  -.5f,-.5f,-.5f,  .5f, -.5f,-.5f,  // v4,v7,v6,v5 (bottom2)
     };


    GLfloat quadColors[72] = { // initialized as RGBA sollid color for each face, 96 elements
        1, 0, 0, 1,   1, 0, 0, 1,   1, 0, 0, 1,   //  (front=red)
        0, 1, 0, 1,   0, 1, 0, 1,   0, 1, 0, 1,   //  (right)
        0, 0, 1, 1,   0, 0, 1, 1,   0, 0, 1, 1,   //  (back)
        1, 1, 0, 1,   1, 1, 0, 1,   1, 1, 0, 1,   //  (left)
        1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1,   //  (bottom)
        1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1,   //  (bottom2)
    };



    GLuint quadIndices[18] = { // 36 elements
        0, 1, 2,    //  (front)
        3, 4, 5,    //  (right)
        6, 7, 8,    //  (back)
        9, 10, 11,    //  (left)
        12, 13, 14,     //  (bottom1)
        15, 16, 17     //  (bottom2)
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int vSize = sizeof(quadVertices);
    unsigned int cSize = sizeof(quadColors);
    

    Quadpyra() {
        initBuffers();
    };

    Quadpyra(float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        initBuffers();
    };

    Quadpyra(float dx, float dy, float dz, float s) {
        scale(s);
        translate(dx, dy, dz);
        initBuffers();
    };

    Quadpyra(float dx, float dy, float dz, float sx, float sy, float sz) {
        scale2(sx, sy, sz);
        translate(dx, dy, dz);
        initBuffers();
    };

    void initBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize + cSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, quadVertices);                  // copy verts at offset 0
        //glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, cubeNormals);               // copy norms after verts
        glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, quadColors);          // copy cols after norms
        //glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize + cSize, tSize, cubeTexCoords); // copy texs after cols

        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vSize); // normal attrib
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(vSize)); //color attrib
        //glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vSize + nSize + cSize)); // tex coord
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };

    void draw(Shader* shader) {
        shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

    void translate(float dx, float dy, float dz) {
        for (int i = 0; i < 54; i++) {
            if (i % 3 == 0) quadVertices[i] += dx;
            else if (i % 3 == 1) quadVertices[i] += dy;
            else if (i % 3 == 2) quadVertices[i] += dz;
        }
    };

    void scale(float s) {
        for (int i = 0; i < 54; i++)
            quadVertices[i] *= s;
    };

    void scale2(float sx, float sy, float sz) {
        for (int i = 0; i < 54; i++) {
            if (i % 3 == 0) quadVertices[i] *= sx;
            else if (i % 3 == 1) quadVertices[i] *= sy;
            else if (i % 3 == 2) quadVertices[i] *= sz;
        }
    }
};


#endif
