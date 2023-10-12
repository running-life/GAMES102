#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <vector>
#include "Shader.h"
#include "global.h"
#include "MyWindow.h"
#include "HM1.h"


// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouse_callback(GLFWwindow* window, int button, int action, int mods);



// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 1280;
float curPoint[2] = {};
std::vector<HM1Point> HM1::controlPoints = std::vector<HM1Point>();
std::vector<HM1Point> HM1::resultPolynomial = std::vector<HM1Point>();
std::vector<HM1Point> HM1::resultGauss = std::vector<HM1Point>();
std::vector<HM1Point> HM1::resultLeastSquare = std::vector<HM1Point>();
std::vector<HM1Point> HM1::resultRidge = std::vector<HM1Point>();
bool HM1::polynomialInterpolationFlag = false;
bool HM1::RBFInterpolationFlag = false;
bool HM1::leastSquareFittingFlag = false;
bool HM1::ridgeFittingFlag = false;


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

    // Create a GLFW window object that we can use for GLFW's functions
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

    ImGuiWindowFlags windowFlags = 0;
    //windowFlags |= ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_NoBackground;

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

    // Define the shader
    Shader shader("./shader/vertex.shader", "./shader/fragment.shader");
    shader.Use();
    


    HM1Point axisVertices[] = {
        // X-axis
       HM1Point( - 1.0f, 0.0f, 1.0f, 1.0f, 1.0f ),    // beginning
       HM1Point(1.0f, 0.0f, 1.0f, 1.0f, 1.0f),  // ending

        // Y-axis
       HM1Point(0.0f, -1.0f, 1.0f, 1.0f, 1.0f),  // beginning
       HM1Point(0.0f,  1.0f, 1.0f, 1.0f, 1.0f)   // ending
    };

    HM1 homework1;




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


    GLuint pointVAO, pointVBO;
    glGenVertexArrays(1, &pointVAO);
    glGenBuffers(1, &pointVBO);

    // 绑定VAO
    glBindVertexArray(pointVAO);

    // 绑定VBO并将控制点的顶点数据复制到VBO
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);

    // 配置顶点属性指针
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    bool controlWindowFlag = true, infoWindowFlag = true;


    auto draw = [&](std::vector<HM1Point> data) {
        glBindVertexArray(pointVAO);
        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * 5 * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
        glLineWidth(4.0);
        glDrawArrays(GL_LINE_STRIP, 0, data.size());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        };

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("HW1", NULL, windowFlags);
        ImGui::Checkbox("Information Window", &infoWindowFlag);
        ImGui::Checkbox("Control Window", &controlWindowFlag);
        ImGui::Checkbox("Polynomial", &HM1::polynomialInterpolationFlag);
        ImGui::Checkbox("Gauss", &HM1::RBFInterpolationFlag);
        ImGui::Checkbox("LeastSquare", &HM1::leastSquareFittingFlag);
        ImGui::Checkbox("Ridge", &HM1::ridgeFittingFlag);
        ImGui::End();

        info_window(&infoWindowFlag);
        control_window(&controlWindowFlag);
        //temp();

        ImGui::Render();

        
        

        // Render
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 4);
        glBindVertexArray(0);


        glBindVertexArray(pointVAO);
        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
        glBufferData(GL_ARRAY_BUFFER, HM1::controlPoints.size() * 5 * sizeof(GLfloat), HM1::controlPoints.data(), GL_STATIC_DRAW);
        glPointSize(10);
        glDrawArrays(GL_POINTS, 0, HM1::controlPoints.size());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        if (HM1::polynomialInterpolationFlag) {
            glBindVertexArray(pointVAO);
            glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
            glBufferData(GL_ARRAY_BUFFER, HM1::resultPolynomial.size() * 5 * sizeof(GLfloat), HM1::resultPolynomial.data(), GL_STATIC_DRAW);
            glLineWidth(4.0);
            glDrawArrays(GL_LINE_STRIP, 0, HM1::resultPolynomial.size());

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        if (HM1::RBFInterpolationFlag) {
            glBindVertexArray(pointVAO);
            glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
            glBufferData(GL_ARRAY_BUFFER, HM1::resultGauss.size() * 5 * sizeof(GLfloat), HM1::resultGauss.data(), GL_STATIC_DRAW);
            glLineWidth(4.0);
            glDrawArrays(GL_LINE_STRIP, 0, HM1::resultGauss.size());

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        if (HM1::leastSquareFittingFlag) {
            draw(HM1::resultLeastSquare);
        }

        if (HM1::ridgeFittingFlag)
            draw(HM1::resultRidge);



        
        

        
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
    glfwDestroyWindow(window);
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
    ImGuiIO& io = ImGui::GetIO();
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !io.WantCaptureMouse) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        xpos /= (WIDTH/2);
        xpos -= 1;
        ypos /= (HEIGHT/2);
        ypos = 1.0 - ypos;
        std::cout << "Mouse clicked at position: " << xpos << ", " << ypos << std::endl;

        curPoint[0] = xpos;
        curPoint[1] = ypos;

    }
}

