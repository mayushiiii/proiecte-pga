#include "lab/lab01/lab01.h"

#include <vector>

using namespace std;
using namespace lab;

Lab01::Lab01()
{
    window->SetSize(1280, 720);
}


Lab01::~Lab01()
{
}

void Lab01::Initialize()
{
    Exercise1();
    Exercise2();
    Exercise3();
    Exercise4();
    Exercise5();
    Exercise6();
}

void Lab01::Exercise1()
{
    SetExercise();

    //TODO(student): Ex. 1 
    {
        SetPanel("1", { -3, -2, 100 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(5, 2);
        glm::vec2 v2(2, 3);
        // vectorul v12 este compunerea dintre v1 si v2
		glm::vec2 v12 = v1 + v2;

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");
		RenderVector(v12, glm::vec3(0, 0, 1), "v12");

        // nu pare sa fie fix ca in cerinta dar daca functia tot are un parametru in plus, why not
        RenderVector(v1, glm::vec3(1, 0, 0), "v1", v2);
        RenderVector(v2, glm::vec3(0, 1, 0), "v2", v1);

    }
}

void Lab01::Exercise2()
{
    SetExercise();

    //TODO(student): Ex. 2
    {
        SetPanel("2", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        {
            glm::vec2 p1(1, 1);
            glm::vec2 p2(3, -3);

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            // nota: diferenta depinde de sensul pe care dorim sa-l obtinem
            // deci daca voiam p12 se facea p2-p1 si vectorul pleca din p1
            RenderVector(p1 - p2, glm::vec3(1, 0, 1), "P21", p2);
        }

        {
            glm::vec2 p3(-1, -2);
            glm::vec2 p4(-5, -4);

            RenderPoint(p3, glm::vec3(1, 0, 0), "P3");
            RenderPoint(p4, glm::vec3(0, 0, 1), "P4");
            RenderVector(p3 - p4, glm::vec3(1, 0, 1), "P43", p4);

        }

        {
            glm::vec2 p5(6, 3);
            glm::vec2 p6(4, 1);

            RenderPoint(p5, glm::vec3(1, 0, 0), "P5");
            RenderPoint(p6, glm::vec3(0, 0, 1), "P6");
            RenderVector(p6 - p5, glm::vec3(1, 0, 1), "P56", p5);

        }

        {
            glm::vec2 p7(5, 4);
            glm::vec2 p8(-3, 4);

            RenderPoint(p7, glm::vec3(1, 0, 0), "P7");
            RenderPoint(p8, glm::vec3(0, 0, 1), "P8");
            RenderVector(p7 - p8, glm::vec3(1, 0, 1), "P78", p8);

        }
    }
}

void Lab01::Exercise3()
{
    SetExercise();

    //TODO(student): Ex. 3
    {
        SetPanel("3", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(4, 3);
        glm::vec2 v2(-3, 3);
        glm::vec2 v3(-6, -2);
        glm::vec2 v4(-2, -4);
        glm::vec2 v5(3, -2);

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");
        RenderVector(v3, glm::vec3(0, 1, 1), "v3");
        RenderVector(v4, glm::vec3(0, 0, 1), "v4");
        RenderVector(v5, glm::vec3(1, 0, 1), "v5");

        // formula vectorului unitate este vectorul / modul vector 
        // deci ar trb sa avem vector / rad(x^2 + y^2)

        //RenderVector(v1 / sqrt((v1.x * v1.x) + (v1.y * v1.y)), glm::vec3(0, 0, 0));
        //RenderVector(v2 / sqrt((v2.x * v2.x) + (v2.y * v2.y)), glm::vec3(0, 0, 0));
        //RenderVector(v3 / sqrt((v3.x * v3.x) + (v3.y * v3.y)), glm::vec3(0, 0, 0));
        //RenderVector(v4 / sqrt((v4.x * v4.x) + (v4.y * v4.y)), glm::vec3(0, 0, 0));
        //RenderVector(v5 / sqrt((v5.x * v5.x) + (v5.y * v5.y)), glm::vec3(0, 0, 0));

        // SAU AS PUTEA **CITI LABORATORUL**
        RenderVector(glm::normalize(v1), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v2), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v3), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v4), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v5), glm::vec3(0, 0, 0));

    }
}

void Lab01::Exercise4()
{
    SetExercise ();

    //TODO(student): Ex. 4
    {
        SetPanel("4", { -3, -2, 100 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(4, 1.5);
        glm::vec2 v2(2, 3);

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");

        // plecand de la formula, unghiul va fi egal cu arccos(v1v2/(|v1||v2|))
        float dot_product = glm::dot(v1, v2);
        float v1_len = glm::length(v1);
        float v2_len = glm::length(v2);

        float vectors_angle = acos(dot_product / (v1_len * v2_len));

        // ne trebuie starting angle deci am putea fie, nu stiu,
        // sa facem aceeasi chestie ca mai sus dar cu v1 si un vec de pe axa ox?
        // fie arctg
        float v1_angle = atan2(v1.y, v1.x);


        RenderArc(v1_angle, vectors_angle, glm::vec3(0, 0, 1));

        // INCERCARE BONUS:
        float v2_angle = atan2(v2.y, v2.x);
        RenderArc(v2_angle, 6.28 - vectors_angle, glm::vec3(1, 0, 0), 1);
        
    }
}

void Lab01::Exercise5()
{
    SetExercise();

    //TODO(student): Ex. 5
    {
        SetPanel("5", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        {
            glm::vec2 p1(1, 1);
            glm::vec2 p2(3, 1);
            glm::vec2 p3(2, 4);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");

            // note: centrul de greutate este la ((x1+x2+x3)/3, (y1+y2+y3)/3) 
            // Aparent se cheama centroid in engleza lol
            // calculam semiperimetrul apoi printam la coordonata aia
            glm::vec2 center((p1.x + p2.x + p3.x)/3, (p1.y+p2.y+p3.y)/3);

            float a = glm::length(p1 - p2);
            float b = glm::length(p1 - p3);
            float c = glm::length(p2 - p3);
            float semiperimeter = (a + b + c) / 2;
            
            float area = sqrt(semiperimeter * (semiperimeter - a) * (semiperimeter - b) * (semiperimeter - c));
            
            // area a trb schimbat in string, am incercat mai multe variante ca-n c dar am datd e asta
            // ma enerveaza ca asa nu pot da precizia la zecimale
            RenderText(center, to_string(area), glm::vec3(0, 0, 0));
        }

        {
            glm::vec2 p1(-1.5, -3.7);
            glm::vec2 p2(-2.5, 3.2);
            glm::vec2 p3(-6.1, 2.9);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");

            glm::vec2 center((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);

            float a = glm::length(p1 - p2);
            float b = glm::length(p1 - p3);
            float c = glm::length(p2 - p3);
            float semiperimeter = (a + b + c) / 2;

            float area = sqrt(semiperimeter * (semiperimeter - a) * (semiperimeter - b) * (semiperimeter - c));

            RenderText(center, to_string(area), glm::vec3(0, 0, 0));

        }

        {
            glm::vec2 p1(2.3, -2.1);
            glm::vec2 p2(7.1, -3.5);
            glm::vec2 p3(7.5, 3.7);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");

            glm::vec2 center((p1.x + p2.x + p3.x) / 3, (p1.y + p2.y + p3.y) / 3);

            float a = glm::length(p1 - p2);
            float b = glm::length(p1 - p3);
            float c = glm::length(p2 - p3);
            float semiperimeter = (a + b + c) / 2;

            float area = sqrt(semiperimeter * (semiperimeter - a) * (semiperimeter - b) * (semiperimeter - c));

            RenderText(center, to_string(area), glm::vec3(0, 0, 0));

        }
    }
}

float AreaCalc(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
    float a = glm::length(p1 - p2);
    float b = glm::length(p1 - p3);
    float c = glm::length(p2 - p3);
    float semiperimeter = (a + b + c) / 2;

    float area = sqrt(semiperimeter * (semiperimeter - a) * (semiperimeter - b) * (semiperimeter - c));
    return area;
}

void Lab01::Exercise6()
{
    SetExercise();

    //TODO(student): Ex. 6
    {
        SetPanel("6", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        glm::vec2 p1(-4, 3);
        glm::vec2 p2(6, 2);
        glm::vec2 p3(1, -4);

        RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

        RenderPoint(p1, glm::vec3(0, 0, 1), "P1");
        RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
        RenderPoint(p3, glm::vec3(0, 0, 1), "P3");

        const float EPSILON = 0.01f;

        std::vector<glm::vec2> check_points{
            glm::vec2(1, 1), glm::vec2(3.21, 3.2),
            glm::vec2(-2.16, 2.9), glm::vec2(2, -2),
            glm::vec2(-1.1, -0.9), glm::vec2(-1.9, 2.1),
            glm::vec2(-4, 0), glm::vec2(-4, -2.5),
            glm::vec2(4.5, 1.1), glm::vec2(5.9, -3.1),
            glm::vec2(7.1, 1.9), glm::vec2(-5.9, 1.5)
        };

        for (const glm::vec2 &check_point : check_points) {
            // sunt multe arii aici, nu m-as pune sa le calculez de fiecare data deci hai sa fac o functie

            bool inside_triangle = abs(AreaCalc(p1, p2, p3) - (AreaCalc(check_point, p1, p3) + AreaCalc(check_point, p1, p2) +
                AreaCalc(check_point, p2, p3))) < EPSILON;
            if (inside_triangle) {
                RenderPoint(check_point, glm::vec3(0, 1, 0), "in");
            }
            else
            {
                RenderPoint(check_point, glm::vec3(1, 0, 0), "out");
            }
        }
    }
}
