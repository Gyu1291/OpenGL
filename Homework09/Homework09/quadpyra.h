#ifndef QUAD_H
#define QUAD_H

#include "shader.h"
#include <math.h>
class Quadpyra {
public:
    bool isflat = true;
    float root5 = sqrt(5);
    // vertex position array
    GLfloat quadVertices[54] = { 
        .0f, .5f, .0f,  -.5f, -.5f, .5f,  .5f,-.5f, .5f,   // v0,v1,v2 (front)
        .0f, .5f, .0f,   .5f,-.5f, .5f,   .5f,-.5f,-.5f,   // v0,v2, v3 (right)
        .0f, .5f, .0f,   .5f, -.5f,-.5f,  -.5f, -.5f, -.5f, // v0,v3, v4 (back)
        .0f, .5f, .0f,  -.5f, -.5f,-.5f,  -.5f,-.5f, .5f, // v0,v4, v1 (left)
        -.5f,-.5f, .5f,   .5f,-.5f, .5f,   -.5f,-.5f, -.5f,  // v1,v2,v4 (bottom1)
        .5f,-.5f, .5f,  -.5f,-.5f,-.5f,  .5f, -.5f,-.5f,  // v2,v4,v3 (bottom2)
     };


    GLfloat quadColors[72] = { // initialized as RGBA sollid color for each face, 72 elements
        1, 0, 0, 1,   1, 0, 0, 1,   1, 0, 0, 1,   //  (front=red)
        0, 1, 0, 1,   0, 1, 0, 1,   0, 1, 0, 1,   //  (right)
        0, 0, 1, 1,   0, 0, 1, 1,   0, 0, 1, 1,   //  (back)
        1, 1, 0, 1,   1, 1, 0, 1,   1, 1, 0, 1,   //  (left)
        1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1,   //  (bottom)
        1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1,   //  (bottom2)
    };


    GLuint quadIndices[18] = { // 18 elements
        0, 1, 2,    //  (front)
        3, 4, 5,    //  (right)
        6, 7, 8,    //  (back)
        9, 10, 11,    //  (left)
        12, 13, 14,     //  (bottom1)
        15, 16, 17     //  (bottom2)
    };

    GLfloat quadTexCoords[36] = { // 36 elements
        0.504f, 0.082f,   0.359f, 0.335f,   0.654f, 0.335f,        //front 
        0.909f, 0.480f,   0.654f, 0.335f,   0.654f, 0.627f,        //right
        0.504f, 0.881f,   0.654f, 0.627f,   0.359f, 0.627f,        //back
        0.104f, 0.480f,   0.359f, 0.627f,   0.359f, 0.335f,        //left   
        0.359f, 0.335f,   0.654f, 0.335f,   0.359f, 0.627f,        //bottom1
        0.654f, 0.335f,   0.359f, 0.627f,   0.654f, 0.627f,        //bottom2   
    };
    
    GLfloat flatNormal[54] = {
        0.0f, 1.0f, 2.0f,    0.0f, 1.0f, 2.0f,    0.0f, 1.0f, 2.0f,
        2.0f, 1.0f, 0.0f,    2.0f, 1.0f, 0.0f,     2.0f, 1.0f, 0.0f,
        0.0f, 1.0f, -2.0f,    0.0f, 1.0f, -2.0f,    0.0f, 1.0f, -2.0f, 
        -2.0f, 1.0f, 0.0f,    -2.0f, 1.0f, 0.0f,    -2.0f, 1.0f, 0.0f,  
        0.0f, -1.0f*root5, 0.0f,    0.0f, -1.0f*root5, 0.0f,    0.0f, -1.0f*root5, 0.0f,
        0.0f, -1.0f*root5, 0.0f,    0.0f, -1.0f*root5, 0.0f,    0.0f, -1.0f*root5, 0.0f,
    };
    //flatNormal의 경우 flat shading을 할 때의 좌표를 미리 계산해서 넣어주었다.
    //각 normal vector의 크기만 맞춰주었다.
    GLfloat smoothNormal[54] = {
        0.0f, root5, 0.0f,                          -2.0f/3.0f, (2.0f-root5)/3.0f, 2.0f/3.0f,     2.0f/3.0f, (2.0f-root5)/3.0f, 2.0f/3.0f,  // v0,v1,v2 (front)
        0.0f, root5, 0.0f,                           2.0f/3.0f, (2.0f-root5)/3.0f, 2.0f/3.0f,     2.0f/3.0f, (2.0f-root5)/3.0f, -2.0f/3.0f,  // v0,v2, v3 (right)
        0.0f, root5, 0.0f,                           2.0f/3.0f, (2.0f-root5)/3.0f, -2.0f/3.0f,     -2.0f/3.0f, (2.0f-root5)/3.0f, -2.0f/3.0f,  // v0,v3, v4 (back)
        0.0f, root5, 0.0f,                           -2.0f/3.0f, (2.0f-root5)/3.0f, -2.0f/3.0f,     -2.0f/3.0f, (2.0f-root5)/3.0f, 2.0f/3.0f,  // v0,v4, v1 (left)
        -2.0f/3.0f, (2.0f-root5)/3.0f, 2.0f/3.0f,     2.0f/3.0f, (2.0f-root5)/3.0f, 2.0f/3.0f,     -2.0f/3.0f, (2.0f-root5)/3.0f, -2.0f/3.0f,  // v1,v2,v4 (bottom1)
        2.0f/3.0f, (2.0f-root5)/3.0f, 2.0f/3.0f,     -2.0f/3.0f, (2.0f-root5)/3.0f, -2.0f/3.0f,     2.0f/3.0f, (2.0f-root5)/3.0f, -2.0f/3.0f,  // v2,v4,v3 (bottom2)
    };
    //smoothNormal의 경우 smooth shading을 할 때의 좌표를 미리 계산해서 넣어 주었다.
    //각 vertex에 normal vector가 존재하게 되고, 이들은 vertex를 둘러싼 면의 normal vector의 평균이다.
    //5개의 vertex가 있고 각각 0,1,0 
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int vSize = sizeof(quadVertices);
    unsigned int nSize = sizeof(flatNormal);
    unsigned int cSize = sizeof(quadColors);
    unsigned int tSize = sizeof(quadTexCoords);

    Quadpyra() { //init method
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
        glBufferData(GL_ARRAY_BUFFER, vSize + nSize + cSize + tSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, quadVertices);                  // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, flatNormal);               // copy norms after verts
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize, cSize, quadColors);          // copy cols after norms
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize + cSize, tSize, quadTexCoords); // copy texs after cols

        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vSize); // normal attrib
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(vSize + nSize)); //color attrib
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vSize + nSize + cSize)); // tex coord
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };





    void changemode(){
        if(isflat) // flat to smooth
        {
            glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize + nSize + cSize + tSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, quadVertices);                  // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, smoothNormal);               // copy norms after verts
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize, cSize, quadColors);          // copy cols after norms
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize + cSize, tSize, quadTexCoords); // copy texs after cols

        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vSize); // normal attrib
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(vSize + nSize)); //color attrib
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vSize + nSize + cSize)); // tex coord
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
            isflat = false;
            
        }
        else    //smooth to flat
        {
            glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize + nSize + cSize + tSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, quadVertices);                  // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, flatNormal);               // copy norms after verts
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize, cSize, quadColors);          // copy cols after norms
        glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize + cSize, tSize, quadTexCoords); // copy texs after cols

        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vSize); // normal attrib
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(vSize + nSize)); //color attrib
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vSize + nSize + cSize)); // tex coord
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
            isflat = true;
            
        }
    }

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
