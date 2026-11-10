#pragma once
#include <vector>
#include <cstdlib>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <string>
#include "Shader.h"
#include "Model.h"


typedef struct _frame
{
    // Posicion del cuerpo
    float posX;
    float posY;
    float posZ;

    valoresYoshi rotaciones;

    // Incrementos de posicion
    float incX;
    float incY;
    float incZ;

    //Incrementos de rotacion
    valoresYoshi incrementos;

    //Tiempo acumulado
    float tiempo;

} FRAME;

std::vector<FRAME> KeyFrame;

//Guarda un keyframe---------------------------------------------------------------------
void saveFrame(void)
{
    FRAME frame{};

    frame.posX = posicionPersonaje.x;
    frame.posY = posicionPersonaje.y;
    frame.posZ = posicionPersonaje.z;

    frame.rotaciones = rotsYoshi;

    frame.tiempo = tiempoAnimacion;

    KeyFrame.push_back(frame);

    printf("Frame guardado: %zu | Tiempo: %.2f\n",
        KeyFrame.size() - 1,
        frame.tiempo);
}

//Guarda animacion en archivo------------------------------------------------------------
bool saveAnimation()
{
    std::string nombreArchivo; // 1. Creamos el string que almacenará el nombre

    std::cout << "Introduce el nombre o ruta del archivo: ";

    // 2. Leemos la línea completa desde la consola (acepta espacios)
    std::getline(std::cin, nombreArchivo);

    // 3. Validamos que no esté vacío (opcional pero recomendado)
    if (nombreArchivo.empty()) {
        std::cout << "Error: No introdujiste ningún nombre." << std::endl;
        return 1;
    }

    std::ofstream archivo(nombreArchivo);

    if (!archivo.is_open())
        return false;

    archivo << 1 << "\n";
    archivo << KeyFrame.size() << "\n";

    for (const FRAME& f : KeyFrame)
    {
        archivo
            << f.posX << " "
            << f.posY << " "
            << f.posZ << " "

            << f.rotaciones.rotHead << " "
            << f.rotaciones.rotBody << " "

            << f.rotaciones.rotArmD_Y << " "
            << f.rotaciones.rotArmD_Z << " "

            << f.rotaciones.rotArmI_Y << " "
            << f.rotaciones.rotArmI_Z << " "

            << f.rotaciones.rotLegD << " "
            << f.rotaciones.rotLegI << " "

            << f.rotaciones.rotHandD << " "
            << f.rotaciones.rotHandI << " "
            << f.tiempo << "\n";
    }

    return true;
}

bool loadAnimation()
{
    std::string nombreArchivo; // 1. Creamos el string que almacenará el nombre

    std::cout << "Introduce el nombre o ruta del archivo: ";

    // 2. Leemos la línea completa desde la consola (acepta espacios)
    std::getline(std::cin, nombreArchivo);

    // 3. Validamos que no esté vacío (opcional pero recomendado)
    if (nombreArchivo.empty()) {
        std::cout << "Error: No introdujiste ningún nombre." << std::endl;
        return 1;
    }

    std::ifstream archivo(nombreArchivo);

    if (!archivo.is_open())
        return false;

    int version;
    size_t numFrames;

    archivo >> version >> numFrames;

    if (!archivo || version != 1 || numFrames > 10000)
        return false;

    std::vector<FRAME> nuevosFrames;

    for (size_t i = 0; i < numFrames; i++)
    {
        FRAME f{};

        archivo
            >> f.posX
            >> f.posY
            >> f.posZ

            >> f.rotaciones.rotHead
            >> f.rotaciones.rotBody

            >> f.rotaciones.rotArmD_Y
            >> f.rotaciones.rotArmD_Z

            >> f.rotaciones.rotArmI_Y
            >> f.rotaciones.rotArmI_Z

            >> f.rotaciones.rotLegD
            >> f.rotaciones.rotLegI

            >> f.rotaciones.rotHandD
            >> f.rotaciones.rotHandI
            >> f.tiempo;

        if (!archivo)
            return false;

        nuevosFrames.push_back(f);
    }

    KeyFrame = std::move(nuevosFrames);

    return true;
}

void resetElements(void)
{
    if (KeyFrame.empty())
        return;

    // Posicion del cuerpo
    posicionPersonaje.x = KeyFrame[0].posX;
    posicionPersonaje.y = KeyFrame[0].posY;
    posicionPersonaje.z = KeyFrame[0].posZ;

    // Rotaciones
    rotsYoshi = KeyFrame[0].rotaciones;
}

valoresYoshi calcularIncrementos(valoresYoshi actual,
    valoresYoshi siguiente)
{
    float pasos = 190;
    valoresYoshi inc{};

    inc.rotHead = (siguiente.rotHead - actual.rotHead) / pasos;
    inc.rotBody = (siguiente.rotBody - actual.rotBody) / pasos;

    inc.rotArmD_Y = (siguiente.rotArmD_Y - actual.rotArmD_Y) / pasos;
    inc.rotArmD_Z = (siguiente.rotArmD_Z - actual.rotArmD_Z) / pasos;

    inc.rotArmI_Y = (siguiente.rotArmI_Y - actual.rotArmI_Y) / pasos;
    inc.rotArmI_Z = (siguiente.rotArmI_Z - actual.rotArmI_Z) / pasos;

    inc.rotLegD = (siguiente.rotLegD - actual.rotLegD) / pasos;
    inc.rotLegI = (siguiente.rotLegI - actual.rotLegI) / pasos;

    inc.rotHandD = (siguiente.rotHandD - actual.rotHandD) / pasos;
    inc.rotHandI = (siguiente.rotHandI - actual.rotHandI) / pasos;

    return inc;
}

void interpolation(void)
{
    float pasos = 190;

    if (playIndex + 1 >= KeyFrame.size())
        return;

    FRAME& actual = KeyFrame[playIndex];
    FRAME& siguiente = KeyFrame[playIndex + 1];

    float deltaTiempo = siguiente.tiempo - actual.tiempo;

    if (deltaTiempo <= 0.0f)
        return;

    // Posicion
    actual.incX = (siguiente.posX - actual.posX) / pasos;
    actual.incY = (siguiente.posY - actual.posY) / pasos;
    actual.incZ = (siguiente.posZ - actual.posZ) / pasos;

    // Rotaciones
    actual.incrementos = calcularIncrementos(
        actual.rotaciones,
        siguiente.rotaciones
    );
}

void startAnimation(void)
{
    if (KeyFrame.size() < 2)
        return;

    playIndex = 0;
    pasoActual = 0;

    resetElements();

    tiempoInicio = glfwGetTime();
    tiempoUltimoPaso = tiempoInicio;

    interpolation();

    play = true;
}

void aplicarIncrementos(void)
{
    FRAME& f = KeyFrame[playIndex];

    // Posicion del cuerpo
    posicionPersonaje.x += f.incX;
    posicionPersonaje.y += f.incY;
    posicionPersonaje.z += f.incZ;

    // Cabeza y cuerpo
    rotsYoshi.rotHead += f.incrementos.rotHead;
    rotsYoshi.rotBody += f.incrementos.rotBody;

    // Brazos
    rotsYoshi.rotArmD_Y += f.incrementos.rotArmD_Y;
    rotsYoshi.rotArmD_Z += f.incrementos.rotArmD_Z;

    rotsYoshi.rotArmI_Y += f.incrementos.rotArmI_Y;
    rotsYoshi.rotArmI_Z += f.incrementos.rotArmI_Z;

    // Piernas
    rotsYoshi.rotLegD += f.incrementos.rotLegD;
    rotsYoshi.rotLegI += f.incrementos.rotLegI;

    // Manos
    rotsYoshi.rotHandD += f.incrementos.rotHandD;
    rotsYoshi.rotHandI += f.incrementos.rotHandI;
}

void playAnimation(void)
{
    float pasos = 190;

    if (!play || KeyFrame.size() < 2)
        return;

    double tiempoActual = glfwGetTime();

    FRAME& actual = KeyFrame[playIndex];
    FRAME& siguiente = KeyFrame[playIndex + 1];

    // Duracion de la transicion
    double deltaTiempo = siguiente.tiempo - actual.tiempo;

    if (deltaTiempo <= 0.0)
    {
        play = false;
        return;
    }

    // Duracion de cada paso
    double tiempoPaso = deltaTiempo / pasos;

    // Pasos que ya deberian haberse ejecutado
    int pasosObjetivo = (int)((tiempoActual - tiempoInicio) / tiempoPaso);

    if (pasosObjetivo > pasos)
        pasosObjetivo = pasos;

    // Ejecutar los incrementos pendientes
    while (pasoActual < pasosObjetivo)
    {
        aplicarIncrementos();
        pasoActual++;
    }

    // Termino la transicion
    if (pasoActual >= pasos)
    {
        // Asegurar que los valores sean exactamente los del siguiente frame
        posicionPersonaje.x = siguiente.posX;
        posicionPersonaje.y = siguiente.posY;
        posicionPersonaje.z = siguiente.posZ;

        rotsYoshi = siguiente.rotaciones;

        // Avanzar al siguiente keyframe
        playIndex++;

        // Comprobar si llegamos al ultimo
        if (playIndex >= KeyFrame.size() - 1)
        {
            play = false;
            return;
        }

        // Preparar la siguiente transicion
        pasoActual = 0;

        tiempoInicio += deltaTiempo;

        interpolation();
    }
}

inline void dibujarYoshiKeyframe(GLint modelLoc, Shader& shader, const PartesYoshi& p, glm::mat4& base,
    glm::mat4& modelMano)
{

    glm::mat4 modelCuerpo = base;
    glm::mat4 model = base, modelTemp;

    // Cuerpo y cabeza (mismo pivote)
    modelCuerpo = model = glm::rotate(model, glm::radians(rotsYoshi.rotBody), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.cuerpo->Draw(shader);
    model = glm::rotate(model, glm::radians(rotsYoshi.rotHead), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.cabeza->Draw(shader);


    // Brazo izquierdo + mano
    model = glm::translate(modelCuerpo, glm::vec3(-0.26443f, -0.12442f, -0.015195f));
    model = glm::rotate(model, glm::radians(rotsYoshi.rotArmI_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotsYoshi.rotArmI_Z), glm::vec3(0.0f, 0.0f, 1.0f));   // alzar
    modelTemp = model = glm::translate(model, glm::vec3(-0.15271f, 0.0f, -0.037633f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.brazoL->Draw(shader);

    model = glm::translate(modelTemp, glm::vec3(-0.12210f, -0.00470f, -0.037183f));
    model = glm::rotate(model, glm::radians(rotsYoshi.rotHandI), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-0.17918f, -0.04711f, -0.087989f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.manoL->Draw(shader);

    // Brazo derecho + mano
    model = glm::translate(modelCuerpo, glm::vec3(0.23581f, -0.14962f, 0.11884f));
    model = glm::rotate(model, glm::radians(rotsYoshi.rotArmD_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotsYoshi.rotArmD_Z), glm::vec3(0.0f, 0.0f, 1.0f));    // alzar
    modelTemp = model = glm::translate(model, glm::vec3(0.12805f, 0.01920f, 0.05665f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.brazoR->Draw(shader);

    model = glm::translate(modelTemp, glm::vec3(0.13382f, 0.00130f, 0.05046f));
    model = glm::rotate(model, glm::radians(rotsYoshi.rotHandD), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMano = model = glm::translate(model, glm::vec3(0.22834f, -0.06276f, 0.02576f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.manoR->Draw(shader);

    // Piernas
    model = glm::translate(modelCuerpo, glm::vec3(-0.25734f, -0.70411f, 0.066808f));
    model = glm::rotate(model, glm::radians(rotsYoshi.rotLegI), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(-0.01991f, -0.31619f, -0.090853f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.piernaL->Draw(shader);

    model = glm::translate(modelCuerpo, glm::vec3(0.17779f, -0.68234f, 0.21343f));
    model = glm::rotate(model, glm::radians(rotsYoshi.rotLegD), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.06489f, -0.33566f, -0.07942f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    p.piernaR->Draw(shader);
}

void bateoAqui() {
    // KEYFRAME 1
    tiempoAnimacion = 0.0f;
    posicionPersonaje.y = 0.5f;

    rotsYoshi = {
        0.0f,       // rotHead
        0.0f,       // rotBody
        0.0f,       // rotArmD_Y
        0.0f,       // rotArmD_Z
        0.0f,       // rotArmI_Y
        0.0f,       // rotArmI_Z
        0.0f,       // rotLegD
        0.0f,       // rotLegI
        0.0f,       // rotHandD
        0.0f        // rotHandI
    };
    saveFrame();


    // KEYFRAME 2
    tiempoAnimacion = 1.0f;
    posicionPersonaje.y = 0.5f;

    rotsYoshi = {
        42.5592f,
        0.0f,
        110.0f,
        0.0f,
        -128.0f,
        0.0f,
        40.0f,
        -40.0f,
        110.0f,
        95.0f
    };
    saveFrame();


    // KEYFRAME 3
    tiempoAnimacion = 2.0f;
    posicionPersonaje.y = 0.5f;

    rotsYoshi = {
        69.0f,
        -80.0f,
        76.5f,
        0.0f,
        -128.0f,
        0.0f,
        64.0f,
        -64.0f,
        159.0f,
        95.0f
    };
    saveFrame();


    // KEYFRAME 4
    tiempoAnimacion = 3.0f;
    posicionPersonaje.y = 0.5f;

    rotsYoshi = {
        -44.0f,
        97.0f,
        43.0f,
        0.0f,
        -128.0f,
        0.0f,
        -60.0f,
        64.0f,
        166.0f,
        95.0f
    };
    saveFrame();


    // KEYFRAME 5
    tiempoAnimacion = 4.0f;
    posicionPersonaje.y = 1.26f;

    rotsYoshi = {
        0.0f,
        258.0f,
        43.0f,
        0.0f,
        0.0f,
        0.0f,
        -45.0f,
        -59.0f,
        166.0f,
        95.0f
    };
    saveFrame();


    // KEYFRAME 6
    tiempoAnimacion = 5.0f;
    posicionPersonaje.y = 0.5f;

    rotsYoshi = {
        0.0f,
        360.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        90.0f,
        95.0f
    };
    saveFrame();
}
