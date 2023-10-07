﻿#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <vector>
#include "Shader.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouse_callback(GLFWwindow* window, int button, int action, int mods);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;


std::vector<float> controlPoints;
unsigned int VAOControlPoints, VBOControlPoints;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    Shader shader("./shader/vertex.shader", "./shader/fragment.shader");
    shader.Use();


    float axisVertices[] = {
        // X-axis
       -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // beginning
        1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // ending

        // Y-axis
         0.0f, -1.0f, 1.0f, 1.0f, 1.0f,  // beginning
         0.0f,  1.0f, 1.0f, 1.0f, 1.0f   // ending
    };

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.5f, 0.6f, 1.0f, // Left  
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Right 
         0.0f,  0.5f, 1.0f, 1.0f, 1.0f // Top   
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


    unsigned int VBOTriangle, VAOTriangle;
    glGenVertexArrays(1, &VAOTriangle);
    glGenBuffers(1, &VBOTriangle);
    glBindVertexArray(VAOTriangle);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0);


    //glGenVertexArrays(1, &VAOControlPoints);
    //glGenBuffers(1, &VBOControlPoints);


    GLuint pointVAO, pointVBO;
    glGenVertexArrays(1, &pointVAO);
    glGenBuffers(1, &pointVBO);

    // 绑定VAO
    glBindVertexArray(pointVAO);

    // 绑定VBO并将控制点的顶点数据复制到VBO
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);

    // 配置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our first triangle
        shader.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 4);
        glBindVertexArray(0);

        //glBindVertexArray(VAOTriangle);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glBindVertexArray(0);

        shader.Use();

        glBindVertexArray(pointVAO);
        // 绑定VBO并将控制点的顶点数据复制到VBO
        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
        glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(GLfloat), controlPoints.data(), GL_STATIC_DRAW);
        glPointSize(15);
        glDrawArrays(GL_POINTS, 0, controlPoints.size() / 5);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        //shader.Use();
        //glPointSize(15.0f);
        //glBindVertexArray(VAOControlPoints);
        //glDrawArrays(GL_POINTS, 0, controlPoints.size()/5 );
        //glBindVertexArray(0);


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Terminate GLFW, clearing any resources allocated by GLFW.

    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        xpos /= (WIDTH/2);
        xpos -= 1;
        ypos /= (HEIGHT/2);
        ypos = 1.0 - ypos;
        std::cout << "Mouse clicked at position: " << xpos << ", " << ypos << std::endl;


        controlPoints.push_back(xpos);
        controlPoints.push_back(ypos);
        controlPoints.push_back(0.0f);
        controlPoints.push_back(0.0f);
        controlPoints.push_back(0.0f);



        

        for (int j = 0; j < controlPoints.size() / 5; ++j) {
            for (int i = 0; i < 5; ++i) {
                std::cout << controlPoints[j * 5 + i] << " ";
            }
            std::cout << std::endl;
        }



    }
}