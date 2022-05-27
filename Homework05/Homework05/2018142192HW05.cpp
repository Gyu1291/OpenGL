// 13_Transformation
//      Scale, rotate, and translate the box
//      keyboard: ESC to quit

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <shader.h>
#include <cmath>
using namespace std;

// Function Prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void render();

// Global variables
GLFWwindow *window = NULL;
Shader *globalShader = NULL;
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;
unsigned int VBO[3], VAO[3], EBO[3];
float vertices[] = {
    // positions
    0.025f,  0.025f, 0.0f, // top right
    0.025f, -0.025f, 0.0f, // bottom right
    -0.025f, -0.025f, 0.0f, // bottom left
    -0.025f,  0.025f, 0.0f  // top left
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

float greenSpeed = glm::radians(360.0f); // 1초에 한번
float yellowSpeed = glm::radians(90.0f); // 4초에 한번
float redSpeed = glm::radians(180.0f); // 2초에 한번


int main()
{
    window = glAllInit();

    // shader loading and compile (by calling the constructor)
    globalShader = new Shader("4.1.transform.vs", "4.1.transform.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(3, EBO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(3, EBO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(3, EBO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwPollEvents();
    }
        
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(3, VAO);
    glDeleteBuffers(3, VBO);
    glDeleteBuffers(3, EBO);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework05", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    
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

void render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // create transformations
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //transform = glm::translate(transform, glm::vec3(0.3f, -0.3f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime()* greenSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
    //transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
    
    // get matrix's uniform location and set matrix
    globalShader->use();
    globalShader->setVec4("outColor", 0.0f, 1.0f, 0.0f, 1.0f);
    unsigned int transformLoc = glGetUniformLocation(globalShader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    
    // render
    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    float time = glfwGetTime();
    float radius = -sin(time / 3) / 2;
    glm::mat4 transform2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    transform2 = glm::rotate(transform2, (float)glfwGetTime()* yellowSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
    transform2 = glm::translate(transform2, glm::vec3(radius, 0.0f, 0.0f));
    //transform2 = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

    // get matrix's uniform location and set matrix
    //globalShader->use();
    globalShader->setVec4("outColor", 1.0f, 1.0f, 0.0f, 1.0f);
    //unsigned int transformLoc2 = glGetUniformLocation(globalShader->ID, "transform2");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform2));
    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    transform2 = glm::rotate(transform2, (float)glfwGetTime()*redSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
    transform2 = glm::translate(transform2, glm::vec3(0.1f, 0.0f, 0.0f));
    transform2 = glm::scale(transform2, glm::vec3(4.0f, 1.0f, 1.0f));
    

    // get matrix's uniform location and set matrix
    //globalShader->use();
    globalShader->setVec4("outColor", 1.0f, 0.0f, 0.0f, 1.0f);
    //unsigned int transformLoc3 = glGetUniformLocation(globalShader->ID, "transform2");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform2));


    // render
    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



    
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
