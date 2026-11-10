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

GLfloat rotBateMuro = -19.0f; //En y

glm::vec3 posicionBates[] = {
    glm::vec3(-27.956f,1.7943f,-0.7471f),
    glm::vec3(- 28.303f,1.7943f,-7.1208f),
    glm::vec3(-28.559f,1.7943f,-3.771f),
    glm::vec3(-28.676f,1.7943f,-1.3227f),
};

glm::vec3 posicionBatesOg[] = {
    glm::vec3(-27.956f,1.7943f,-0.7471f),
    glm::vec3(-28.303f,1.7943f,-7.1208f),
    glm::vec3(-28.559f,1.7943f,-3.771f),
    glm::vec3(-28.676f,1.7943f,-1.3227f),
};

struct Bates {
    Model* bateStel, * bateAl, * bateMadera, * bateHmmm;
};

void dibujarBates(GLint modelLoc, Shader& shader, glm::mat4& mano, int& noBate, Bates& bates) 
{
    Model* modelosBates[] = {
    bates.bateStel,
    bates.bateAl,
    bates.bateMadera,
    bates.bateHmmm
    };
    for (int i = 0; i < 4; i++) //Dibuja todos los bates en una pared
    {
        if (i == noBate)
            continue;

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, posicionBates[i]);

        model = glm::rotate(model, glm::radians(90.0f),glm::vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
            glm::value_ptr(model));

        modelosBates[i]->Draw(shader);
    }

    if (noBate >= 0 && noBate < 4 || estado == 1) //Dibuja un bate en la mano de yoshi
    {
        if (noBate == -1) noBate = 2;
        glm::mat4 model = mano;

        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
            glm::value_ptr(model));

        modelosBates[noBate]->Draw(shader);
    }
}


int detectarBate(glm::vec3 posicionPersonaje)
{
    glm::vec3 posicionActual = posicionPersonaje +
        glm::vec3(0.0f, 1.159f, 0.0f);

    for (int i = 0; i < 4; i++)
    {
        float distancia = glm::distance(posicionActual, posicionBates[i]);

        if (distancia <= 1.0f)
        {
            return i;
        }
    }
    return -1;
}

void soltarBate(int& noBate)
{
    posicionBates[noBate] = posicionBatesOg[noBate];
    noBate = -1;
    return;
}

