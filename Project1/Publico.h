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


struct PartesYoshi {
    Model* cuerpo, * cabeza, * brazoL, * manoL, * brazoR, * manoR, * piernaL, * piernaR;
};


// swingBrazo: balanceo adelante/atrás (correr)   alzarBrazo: levantar brazos (0 = a los lados, 90 = rectos hacia arriba)
inline void dibujarYoshi(GLint modelLoc, Shader& shader, const PartesYoshi& p, const glm::mat4& base,
    GLfloat swingBrazo, GLfloat alzarBrazo, GLfloat rotMano, GLfloat rotPierna,
    glm::mat4& modelMano,bool simple = false)
{
    const glm::mat4 modelCuerpo = base;
    glm::mat4 model = base, modelTemp;

    // Cuerpo y cabeza (mismo pivote)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.cuerpo->Draw(shader);
    p.cabeza->Draw(shader);

    if (simple) return;

    // Brazo izquierdo + mano
    model = glm::translate(modelCuerpo, glm::vec3(-0.26443f, -0.12442f, -0.015195f));
    model = glm::rotate(model, glm::radians(swingBrazo), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-alzarBrazo), glm::vec3(0.0f, 0.0f, 1.0f));   // alzar
    modelTemp = model = glm::translate(model, glm::vec3(-0.15271f, 0.0f, -0.037633f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.brazoL->Draw(shader);

    model = glm::translate(modelTemp, glm::vec3(-0.12210f, -0.00470f, -0.037183f));
    model = glm::rotate(model, glm::radians(rotMano), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-0.17918f, -0.04711f, -0.087989f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.manoL->Draw(shader);

    // Brazo derecho + mano
    model = glm::translate(modelCuerpo, glm::vec3(0.23581f, -0.14962f, 0.11884f));
    model = glm::rotate(model, glm::radians(swingBrazo), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(alzarBrazo), glm::vec3(0.0f, 0.0f, 1.0f));    // alzar
    modelTemp = model = glm::translate(model, glm::vec3(0.12805f, 0.01920f, 0.05665f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.brazoR->Draw(shader);

    model = glm::translate(modelTemp, glm::vec3(0.13382f, 0.00130f, 0.05046f));
    model = glm::rotate(model, glm::radians(rotMano), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMano = model = glm::translate(model, glm::vec3(0.22834f, -0.06276f, 0.02576f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.manoR->Draw(shader);

    // Piernas
    model = glm::translate(modelCuerpo, glm::vec3(-0.25734f, -0.70411f, 0.066808f));
    model = glm::rotate(model, glm::radians(rotPierna), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-0.01991f, -0.31619f, -0.090853f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.piernaL->Draw(shader);

    model = glm::translate(modelCuerpo, glm::vec3(0.17779f, -0.68234f, 0.21343f));
    model = glm::rotate(model, glm::radians(-rotPierna), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.06489f, -0.33566f, -0.07942f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.piernaR->Draw(shader);

}
inline void dibujarHuesitos(
    GLint modelLoc,
    Shader& shader,
    const PartesYoshi& p,
    glm::mat4& base,
    GLfloat rotCabeza,
    GLfloat rotacionBrazo,
    GLfloat rotMano,
    GLfloat rotPierna,
    glm::mat4& modelMano,
    bool simple = false)
{

    glm::mat4 modelCuerpo = base;
    glm::mat4 model = base, modelTemp;

    //// CUERPO
    model = modelCuerpo = glm::rotate(
        base,
        glm::radians(-180.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE, glm::value_ptr(modelCuerpo)
    );
    p.cuerpo->Draw(shader);

    // CABEZA
    model = glm::translate(
        modelCuerpo,
        glm::vec3(0.031647f, 0.39327f, 0.19935f)
    );

    model = glm::rotate(
        model,
        glm::radians(rotCabeza),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    model = glm::translate(
        model,
        glm::vec3(0.0f, 0.35028f, 0.0f)
    );

    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE, glm::value_ptr(model)
    );
    p.cabeza->Draw(shader);

    if (simple) return;

    // BRAZO DERECHO
    model = glm::translate(
        modelCuerpo,
        glm::vec3(-0.14468f, 0.22898f, 0.16496f)
    );

    model = glm::rotate(
        model,
        glm::radians(rotacionBrazo),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    modelTemp = model = glm::translate(
        model,
        glm::vec3(-0.17352f, 0.000422f, 0.0f)
    );

    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE, glm::value_ptr(model)
    );
    p.brazoR->Draw(shader);

    // MANO DERECHA
    model = glm::translate(
        modelTemp,
        glm::vec3(0.0f, 0.000422f, 0.0f)
    );

    model = glm::rotate(
        model,
        glm::radians(rotMano),
        glm::vec3(1.0f, 1.0f, 0.0f)
    );

    model = glm::translate(
        model,
        glm::vec3(-0.27616f, -0.00208f, 0.067436f)
    );

    modelMano = model;

    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE, glm::value_ptr(model)
    );
    p.manoR->Draw(shader);

    // BRAZO IZQUIERDO
    model = glm::translate(
        modelCuerpo,
        glm::vec3(0.18415f, 0.22898f, 0.11287f)
    );

    model = glm::rotate(
        model,
        glm::radians(rotacionBrazo),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    modelTemp = model = glm::translate(
        model,
        glm::vec3(0.17352f, 0.000422f, 0.0f)
    );

    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE, glm::value_ptr(model)
    );
    p.brazoL->Draw(shader);

    // MANO IZQUIERDA
    model = glm::translate(
        modelTemp,
        glm::vec3(0.0756f, 0.0f, 0.0f)
    );

    model = glm::rotate(
        model,
        glm::radians(rotMano),
        glm::vec3(1.0f, 1.0f, 0.0f)
    );

    model = glm::translate(
        model,
        glm::vec3(0.24735f, -0.010089f, -0.001561f)
    );

    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE, glm::value_ptr(model)
    );
    p.manoL->Draw(shader);

    // PIERNA DERECHA
    model = glm::translate(
        modelCuerpo,
        glm::vec3(-0.17231f, -0.05575f, 0.04356f)
    );

    model = glm::rotate(
        model,
        glm::radians(rotPierna),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    model = glm::translate(
        model,
        glm::vec3(0.0f, -0.38253f, 0.075f)
    );

    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE, glm::value_ptr(model)
    );
    p.piernaR->Draw(shader);

    // PIERNA IZQUIERDA
    model = glm::translate(
        modelCuerpo,
        glm::vec3(0.17747f, -0.05575f, -0.011843f)
    );

    model = glm::rotate(
        model,
        glm::radians(rotPierna),
        glm::vec3(-1.0f, 0.0f, 0.0f)
    );

    model = glm::translate(
        model,
        glm::vec3(0.0f, -0.32620f, 0.0f)
    );

    glUniformMatrix4fv(
        modelLoc, 1, GL_FALSE, glm::value_ptr(model)
    );
    p.piernaL->Draw(shader);
}

struct Espectador {
    glm::vec3 pos;
    GLfloat rotY;     // hacia dónde mira (grados)
    GLfloat fase;
    GLfloat escala;
};

class Publico
{
public:
    GLfloat distMax = 120.0f;   // más lejos que esto no se dibuja
    GLfloat distLOD = 60.0f;    // más lejos que esto: solo cuerpo y cabeza

    void init(const PartesYoshi& p) { partes = p; }

    GLfloat alturaYoshi = 1.159f;   // del piso al origen del torso (escala 1)

    void agregarLado(glm::vec3 ini, glm::vec3 fin, int n, GLfloat rotY)
    {
        for (int i = 0; i < n; i++) {
            Espectador e;
            GLfloat t = (n > 1) ? (GLfloat)i / (n - 1) : 0.0f;
            e.escala = 0.9f + (rand() % 20) / 100.0f;
            e.pos = glm::mix(ini, fin, t);
            e.pos.y += alturaYoshi * e.escala;
            e.rotY = rotY;
            e.fase = (rand() % 628) / 100.0f;
            lista.push_back(e);
        }
    }

    void bloque4(glm::vec3 ini, glm::vec3 fin, GLfloat rotY)
    {
        agregarLado(ini, fin, 8, rotY);  
    }

    void alternar() { aplaudiendo = !aplaudiendo; }

    void actualizar(GLfloat dt)
    {
        GLfloat objetivo = aplaudiendo ? 1.0f : 0.0f;
        intensidad += (objetivo - intensidad) * glm::min(1.0f, 5.0f * dt);
        if (fabs(intensidad - objetivo) < 0.001f) intensidad = objetivo;
    }

    // Llamar con el shader de iluminación ya activo
    void dibujar(GLint modelLoc, Shader& shader,
        const glm::vec3& camPos, const glm::vec3& camFront, GLfloat tiempo)
    {
        for (const auto& e : lista)
        {
            glm::vec3 d = e.pos - camPos;
            GLfloat dist = glm::length(d);

            if (dist > distMax) continue;
            if (dist > 5.0f && glm::dot(glm::normalize(d), camFront) < -0.2f) continue;

            bool simple = (dist > distLOD);
            GLfloat salto = 0.0f, alzar = 0.0f;

            if (intensidad > 0.0f) {
                salto = intensidad * fabs(sin(tiempo * 6.0f + e.fase)) * 0.6f;
                // brazos arriba (~90°) con un pequeño vaivén de celebración
                if (!simple)
                    alzar = intensidad * (90.0f + 20.0f * sin(tiempo * 10.0f + e.fase * 0.5f));
            }

            glm::mat4 m(1.0f);
            m = glm::translate(m, e.pos + glm::vec3(0.0f, salto, 0.0f));
            m = glm::rotate(m, glm::radians(e.rotY), glm::vec3(0.0f, 1.0f, 0.0f));
            m = glm::scale(m, glm::vec3(e.escala));
            glm::mat4 modelMano(1);
            dibujarYoshi(modelLoc, shader, partes, m, 0.0f, alzar, 0.0f, 0.0f, modelMano,simple);
        }
    }

private:
    PartesYoshi partes;
    std::vector<Espectador> lista;
    bool aplaudiendo = false;
    GLfloat intensidad = 0.0f;
};