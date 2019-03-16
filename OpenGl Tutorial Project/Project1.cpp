#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

using namespace std;

//resizes the window if the user changes its size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

//interprets key presses
void ProcessInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//Contains the source code for the vertex shader
const char *VertexShaderSource = {
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0"
};

//source code for the fragment shader
const char *FragmentShaderSource = {
    "#version 330 core \n"
    "out vec4 FragColour; \n"
    "void main() \n"
    "{ \n"
    "   FragColour = vec4(1.0, 0.5, 0.2, 1.0); \n"
    "} \n"
};

const char *FragmentShaderFadeSource = {
    "#version 330 core \n"
    "out vec4 FragColour; \n"
    "void main() \n"
    "{ \n"
    "   FragColour = vec4(gl_FragCoord.xyz, 1.0); \n"
    "} \n"
};

//the main program
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{

    //initializes GLFW and GLAD
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    //defines points to be rendered
    float Triangle1[] = {
        0.5, 0.5, 0.0,
        0.5, -0.5, 0.0,
        -0.5, -0.5, 0.0,
    };

    float Triangle2[] = {
        0.4, 0.5, 0.0,
        -0.6, 0.5, 0.0,
        -0.6, -0.5, 0.0,
        -0.8, 0.5, 0.0
    };

    //defines order vertices are connected in
    unsigned int Indices1[] = {
        0, 1, 2, 
    };

    unsigned int Indices2[] = {
        0, 1, 2, 
        1, 2, 3
    };

    //generate VAO, VBO, EBO
    unsigned int VAO[2], VBO[2], EBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(2, EBO);

    //bind first set of VAO, VBO, EBO
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

    //store data in buffers
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle1), Triangle1, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices1), Indices1, GL_STATIC_DRAW);

    //configure opengl to interpret the vertices array corectly
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //bind second set of VAO, VBO, EBO
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);

    //store data in buffers
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle2), Triangle2, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);

    //configure opengl to interpret the vertices array corectly
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //compiles the vertex shader
    unsigned int VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(VertexShader);

    //checks vertex shader was compiled successfully
    int Success;
    char InfoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, InfoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << "\n";
    }

    //compile fragment shader
    unsigned int FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);

    //checks fragment shader was compiled successfully
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(FragmentShader, 512, NULL, InfoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << InfoLog << "\n";
    }

    //compile fragment shader fade
    unsigned int FragmentShaderFade;
    FragmentShaderFade = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderFade, 1, &FragmentShaderFadeSource, NULL);
    glCompileShader(FragmentShaderFade);

    //checks fragment shader was compiled successfully
    glGetShaderiv(FragmentShaderFade, GL_COMPILE_STATUS, &Success);
    if (!Success) {
        glGetShaderInfoLog(FragmentShaderFade, 512, NULL, InfoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << InfoLog << "\n";
    }

    //setting up the shader program
    unsigned int ShaderProgram =glCreateProgram();

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    //check shader program was made successfully
    glGetShaderiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
        cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" << InfoLog << "\n";
    }

    //setting up the shader fade program
    unsigned int ShaderFadeProgram =glCreateProgram();

    glAttachShader(ShaderFadeProgram, VertexShader);
    glAttachShader(ShaderFadeProgram, FragmentShaderFade);
    glLinkProgram(ShaderFadeProgram);

    //check shader program was made successfully
    glGetShaderiv(ShaderFadeProgram, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderFadeProgram, 512, NULL, InfoLog);
        cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" << InfoLog << "\n";
    }

    //clean up, deleting the vertex shader and fragment shader
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    glDeleteShader(FragmentShaderFade);

    //render loop
    while(!glfwWindowShouldClose(window)) {
        //reads any key inputs
        ProcessInput(window);

        //sets the colour of clear pixels
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        //drawing settings
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(ShaderProgram);

        //draws in shapes
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glUseProgram(ShaderFadeProgram);

        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //moves the image in the buffer window to the display window
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }   

    //cleanly terminates program
    glfwTerminate();
    return 0;
}