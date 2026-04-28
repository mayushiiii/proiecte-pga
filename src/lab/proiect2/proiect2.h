#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"


namespace lab
{
    class Proiect2 : public gfxc::SimpleScene
    {
    public:
        Proiect2();
        ~Proiect2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        Texture2D* CreateTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = nullptr, Texture2D* texture2 = nullptr);
        Texture2D* CreateStripedTexture();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        Texture2D* LoadTexture(const char* imagePath);
        void LoadShader(const std::string& name);

        std::unordered_map<std::string, Texture2D*> mapTextures;


        GLuint scene_fbo;
        GLuint scene_color_texture;
        GLuint scene_depth_texture;

        void CreateFramebuffer(int width, int height);
        void DrawScene(bool useSSR);
    };
}   // namespace lab
