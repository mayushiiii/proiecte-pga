#include "lab/Tema/Tema.h"
#include "lab/lab05/lab05.h"
#include "lab/lab08/lab08.h"
#include <stb/stb_image.h>
#include <random>
#include <ctime>
#include "lab/tema/tema.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace lab;


Tema::Tema()
{
    controlled_light_source_index = 0;
    angle = 0;
    
}


Tema::~Tema()
{
}


void Tema::SpawnOrange(const glm::vec3& pos)
{
    glm::mat4 model = glm::translate(glm::mat4(1), pos);
    model = glm::scale(model, glm::vec3(0.3f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], model, mapTextures["orange"]);
}

void Tema::SpawnApple(const glm::vec3& pos)
{
    appleModel.appleMiddle = glm::translate(glm::mat4(1), pos);
    appleModel.appleMiddle = glm::scale(appleModel.appleMiddle, glm::vec3(0.3f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], appleModel.appleMiddle, mapTextures["apple"]);
    
    appleModel.appleTip = glm::translate(glm::mat4(1), pos + glm::vec3(0, 0.2f, 0));
    appleModel.appleTip = glm::scale(appleModel.appleTip, glm::vec3(0.03f, 0.3f, 0.03f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], appleModel.appleTip, mapTextures["brown"]);
    
    appleModel.appleLeaf = glm::translate(glm::mat4(1), pos + glm::vec3(0.075f, 0.2f, 0));
    appleModel.appleLeaf = glm::rotate(appleModel.appleLeaf, glm::radians(45.0f), glm::vec3(0, 0, 1));
    appleModel.appleLeaf = glm::scale(appleModel.appleLeaf, glm::vec3(0.2f, 0.06f, 0.01f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], appleModel.appleLeaf, mapTextures["green"]);
}

void Tema::SpawnCherry(const glm::vec3& pos)
{
    cherryModel.cherryMiddle = glm::translate(glm::mat4(1), pos);
    cherryModel.cherryMiddle = glm::rotate(cherryModel.cherryMiddle, cherryRotationAngle, glm::vec3(0, 1, 0));
    cherryModel.cherryMiddle = glm::scale(cherryModel.cherryMiddle, glm::vec3(0.2f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], cherryModel.cherryMiddle, mapTextures["cherry"]);

    cherryModel.cherryStem = glm::translate(glm::mat4(1), pos + glm::vec3(0, 0.2f, 0));
    cherryModel.cherryStem = glm::rotate(cherryModel.cherryStem, cherryRotationAngle, glm::vec3(0, 1, 0));
    cherryModel.cherryStem = glm::scale(cherryModel.cherryStem, glm::vec3(0.03f, 0.5f, 0.03f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], cherryModel.cherryStem, mapTextures["brown"]);
    
    cherryModel.cherryLeaf = glm::translate(glm::mat4(1), pos + glm::vec3(-0.05f, 0.35f, 0));
    cherryModel.cherryLeaf = glm::translate(cherryModel.cherryLeaf, glm::vec3(0.05f, 0, 0));
    cherryModel.cherryLeaf = glm::rotate(cherryModel.cherryLeaf, cherryRotationAngle, glm::vec3(0, 1, 0));
    cherryModel.cherryLeaf = glm::translate(cherryModel.cherryLeaf, glm::vec3(-0.05f, 0, 0));
    cherryModel.cherryLeaf = glm::rotate(cherryModel.cherryLeaf, glm::radians(60.0f), glm::vec3(0, 0, 1));
    cherryModel.cherryLeaf = glm::scale(cherryModel.cherryLeaf, glm::vec3(0.2f, 0.05f, 0.01f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], cherryModel.cherryLeaf, mapTextures["green"]);
}

void Tema::SpawnBanana(const glm::vec3& pos)
{
    bananaModel.bananaLeft = glm::translate(glm::mat4(1), pos + glm::vec3(0.05f, 0.1f, -0.1f));
    bananaModel.bananaLeft = glm::rotate(bananaModel.bananaLeft, glm::radians(90.0f), glm::vec3(0, 0, 1));
    bananaModel.bananaLeft = glm::scale(bananaModel.bananaLeft, glm::vec3(0.1f, 0.4f, 0.1f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], bananaModel.bananaLeft, mapTextures["banana"]);

    bananaModel.bananaRight = glm::translate(glm::mat4(1), pos);
    bananaModel.bananaRight = glm::rotate(bananaModel.bananaRight, glm::radians(90.0f), glm::vec3(0, 0, 1));
    bananaModel.bananaRight = glm::scale(bananaModel.bananaRight, glm::vec3(0.1f, 0.4f, 0.1f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], bananaModel.bananaRight, mapTextures["banana"]);

    bananaModel.bananaStem = glm::translate(glm::mat4(1), pos + glm::vec3(-0.15f, 0.1f, -0.025f));
    bananaModel.bananaStem = glm::scale(bananaModel.bananaStem, glm::vec3(0.05f, 0.1f, 0.05f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], bananaModel.bananaStem, mapTextures["brown"]);

    // update lantern/light
    // TODO: if spawning multiple objects, consider changing this approach
    point_light_positions[9] = glm::vec3(glm::vec3(bananaModel.bananaStem[3])); // stem world position
    point_light_colors[9] = glm::vec3(1, 1, 0);
}

void Tema::SpawnGrape(const glm::vec3& pos)
{
    float color_x = (sin(Engine::GetElapsedTime()) + 1) / 2;
    float color_y = (sin(Engine::GetElapsedTime() + glm::radians(120.0f)) + 1) / 2;
    float color_z = (sin(Engine::GetElapsedTime() + glm::radians(240.0f)) + 1) / 2;
    
    for (int i = 0; i < 3; ++i) {
        grapeModel.grapeBits[i] = glm::translate(glm::mat4(1), glm::vec3(pos.x, pos.y - i * 0.1f, pos.z));
        grapeModel.grapeBits[i] = glm::rotate(grapeModel.grapeBits[i], grapeRotationAngle, glm::vec3(0, 1, 0));
        grapeModel.grapeBits[i] = glm::scale(grapeModel.grapeBits[i], glm::vec3(0.3f - i * 0.1f, 0.2f - i * 0.04f, 0.3f - i * 0.1f));
        RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], grapeModel.grapeBits[i], mapTextures["grape"]);
    }

    grapeModel.grapeStem = glm::translate(glm::mat4(1), pos + glm::vec3(0, 0.1f, 0));
    grapeModel.grapeStem = glm::rotate(grapeModel.grapeStem, grapeRotationAngle, glm::vec3(0, 1, 0));
    grapeModel.grapeStem = glm::scale(grapeModel.grapeStem, glm::vec3(0.02f, 0.3f, 0.02f));
    RenderSimpleMesh(meshes["cube"], shaders["CurvedShader"], grapeModel.grapeStem, mapTextures["brown"]);

    // TODO: if spawning multiple objects, consider changing this approach
    
    point_light_positions[10] = glm::vec3(pos.x, pos.y + 0.2f, pos.z);
    point_light_colors[10] = glm::vec3(color_x, color_y, color_z);
}

void Tema::SpawnDurian(const glm::vec3& pos)
{
    glm::mat4 model = glm::translate(glm::mat4(1), pos);
    model = glm::scale(model, glm::vec3(0.15f));
    //cout << "spawned durian at " << pos << endl;
    RenderSimpleMesh(meshes["durian"], shaders["CurvedShader"], model, mapTextures["brown"]);
}

void Tema::AddFruitToList(const std::string& type, const glm::vec3& pos, float size)
{
    fruitPosition f;
    f.position = pos;
    f.type = type;
    f.size = size;
    fruits.push_back(f);
}

void Tema::RenderFruit(const fruitPosition& f)
{
    if (f.type == "orange") SpawnOrange(f.position);
    else if (f.type == "apple") SpawnApple(f.position);
    else if (f.type == "cherry") SpawnCherry(f.position);
    else if (f.type == "banana") SpawnBanana(f.position);
    else if (f.type == "durian") SpawnDurian(f.position);
    else if (f.type == "grape") SpawnGrape(f.position);
}

bool Tema::CollidesWith2D(float x1, float z1, float x2, float z2, float size1, float size2)
{

    float center1x = x1 + size1 / 2;
    float center2x = x2 + size2 / 2;
    float center1z = z1 + size1 / 2;
    float center2z = z2 + size2 / 2;
    float length1 = size1 / 2;
    float length2 = size2 / 2;
    float distance = glm::distance(glm::vec2(center1x,center1z), glm::vec2(center2x,center2z));
    return distance < length1 + length2;

}

void Tema::DrawPlayer()
{
    // head
    {
        glm::mat4 model = glm::translate(glm::mat4(1), playerModel.position);
        model = glm::rotate(model, playerModel.rotation, glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(0, 0.4f, 0));
        model = glm::scale(model, glm::vec3(0.03f));
        playerModel.body.head = model;
        RenderSimpleMesh(meshes["coffee"], shaders["CurvedShader"], playerModel.body.head, mapTextures["orange"]);
    }

    // chest
    {
        glm::mat4 model = glm::translate(glm::mat4(1), playerModel.position);
        model = glm::rotate(model, playerModel.rotation, glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.5f, 0.7f, 0.25f));
        playerModel.body.chest = model;
        RenderSimpleMesh(meshes["box"], shaders["CurvedShader"], playerModel.body.chest, mapTextures["orange"]);
    }

    // left arm
    {
        glm::mat4 model = glm::translate(glm::mat4(1), playerModel.position);
        model = glm::rotate(model, playerModel.rotation, glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(-0.35f, -0.2f, 0));
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::rotate(model, playerModel.angle, glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.15f, 0.5f, 0.15f));
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        playerModel.body.leftArm = model;
        RenderSimpleMesh(meshes["box"], shaders["CurvedShader"], playerModel.body.leftArm, mapTextures["orange"]);
    }

    // right arm
    {
        glm::mat4 model = glm::translate(glm::mat4(1), playerModel.position);
        model = glm::rotate(model, playerModel.rotation, glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(0.35f, -0.2f, 0));
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::rotate(model, -playerModel.angle, glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.15f, 0.5f, 0.15f));
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        playerModel.body.rightArm = model;
        RenderSimpleMesh(meshes["box"], shaders["CurvedShader"], playerModel.body.rightArm, mapTextures["orange"]);
    }

    // left leg
    {
        glm::mat4 model = glm::translate(glm::mat4(1), playerModel.position);
        model = glm::rotate(model, playerModel.rotation, glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(-0.15f, -0.8f, 0));
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::rotate(model, -playerModel.angle, glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.2f));
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        playerModel.body.leftLeg = model;
        RenderSimpleMesh(meshes["box"], shaders["CurvedShader"], playerModel.body.leftLeg, mapTextures["orange"]);
    }

    // right leg
    {
        glm::mat4 model = glm::translate(glm::mat4(1), playerModel.position);
        model = glm::rotate(model, playerModel.rotation, glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(0.15f, -0.8f, 0));
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::rotate(model, playerModel.angle, glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.2f));
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        playerModel.body.rightLeg = model;
        RenderSimpleMesh(meshes["box"], shaders["CurvedShader"], playerModel.body.rightLeg, mapTextures["orange"]);
    }
}

void Tema::KillPlayer()
{
    playerModel.isMoving = false;
    playerModel.speed = 0.0f;
    playerModel.isAlive = false;
    
    // move the entire body downwards
    playerModel.body.head = glm::translate(playerModel.body.head, glm::vec3(0, 0.5f, 0));
    playerModel.body.chest = glm::translate(playerModel.body.chest, glm::vec3(0, -0.5f, 0));
    playerModel.body.leftArm = glm::translate(playerModel.body.leftArm, glm::vec3(0.5, 0.5f, 0));
    playerModel.body.rightArm = glm::translate(playerModel.body.rightArm, glm::vec3(-0.5f, -0.5f, 0));
    playerModel.body.leftLeg = glm::translate(playerModel.body.leftLeg, glm::vec3(0.5f, -0.5f, 0));
    playerModel.body.rightLeg = glm::translate(playerModel.body.rightLeg, glm::vec3(-0.5f, -0.5f, 0));
    
    //cout << "Player has died!" << endl;
}

void Tema::CheckPlayerMovement(float deltaTimeSeconds)
{
    float body_rotation_angle;
    if (playerModel.speed >= 5.0f) {
        body_rotation_angle = 90.0f;
    }
    else {
        body_rotation_angle = 45.0f;
    }
    playerModel.angle = std::fmod(playerModel.angle, glm::radians(360.0f)); // may be negative
    if (playerModel.isMoving) {
        playerModel.angle += playerModel.angleSpeed * deltaTimeSeconds * playerModel.direction;
        if (playerModel.angle > glm::radians(body_rotation_angle)) {
            playerModel.angle = glm::radians(body_rotation_angle);
            playerModel.direction = -1;
        }
        else if (playerModel.angle < glm::radians(-body_rotation_angle)) {
            playerModel.angle = glm::radians(-body_rotation_angle);
            playerModel.direction = 1;
        }
    }
    else if (playerModel.angle > glm::radians(1.0f)) {
        playerModel.angle -= playerModel.angleSpeed * deltaTimeSeconds;
    }
    else if (playerModel.angle < glm::radians(-1.0f)) {
        playerModel.angle += playerModel.angleSpeed * deltaTimeSeconds;
    }
    else {
        playerModel.angle = 0.0f;
        playerModel.direction = 1;
    }
}

void Tema::Init()
{
    // Create a mesh box using custom data
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0.5, -0.5, 0.5), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0, 0, 1)),
            VertexFormat(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(1, 1, 0)),
            VertexFormat(glm::vec3(0.5, -0.5, -0.5), glm::vec3(1, 0, 1)),
            VertexFormat(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(1, 1, 1)),
            VertexFormat(glm::vec3(0.5, 0.5, -0.5), glm::vec3(0, 0, 0)),
        };

        vector<unsigned int> indices
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };
        
        // Create the mesh from the data
        CreateMesh("cube", vertices, indices);
    }
    
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    {
        Mesh* mesh = new Mesh("heart");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters", "heart-fruit", "source", "Heart Fruit"), "Heart Fruit.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    {
        Mesh* mesh = new Mesh("durian");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters", "fruit", "source"), "Fruit.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    {
        Mesh* mesh = new Mesh("coffee");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters", "coffee-mug", "source", "Canecazip"), "CanecaOBJ.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bunny");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program 
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "Lab07", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "Lab07", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    
    {
        Shader *shader = new Shader("CurvedShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Curvature.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "Lab07", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        point_light_positions[10] = glm::vec3(0, 1, 1);
        spot_light_positions[9] = glm::vec3(1, 1, 1);
        for (int i = 0; i < 11; i++) {
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
        radius = 0.5f;
        radius_step = 0.1f;
        radius_min = 0.5f;
        radius_max = 0.6f;
    
    {
        {
            Texture2D* texture = LoadTexture("src\\lab\\lab08\\images\\water.jpg");
            mapTextures["water"] = texture;
        }
        
        {
            Texture2D* texture = LoadTexture("assets\\textures\\ground.jpg");
            mapTextures["ground"] = texture;
        }
        
        {
            Texture2D* texture = LoadTexture("assets\\textures\\fruit\\orange.jpeg");
            mapTextures["orange"] = texture;
        }
        
        {
            Texture2D* texture = LoadTexture("assets\\textures\\fruit\\apple.jpg");
            mapTextures["apple"] = texture;
        }
        {
            Texture2D* texture = LoadTexture("assets\\textures\\fruit\\cherry.jpg");
            mapTextures["cherry"] = texture;
        }
        
        {
            Texture2D* texture = LoadTexture("assets\\textures\\fruit\\banana.jpg");
            mapTextures["banana"] = texture;
        }
        
        {
            Texture2D* texture = LoadTexture("assets\\textures\\fruit\\grape.jpg");
            mapTextures["grape"] = texture;
        }
        
        {
            Texture2D* texture = LoadTexture("assets\\textures\\fruit\\brown.jpg");
            mapTextures["brown"] = texture;
        }
        
        {
            Texture2D* texture = LoadTexture("assets\\textures\\fruit\\green.jpg");
            mapTextures["green"] = texture;
        }
        
        {
            Texture2D* texture = LoadTexture("assets\\textures\\fruit\\images.jpg");
            mapTextures["dancefloor"] = texture;
        }
        
        color_texture = 0;
        depth_texture = 0;
        
    }
        
    }
    // models init
    {
        orangeModel = glm::mat4(1);
    
        // apple
        {      
            appleModel.appleMiddle = glm::mat4(1);
            appleModel.appleTip = glm::mat4(1);
            appleModel.appleLeaf = glm::mat4(1);
        }
    
        {
            cherryModel.cherryMiddle = glm::mat4(1);
            cherryModel.cherryStem = glm::mat4(1);
            cherryModel.cherryLeaf = glm::mat4(1);
            cherryRotateSpeed = 2.0f;
            cherryRotationAngle = 0.0f;
        }
    
        {
            bananaModel.bananaLeft = glm::mat4(1);
            bananaModel.bananaRight = glm::mat4(1);
            bananaModel.bananaStem = glm::mat4(1);
        }
    
        for (int i = 0; i < 3; i++) {
            grapeModel.grapeBits[i] = glm::mat4(1);
        }
        grapeModel.grapeStem = glm::mat4(1);
    
        grapeRotateSpeed = 2.0f;
        grapeRotationAngle = 0.0f;
    
        playerModel.body.chest = glm::mat4(1);
        playerModel.body.head = glm::mat4(1);
        playerModel.body.leftArm = glm::mat4(1);
        playerModel.body.rightArm = glm::mat4(1);
        playerModel.body.leftLeg = glm::mat4(1);
        playerModel.body.rightLeg = glm::mat4(1);
        playerModel.position = glm::vec3(0, 0.8f, 0);
        playerModel.rotation = 0;
        playerModel.speed = 3.0f;
        playerModel.angle = 0.0f;
        playerModel.angleSpeed = 3.0f; // radians per second
        playerModel.isMoving = false;
        playerModel.direction = 1; // 1 - forward, -1 - backward
        playerModel.health = 3; 
        playerModel.isAlive = true;
        playerModel.totalScore = 0;
    }
    messageShowed = false;
    speedBoostTimer = 0;
    cameraOffset = glm::vec3(0.0f, 2.0f, 5.0f);
    cameraPosition = playerModel.position ;
    rng.seed((unsigned)time(nullptr));
    spawnTimer = 0.0f;
    spawnInterval = 2.0f;
    fruits = std::vector<fruitPosition>();
}

Texture2D* Tema::LoadTexture(const char* imagePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

    return CreateTexture(width, height, channels, imageData);
}

Texture2D* Tema::CreateTexture(unsigned int width, unsigned int height,
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

Mesh* Tema::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Tema::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema::Update(float deltaTimeSeconds)
{
    
    glm::vec3 desiredCameraPos = playerModel.position + cameraOffset;

    cameraPosition = glm::mix(
        cameraPosition,
        desiredCameraPos,
        5.0f * deltaTimeSeconds
    );

    GetSceneCamera()->SetPosition(cameraPosition);

    
    angle += glm::radians(6.0f) * deltaTimeSeconds;

    for (int i = 0; i < 9; i++) {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0), angle + i * glm::radians(360.0f) / 9, glm::vec3(0, 1, 0));

        point_light_positions[i] = glm::vec3(glm::mat3(rotation) * glm::vec3(5, 1.5 + sin(Engine::GetElapsedTime() + i/2.0f), 0));
    }
    
    // animate rotating groups
    cherryRotationAngle += deltaTimeSeconds * cherryRotateSpeed;
    grapeRotationAngle += grapeRotateSpeed * deltaTimeSeconds;
      
    glClearColor(0.1f, 0.1f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    CheckPlayerMovement(deltaTimeSeconds);
    if (speedBoostTimer > 0.0f) {
        //cout << "Speed boost active! Time left: " << speedBoostTimer << " seconds. also time is "<< deltaTimeSeconds << endl;
        playerModel.speed = 8.0f;
        speedBoostTimer -= deltaTimeSeconds* 0.01f;
    }
    else {
        speedBoostTimer = 0.0f;
        playerModel.speed = 3.0f;
    }

    // spawn once every spawnInterval seconds
    spawnTimer += deltaTimeSeconds;
    if (spawnTimer >= spawnInterval) {
        spawnTimer -= spawnInterval;

        // random distributions
        std::uniform_real_distribution<float> distXZ(-20.0f, 20.0f);
        std::uniform_real_distribution<float> scaleDist(0.6f, 1.2f);
        std::uniform_int_distribution<int> typeDist(0, 5);

        int t = typeDist(rng);
        float scale = 1.0f;
        glm::vec3 pos;
        std::string type;

        switch (t) {
        case 0:
            type = "orange";
            pos = glm::vec3(distXZ(rng), 0.5f * scale, distXZ(rng));
            break;
        case 1:
            type = "apple";
            pos = glm::vec3(distXZ(rng), 0.6f * scale, distXZ(rng));
            break;
        case 2:
            type = "cherry";
            pos = glm::vec3(distXZ(rng), 0.5f * scale, distXZ(rng));
            break;
        case 3:
            type = "banana";
            pos = glm::vec3(distXZ(rng), 0.5f * scale, distXZ(rng));
            break;
        case 4:
            type = "durian";
            pos = glm::vec3(distXZ(rng), 0.3f * scale, distXZ(rng));
            break; 
        default:
            type = "grape";
            pos = glm::vec3(distXZ(rng), 0.5f * scale, distXZ(rng));
            break;
        }

        AddFruitToList(type, pos, scale);
    }

    // remove fruits the player collides with
    {
        // approximate player footprint (matches chest width used when rendering)
        const float playerSize = 0.5f;

        for (size_t i = 0; i < fruits.size(); ++i) {
            float fruitSize = (fruits[i].size > 0.0f) ? fruits[i].size : 0.3f; // fallback if size unset


            float playerTopLeftX = playerModel.position.x - playerSize * 0.5f;
            float playerTopLeftZ = playerModel.position.z - playerSize * 0.5f;
            float fruitTopLeftX  = fruits[i].position.x - fruitSize * 0.5f;
            float fruitTopLeftZ  = fruits[i].position.z - fruitSize * 0.5f;

            if (CollidesWith2D(playerTopLeftX, playerTopLeftZ, fruitTopLeftX, fruitTopLeftZ, playerSize, fruitSize)) {
                if (fruits[i].type == "durian") {
                    playerModel.health -= 1;
                    playerModel.totalScore -= 10;
                    if (playerModel.health < 0) playerModel.health = 0;
                }
                else if (fruits[i].type == "grape") {
                    speedBoostTimer = 1000.0f; // 5 seconds of speed boost
                    playerModel.totalScore += 15;
                }
                else if (fruits[i].type == "cherry")
                    {
                    if (playerModel.health < 3) {
                        playerModel.health += 1;
                    }
                    playerModel.totalScore += 10;
                }
                else if (fruits[i].type == "banana") {
                    playerModel.totalScore += 5;
                }
                else {
                    playerModel.totalScore += 10;
                }
                // remove collided fruit
               fruits.erase(fruits.begin() + i);
               --i; // adjust index after erase
            }
        }
    }

    // render all remaining fruits
    for (const auto& f : fruits) {
        RenderFruit(f);
    }
    // Render ground
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0, 0.01f, 0));
        model = glm::scale(model, glm::vec3(2.5f));
        RenderSimpleMesh(meshes["plane"], shaders["CurvedShader"], model, mapTextures["green"]);
    }

    // Render the point lights in the scene
    for (int i = 0; i < 9; i++)
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, point_light_positions[i]);
        model = glm::scale(model, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["Simple"], model);
    }
    
    
    if (playerModel.health > 0)
    {
        DrawPlayer();
    }
    else
    {
        KillPlayer();
        if (!messageShowed) {
            cout << "Game Over! Your total score is: " << playerModel.totalScore << endl;
            messageShowed = true;
        }
    }
    
    glDisable(GL_DEPTH_TEST);
    // show health points on lower corner of screen
    for (int i = 0; i < playerModel.health; i++) {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(playerModel.position.x- 0.2f + i * 0.2f, playerModel.position.y + 1.0f, playerModel.position.z ));
        model = glm::scale(model, glm::vec3(0.1f));
        RenderSimpleMesh(meshes["heart"], shaders["LabShader"], model, mapTextures["apple"]);
    }
    glEnable(GL_DEPTH_TEST);


}


void Tema::FrameEnd()
{
    //DrawCoordinateSystem();
}

void Tema::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);
    
    
    // Send shader uniforms for light & material properties


    // denumirile variabilelor le dau dupa cum sunt si in fragment shader, pt easy access

    glUniform1i(glGetUniformLocation(shader->program, "point_lights_count"), 11);
    glUniform3fv(glGetUniformLocation(shader->program, "point_light_positions"), 11, glm::value_ptr(point_light_positions[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "point_light_colors"), 11, glm::value_ptr(point_light_colors[0]));


    glm::vec3 eye_position = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(playerModel.position));

    glm::vec3 material_ka = glm::vec3(1.0f);
    glm::vec3 material_kd = glm::vec3(1.0f);
    glm::vec3 material_ks = glm::vec3(1.0f);
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
    
    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    if (texture2)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    }
    

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema::CreateFramebuffer(int width, int height)
{
    // TODO(student): In this method, use the attributes
    // 'framebuffer_object', 'color_texture' and 'depth_texture'
    // declared in lab09.h

    // TODO(student): Generate and bind the framebuffer
    glGenFramebuffers(1, &framebuffer_object);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);

    // TODO(student): Generate, bind and initialize the color texture
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // TODO(student): Bind the color texture to the
    // framebuffer as a color attachment at position 0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);

    // TODO(student): Generate, bind and initialize the depth texture
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // TODO(student): Bind the depth texture to the framebuffer as a depth attachment
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

    // TODO(student): Set the color texture as the draw texture
    GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);

    // TODO(student): Check the status of the framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    // Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        const float speed = 2;

        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
        
    }
    
    if (window->KeyHold(GLFW_KEY_LEFT_SHIFT)) {
        playerModel.speed += 3.0f;
    }
    
    if (window->KeyHold(GLFW_KEY_W)) {
        playerModel.position += glm::vec3(0, 0, -1) * playerModel.speed * deltaTime;
        playerModel.rotation = 0.0f;
        playerModel.isMoving = true;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        playerModel.position += glm::vec3(0, 0, 1) * playerModel.speed * deltaTime;
        playerModel.rotation = glm::radians(180.0f);
        playerModel.isMoving = true;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        playerModel.position += glm::vec3(-1, 0, 0) * playerModel.speed * deltaTime;
        playerModel.rotation = glm::radians(90.0f);
        playerModel.isMoving = true;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        playerModel.position += glm::vec3(1, 0, 0) * playerModel.speed * deltaTime;
        playerModel.rotation = glm::radians(-90.0f);
        playerModel.isMoving = true;
    }
    
    if (window->KeyHold(GLFW_KEY_W) && window->KeyHold(GLFW_KEY_A)) {
        playerModel.rotation = glm::radians(45.0f);
    }
    if (window->KeyHold(GLFW_KEY_W) && window->KeyHold(GLFW_KEY_D)) {
        playerModel.rotation = glm::radians(-45.0f);
    }
    if (window->KeyHold(GLFW_KEY_S) && window->KeyHold(GLFW_KEY_A)) {
        playerModel.rotation = glm::radians(135.0f);
    }
    if (window->KeyHold(GLFW_KEY_S) && window->KeyHold(GLFW_KEY_D)) {
        playerModel.rotation = glm::radians(-135.0f);
    }
    
    // if holding no key, player is not moving
    if (!window->KeyHold(GLFW_KEY_W) && !window->KeyHold(GLFW_KEY_S) &&
        !window->KeyHold(GLFW_KEY_A) && !window->KeyHold(GLFW_KEY_D)) {
        playerModel.isMoving = false;
    }
    
    
}

void Tema::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema::OnWindowResize(int width, int height)
{
}
