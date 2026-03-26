#include "lab/lab02/triangle_rasterizer.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace lab;

void TriangleRasterizer::Rasterize(
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3,
    Image *image,
    DepthImage *depthImage)
{
    float xMin = min(v1.position.x, min(v2.position.x, v3.position.x));
    float yMin = min(v1.position.y, min(v2.position.y, v3.position.y));
    float xMax = max(v1.position.x, max(v2.position.x, v3.position.x));
    float yMax = max(v1.position.y, max(v2.position.y, v3.position.y));

    for (int row = yMin; row < yMax; row++) {
        for (int column = xMin; column < xMax; column++) {

            if (row < 0 || row >= image->GetHeight() ||
                column < 0 || column >= image->GetWidth()) {
                continue;
            }

            glm::vec2 p(column + 0.5f, row + 0.5f);

            if (CheckPointInsideTriangle(p, v1, v2, v3)) {

                float depth = ComputePixelDepth(p, v1, v2, v3);
                float pixelDepth = depthImage->Get(row, column).x;

                if (pixelDepth > depth) {
                    image->Set(row, column, ComputePixelColor (p, v1, v2, v3));
                    depthImage->Set(row, column, glm::vec3(depth, 0, 0));
                }
            }
        }
    }
}


// NOTA: Nu pot vedea laboratorul. Gen nu mi se afiseaza nimic. Help
float TriangleRasterizer::ComputeTriangleArea(
    const glm::vec2 &v1,
    const glm::vec2 &v2,
    const glm::vec2 &v3)
{
    // TODO(student): Ex. 1
    // am facut functia asta in lab 1 si am copiat o aici
    float a = glm::length(v1 - v2);
    float b = glm::length(v1 - v3);
    float c = glm::length(v2 - v3);
    float semiperimeter = (a + b + c) / 2;

    float area = sqrt(semiperimeter * (semiperimeter - a) * (semiperimeter - b) * (semiperimeter - c));


    return area;
}

bool TriangleRasterizer::CheckPointInsideTriangle(
    const glm::vec2 &p,
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3)
{
    // ca mai sus, se aplica aceeasi logica ca la ultimul ex de la labul 1. 
    // doar ca acum avem vertexformat de unde luam pozitia oricum
    const float EPSILON = 5.0f;

    // TODO(student): Ex. 1

    float area1 = ComputeTriangleArea(p, v2.position, v3.position);
    float area2 = ComputeTriangleArea(v1.position, p, v3.position);
    float area3 = ComputeTriangleArea(v1.position, v2.position, p);

    float totalArea = ComputeTriangleArea(v1.position, v2.position, v3.position);

    return abs(totalArea - (area1 + area2 + area3)) < EPSILON;
}

glm::vec3 TriangleRasterizer::ComputePixelColor(
    const glm::vec2 &p,
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3)
{
    // TODO(student): Ex. 2

    // avem formula in laborator si functia de calculat arie.
    float totalArea = ComputeTriangleArea(v1.position, v2.position, v3.position);

    // se det coordonatele baricentrice (presupun ca e o aproximare pt coord centrului pixelului)
    float u = ComputeTriangleArea(p, v1.position, v2.position) / totalArea;
    float v = ComputeTriangleArea(p, v1.position, v3.position) / totalArea;
    float w = ComputeTriangleArea(p, v2.position, v3.position) / totalArea;

    // si apoi culoarea este tot formula data

    glm::vec3 color = u * v3.color + v * v2.color + w * v1.color;
    return color;
}

float TriangleRasterizer::ComputePixelDepth(
    const glm::vec2 &p,
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3)
{
    // TODO(student): Ex. 3

    // se fac coord de mai sus, la fel
    float totalArea = ComputeTriangleArea(v1.position, v2.position, v3.position);
    float u = ComputeTriangleArea(p, v1.position, v2.position) / totalArea;
    float v = ComputeTriangleArea(p, v1.position, v3.position) / totalArea;
    float w = ComputeTriangleArea(p, v2.position, v3.position) / totalArea;

    float z = u * v3.position.z + v * v2.position.z + w * v1.position.z;

    return z;
}
