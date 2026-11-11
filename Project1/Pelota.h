#pragma once
#include <vector>
#include <cstdlib>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include "Shader.h"
#include "Model.h"

struct Pelota
{
    glm::vec3 posicion;
    glm::vec3 velocidad;
    glm::vec3 aceleracion;

    float radio;

    bool activa;
    bool bateada;
};

Pelota pelota = {
    glm::vec3(0.0f),
    glm::vec3(0.0f),
    glm::vec3(0.0f),
    0.15f,
    false,
    false
};

void lanzarPelota()
{
    pelota.posicion = glm::vec3(15.0f, 1.5461f, -16.658f); //Inicio

    pelota.velocidad = glm::vec3((0.0f, 1.5461f, 0.0f));//Final en 0,1.54,0

    pelota.aceleracion = glm::vec3(0.0f, -9.81f, 0.0f);

    pelota.activa = true;
    pelota.bateada = false;
}

void actualizarPelota(float deltaTime)
{
    if (!pelota.activa)
        return;

    pelota.velocidad += pelota.aceleracion * deltaTime;

    pelota.posicion += pelota.velocidad * deltaTime;
}

bool colisionPelotaBate(glm::mat4& mano)
{
    float radioPelota = 1.0f;
    float radioBate = 1.0f;
    glm::vec3 extremoLocalA(-1.21990f, -0.027327f, -0.70012f);
    glm::vec3 extremoLocalB(0.54982f, 0.365575f, 0.33420f);

    glm::vec3 extremoA = glm::vec3(
        mano * glm::vec4(extremoLocalA, 1.0f)
    );

    glm::vec3 extremoB = glm::vec3(
        mano * glm::vec4(extremoLocalB, 1.0f)
    );

    glm::vec3 AB = extremoB - extremoA;

    float t = glm::dot(pelota.posicion - extremoA, AB) /
        glm::dot(AB, AB);

    t = glm::clamp(t, 0.0f, 1.0f);

    glm::vec3 puntoCercano = extremoA + t * AB;

    float distancia = glm::length(pelota.posicion - puntoCercano);

    return distancia <= radioPelota + radioBate;
}

void batearPelota()
{
    pelota.bateada = true;

    pelota.velocidad = glm::vec3(
        18.0f,
        10.0f,
        -18.0f
    );

    pelota.aceleracion = glm::vec3(
        0.0f,
        -9.81f,
        0.0f
    );
}

void iniciarAnimacionLanzamiento()
{
    playIndexHuesitos = 0;
    lanzamientoDisparado = false;

    pelota.activa = false;
    pelota.bateada = false;

    playHuesitos = true;
}