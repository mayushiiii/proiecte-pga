#pragma once

#include <string>
#include <unordered_map>
#include <random>
#include "components/simple_scene.h"
#include "components/transform.h"


namespace lab
{
    class Tema : public gfxc::SimpleScene
    {
    public:
        Tema();
        ~Tema();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D *texture1 = NULL, Texture2D *texture2 = NULL);
        Texture2D* LoadTexture(const char* imagePath);
        Texture2D* CreateTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);
        void CreateFramebuffer(int width, int height);
        
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        
        void SpawnGrape(const glm::vec3& pos);
        void SpawnBanana(const glm::vec3& pos);
        void SpawnCherry(const glm::vec3& pos);
        void SpawnApple(const glm::vec3& pos);
        void SpawnOrange(const glm::vec3& pos);
        void SpawnDurian(const glm::vec3& pos);
        void AddFruitToList(const std::string& type, const glm::vec3& pos, float size);
        
        void DrawPlayer();
        void KillPlayer();
        void CheckPlayerMovement(float deltaTimeSeconds);
        bool CollidesWith2D(float x1, float z1, float x2, float z2, float size1, float size2);

        gfxc::Camera uiCamera;
        
        //glm::mat4 projectionMatrix;
        glm::mat4 orangeModel;
        
        struct apple
        {
            glm::mat4 appleMiddle;
            glm::mat4 appleTip;
            glm::mat4 appleLeaf;
        };
        apple appleModel;
        
        struct cherry
        {
            glm::mat4 cherryMiddle;
            glm::mat4 cherryStem;
            glm::mat4 cherryLeaf;
        };
        cherry cherryModel;
        
        float cherryRotateSpeed;
        float cherryRotationAngle;

        struct banana
        {
            glm::mat4 bananaLeft;
            glm::mat4 bananaRight;
            glm::mat4 bananaStem;
        };
        banana bananaModel;
        
        struct grape
        {
            // array of mat4
            glm::mat4 grapeBits[3];
            glm::mat4 grapeStem;
        };
        grape grapeModel;
        
        float grapeRotateSpeed;
        float grapeRotationAngle;
        
        struct playerBody
        {
            glm::mat4 chest;
            glm::mat4 head;
            glm::mat4 leftArm;
            glm::mat4 rightArm;
            glm::mat4 leftLeg;
            glm::mat4 rightLeg;
        };
    
        struct player
        {
            glm::vec3 position;
            float rotation;
            float speed;
            float angle;
            float angleSpeed;
            bool isMoving;
            int direction;
            int health;
            bool isAlive;
            int totalScore;
            playerBody body;
        };
        player playerModel;
        int speedBoostTimer;
        glm::vec3 cameraOffset;
        glm::vec3 cameraPosition;
        bool messageShowed;
        
        
        struct fruitPosition
        {
            glm::vec3 position;
            std::string type;
            float size;
        };
        std::vector<fruitPosition> fruits;
        
        void RenderFruit(const fruitPosition& f);
        
        float spawnTimer;
        float spawnInterval;
        std::mt19937 rng;
        
        glm::vec3 point_light_positions[20];
        glm::vec3 spot_light_positions[20];
        glm::vec3 spot_light_directions[20];
        glm::vec3 point_light_colors[20];
        glm::vec3 spot_light_colors[20];
        float spot_light_angles[10];

        int controlled_light_source_index;
        glm::vec3 controlled_light_position;
        float angle;

        float spot_angular_speed;
        float spot_cone_angle_step;

        float radius;
        float radius_step;
        float radius_min;
        float radius_max;
        
        std::unordered_map<std::string, Texture2D *> mapTextures;
        
        unsigned int framebuffer_object;
        unsigned int color_texture;
        unsigned int depth_texture;


    };
}   // namespace lab
