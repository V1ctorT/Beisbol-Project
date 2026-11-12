#pragma once
#include "Publico.h"

class Corredores
{
public:
    glm::vec3 centro     = glm::vec3(0.0f);  // centro del recorrido (y = suelo + ~1.0)
    GLfloat   radioX     = 10.0f;            // semieje en X
    GLfloat   radioZ     = 4.0f;             // semieje en Z
    GLfloat   exponente  = 2.0f;             // 2 = elipse, 4-8 = casi rectángulo
    GLfloat   velocidad  = 4.0f;             // unidades por segundo (negativo = sentido contrario)
    GLfloat   frecuencia = 6.0f;             // velocidad del braceo y las zancadas
    GLfloat   escala     = 1.0f;

    GLfloat rotPista = 0.0f;   // giro de toda la pista en grados (90 = largo a lo largo de Z)
    GLfloat giroModelo = 180.0f;

    void init(const PartesYoshi& p) { partes = p; }

    // Agrega n corredores repartidos uniformemente en el recorrido
    void agregar(int n)
    {
        fases.clear();
        for (int i = 0; i < n; i++)
            fases.push_back(glm::two_pi<float>() * i / n);
    }

    // Llamar con el shader de iluminación ya activo
    void dibujar(GLint modelLoc, Shader& shader, GLfloat t, glm::mat4& base, int yh, Gorras& g)
    {
        // perímetro aproximado (Ramanujan) para que la velocidad sea más o menos constante
        GLfloat a = radioX, b = radioZ;
        GLfloat perimetro = glm::pi<float>() * (3.0f * (a + b) - sqrt((3.0f * a + b) * (a + 3.0f * b)));
        GLfloat omega = velocidad / perimetro * glm::two_pi<float>();


        for (size_t i = 0; i < fases.size(); i++)
        {
            GLfloat th = fases[i] + omega * t;
            glm::vec3 p = punto(th);
            glm::vec3 q = punto(th + (omega >= 0 ? 0.02f : -0.02f));   // punto siguiente = hacia dónde mira
            GLfloat rotY = glm::degrees(atan2(q.x - p.x, q.z - p.z)) + giroModelo;

            GLfloat swing = sin(t * frecuencia + fases[i]) * 25.0f;
            GLfloat rebote = fabs(sin(t * frecuencia + fases[i])) * 0.1f;


            glm::mat4 m(1);
            tra = centro + p + glm::vec3(0.0f, rebote, 0.0f);
            m = glm::translate(m, tra);
            m = glm::rotate(m, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
            m = glm::scale(m, glm::vec3(escala));
            glm::mat4 modelMano(1);
            if (yh == 1) {
                dibujarYoshi(modelLoc, shader, partes, m, swing, 0.0f, swing, swing * 3.0f,
                    modelMano, g);
            }
            else {
                dibujarHuesitos(modelLoc, shader, partes, m, swing, 0.0f, swing, swing * 3.0f, 
                    modelMano, g);
            }

        }
    }

private:
    PartesYoshi partes;
    std::vector<GLfloat> fases;

    glm::vec3 punto(GLfloat th) const
    {
        GLfloat c = cos(th), s = sin(th);
        GLfloat k = 2.0f / exponente;
        GLfloat x = radioX * (c >= 0 ? 1.0f : -1.0f) * pow(fabs(c), k);
        GLfloat z = radioZ * (s >= 0 ? 1.0f : -1.0f) * pow(fabs(s), k);
        glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::radians(rotPista), glm::vec3(0, 1, 0));
        return glm::vec3(r * glm::vec4(x, 0.0f, z, 1.0f));
    }
};
