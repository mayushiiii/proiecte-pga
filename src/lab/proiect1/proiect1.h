#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include <unordered_map>


namespace lab
{
    class Proiect1 : public gfxc::SimpleScene
    {
     public:
        Proiect1();
        ~Proiect1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        struct MyMaterial {
            Texture2D* diffuse = nullptr;
            Texture2D* normal = nullptr;
            Texture2D* height = nullptr;   // for parallax later
            
        };


        
        void Proiect1::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, MyMaterial* material);
        Texture2D* Proiect1::LoadTexture(const char* imagePath);
        Texture2D* Proiect1::CreateTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);
        void Proiect1::ComputeTangents(std::vector<VertexFormat>& vertices,const std::vector<unsigned int>& indices);

        std::unordered_map<std::string, MyMaterial> materials;
        
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::vec3 point_light_positions[10];
        glm::vec3 spot_light_positions[10];
        glm::vec3 spot_light_directions[10];
        glm::vec3 point_light_colors[10];
        glm::vec3 spot_light_colors[10];
        float spot_light_angles[10];

        int controlled_light_source_index;
        glm::vec3 controlled_light_position;
        float angle;
        float wall_angle;

        float spot_angular_speed;
        float spot_cone_angle_step;

        float radius;
        float radius_step;
        float radius_min;
        float radius_max;

    };
}   // namespace lab
