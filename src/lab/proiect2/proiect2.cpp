#include "lab/Proiect2/Proiect2.h"

#include <vector>
#include <string>
#include <iostream>
#include <stb/stb_image.h>

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Proiect2::Proiect2()
{
    scene_fbo = 0;
    scene_color_texture = 0;
    scene_depth_texture = 0;
}


Proiect2::~Proiect2()
{
}


void Proiect2::Init()
{
    //texturi
    mapTextures["crate"] = LoadTexture("src\\lab\\lab08\\images\\crate.jpg");
    mapTextures["earth"] = LoadTexture("src\\lab\\lab08\\images\\earth.png");
    mapTextures["bamboo"] = LoadTexture("assets\\models\\vegetation\\bamboo\\bamboo.png");

    //mesh-uri
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bamboo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo"), "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    //shadere

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Earth");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "objects", "Earth.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "objects", "Earth.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Box");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "objects", "Box.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "objects", "Box.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Floor");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "objects", "Floor.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "objects", "Floor.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("FloorSSR");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "FloorSSR.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect2", "shaders", "FloorSSR.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    glm::ivec2 resolution = window->GetResolution();
    CreateFramebuffer(resolution.x, resolution.y);
}


void Proiect2::CreateFramebuffer(int width, int height)
{
    glGenFramebuffers(1, &scene_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, scene_fbo);

    glGenTextures(1, &scene_color_texture);
    glBindTexture(GL_TEXTURE_2D, scene_color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene_color_texture, 0);

    glGenTextures(1, &scene_depth_texture);
    glBindTexture(GL_TEXTURE_2D, scene_depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, scene_depth_texture, 0);

    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Framebuffer is not complete!" << endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




Texture2D* Proiect2::LoadTexture(const char* imagePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

    return CreateTexture(width, height, channels, imageData);
}


Texture2D* Proiect2::CreateTexture(unsigned int width, unsigned int height,
    unsigned int channels, unsigned char* data)
{
    GLuint textureID = 0;
    unsigned int size = width * height * channels;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    GLint internalFormat[] = { GL_NONE, GL_RED, GL_RG, GL_RGB, GL_RGBA };
    GLint pixelFormat[] = { GL_NONE, GL_RED, GL_RG, GL_RGB, GL_RGBA };

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[channels], width, height, 0, pixelFormat[channels], GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


void Proiect2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Proiect2::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // randam scena intr-un framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, scene_fbo);
    glViewport(0, 0, resolution.x, resolution.y);
    glClearColor(0, 0, 0, 1); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawScene(false); //nu vrem ca color texture sa contina podeaua insasi

    // randam scena pe ecran
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, resolution.x, resolution.y);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawScene(true);
}

void Proiect2::DrawScene(bool useSSR)
{
    // podea (doar in pass-ul final)
    if (useSSR)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -4.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.35f));

        RenderSimpleMesh(meshes["plane"], shaders["FloorSSR"], modelMatrix, nullptr);
    }

    // bambus
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f, 0.0f, -4.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
        RenderSimpleMesh(meshes["bamboo"], shaders["LabShader"], modelMatrix, mapTextures["bamboo"]);
    }

    // cutie
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.85f, -4.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
        RenderSimpleMesh(meshes["box"], shaders["Box"], modelMatrix, mapTextures["crate"]);
    }

    // planeta
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 1.0f, -4.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
        RenderSimpleMesh(meshes["sphere"], shaders["Earth"], modelMatrix, mapTextures["earth"]);
    }
}

void Proiect2::FrameEnd()
{
    //DrawCoordinateSystem();
}


void Proiect2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need
    int time_position = glGetUniformLocation(shader->program, "time");
    glUniform1f(time_position, Engine::GetElapsedTime());

    float scale_factor = 4.0f;
    int scale_factor_position = glGetUniformLocation(shader->program, "scale_factor");
    glUniform1f(scale_factor_position, 4.0f);

    int has_texture_location = glGetUniformLocation(shader->program, "has_texture");
    glUniform1i(has_texture_location, texture1 != nullptr);

	// trimitem informatii in plus pentru shaderul de SSR
    if (shader == shaders["FloorSSR"])
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, scene_color_texture);
        glUniform1i(glGetUniformLocation(shader->program, "colorTexture"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, scene_depth_texture);
        glUniform1i(glGetUniformLocation(shader->program, "depthTexture"), 1);

        glm::ivec2 resolution = window->GetResolution();
        glUniform2f(glGetUniformLocation(shader->program, "screenResolution"), resolution.x, resolution.y);

        glUniformMatrix4fv(glGetUniformLocation(shader->program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glm::mat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);
        glUniformMatrix4fv(glGetUniformLocation(shader->program, "inverseProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(inverseProjectionMatrix));
    }
    else
    {
        if (texture1)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
            glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
        }

        if (texture2)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
            glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
        }
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Proiect2::OnInputUpdate(float deltaTime, int mods)
{
}


void Proiect2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Proiect2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Proiect2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Proiect2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Proiect2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Proiect2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Proiect2::OnWindowResize(int width, int height)
{
}