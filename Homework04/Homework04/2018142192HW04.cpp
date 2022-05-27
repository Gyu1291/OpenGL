// 11_Polyline
//    - Mouse: left button: add a vertex, right button: stop input
//    - Keyboard 'r' to reset all inputs, ESC to quit

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader.h>
#include <math.h>

using namespace std;

// callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// other functions
void normalize_cursor_position(double x, double y, float& nx, float& ny);
void update_vb_vertex(int vIndex, float x, float y);
void render(GLFWwindow* window);

// global settings
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;
const int MAX_VERTEX = 200;
const int MAX_VERTEX_SIZE = MAX_VERTEX * 3;
const int MAX_COLOR_SIZE = MAX_VERTEX * 4;

float a, b, c, d, A, B, C, D;
float solT[2], solX[2], solY[2];
int nPoints = 0;
//int numOfSol = 0;
int numOfSol = 0;
float x_i, y_i;
bool stopInput = false;
bool nextPoint = false;
bool drawPoint = false;

Shader ourShader;
unsigned int VBO[6], VAO[3];
float vertices[MAX_VERTEX_SIZE];
float colors[MAX_COLOR_SIZE];
float colors2[MAX_COLOR_SIZE];
float colors3[MAX_COLOR_SIZE];
float parabola[65 * 3];
float points[MAX_VERTEX_SIZE];


float lineXpos[2];
float lineYpos[2];




int main()
{
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework04", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // shader loading and compile (by calling the constructor)
    ourShader.initShader("3.1.shader.vs", "3.1.shader.fs");

    // initialize color buffer data
    for (int i = 0, j = 0; i < MAX_VERTEX; i++, j = (j + 1) % 6) {
        colors[i * 4] = 0.0f;
        colors[i * 4 + 1] = 1.0f;
        colors[i * 4 + 2] = 0.0f;
        colors[i * 4 + 3] = 1.0f;
    }
    for (int i = 0, j = 0; i < MAX_VERTEX; i++, j = (j + 1) % 6) {
        colors2[i * 4] = 1.0f;
        colors2[i * 4 + 1] = 0.0f;
        colors2[i * 4 + 2] = 0.0f;
        colors2[i * 4 + 3] = 1.0f;
    }
    for (int i = 0, j = 0; i < MAX_VERTEX; i++, j = (j + 1) % 6) {
        colors3[i * 4] = 1.0f;
        colors3[i * 4 + 1] = 1.0f;
        colors3[i * 4 + 2] = 0.0f;
        colors3[i * 4 + 3] = 1.0f;
    }

    for (int i = 0; i < 65; i++) {
        x_i = -1.0f + (1.0f / 32.0f) * i;
        y_i = (2.0f * x_i * x_i) - 0.8f * x_i - 0.42f;

        parabola[i * 3] = x_i;
        parabola[i * 3 + 1] = y_i;
        parabola[i * 3 + 2] = 0.0f;
    }


    glGenVertexArrays(3, VAO);  // vertex array object
    glGenBuffers(6, VBO);        // vertex and color buffers

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //이 아래부터는 parabola를 위한 부분
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(parabola), parabola, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), colors2, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glBindVertexArray(VAO[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors3), colors3, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glfwTerminate();

    return 0;
}

void render(GLFWwindow* window) {

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_LINE_STRIP, 0, 65);

    if (nPoints > 0) {
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_LINES, 0, nPoints);

    }

    if (drawPoint) {

        glBindVertexArray(VAO[2]);
        glPointSize(10.0f);
        glDrawArrays(GL_POINTS, 0, numOfSol);
    }



    glfwSwapBuffers(window);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    //영상에서는 reset을 구현하였지만 현재 코드에 구현되어 있지는 않습니다.
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!stopInput) {
        float cx, cy;
        if (!nextPoint) {
            nPoints++;
            nextPoint = true;
        }
        normalize_cursor_position(xpos, ypos, cx, cy);
        update_vb_vertex(nPoints - 1, cx, cy);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    float nx, ny;

    if (button == GLFW_MOUSE_BUTTON_LEFT && !stopInput) {
        if (action == GLFW_PRESS) {
            nPoints = 1;
            glfwGetCursorPos(window, &xpos, &ypos);
            normalize_cursor_position(xpos, ypos, nx, ny);
            lineXpos[0] = nx;
            lineYpos[0] = ny;
            if (!stopInput) {
                nextPoint = false;
            }


        }
        else if (action == GLFW_RELEASE) {
            glfwGetCursorPos(window, &xpos, &ypos);
            normalize_cursor_position(xpos, ypos, nx, ny);
            update_vb_vertex(nPoints, nx, ny);

            lineXpos[1] = nx;
            lineYpos[1] = ny;
            stopInput = true;

            a = lineXpos[1] - lineXpos[0];
            b = lineXpos[0];
            c = lineYpos[1] - lineYpos[0];
            d = lineYpos[0];

            A = 2 * a * a;
            B = 4 * a * b - 0.8f * a - c;
            C = 2 * b * b - 0.8f * b - d - 0.42f;
            D = B * B - 4.0f * A * C;

            drawPoint = true;


            if (drawPoint) {
                
                int count = 0;
                if (D > 0) {
                    solT[0] = (-B + sqrt(D)) / (2 * A);
                    solT[1] = (-B - sqrt(D)) / (2 * A);

                    for (int i = 0; i < 2; i++) {
                        if (solT[i] <= 1 && solT[i] >= 0) {
                            solX[count] = a * solT[i] + b;
                            solY[count] = c * solT[i] + d;
                            count++;
                        }
                    }
                    //std::cout << solT[0] << std::endl;

                }
                else if (D == 0) {
                    solT[0] = (-B) / 2 * A;
                    if (solT[0] <= 1 && solT[0] >= 0) {
                        solX[0] = a * solT[0] + b;
                        solY[0] = c * solT[0] + d;
                        count = 1;
                    }
                }

                for (int i = 0; i < count; i++) {
                    float n[3];
                    n[0] = solX[i];
                    n[1] = solY[i];
                    n[2] = 0.0f;
                    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
                    glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(float) * 3, sizeof(float) * 3, n);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                }
                std::cout << count << std::endl; //결과를 확실하게 알 수 있게 교점의 개수를 print하도록 하였다.
                numOfSol = count; 
            }


        }
    }
}

void normalize_cursor_position(double x, double y, float& nx, float& ny)
{
    nx = ((float)x / (float)SCR_WIDTH) * 2.0f - 1.0f;
    ny = -1.0f * (((float)y / (float)SCR_HEIGHT) * 2.0f - 1.0f);
}

void update_vb_vertex(int vIndex, float x, float y)
{
    float n[3];
    n[0] = x;
    n[1] = y;
    n[2] = 0.0f;
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vIndex * sizeof(float) * 3, sizeof(float) * 3, n);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


