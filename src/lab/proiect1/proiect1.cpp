#include "lab/proiect1/proiect1.h"
#include "lab/lab08/lab08.h"
#include <stb/stb_image.h>

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Proiect1::Proiect1()
{
    controlled_light_source_index = 0;

    angle = 0;
    wall_angle = 90;
    multiple_lights = false;
    shading_mode = 1;
    shader_used = "LabShader";
}


Proiect1::~Proiect1()
{
}


void Proiect1::Init()
{
    
    // shadere 
    
    // simplu

    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    
    // normal map fara tbn
    {
        Shader *shader = new Shader("NormalMap2DShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "NormalMapping2D.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    
    // normal map
    {
        Shader *shader = new Shader("NormalMapShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "NormalMapping.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    
    // parallax simplu
    {
        Shader *shader = new Shader("ParallaxMapShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "ParallaxMapping.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    // parallax steep
    {
        Shader *shader = new Shader("SteepParallaxMapShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "SteepParallaxMapping.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    // parallax occlusion
    {
        Shader *shader = new Shader("ParallaxOcclusionMapShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect1", "shaders", "ParallaxOcclusionMapping.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    
    // mesh-uri
    
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
        //ComputeTangents(mesh->vertices, mesh->indices);
    }
    
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;

        //ComputeTangents(mesh->vertices, mesh->indices);
    }
    
    {
        Mesh* mesh = new Mesh("wall");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "wall.obj");
        meshes[mesh->GetMeshID()] = mesh;
        std::cout << "Vertices: " << mesh->positions.size() << std::endl;
        std::cout << "Indices: " << mesh->indices.size() << std::endl;
        std::cout << "TexCoords: " << mesh->texCoords.size() << std::endl;
        std::cout << "Tangents" << mesh->tangents[0] << std::endl;
        //ComputeTangents(mesh->vertices, mesh->indices);
    }

    // materiale
    
    materials["wall"] = {
        LoadTexture("src\\lab\\proiect1\\textures\\wall_diffuse.png"),
        LoadTexture("src\\lab\\proiect1\\textures\\wall_normal.png"),
        LoadTexture("src\\lab\\proiect1\\textures\\wall_height.png"),
    };
    

    // Light & material properties
    {
        point_light_positions[9] = glm::vec3(0, 1, 1);
        spot_light_positions[9] = glm::vec3(1, 1, 1);


        for (int i = 0; i < 10; i++) {
            spot_light_directions[i] = glm::vec3(0, -1, 0);
            spot_light_angles[i] = glm::radians (10.0f + rand() % 50);

            point_light_colors[i] = glm::vec3(
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f
            );
            spot_light_colors[i] = glm::vec3(
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f
            );
        }
        
        {
            point_light_colors[9] = glm::vec3(1.0f,1.0f,1.0f);
            spot_light_colors[9] = glm::vec3(1.0f,1.0f,1.0f);
        }


        spot_angular_speed = 50.0f;
        spot_cone_angle_step = 2.0f;

        radius = 5.0f;
        radius_step = 0.1f;
        radius_min = 0.5f;
        radius_max = 0.6f;

    }
}

void Proiect1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Proiect1::Update(float deltaTimeSeconds)
{
    
    // Pereti verticali : aici diferentele nu se vor vedea la fel de tare 
    {
        // perete simplu, fara map-uri
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -1.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["LabShader"], model, &materials["wall"]);
        }
    
        // perete cu normal map, fara tangente
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(2.0f, 1.0f, -1.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["NormalMap2DShader"], model, &materials["wall"]);
        }
    
        // perete cu normal map, fara tangente
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(6.0f, 1.0f, -1.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["NormalMapShader"], model, &materials["wall"]);
        }
        // perete cu parallax map simplu
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(10.0f, 1.0f, -1.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["ParallaxMapShader"], model, &materials["wall"]); 
        }
        // perete cu parallax map steep
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(14.0f, 1.0f, -1.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["SteepParallaxMapShader"], model, &materials["wall"]); 
        }
        // perete cu parallax map occlusion
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(18.0f, 1.0f, -1.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["ParallaxOcclusionMapShader"], model, &materials["wall"]); 
        }
    }
    
    // pereti orizontali pt a vizualiza diferenta facuta de tangente
    {
        // perete simplu, fara map-uri
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(-2.0f, -1.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["LabShader"], model, &materials["wall"]);
        }
    
        // perete cu normal map, fara tangente
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(2.0f, -1.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["NormalMap2DShader"], model, &materials["wall"]);
        }
    
        // perete cu normal map, fara tangente
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(6.0f, -1.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["NormalMapShader"], model, &materials["wall"]);
        }
        // perete cu parallax map simplu
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(10.0f, -1.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["ParallaxMapShader"], model, &materials["wall"]);
        }
        // perete cu parallax map steep
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(14.0f, -1.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["SteepParallaxMapShader"], model, &materials["wall"]);
        }
        // perete cu parallax map occlusion
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(18.0f, -1.0f, 1.0f));
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1, 0, 0));
            RenderSimpleMesh(meshes["wall"], shaders["ParallaxOcclusionMapShader"], model, &materials["wall"]);
        }
    }
    
    // perete stingher cu toggle-uri multiple
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 7.0f));
        model = glm::rotate(model, glm::radians(wall_angle), glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["wall"], shaders[shader_used], model, &materials["wall"]);
    }
    
    
    // am adaugat toggle pt mai multe surse de lumina, mi se pare ca se vad cam aiurea tho deci schimbati daca vreti
    // merge o sg data pt ca apoi raman in shader datele :))))))))))
    if (multiple_lights)
    {
        {
            angle += glm::radians(6.0f) * deltaTimeSeconds;

            for (int i = 0; i < 9; i++) {
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0), angle + i * glm::radians(360.0f) / 9, glm::vec3(0, 1, 0));

                point_light_positions[i] = glm::vec3(glm::mat3(rotation) * glm::vec3(5, 1.5 + sin(Engine::GetElapsedTime() + i/2.0f), 0));
                spot_light_positions[i] = glm::vec3(glm::mat3(rotation) * glm::vec3(3, 1.5 + sin(Engine::GetElapsedTime() + i / 2.0f), 0));
            }
        }
        // Render the point lights in the scene
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, point_light_positions[i]);
            model = glm::scale(model, glm::vec3(0.1f));
            RenderMesh(meshes["sphere"], shaders["LabShader"], model);
        }

        // Render the spot lights in the scene
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, spot_light_positions[i]);
            model = glm::scale(model, glm::vec3(0.1f));
            RenderMesh(meshes["sphere"], shaders["LabShader"], model);
        }
    }
    
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, point_light_positions[9]);
        model = glm::scale(model, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["LabShader"], model);
    }
    
}


void Proiect1::FrameEnd()
{
    // linie comentata sa ascunda grid-ul
    //DrawCoordinateSystem();
}

void Proiect1::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, MyMaterial* material)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);
    
    
    // Send shader uniforms for light & material properties
    
    glUniform1i(glGetUniformLocation(shader->program, "point_lights_count"), 11);
    glUniform3fv(glGetUniformLocation(shader->program, "point_light_positions"), 11, glm::value_ptr(point_light_positions[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "point_light_colors"), 11, glm::value_ptr(point_light_colors[0]));


    glm::vec3 eye_position = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eye_position));

    glm::vec3 material_ka = glm::vec3(1.0f);
    glm::vec3 material_kd = glm::vec3(2.0f);
    glm::vec3 material_ks = glm::vec3(2.0f);
    int material_shininess = 30;
    // TODO(student): Set material property uniforms (shininess, ka, kd, ks)
    glUniform3fv(glGetUniformLocation(shader->program, "material_ka"), 1, glm::value_ptr(material_ka));
    glUniform3fv(glGetUniformLocation(shader->program, "material_kd"), 1, glm::value_ptr(material_kd));
    glUniform3fv(glGetUniformLocation(shader->program, "material_ks"), 1, glm::value_ptr(material_ks));
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), material_shininess);

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
    
    // Send the radius uniform
    int loc_radius = glGetUniformLocation(shader->program, "radius");
    glUniform1f(loc_radius, radius);
    
    // map-uri pentru shadere ! normal map = diffuse + normal, parallax = diffuse + normal + height
    if (material)
    {
        if (material->diffuse)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material->diffuse->GetTextureID());
            glUniform1i(glGetUniformLocation(shader->program, "diffuseMap"), 0);
        }

        if (material->normal)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, material->normal->GetTextureID());
            glUniform1i(glGetUniformLocation(shader->program, "normalMap"), 1);
        }

        if (material->height)
        {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, material->height->GetTextureID());
            glUniform1i(glGetUniformLocation(shader->program, "heightMap"), 2);
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


Texture2D* Proiect1::LoadTexture(const char* imagePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

    return CreateTexture(width, height, channels, imageData);
}

Texture2D* Proiect1::CreateTexture(unsigned int width, unsigned int height,
    unsigned int channels, unsigned char* data)
{
    GLuint textureID = 0;
    unsigned int size = width * height * channels;

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER and MAG_FILTER) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();


    // TODO(student): Use the "glTexImage2D" directive to load the information
    // into the graphics processor's memory. Use the correct format based on
    // the number of components:
    //   - 1 color channel - GL_RED
    //   - 2 color channels - GL_RG
    //   - 3 color channels - GL_RGB
    //   - 4 color channels - GL_RGBA
    GLint internalFormat[] = { GL_NONE, GL_RED, GL_RG, GL_RGB, GL_RGBA };
    GLint pixelFormat[] = { GL_NONE, GL_RED, GL_RG, GL_RGB, GL_RGBA };

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[channels], width, height, 0, pixelFormat[channels], GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

// nota aici :
// aici sunt calculate tangentele manual, ar fi de revazut; am ajuns sa nu mai testez metoda asta pt ca am optat
// in schimb ca atunci cand imi incarc un mesh dintr-un fisier, sa setez inca un flag pt
// a calcula tangentele si de acolo iau ce este necesar pt matricea TBN 
// daca vrea cnv sa faca partea asta sa mearga ar fi de umblat la vertecsi pt ca cu loadmesh pare ca au alt nume
// si ar fi de trimis catre shader, ceea ce eu cu metoda mea incarc direct catre gpu atunci cand incarc mesh-ul

void Proiect1::ComputeTangents(std::vector<VertexFormat>& vertices,
                     const std::vector<unsigned int>& indices)
{
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        VertexFormat& v0 = vertices[indices[i]];
        VertexFormat& v1 = vertices[indices[i + 1]];
        VertexFormat& v2 = vertices[indices[i + 2]];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;

        glm::vec2 deltaUV1 = v1.text_coord- v0.text_coord;
        glm::vec2 deltaUV2 = v2.text_coord - v0.text_coord;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        glm::vec3 bitangent;

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // accumulate (important for shared vertices)
        v0.tangent += tangent;
        v1.tangent += tangent;
        v2.tangent += tangent;

        v0.bitangent += bitangent;
        v1.bitangent += bitangent;
        v2.bitangent += bitangent;
    }

    // normalize
    for (auto& v : vertices)
    {
        v.tangent = glm::normalize(v.tangent);
        v.bitangent = glm::normalize(v.bitangent);
    }
}

void Proiect1::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        const float speed = 2;

        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        glm::vec3* light_position = nullptr;

        if (controlled_light_source_index == 0) {
            light_position = &point_light_positions[9];
        }

        if (controlled_light_source_index == 1) {
            light_position = &spot_light_positions[9];
        }

        // Control light position using on W, A, S, D, E, Q
        if (window->KeyHold(GLFW_KEY_W)) (*light_position) -= forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_A)) (*light_position) -= right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S)) (*light_position) += forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D)) (*light_position) += right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_E)) (*light_position) += up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_Q)) (*light_position) -= up * deltaTime * speed;
    }

    {
        glm::vec3 &light_direction = spot_light_directions[9];
        float &angle = spot_light_angles[9];
        // TODO(student): Change the lighting direction and angle of the spot
        // light source from the keyboard. From the keys, implement the possibility
        // of rotating the lighting direction relative to the OX and OZ axes, in both
        // directions and the possibility of increasing and decreasing the angle.

        // sus/jos/stanga/dreapta = directie; se face o rotatie
        // sus/jos = rotatie pe axa ox
        // stanga/dreapta = rotatie pe axa oz

        if (window->KeyHold(GLFW_KEY_UP)) light_direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime * spot_angular_speed), glm::vec3(1, 0, 0)) * glm::vec4(light_direction, 1.0f));
        if (window->KeyHold(GLFW_KEY_DOWN)) light_direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-deltaTime * spot_angular_speed), glm::vec3(1, 0, 0)) * glm::vec4(light_direction, 1.0f));
        if (window->KeyHold(GLFW_KEY_LEFT)) light_direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime * spot_angular_speed), glm::vec3(0, 0, 1)) * glm::vec4(light_direction, 1.0f));
        if (window->KeyHold(GLFW_KEY_RIGHT)) light_direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-deltaTime * spot_angular_speed), glm::vec3(0, 0, 1)) * glm::vec4(light_direction, 1.0f));

        /*// aici se schimba unghiul
        if (window->KeyHold(GLFW_KEY_1)) angle += deltaTime * spot_cone_angle_step;
        if (window->KeyHold(GLFW_KEY_2)) angle -= deltaTime * spot_cone_angle_step;*/

    }
}


void Proiect1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        multiple_lights = !multiple_lights;
    }
    
    if (key == GLFW_KEY_R)
    {
        if (wall_angle <= 90) {
            wall_angle += 30;
        }
        else {
            wall_angle = 0;
        }
    }
    
    if (key == GLFW_KEY_1)
    {
        shader_used = "LabShader";
    }
    if (key == GLFW_KEY_2)
    {
        shader_used = "NormalMap2DShader";
    }
    if (key == GLFW_KEY_3)
    {
        shader_used = "NormalMapShader";
    }
    if (key == GLFW_KEY_4) {
        shader_used = "ParallaxMapShader";
    }
    if (key == GLFW_KEY_5) {
        shader_used = "SteepParallaxMapShader";
    }
    if (key == GLFW_KEY_6) {
        shader_used = "ParallaxOcclusionMapShader";
    }
    
}


void Proiect1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Proiect1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Proiect1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Proiect1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Proiect1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Proiect1::OnWindowResize(int width, int height)
{
}
