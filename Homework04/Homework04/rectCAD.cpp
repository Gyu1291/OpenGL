// 12_RectCAD
// Mouse: Pressing left button and drag the rectangle until releasing the button
// Keyboard: 'i': compute all intersections among all edges of rectangles and the diagonal line
//                (from upper right to lower left)

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <shader.h>

using namespace std;

// types
class MyObject {
public:
    MyObject() { };
    MyObject(int colorIndex, Shader *shader) {
        completed = false;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        this->colorIndex = colorIndex;
        this->shader = shader;
        nVertex = 4;
    };
    void setV(int i, int j, float x, float y) {
        v[i] = x;
        v[j] = y;
    }
    int nVertex;
    float v[8]; // vertex data
    bool completed;
    unsigned int VAO;  // vertex array object
    unsigned int VBO;  // vertex buffer object
    unsigned int colorIndex; // index of colorOriginal
    Shader *shader;          // shader to be used
};

// function prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void normalize_cursor_position(double x, double y, float &nx, float &ny);
void update_vb_vertex(MyObject *obj, float x, float y);
void compute_intersections();
void render();

// constants
const int MAX_OBJECT = 100;
float colorOriginal[] = {
    1.0, 0.0, 0.0, 1.0, // red
    1.0, 1.0, 0.0, 1.0, // yellow
    0.0, 1.0, 0.0, 1.0, // green
    0.0, 1.0, 1.0, 1.0, // cyan
    0.0, 0.0, 1.0, 1.0, // blue
    1.0, 0.0, 1.0, 1.0  // magenta
};

// global variables
GLFWwindow *window = NULL;
Shader *globalShader = NULL;
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;
int nObject = 0;
MyObject *objList[MAX_OBJECT];
bool dragging = false;
unsigned int colorIndex = 0;
int nInter = 0;
float interV[MAX_OBJECT * 2 * 2];
unsigned int interVAO;
unsigned int interVBO;
bool showInter = false;

int main()
{
    window = glAllInit();
    
    // initialize object list
    for (int i = 0; i < MAX_OBJECT; i++)
        objList[i] = NULL;

    // shader loading and compile (by calling the constructor)
    globalShader = new Shader("3.2.shader.vs", "3.2.shader.fs");

    // render loop
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow *glAllInit()
{
    GLFWwindow *window;
    
    // glfw: initialize and configure
    if (!glfwInit()) {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework4", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    // set some OpenGL states (for transparency and background color)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    
    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "GLEW initialisation failed!" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }
    
    return window;
}

void render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < nObject; i++) {
        
        objList[i]->shader->use();
        glBindVertexArray(objList[i]->VAO);
        
        if (objList[i]->completed) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            unsigned int index = objList[i]->colorIndex * 4;
            objList[i]->shader->setVec4("inColor", colorOriginal[index],
                                        colorOriginal[index + 1],
                                        colorOriginal[index + 2],
                                        0.5f);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
        
        objList[i]->shader->setVec4("inColor", 0.5f, 0.5f, 0.5f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glBindVertexArray(0);
    }
    
    if (showInter && nInter > 0) {
        glPointSize(7.0f);
        globalShader->use();
        globalShader->setVec4("inColor", 0.0f, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(interVAO);
        glDrawArrays(GL_POINTS, 0, nInter);
        glBindVertexArray(0);
    }
    
    glfwSwapBuffers(window);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        if (!showInter) {
            if (nInter == 0) {
                glGenVertexArrays(1, &interVAO);
                glGenBuffers(1, &interVBO);
            }
            else {
                nInter = 0;
            }
            compute_intersections();
        }
        else showInter = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT= height;
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    float nx, ny;
    if (dragging) {
        normalize_cursor_position(xpos, ypos, nx, ny);
        update_vb_vertex(objList[nObject-1], nx, ny);
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    double xpos, ypos;
    float nx, ny;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            glfwGetCursorPos(window, &xpos, &ypos);
            normalize_cursor_position(xpos, ypos, nx, ny);
            
            if (!objList[nObject]) {
                objList[nObject] = new MyObject(colorIndex, globalShader);
                colorIndex = (colorIndex + 1) % 6;
                MyObject *obj = objList[nObject];
                
                obj->setV(0, 1, nx, ny);
                obj->setV(2, 3, nx, ny);
                obj->setV(4, 5, nx, ny);
                obj->setV(6, 7, nx, ny);
                
                glBindVertexArray(obj->VAO);
                glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(obj->v), obj->v, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
            
            objList[nObject]->setV(0, 1, nx, ny);
            objList[nObject]->completed = false;
            nObject++;
            dragging = true;
        }
        else if (action == GLFW_RELEASE) {
            dragging = false;
            glfwGetCursorPos(window, &xpos, &ypos);
            normalize_cursor_position(xpos, ypos, nx, ny);
            if (fabs(nx - objList[nObject - 1]->v[0]) < 0.01 &&
                fabs(ny - objList[nObject - 1]->v[1]) < 0.01) {
                // Ignore the too small rectangle
                nObject--;
            }
            else {
                update_vb_vertex(objList[nObject - 1], nx, ny);
                objList[nObject - 1]->completed = true;
            }
        }
    }
}

void normalize_cursor_position(double x, double y, float &nx, float &ny)
{
    nx = ((float)x / (float)SCR_WIDTH) * 2.0f - 1.0f;
    ny = -1.0f * (((float)y / (float)SCR_HEIGHT) * 2.0f - 1.0f);
}

void update_vb_vertex(MyObject *obj, float x, float y)
{
    obj->setV(2, 3, obj->v[0], y);
    obj->setV(4, 5, x, y);
    obj->setV(6, 7, x, obj->v[1]);
    
    glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(obj->v), obj->v);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void compute_intersections() {
    // implicit 2D line through (-1,1) and (1,1): y = x
    // parametric 2D line for each edge in a rectangle (for i = 0, 2, 4, 6)
    //     x1 = v[j], y1 = v[j+1], x2 = v[j+2], y2 = v[j+3]
    //     for i = 0, 4: x(t) = x1, y(t) = (1-t)*y1 + t*y2 = x1, so, t = (x1 - y1) / (y2 - y1)
    //     for i = 2, 6: y(t) = y1, x(t) = (1-t)*x1 + t*x2 = y1, so, t = (y1 - x1) / (x2 - x1)
    // if 0 <= t <= 1, then intersection exists.
    
    for (int i = 0; i < nObject; i++) { // for each object
        for (int j = 0; j < 7; j += 2) {
            float t;
            float x1 = objList[i]->v[j],
                  y1 = objList[i]->v[j+1],
                  x2 = objList[i]->v[j+2],
                  y2 = objList[i]->v[j+3];
            if (j == 0 || j == 4) {
                t = (x1 - y1) / (y2 - y1);
                if (0.0f <= t && t <= 1.0f) {
                    interV[nInter * 2] = x1;
                    interV[nInter * 2 + 1] = x1;
                    nInter++;
                }
            }
            else {
                t = (y1 - x1) / (x2 - x1);
                if (0.0f <= t && t <= 1.0f) {
                    interV[nInter * 2] = y1;
                    interV[nInter * 2 + 1] = y1;
                    nInter++;
                }
            }
        }
    }
    
    showInter = true;
    
    glBindVertexArray(interVAO);
    glBindBuffer(GL_ARRAY_BUFFER, interVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(interV), interV, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
