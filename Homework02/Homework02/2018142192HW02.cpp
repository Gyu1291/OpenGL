#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 창 크기
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;

bool isGreen = true; //시작은 초록색이다.

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
    "   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n" //초록색을 의미한다.
    "}\n\0";
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n" //빨간색을 의미한다.
    "}\n\0";



//Key_callback을 사용해 P키를 눌렀을 때 색이 바뀌게 해 준다. 이 때 isGreen이라는 bool변수를 활용한다.
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

    // 창 생성
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
    //서로 다른 ShaderProgram을 사용하므로 ShaderProgram 2개를 만들어주었다.
    
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

    // 두 경우 모두 6개의 Vertex만을 사용하여 그려내었다.
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
    glGenVertexArrays(2, VAOs); // 두 개의 VAO를 동시에 생성해주었다.
    glGenBuffers(2, VBOs); // 두 개의 VBO를 동시에 생성해주었다.
    
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
            glDrawArrays(GL_TRIANGLE_FAN, 0, 6); //isGreen=True일 경우 초록색 육각형을 TriangleFan으로 그려준다.
        }
        else {
            glUseProgram(shaderProgramRed);
            glBindVertexArray(VAOs[1]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);	// isGreen=False일 경우 빨간색 육각형을 TriangleStrip으로 그려준다.
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
