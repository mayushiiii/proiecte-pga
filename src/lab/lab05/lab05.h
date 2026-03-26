#pragma once

#include <vector>

#include "components/simple_scene.h"

#include "lab/lab03/transform2D.h"

namespace lab
{
    class Lab05 : public gfxc::SimpleScene
    {
    public:
        Lab05();
        ~Lab05();

        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void Init() override;

    private:
        void DrawObjects(gfxc::Camera* camera, const transform2D::ViewportSpace& viewport_space);
        void DrawPlanets(gfxc::Camera* camera, const transform2D::ViewportSpace& viewport_space);

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
            const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

    protected:
        transform2D::ViewportSpace viewport_space;
        glm::vec3 color;
        GLenum cullFace;

        float cube1_direction = 1.0f;
        float cube1_speed = 5.0f;
        float cube1_position = 0.0f;

        float cube2_angle = 0.0f;
        float cube2_rotate_speed = 100.0f;

        float cube3_scale = 1.0f;
        float cube3_scale_speed = 0.5f;

        bool can_move = true;
        float cube4_speed = 10.0f;
        glm::vec3 cube4_position = { 0.0f, 0.0f, 0.0f };

    };
}   // namespace lab
