#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// â ũ��
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;

bool isGreen = true; //������ �ʷϻ��̴�.

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShader1Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n" //�ʷϻ��� �ǹ��Ѵ�.
    "}\n\0";
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n" //�������� �ǹ��Ѵ�.
    "}\n\0";



//Key_callback�� ����� PŰ�� ������ �� ���� �ٲ�� �� �ش�. �� �� isGreen�̶�� bool������ Ȱ���Ѵ�.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS) { 
        if (isGreen) {
            isGreen = false;
        }
        else {
            isGreen = true;
        }
    }
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // â ����
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework02", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderGreen = glCreateShader(GL_FRAGMENT_SHADER); 
    unsigned int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER); 

    unsigned int shaderProgramGreen = glCreateProgram();
    unsigned int shaderProgramRed = glCreateProgram(); 
    //���� �ٸ� ShaderProgram�� ����ϹǷ� ShaderProgram 2���� ������־���.
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderGreen, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderGreen);
    glShaderSource(fragmentShaderRed, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderRed);
    

    glAttachShader(shaderProgramGreen, vertexShader);
    glAttachShader(shaderProgramGreen, fragmentShaderGreen);
    glLinkProgram(shaderProgramGreen);
    

    glAttachShader(shaderProgramRed, vertexShader);
    glAttachShader(shaderProgramRed, fragmentShaderRed);
    glLinkProgram(shaderProgramRed);

    // �� ��� ��� 6���� Vertex���� ����Ͽ� �׷�������.
    // ------------------------------------------------------------------
    float firstHexagon[] = {
        0.0f, 0.5f, 0.0f,
        -0.35f, 0.25f, 0.0f,
        -0.35f, -0.25f, 0.0f,
        0.0f, -0.5f, 0.0f,
        0.35f, -0.25f, 0.0f,
        0.35f, 0.25f, 0.0f,
        

    };
    float secondHexagon[] = {
        0.0f, 0.5f, 0.0f,  // 
        -0.35f, 0.25f, 0.0f,  // 
        0.35f, 0.25f, 0.0f,
        -0.35f, -0.25f, 0.0f,
        0.35f, -0.25f, 0.0f,
        0.0f, -0.5f, 0.0f,
        
        
        
    };
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // �� ���� VAO�� ���ÿ� �������־���.
    glGenBuffers(2, VBOs); // �� ���� VBO�� ���ÿ� �������־���.
    
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstHexagon), firstHexagon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	
    glEnableVertexAttribArray(0);
   
    glBindVertexArray(VAOs[1]);	
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondHexagon), secondHexagon, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); 
    glEnableVertexAttribArray(0);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (isGreen) {
            glUseProgram(shaderProgramGreen);
            
            glBindVertexArray(VAOs[0]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 6); //isGreen=True�� ��� �ʷϻ� �������� TriangleFan���� �׷��ش�.
        }
        else {
            glUseProgram(shaderProgramRed);
            glBindVertexArray(VAOs[1]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);	// isGreen=False�� ��� ������ �������� TriangleStrip���� �׷��ش�.
        }

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    
    glfwTerminate();
    return 0;
}



void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}
