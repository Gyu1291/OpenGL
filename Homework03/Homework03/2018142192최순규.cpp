#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <shader.h> 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callbacks(GLFWwindow* window, int button, int action, int mods);


unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;
bool isGreen = false;
bool verticalFlip = false;
float colorR = 1.0f, colorG = 0.0f, colorB = 0.0f;
double cursorX = 0.0, cursorY = 0.0;
float posX = 0.0f, posY = 0.0f;

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework3", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callbacks);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }


    Shader ourShader("2.4.shader.vs", "2.4.shader.fs");

    float vertices[] = {
        -0.1f,  0.1f, 0.0f,  
         0.1f,  0.1f, 0.0f,  
        -0.1f, -0.1f, 0.0f,
         0.1f, -0.1f, 0.0f,
    };
    unsigned int indices[] = { 
        0, 1, 2, 3  
    };
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        ourShader.setFloat("posX", posX);
        ourShader.setFloat("posY", posY);
        ourShader.setVec4("outColor", colorR, colorG, colorB, 1.0);
        
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}



void mouse_button_callbacks(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (isGreen)
        {
            colorR = 1.0f;
            colorG = 0.0f;
            colorB = 0.0f;
            isGreen = false;
        }
        else
        {
            colorR = 0.0f;
            colorG = 1.0f;
            colorB = 0.0f;
            isGreen = true;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &cursorX, &cursorY);
        posX = static_cast<float>((cursorX-300)/300);
        posY = static_cast<float>((-cursorY+300)/300);
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}
