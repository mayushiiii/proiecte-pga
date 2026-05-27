#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"


namespace lab
{
    class Proiect3 : public gfxc::SimpleScene
    {
     public:
        Proiect3();
        ~Proiect3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void CreateOceanGrid(int resolution);
        void RenderOceanGrid(Shader* shader, const glm::mat4& model, const glm::vec3& object_color);
        GLuint CreateComputeTexture(int resolution);
        void ComputeIFFT2D(GLuint& textureSpectrum, GLuint& pingpong0, GLuint& pingpong1, int resolution);

        glm::vec3 point_light_positions[10];
        glm::vec3 spot_light_positions[10];
        glm::vec3 spot_light_directions[10];
        glm::vec3 point_light_colors[10];
        glm::vec3 spot_light_colors[10];
        float spot_light_angles[10];

        int controlled_light_source_index;
        glm::vec3 controlled_light_position;
        float angle;
        bool show_lights = false;

        float spot_angular_speed;
        float spot_cone_angle_step;

        float radius;
        float radius_step;
        float radius_min;
        float radius_max;


        GLuint oceanVAO;
        GLuint oceanVBO;
        GLuint oceanIBO;

        unsigned int oceanIndexCount;
        int resolution;
        GLuint initial_spectrum_texture;
        GLuint ping_pong0_texture;
        GLuint ping_pong1_texture;
        GLuint displacement_texture;
        GLuint tex_spectrumY;
        GLuint tex_spectrumX;
        GLuint tex_spectrumZ;
        bool polygon = false;
    };
}   // namespace lab
