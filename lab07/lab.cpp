// Include C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <ctime>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include ImGui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include <common/model.h>
#include <common/texture.h>
#include <common/light.h> 
#include <stb_image_aug.h>

#include "Cube.h"
#include "Sphere.h"
#include "Box.h"
#include "MassSpringDamper.h"
#include "Collision.h"
#include "Avatar.h"
#include "Mushroom.h"

using namespace std;
using namespace glm;


// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();
struct Material;
void uploadMaterial(const Material& mtl);
void uploadLight(const Light& light);

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Pop-Up World"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024


// Global variables
GLFWwindow* window;
Camera* camera;
Light* light;
GLuint shaderProgram;
GLuint miniMapProgram;
GLuint projectionMatrixLocation, viewMatrixLocation, modelMatrixLocation;
GLuint LaLocation, LdLocation, LsLocation, lightPositionLocation, lightPowerLocation,lightColorLocation;
GLuint ScaleMatrixLocation;
GLuint KdLocation, KsLocation, KaLocation, NsLocation;
GLuint GrassTexture,GrassSampler;
GLuint CatTexture, CatSampler, CatTextureDiff, CatSamplerDiff;
GLuint CloudTexture, CloudSampler;
GLuint PlanetTexture, PlanetSampler;
GLuint KormosTexture, KormosSampler;
GLuint MushroomTexture, MushroomSampler;
GLuint MushroomTexture2, MushroomSampler2;
GLuint MushroomTexture3, MushroomSampler3;
GLuint MoonTexture, MoonSampler;
GLuint DweTexture, DweSampler;
GLuint useTextureLocation;
GLuint swapLocation;
GLuint cartoonLocation, cartoonLocation1;

// locations for depthProgram
GLuint depthProgram;
GLuint shadowViewProjectionLocation;
GLuint depthMapSampler;
GLuint lightVPLocation;
GLuint shadowModelLocation;

// locations for miniMapProgram
//GLuint quadTextureSamplerLocation;
GLuint depthFBO, depthTexture;
GLuint depthFBO2, depthTexture2;

// skybox
GLuint skyboxVAO, skyboxVBO;
GLuint SkyboxShaderProgram;
GLuint cubemapTexture;

// extern variables
extern float avatarGroundLevel;
extern bool transitioning;

//extern bool sevenKeyPressed;
//extern int cartoonLike;


//------------DRAWABLES-----------
Drawable* tree;
Drawable* cloud;
Drawable* planet;
Drawable* mushroom;
Drawable* mushroom2;
Drawable* mushroom3;
//Drawable* quad;
Drawable* dwelling;

vec3 lightPos = vec3(0,30.0f,-30.0f);
//vec3 lightPos = vec3(0,8.0f,25.0f);
float t = static_cast<float>(glfwGetTime());
std::vector<vec3> cloudPositions;
std::vector<vec3> mushroomPositions;
float ythreshold = 5.0f;

//int randomNumberX = (std::rand() % 7) - 2;
//int randomNumberZ = (std::rand() % 41);
struct Platform {
    float xmin, xmax; // Horizontal boundaries (x-axis)
    float ymin, ymax; // Vertical boundaries (y-axis)
    float zmin, zmax; // Depth boundaries (z-axis)
};

struct Material {
    glm::vec4 Ka;
    glm::vec4 Kd;
    glm::vec4 Ks;
    float Ns;
};
const Material White_Rubber{
    vec4(0.0, 0.0, 0.0, 0.2),
    vec4(0.5, 0.5, 0.5,  0.2),
    vec4(0.7, 0.7, 0.7,  0.2),
    0.078125f
};
const Material Yellow_Rubber{
    vec4(0.05, 0.05, 0, 1),
    vec4(0.5, 0.5, 0.4, 1),
    vec4(0.7, 0.7, 0.04, 1),
    0.078125f
};
const Material Emerald{
    vec4(0.633, 0.727811, 0.633, 0.25),
    vec4(0.07568, 0.61424, 0.07568, 0.25),
    vec4(0.0215, 0.1745, 0.0215, 0.25),
    76.8f
};
const Material Emerald2{
    vec4(0.633, 0.727811, 0.633, 0.7),
    vec4(0.07568, 0.61424, 0.07568, 0.7),
    vec4(0.0215, 0.1745, 0.0215, 0.7),
    76.8f
};
const Material Bronze{
    vec4(0.393548, 0.271906, 0.166721, 1),
    vec4(0.714, 0.4284, 0.18144, 1),
    vec4(0.2125, 0.1275, 0.054, 1),
    25.6f
};

const Material Silver{
    vec4(0.773911, 0.773911, 0.773911, 1),
    vec4(0.2775, 0.2775, 0.2775, 1),
    vec4(0.23125, 0.23125, 0.23125, 1),
    89.6
};
const Material Gold{
    vec4(0.628281f, 0.555802f, 0.366065f, 1.0f),
    vec4(0.75164f, 0.60648f, 0.22648f, 1.0f),
    vec4(0.24725f, 0.1995f, 0.0745f, 1.0f),
    51.2f

};
const Material Brass{
    vec4(0.992157, 0.941176, 0.807843, 1),
    vec4(0.780392, 0.568627, 0.113725, 1),
    vec4(0.329412, 0.223529, 0.027451, 1),
    27.8974f

};


Sphere* sphere;
Avatar* avatar;
Box* box;
Box* box2;
MassSpringDamper* msd;
Mushroom* mushroom1;
Mushroom* mushroom11;

// Standard acceleration due to gravity
const float g = 9.80665f;
bool gameover = false;


void renderHelpingWindow() {   
    if (gameover == false) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Helper Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);// Create a window called "Hello, world!" and append into it.

        ImGui::Text("Press 'W' to move forward");
        ImGui::Text("Press 'Space' to jump");


        if (camera->cartoonLike % 2 == 0) ImGui::Text("Press 7 to enable cartoon-like mode");
        else ImGui::Text("Press 7 to disable cartoon-like mode");
        ImGui::Text("Press 2 to switch to 2D mode");
        ImGui::Text("Press 3 to return to 3D mode");

        //ImGui::Text("Performance %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    else {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Helper Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);// Create a window called "Hello, world!" and append into it.

        ImGui::Text("------GAME OVER------");

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    
    
}


vec3 generateRandomPosition(float y) {
    float x = (std::rand() % 21 - 10);    // Random x in [-5, 5]
    float z = (std::rand() % 101);       // Random z in [0, 40]
    vec3 position = vec3(x, y, z-40);
    return position;
}
void initializeCloudPositions(size_t numClouds, float y) {
    for (size_t i = 0; i < numClouds; ++i) {
        vec3 position = generateRandomPosition(y);
        cloudPositions.push_back(position);
    }
}
vec3 generateRandomPositionMushroom(float y) {
    float x = -5.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / 10.0f);  // Random x in [-3, 3]
    float z = 20.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / 40.0f);       // Random z in [0, 40]
    vec3 position = vec3(x, y, z);
    return position;
}
void initializeMushRoomdPositions(size_t mushrooms, float y) {
    for (size_t i = 0; i < mushrooms; ++i) {
        vec3 position = generateRandomPositionMushroom(y);
        mushroomPositions.push_back(position);
    }
}
void uploadMaterial(const Material& mtl) {
    glUniform4f(KaLocation, mtl.Ka.r, mtl.Ka.g, mtl.Ka.b, mtl.Ka.a);
    glUniform4f(KdLocation, mtl.Kd.r, mtl.Kd.g, mtl.Kd.b, mtl.Kd.a);
    glUniform4f(KsLocation, mtl.Ks.r, mtl.Ks.g, mtl.Ks.b, mtl.Ks.a);
    glUniform1f(NsLocation, mtl.Ns);
}

void uploadLight(const Light& light) {
    glUniform4f(LaLocation, light.La.r, light.La.g, light.La.b, light.La.a);
    glUniform4f(LdLocation, light.Ld.r, light.Ld.g, light.Ld.b, light.Ld.a);
    glUniform4f(LsLocation, light.Ls.r, light.Ls.g, light.Ls.b, light.Ls.a);    
    glUniform3f(lightPositionLocation, light.lightPosition_worldspace.x,light.lightPosition_worldspace.y, light.lightPosition_worldspace.z);
    glUniform1f(lightPowerLocation, light.power);
    
}

static unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Failed to load texture: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void createContext() {
    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    std::cout << "Max texture units: " << maxTextureUnits << std::endl;
    float currentTime = static_cast<float>(glfwGetTime());
    float dt = currentTime - t;

    t += dt;
    shaderProgram = loadShaders("StandardShading.vertexshader","StandardShading.fragmentshader");
    depthProgram = loadShaders("Depth.vertexshader", "Depth.fragmentshader");
    SkyboxShaderProgram = loadShaders("Skybox.vertexshader", "Skybox.fragmentshader");
    //miniMapProgram = loadShaders("SimpleTexture.vertexshader", "SimpleTexture.fragmentshader");
    //grass
    GrassTexture = loadBMP("bmps/3xortaraki.bmp");
    GrassSampler = glGetUniformLocation(shaderProgram, "GrassSampler");
    //cat
    CatTexture = loadBMP("bmps/cat.bmp");
    CatSampler = glGetUniformLocation(shaderProgram, "CatSampler");
    

    CatTextureDiff = loadBMP("bmps/Cat_diffuse.bmp");
    CatSamplerDiff = glGetUniformLocation(shaderProgram, "CatSamplerDiff");

    //kormos
    KormosTexture = loadBMP("bmps/TreeDiffuse2.bmp");
    KormosSampler = glGetUniformLocation(shaderProgram, "KormosSampler");
    //mushroom
    MushroomTexture = loadBMP("bmps/boletus_color.bmp");
    MushroomSampler = glGetUniformLocation(shaderProgram, "MushroomSampler");
    //mushroom2
    MushroomTexture2 = loadBMP("bmps/amanita_color.bmp");
    MushroomSampler2 = glGetUniformLocation(shaderProgram, "MushroomSampler2");
    //mushroom3
    MushroomTexture3 = loadBMP("bmps/morel_color.bmp");
    MushroomSampler3 = glGetUniformLocation(shaderProgram, "MushroomSampler3");
    //moon
    MoonTexture = loadBMP("bmps/moon.bmp");
    MoonSampler = glGetUniformLocation(shaderProgram, "MoonSampler");
    //Dwelling
    DweTexture = loadBMP("bmps/dwelling.bmp");
    DweSampler = glGetUniformLocation(shaderProgram, "DweSampler");
    // skybox 
    std::vector<std::string> faces = {
    "bmps/skybox4/vz_dawn_right.png", "bmps/skybox4/vz_dawn_left.png", "bmps/skybox4/vz_dawn_up.png", "bmps/skybox4/vz_dawn_down.png", "bmps/skybox4/vz_dawn_front.png", "bmps/skybox4/vz_dawn_back.png"
    
    };
    cubemapTexture = loadCubemap(faces);

    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");
    KaLocation = glGetUniformLocation(shaderProgram, "mtl.Ka");
    KdLocation = glGetUniformLocation(shaderProgram, "mtl.Kd");
    KsLocation = glGetUniformLocation(shaderProgram, "mtl.Ks");
    NsLocation = glGetUniformLocation(shaderProgram, "mtl.Ns");
    LaLocation = glGetUniformLocation(shaderProgram, "light.La");
    LdLocation = glGetUniformLocation(shaderProgram, "light.Ld");
    LsLocation = glGetUniformLocation(shaderProgram, "light.Ls");
    lightPositionLocation = glGetUniformLocation(shaderProgram, "light.lightPosition_worldspace");
    lightColorLocation = glGetUniformLocation(shaderProgram, "light.lcolor");
    lightPowerLocation = glGetUniformLocation(shaderProgram, "light.power");
    useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");
    swapLocation = glGetUniformLocation(shaderProgram, "swap");
    cartoonLocation = glGetUniformLocation(shaderProgram, "cartoonish");
    cartoonLocation1 = glGetUniformLocation(SkyboxShaderProgram, "cartoonish");


    ScaleMatrixLocation = glGetUniformLocation(shaderProgram, "ScaleMatrix");

    // locations for shadow rendering
    depthMapSampler = glGetUniformLocation(shaderProgram, "shadowMapSampler");
    lightVPLocation = glGetUniformLocation(shaderProgram, "lightVP");
    // --- depthProgram ---
    shadowViewProjectionLocation = glGetUniformLocation(depthProgram, "VP");
    shadowModelLocation = glGetUniformLocation(depthProgram, "M");

    // --- miniMapProgram ---
    //
    // quadTextureSamplerLocation = glGetUniformLocation(miniMapProgram, "textureSampler");

    box = new Box(vec3(0.0f, 0.0f, 40.0f), 5);
    box2 = new Box(vec3(0.0f, 1.0f, 0.0f), 5);

    
   /* mushroom1 = new Mushroom(vec3(0.0f, -3.0f, 17.0f), vec3(0,0, 0), vec3(0, 3, 0), 1, 1);
    mushroom11 = new Mushroom(vec3(0.0f, -3.0f, 12.0f), vec3(0, 0, 0), vec3(0, 5, 0), 1, 1);*/
    
    // Task 2a: change the initial velocity and position, mass and radius
    sphere = new Sphere(vec3(light->lightPosition_worldspace.x, light->lightPosition_worldspace.y, light->lightPosition_worldspace.z), vec3(0,0,0),0.8f,  10);
  
    //avatar = new Avatar(vec3(0, 0.29f, 15), vec3(0,0,0),0.8f,  10);mushroom1
    avatar = new Avatar(vec3(0,  avatarGroundLevel, 55), vec3(0, 0, 0), 0.8f, 0.1);

    tree = new Drawable("models/treeBR.obj");
    cloud = new Drawable("models/cloud.obj");
    mushroom = new Drawable("models/boletus.obj");
    mushroom2 = new Drawable("models/amanita_new.obj");
    mushroom3 = new Drawable("models/morel_low.obj");
    planet = new Drawable("models/sphere.obj");
    dwelling = new Drawable("models/Dwelling.obj");

    // ------------------------ Skybox_Creation -------------------------------
    float skyboxVertices[] = {
        // Positions          
        -1.0f,  1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f
    };
    
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //// Tell opengl to generate a framebuffer
    glGenFramebuffers(1, &depthFBO);
    // Binding the framebuffer, all changes bellow will affect the binded framebuffer
    // **Don't forget to bind the default framebuffer at the end of initialization
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);


    // We need a texture to store the depth image
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    // Telling opengl the required information about the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// Task 4.5 Texture wrapping methods
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);							// GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    //*/

    // Task 4.5 Don't shadow area out of light's viewport
    //*/
    // Step 1 : (Don't forget to comment out the respective lines above
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // Set color to set out of border 
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // Next go to fragment shader and add an iff statement, so if the distance in the z-buffer is equal to 1, 
    // meaning that the fragment is out of the texture border (or further than the far clip plane) 
    // then the shadow value is 0.
    //*/

    //*/ Task 3.2 Continue
    // Attaching the texture to the framebuffer, so that it will monitor the depth component
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);


    // Since the depth buffer is only for the generation of the depth texture, 
    // there is no need to have a color output
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    //-------------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------------------------------------
}

void free() {
    delete sphere;
    delete avatar;
    delete box;
    //delete msd;
    glDeleteProgram(shaderProgram);
    glDeleteProgram(depthProgram);
    //glDeleteProgram(miniMapProgram);
    glfwTerminate();
}

void createPlatform(Box* box,float z, float y) {
    // ----------------------------------------------------------------- //
    // --------------------- Drawing scene objects --------------------- //	
    // ----------------------------------------------------------------- //

    float currentTime = static_cast<float>(glfwGetTime());
    float dt = currentTime - t;

    t += dt;
   
    //box1 
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(useTextureLocation, 1);
    glUniform1i(swapLocation, 1);
    glBindTexture(GL_TEXTURE_2D, GrassTexture);

    glUniform1i(GrassSampler, 0);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &box->modelMatrix[0][0]);

    box->update(t, dt);
    box->draw();

    // Task 1: cube simulation (linear motion Newton's First Law)

    // A rigid body can rotate and translate, while the Newton-Euler
    // equations define its dynamics. In order to visualize the cube one
    // must first advance the equations in time (numerical integration) and
    // then use the model matrix to position the drawable in space.

    ////------TREES---------
    glActiveTexture(GL_TEXTURE3);
    glUniform1i(useTextureLocation, 3);
    glBindTexture(GL_TEXTURE_2D, KormosTexture);
    glUniform1i(KormosSampler, 3);
    glUniform1i(swapLocation, 1);

    //mat4 treeS = scale(mat4(), vec3(0.004f, 0.004f, 0.004f));

    tree->bind();
    for (int tt = -3; tt <= 6; tt += 3 * 2.5) {
        for (int t = -20; t <= 20; t += 8.5) {
           
            mat4 treeT = translate(mat4(1.0f), vec3(tt - 1, y , t+z));
            mat4 treeMatrix = treeT ;
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &treeMatrix[0][0]);
            tree->draw();
        }
    }
    
    

    /////________________________MUSHROOM 1_____________________
    glActiveTexture(GL_TEXTURE4);
    glUniform1i(useTextureLocation, 4);
    glUniform1i(swapLocation, 1);
    glBindTexture(GL_TEXTURE_2D, MushroomTexture);
    glUniform1i(MushroomSampler, 4);
    mushroom->bind();
    for (const vec3& position : mushroomPositions) {
        mat4 scalemushroom = scale(mat4(), vec3(0.5f, 0.6f, 0.5f));
        mat4 mushroomT = translate(mat4(1.0f), position);
        mat4 mushroomMatrix = mushroomT * scalemushroom;
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mushroomMatrix[0][0]);
        mushroom->draw();
    }
    //________________________MUSHROOM 2_____________________
    glActiveTexture(GL_TEXTURE5);
    glUniform1i(useTextureLocation, 5);
    glUniform1i(swapLocation, 1);
    glBindTexture(GL_TEXTURE_2D, MushroomTexture2);
    glUniform1i(MushroomSampler2, 5);
    mushroom2->bind();
    for (int tt = -2.5; tt <= 6; tt += 2 * 2.5) {
        for (int t = -20; t <= 20; t += 6) {
            mat4 scalemushroom2 = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
            mat4 mushroomT2 = translate(mat4(1.0f), vec3(tt - 1, y + 0.4, t + z));
            mat4 mushroomMatrix2 = mushroomT2* scalemushroom2;
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mushroomMatrix2[0][0]);
            mushroom2->draw();
        }
    }
    //________________________MUSHROOM 3_____________________
    glActiveTexture(GL_TEXTURE6);
    glUniform1i(useTextureLocation, 6);
    glUniform1i(swapLocation, 1);
    glBindTexture(GL_TEXTURE_2D, MushroomTexture3);
    glUniform1i(MushroomSampler3, 6);
    mushroom3->bind();
    for (int tt = -3; tt <= 6; tt += 3 * 2.5) {
        for (int t = -20; t <= 20; t += 7) {
            mat4 scalemushroom3 = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
            mat4 mushroomT3 = translate(mat4(1.0f), vec3(tt - 1,y+ 0.4, t + z));
            mat4 mushroomMatrix3 = mushroomT3 * scalemushroom3;
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &mushroomMatrix3[0][0]);
            mushroom3->draw();
        }
    }
   
}
void depth_pass(mat4 viewMatrix, mat4 projectionMatrix) {
    float currentTime = static_cast<float>(glfwGetTime());
    float dt = currentTime - t;
    t += dt;


    // Setting viewport to shadow map size
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    // Binding the depth framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);

    // Cleaning the framebuffer depth information (stored from the last render)
    glClear(GL_DEPTH_BUFFER_BIT);

    // Selecting the new shader program that will output the depth component
    glUseProgram(depthProgram);

    // sending the view and projection matrix to the shader
    mat4 view_projection = projectionMatrix * viewMatrix;
    glUniformMatrix4fv(shadowViewProjectionLocation, 1, GL_FALSE, &view_projection[0][0]);


    // ---- rendering the scene ---- //
    //avatar
    avatar->update(t, dt);
    glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &avatar->modelMatrix[0][0]);
    avatar->draw();

    createPlatform(box2, box2->p.z, box2->p.y);
    createPlatform(box, box->p.z, box->p.y);

    // light
    ///light1
    mat4 light1modelMatrix = translate(mat4(), light->lightPosition_worldspace) * scale(mat4(), vec3(0.1f));
    glUniformMatrix4fv(shadowModelLocation, 1, GL_FALSE, &light1modelMatrix[0][0]);

    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &sphere->modelMatrix[0][0]);
    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //*/
}
static void lighting_pass(mat4 viewMatrix, mat4 projectionMatrix) {
    float currentTime = static_cast<float>(glfwGetTime());
    float dt = currentTime - t;
    t += dt;

    // Step 1: Binding a frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    // Step 2: Clearing color and depth info
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Step 3: Selecting shader program
    glUseProgram(shaderProgram);

    // Making view and projection matrices uniform to the shader program
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    glUniform2f(ScaleMatrixLocation, 10.0f, 10.0f);

    uploadLight(*light);

    // Task 4.1 Display shadows on the 
    //*/
    // Sending the shadow texture to the shaderProgram
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(depthMapSampler, 0);

    // Sending the light View-Projection matrix to the shader program
    mat4 lightVP = light->lightVP();
    glUniformMatrix4fv(lightVPLocation, 1, GL_FALSE, &lightVP[0][0]);
    // ------SUN---------
    glUniform1i(useTextureLocation, 0);
    glUniform1i(swapLocation, -1);
    uploadMaterial(Gold);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &sphere->modelMatrix[0][0]);

    sphere->update(t, dt);
    sphere->draw();
    //*/

    // texture 2
    glActiveTexture(GL_TEXTURE2);
    glUniform1i(useTextureLocation, 2);
    glBindTexture(GL_TEXTURE_2D, CatTextureDiff);
    glUniform1i(CatSamplerDiff, 2);
    // Set texture wrapping parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    avatar->update(t, dt);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &avatar->modelMatrix[0][0]);
    avatar->draw();

    

    ////------CLOUDS---------

    glUniform1i(useTextureLocation, 0);
    glUniform1i(swapLocation, -1);
    uploadMaterial(Silver);
    mat4 cloudScaling = scale(mat4(1.0f), vec3(0.005f, 0.005f, 0.005f));

    cloud->bind();

    for (const vec3& position : cloudPositions) {
        mat4 cloudTranslation = translate(mat4(1.0f), position);
        mat4 cloudMatrix = cloudTranslation * cloudScaling;

        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &cloudMatrix[0][0]);
        cloud->draw();
    }
    createPlatform(box2, box2->p.z, box2->p.y);
    createPlatform(box, box->p.z , box->p.y);
  
    glActiveTexture(GL_TEXTURE7);
    glUniform1i(useTextureLocation, 7);
    glBindTexture(GL_TEXTURE_2D, DweTexture);
    glUniform1i(DweSampler, 7);

    dwelling->bind();
    mat4 dwellingR = rotate(mat4(1.0f), -3.14f / 2.0f, vec3(1.0f, 0.0f, 0.0f)) ;
    mat4 dwellingT = translate(mat4(1.0f), vec3(0.0f, 1.0f, -24));
    mat4 dwellingS = scale(mat4(1.0f), vec3(0.01f));
    mat4 dwellingMatrix = dwellingT * dwellingR* dwellingS;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &dwellingMatrix[0][0]);
    dwelling->draw();

    // ------------------------------------------------------------------------------------- SkyBox -----------------------------------------------------------------------------------------------------------------

    glDepthFunc(GL_LEQUAL);  // Make sure the skybox is rendered at the farthest depth
    glUseProgram(SkyboxShaderProgram);

    // Remove translation from the view matrix
    mat4 Skyview = camera->viewMatrix;
    mat4 Skyprojection = camera->projectionMatrix;
    mat4 Skymodel = scale(mat4(1.0f), vec3(250.0f));


    glUniformMatrix4fv(glGetUniformLocation(SkyboxShaderProgram, "view"), 1, GL_FALSE, &Skyview[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(SkyboxShaderProgram, "projection"), 1, GL_FALSE, &Skyprojection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(SkyboxShaderProgram, "model"), 1, GL_FALSE, &Skymodel[0][0]);


    // Render the cube
    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);  // Restore depth function
    // binding the default framebuffer again


}

vec3 dimensionsZY(Box* box) {
    // dimension ZY of box 
    float zmin = box->p.z - (box->scaleFactor.z);
    float zmax = box->p.z + (box->scaleFactor.z);
    
    //float ymin = box->p.y - box->scaleFactor.y;
    float ymax = box->p.y + box->scaleFactor.y;
    return vec3(zmin, zmax,  ymax);
}

void mainLoop() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 10.0f; // Makes corners rounded


    bool isOnPlatform = false;
    
    camera->position = glm::vec3(0, 5, 70);


// ------ values for first Person View ------
    bool firstPersonview = false;
    float h = 2.2f + 0.29f; // distance between the ground & cat's eyes and the ground from 0
    float phi = 0.0f;
    float deltaz = 0.0f;
    float flag = 0.0f;
    float ydim = camera->position.y;

    do {

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_DEPTH_TEST);

        light->update();
        sphere->x = light->lightPosition_worldspace;
        mat4 light_proj = light->projectionMatrix;
        mat4 light_view = light->viewMatrix;
        depth_pass(light_view, light_proj);

        float currentTime = static_cast<float>(glfwGetTime());
        float dt = currentTime - t;


        // Task 2e: change dt to 0.001f and observe the total energy, then change
        // the numerical integration method to Runge - Kutta 4th order (in RigidBody.cpp)


        glUseProgram(shaderProgram);

        uploadLight(*light);
        glUniform1i(useTextureLocation, 0);
        //glUniform1i(swapLocation, -1);

        // camera
        camera->update();
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;


        /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            if (currentTime > 2) {
                avatar->avatar = new Drawable("models/cat_armature2.obj");
            }

            else { avatar->avatar = new Drawable("models/cat_armature.obj"); }
        }*/
        // ------------------------------------------------------------------------------------ Forward Movemnt For Avatar (W) ------------------------------------------------------------------------------------
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && camera->Tpressed == false && camera->Onepressed == false) {
            avatar->x.z = camera->position.z - 15.0f;
           
        }
        else if (glfwGetKey(camera->window, GLFW_KEY_W) == GLFW_PRESS && (camera->Tpressed == true || camera->Onepressed == true)) {
            // 15.0f is the distance between the camera and avatar in z axis 
            if (camera->Tpressed == true) {
                avatar->x.z = camera->position.z;
            }
            else if (camera->Onepressed == true) {
                avatar->x.z = camera->position.z + 1.5f;
            }

        }

        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------ First Person View -----------------------------------------------------------------------------------------------
        if (firstPersonview) {   
            float c = camera->deltaTime;
            if (deltaz - (15.0f + h) > - 0.0001 ) {
                c = 0;            
            }
            camera->position = vec3(camera->position.x , camera->position.y - (camera->position.y - h)  * camera->deltaTime *3.5, camera->position.z - (15.0f + h) * c);
            deltaz += (15.0f + h) * camera->deltaTime;
            if (deltaz > (15.0f + h) && camera->position.y - h < 0.0001) {
                //position.y = avatar->x.y
                //flag = 2.0f;
                firstPersonview = false;
             
            }
         
        }
        if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
            camera->Onepressed = true;
            if (!firstPersonview) {
                firstPersonview = true;

            }
        }
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            lighting_pass(light_view, light_proj);
        }
        else {
            // Render the scene from camera's perspective
            lighting_pass(viewMatrix, projectionMatrix);
        }
        

        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        
        // ----------------------------- make it seem CartoonLike ------------------------------
        if (camera->cartoonLike == true) {
            glUseProgram(shaderProgram);
            glUniform1i(cartoonLocation, 1);
            glUseProgram(SkyboxShaderProgram);
            glUniform1i(cartoonLocation1, 1);
        }
        else {
            glUseProgram(shaderProgram);
            glUniform1i(cartoonLocation, 0);
            glUseProgram(SkyboxShaderProgram);
            glUniform1i(cartoonLocation1, 0);
        }
        //platform1 
        vec3 box_1 = dimensionsZY(box);
        vec3 box_2 = dimensionsZY(box2);
        // check for 1rst platform
        if ((avatar->x.z >= box_1.x && avatar->x.z <= box_1.y) ){
            isOnPlatform = true;    
            avatarGroundLevel = box_1.z;           
        }
        
        // check for 2nd platform
        else if (avatar->x.z >= box_2.x && avatar->x.z <= box_2.y) {
            isOnPlatform = true;
            avatarGroundLevel = box_2.z;
            //camera->position.y = ydim +  avatarGroundLevel;
            }
        
        else {
            avatar->forcing = [&](float t, const std::vector<float>& y) -> std::vector<float> {
            std::vector<float> f(6, 0.0f);
            f[1] = -avatar->m * g; // Apply gravity
            isOnPlatform = false;
            return f;
            };
        }
        if (currentTime < 8) {
            renderHelpingWindow();
        }

        else if (avatar->x.z <= box_2.x+2) {
            gameover = true;
            renderHelpingWindow();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        t += dt;

        
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}

void initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
            " If you have an Intel GPU, they are not 3.3 compatible." +
            "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

    // Gray background color
    glClearColor(0.1f, 0.1f, 0.3f, 0.0f);
    glClearColor(0.18f, 0.31f, 0.466f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    // glFrontFace(GL_CCW);

    // enable point size when drawing points
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Log
    logGLParameters();

    // Create camera
    camera = new Camera(window);
    
    light = new Light(window,
        vec4{ 1, 1, 1, 1 },
        vec4{ 1, 1, 1, 1 },
        vec4{ 1, 1, 1, 1 },        
        lightPos,
        20.0f
    );

}

int main(void) {
    try {
        
       // std::srand(static_cast<unsigned int>(std::time(nullptr)));
        initializeCloudPositions(50, 13.0f);
        initializeMushRoomdPositions(20, 0.4f);
        initialize();
        createContext();

        
        mainLoop();
        
        free();
        
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
        getchar();
        free();
        return -1;
    }

    return 0;
}