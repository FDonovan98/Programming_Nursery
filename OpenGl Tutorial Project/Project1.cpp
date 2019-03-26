#include <iostream>
#include <cmath>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <shaders.h>

using namespace std;

//resizes the window if the user changes its size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)  {
    glViewport(0, 0, width, height);
}

//interprets key presses
void ProcessInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//the main program
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    /* #region Initilization */

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

    /* #endregion */

    /* #region pointsToRender */

    //defines points to be rendered
    float Triangle1[] = {
        //position      //colour
         0.5,  0.5, 0.0,  1.0, 0.0, 0.0,
         0.5, -0.5, 0.0,  0.0, 1.0, 0.0,
        -0.5, -0.5, 0.0,  0.0, 0.0, 1.0
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

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    /* #endregion */

    Shader strobeShader("Dependencies/Shaders/strobeShader.vs", "Dependencies/Shaders/strobeShader.fs");

    Shader gradientShader("Dependencies/Shaders/gradientShader.vs", "Dependencies/Shaders/gradientShader.fs");

    //render loop
    while(!glfwWindowShouldClose(window)) {
        //reads any key inputs
        ProcessInput(window);

        //sets the colour of clear pixels
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        //set ourColor to change with time
        float timeValue = glfwGetTime();
        float strobe = sin(timeValue) / 2.0f + 0.5f;

        //drawing settings
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        gradientShader.use();

        //draws in shapes
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        //drawing settings
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        strobeShader.use();
        strobeShader.setFloat("color", strobe);

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