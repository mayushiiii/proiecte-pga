#pragma once

#include "components/software_rendering_scene.h"

#include "lab/lab02/triangle_rasterizer.h"
#include "lab/lab03/transform2D.h"

namespace lab
{
    class Lab03 : public gfxc::SoftwareRenderScene
    {
     public:
        Lab03();
        ~Lab03();

        void Initialize() override;

     private:
        void DrawShapes();
        void DrawStar();
        void Rasterize(
            const std::vector<VertexFormat> &vertices,
            const std::vector<unsigned int> &indices,
            const glm::mat3 transformation);
        void OnInputUpdate(float deltaTime, int mods) override;

        transform2D::LogicSpace logic_space;
        transform2D::ViewportSpace viewport_space;
    };
}   // namespace lab
