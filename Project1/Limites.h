#pragma once
#include <vector>
#include <glm/glm.hpp>

std::vector<glm::vec2> limitesEscenario =
{
    // Parte inicial
    glm::vec2(-28.048f, -11.327f),
    glm::vec2(-28.8207f, -0.609548f),
    glm::vec2(-22.7657f, 12.98f),
    glm::vec2(-12.3025f, 23.3764f),
    glm::vec2(0.933863f, 29.298f),
    glm::vec2(11.7172f, 28.9221f),

    // A partir de aquí se invierte el signo de la segunda componente
    glm::vec2(12.2588f, 35.5623f),
    glm::vec2(34.4774f, 33.9817f),
    glm::vec2(34.0573f, 27.3305f),
    glm::vec2(44.9291f, 26.5568f),
    glm::vec2(64.6836f, 16.9882f),
    glm::vec2(79.2783f, 0.426039f),
    glm::vec2(86.8349f, -19.8372f),
    glm::vec2(86.3482f, -41.647f),
    glm::vec2(77.9949f, -61.9982f),
    glm::vec2(42.418f, -86.083f),
    glm::vec2(-0.074535f, -79.0108f),
    glm::vec2(-25.9714f, -44.5165f),
    glm::vec2(-26.7997f, -33.7f),
    glm::vec2(-33.3847f, -34.1638f),
    glm::vec2(-35.0413f, -11.8899f),
    glm::vec2(-28.4011f, -11.3487f)
};

std::vector<glm::vec2> limitesCorredores1 =
{
    glm::vec2(2.98201f, 12.0375f),
    glm::vec2(3.04201f, 19.5875f),
    glm::vec2(29.212f, 18.0875f),
    glm::vec2(29.212f, 12.5275f)
};

std::vector<glm::vec2> limitesCorredores2 =
{
    glm::vec2(-12.518f, -35.8725f),
    glm::vec2(-18.748f, -35.8725f),
    glm::vec2(-18.748f, -9.4325f),
    glm::vec2(-12.548f, -9.4325f)
};


bool dentroEscenario(glm::vec2 punto)
{
    bool dentro = false;

    int n = limitesEscenario.size();

    for (int i = 0, j = n - 1; i < n; j = i++)
    {
        glm::vec2 A = limitesEscenario[i];
        glm::vec2 B = limitesEscenario[j];

        bool intersecta =
            ((A.y > punto.y) != (B.y > punto.y)) &&
            (punto.x <
                (B.x - A.x) *
                (punto.y - A.y) /
                (B.y - A.y) + A.x);

        if (intersecta)
        {
            dentro = !dentro;
        }
    }

    return dentro;
}

int dentroCorredores(glm::vec2 punto)
{
    int n = limitesCorredores1.size();

    for (int i = 0, j = n - 1; i < n; j = i++)
    {
        glm::vec2 A = limitesCorredores1[i];
        glm::vec2 B = limitesCorredores1[j];

        bool intersecta =
            ((A.y > punto.y) != (B.y > punto.y)) &&
            (punto.x <
                (B.x - A.x) *
                (punto.y - A.y) /
                (B.y - A.y) + A.x);

        if (intersecta)
        {
            return 1; //Corredores Derecha
        }
    }

    n = limitesCorredores2.size();

    for (int i = 0, j = n - 1; i < n; j = i++)
    {
        glm::vec2 A = limitesCorredores2[i];
        glm::vec2 B = limitesCorredores2[j];

        bool intersecta =
            ((A.y > punto.y) != (B.y > punto.y)) &&
            (punto.x <
                (B.x - A.x) *
                (punto.y - A.y) /
                (B.y - A.y) + A.x);

        if (intersecta)
        {
            return 2; //Corredores Izquierda
        }
    }

    return -1; //Si no esta, devuelve -1
}

void moverPersonaje(glm::vec3 direccion,glm::vec3 derecha)
{
    float velocity = 0.05f;

    glm::vec3 nuevaPosicion = posicionPersonaje;

    // W - Adelante
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        nuevaPosicion += direccion * velocity;
    }

    // S - Atrás
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        nuevaPosicion -= direccion * velocity;
    }

    // A - Izquierda
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        nuevaPosicion -= derecha * velocity;
    }

    // D - Derecha
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        nuevaPosicion += derecha * velocity;
    }

    // Comprobar límites del escenario
    if (dentroEscenario(
        glm::vec2(nuevaPosicion.x, nuevaPosicion.z)))
    {
        posicionPersonaje = nuevaPosicion;
    }

}