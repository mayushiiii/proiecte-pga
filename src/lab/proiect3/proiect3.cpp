#include "lab/proiect3/proiect3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Proiect3::Proiect3()
{
    controlled_light_source_index = 0;
    angle = 0;
    show_lights = false;
    resolution = 256;
    polygon = false;
}


Proiect3::~Proiect3()
{
}

void Proiect3::Init()
{
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    //ocean shaders
    {
        Shader *shader = new Shader("OceanShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect3", "shaders", "OceanVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect3", "shaders", "OceanFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    //initial spectrum compute shader
    {
        Shader *shader = new Shader("InitialSpectrum");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect3", "shaders", "InitialSpectrum.comp.glsl"), GL_COMPUTE_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    //time evolution sompute shader
    {
        Shader *shader = new Shader("TimeEvolutionShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect3", "shaders", "TimeEvolution.comp.glsl"), GL_COMPUTE_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    //fft compute shader
    {
        Shader *shader = new Shader("FFTShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect3", "shaders", "FFT.comp.glsl"), GL_COMPUTE_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    //combine values compute shader
    {
        Shader *shader = new Shader("CombineShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "proiect3", "shaders", "Combine.comp.glsl"), GL_COMPUTE_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        point_light_positions[9] = glm::vec3(0, 1, -5);
        spot_light_positions[9] = glm::vec3(1, 1,-5);


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
            point_light_colors[9] = glm::vec3(1.0f, 1.0f, 1.0f);
            spot_light_colors[9] = glm::vec3(1.0f, 1.0f, 1.0f);
        }

        spot_angular_speed = 50.0f;
        spot_cone_angle_step = 2.0f;

        radius = 5.0f;
        radius_step = 0.1f;
        radius_min = 0.5f;
        radius_max = 0.6f;

    }

    CreateOceanGrid(resolution);
    initial_spectrum_texture = CreateComputeTexture(resolution);
    ping_pong0_texture = CreateComputeTexture(resolution);
    ping_pong1_texture = CreateComputeTexture(resolution);
    displacement_texture = CreateComputeTexture(resolution);

    //vedem ce se intampla de aici (primu compute shader)
    glUseProgram(shaders["InitialSpectrum"]->program);

    glBindImageTexture(0, initial_spectrum_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glUniform1i(glGetUniformLocation(shaders["InitialSpectrum"]->program, "N"), resolution);
    glUniform1f(glGetUniformLocation(shaders["InitialSpectrum"]->program, "L"), 1000.0f);
    glUniform1f(glGetUniformLocation(shaders["InitialSpectrum"]->program, "amplitude"), 2.0f);
    glUniform1f(glGetUniformLocation(shaders["InitialSpectrum"]->program, "windSpeed"), 10.0f);
    glUniform2f(glGetUniformLocation(shaders["InitialSpectrum"]->program, "windDirection"), 1.0f, 1.0f);

    glDispatchCompute(resolution / 16, resolution / 16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    tex_spectrumX = CreateComputeTexture(resolution);
    tex_spectrumY = CreateComputeTexture(resolution);
    tex_spectrumZ = CreateComputeTexture(resolution);    
    
}

void Proiect3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.8, 0.96, 0.98, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Proiect3::Update(float deltaTimeSeconds)
{
    //ocean Time Evolution compute shader
    glUseProgram(shaders["TimeEvolutionShader"]->program);
    glBindImageTexture(0, initial_spectrum_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, tex_spectrumY, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glBindImageTexture(2, tex_spectrumX, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glBindImageTexture(3, tex_spectrumZ, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    
    glUniform1i(glGetUniformLocation(shaders["TimeEvolutionShader"]->program, "N"), resolution);
    glUniform1f(glGetUniformLocation(shaders["TimeEvolutionShader"]->program, "L"), 1000.0f);
    glUniform1f(glGetUniformLocation(shaders["TimeEvolutionShader"]->program, "time"), Engine::GetElapsedTime());

    glDispatchCompute(resolution / 16, resolution / 16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        
    ComputeIFFT2D(tex_spectrumX, ping_pong0_texture, ping_pong1_texture, resolution);
    ComputeIFFT2D(tex_spectrumY, ping_pong0_texture, ping_pong1_texture, resolution);
    ComputeIFFT2D(tex_spectrumZ, ping_pong0_texture, ping_pong1_texture, resolution);
    //displacement_texture = tex_spectrumY;

    glUseProgram(shaders["CombineShader"]->program);
    glBindImageTexture(0, tex_spectrumX, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, tex_spectrumY, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(2, tex_spectrumZ, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(3, displacement_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glUniform1i(glGetUniformLocation(shaders["CombineShader"]->program, "N"), resolution);
    glUniform1f(glGetUniformLocation(shaders["CombineShader"]->program, "choppiness"), 0.2f);
    glDispatchCompute(resolution / 16, resolution / 16, 1);
    glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT);

    
    //ocean
    glm::mat4 ocean = glm::mat4(1);
    ocean = glm::translate(ocean, glm::vec3(0, 0, -10));
    ocean = glm::scale(ocean, glm::vec3(15));
    RenderOceanGrid(shaders["OceanShader"], ocean, glm::vec3(0.02f, 0.16f, 0.25f));

    if (show_lights)
    {
        {
            angle += glm::radians(6.0f) * deltaTimeSeconds;

            for (int i = 0; i < 9; i++) {
                glm::mat4 rotation = glm::rotate(glm::mat4(1.0), angle + i * glm::radians(360.0f) / 9, glm::vec3(0, 1, 0));

                point_light_positions[i] = glm::vec3(glm::mat3(rotation) * glm::vec3(5, 1.5 + sin(Engine::GetElapsedTime() + i / 2.0f), 0));
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


void Proiect3::CreateOceanGrid(int resolution) 
{
    //generate grid
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    for (int z = 0; z <= resolution; z++) 
    {
        for (int x = 0; x <= resolution; x++)
        {
            float u = (float)x / resolution;
            float v = (float)z / resolution;

            float xpos = u * 2.0f - 1.0f;
            float zpos = v * 2.0f - 1.0f;

            VertexFormat vertex(glm::vec3(xpos, 0, zpos), glm::vec3(1), glm::vec3(0, 1, 0), glm::vec2(u, v));

            vertices.push_back(vertex);
        }
    }

    for (int z = 0; z < resolution; z++)
    {
        for (int x = 0; x < resolution; x++)
        {
            int start = z * (resolution + 1) + x;

            indices.push_back(start);
            indices.push_back(start + resolution + 1);
            indices.push_back(start + 1);

            indices.push_back(start + 1);
            indices.push_back(start + resolution + 1);
            indices.push_back(start + resolution + 2);
        }
    }

    oceanIndexCount = (unsigned int)indices.size();

    //vao
    glGenVertexArrays(1, &oceanVAO);
    glBindVertexArray(oceanVAO);

    //vbo
    glGenBuffers(1, &oceanVBO);
    glBindBuffer(GL_ARRAY_BUFFER, oceanVBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), vertices.data(), GL_STATIC_DRAW);

    //ibo
    glGenBuffers(1, &oceanIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oceanIBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, position));

    glEnableVertexAttribArray(0);

    //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, normal));

    glEnableVertexAttribArray(1);

    //uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, text_coord));

    glEnableVertexAttribArray(2);

    //unbind vao
    glBindVertexArray(0);
}

void Proiect3::RenderOceanGrid(Shader* shader, const glm::mat4 & model, const glm::vec3 &object_color)
{
    if (!shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    glUniform1i(glGetUniformLocation(shader->program, "point_lights_count"), 10);

    glUniform3fv(glGetUniformLocation(shader->program, "point_light_positions"), 10, glm::value_ptr(point_light_positions[0]));

    glUniform3fv(glGetUniformLocation(shader->program, "point_light_colors"), 10, glm::value_ptr(point_light_colors[0]));

    glm::vec3 eye_position = GetSceneCamera()->m_transform->GetWorldPosition();

    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eye_position));

    glUniform3fv(glGetUniformLocation(shader->program, "material_ka"), 1, glm::value_ptr(object_color));

    glUniform3fv(glGetUniformLocation(shader->program, "material_kd"), 1, glm::value_ptr(object_color));

    glm::vec3 whiteSpecular = glm::vec3(1.0f);

    glUniform3fv(glGetUniformLocation(shader->program, "material_ks"), 1, glm::value_ptr(whiteSpecular));
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), 120);

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 view = GetSceneCamera()->GetViewMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = GetSceneCamera()->GetProjectionMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projection));

    //displacement texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, displacement_texture);
    glUniform1i(glGetUniformLocation(shader->program, "displacement_map"), 0);

    glBindVertexArray(oceanVAO);
    if(polygon)
    {   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, oceanIndexCount, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, oceanIndexCount, GL_UNSIGNED_INT, 0);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glBindVertexArray(0);
}

GLuint Proiect3::CreateComputeTexture(int resolution)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // GL_RGBA32F is strictly required to store high-precision complex numbers
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, resolution, resolution, 0, GL_RGBA, GL_FLOAT, NULL);

    // Use GL_NEAREST because we are reading exact data points, not blending colors
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void Proiect3::ComputeIFFT2D(GLuint& textureSpectrum, GLuint& pingpong0, GLuint& pingpong1, int resolution)
{
    glUseProgram(shaders["FFTShader"]->program);
    glUniform1i(glGetUniformLocation(shaders["FFTShader"]->program, "N"), resolution);

    int log2N = (int)std::log2(resolution);
    int currentStage = 0;

    // --- Pass A: Copy initial spectrum data into tracking pingpong 0 ---
    // (This step aligns our single channels for localized manipulation loops)
    glCopyImageSubData(textureSpectrum, GL_TEXTURE_2D, 0, 0, 0, 0, pingpong0, GL_TEXTURE_2D, 0, 0, 0, 0, resolution, resolution, 1);

    GLuint srcTex = pingpong0;
    GLuint dstTex = pingpong1;

    // --- 1. HORIZONTAL PASSES (Rows) ---
    glUniform1i(glGetUniformLocation(shaders["FFTShader"]->program, "direction"), 0);
    for (int stage = 0; stage < log2N; stage++) {
        glUniform1i(glGetUniformLocation(shaders["FFTShader"]->program, "stage"), stage);

        glBindImageTexture(0, srcTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, dstTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        glDispatchCompute(resolution / 16, resolution  / 16, 1);

        std::swap(srcTex, dstTex); // Swap buffer allocations
    }

    // --- 2. VERTICAL PASSES (Columns) ---
    glUniform1i(glGetUniformLocation(shaders["FFTShader"]->program, "direction"), 1);
    for (int stage = 0; stage < log2N; stage++) {
        glUniform1i(glGetUniformLocation(shaders["FFTShader"]->program, "stage"), stage);

        glBindImageTexture(0, srcTex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, dstTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        glDispatchCompute(resolution / 16, resolution / 16, 1);

        std::swap(srcTex, dstTex);
    }

    // The output lands dynamically inside whichever pointer is marked as "srcTex" 
    // after the final pointer swap operations are resolved.
    if (srcTex != textureSpectrum) {
        glCopyImageSubData(srcTex, GL_TEXTURE_2D, 0, 0, 0, 0,
            textureSpectrum, GL_TEXTURE_2D, 0, 0, 0, 0, resolution, resolution, 1);
    }
}

void Proiect3::FrameEnd()
{
    //DrawCoordinateSystem();
}

void Proiect3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & model, const glm::vec3 &object_color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    glUniform1i(glGetUniformLocation(shader->program, "point_lights_count"), 10);
    glUniform1i(glGetUniformLocation(shader->program, "point_lights_count"), 10);
    glUniform3fv(glGetUniformLocation(shader->program, "point_light_positions"), 10, glm::value_ptr(point_light_positions[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "point_light_colors"), 10, glm::value_ptr(point_light_colors[0]));


    glm::vec3 eye_position = GetSceneCamera()->m_transform->GetWorldPosition();
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eye_position));

    glm::vec3 material_ka = object_color;
    glm::vec3 material_kd = object_color;
    glm::vec3 material_ks = object_color;
    int material_shininess = 30;

    glUniform3fv(glGetUniformLocation(shader->program, "material_ka"), 1, glm::value_ptr(material_ka));
    glUniform3fv(glGetUniformLocation(shader->program, "material_kd"), 1, glm::value_ptr(material_kd));
    glUniform3fv(glGetUniformLocation(shader->program, "material_ks"), 1, glm::value_ptr(material_ks));
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), material_shininess);

    // Send the model matrix uniform
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

    // Send the view matrix unfirom
    glm::mat4 view = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));

    // Send the projection matrix uniform
    glm::mat4 projection = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

    // Send the radius uniform
    int loc_radius = glGetUniformLocation(shader->program, "radius");
    glUniform1f(loc_radius, radius);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Proiect3::OnInputUpdate(float deltaTime, int mods)
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

        if (window->KeyHold(GLFW_KEY_UP)) light_direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime * spot_angular_speed), glm::vec3(1, 0, 0)) * glm::vec4(light_direction, 1.0f));
        if (window->KeyHold(GLFW_KEY_DOWN)) light_direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-deltaTime * spot_angular_speed), glm::vec3(1, 0, 0)) * glm::vec4(light_direction, 1.0f));
        if (window->KeyHold(GLFW_KEY_LEFT)) light_direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime * spot_angular_speed), glm::vec3(0, 0, 1)) * glm::vec4(light_direction, 1.0f));
        if (window->KeyHold(GLFW_KEY_RIGHT)) light_direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(-deltaTime * spot_angular_speed), glm::vec3(0, 0, 1)) * glm::vec4(light_direction, 1.0f));

        // aici se schimba unghiul
        if (window->KeyHold(GLFW_KEY_1)) angle += deltaTime * spot_cone_angle_step;
        if (window->KeyHold(GLFW_KEY_2)) angle -= deltaTime * spot_cone_angle_step;

    }
}


void Proiect3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_L) {
        show_lights = !show_lights;
    }
    if (key == GLFW_KEY_P) {
        polygon = !polygon;
    }
}


void Proiect3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Proiect3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Proiect3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Proiect3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Proiect3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Proiect3::OnWindowResize(int width, int height)
{
}
