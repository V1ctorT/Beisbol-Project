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

    //Posicion de la pelota
    float pelX;
    float pelY;
    float pelZ;
    //Incrementos pelota
    float incPelX;
    float incPelY;
    float incPelZ;

    //Rotacion pelota
    float rotPel;
    float incPel;

    //Tiempo acumulado
    float tiempo;

} FRAME;

std::vector<FRAME> KeyFrame; //Yoshi
std::vector<FRAME> KeyFrameHuesitos;

//Guarda un keyframe---------------------------------------------------------------------
void saveFrame(std::vector<FRAME>& KeyFrame, glm::vec3& posicionPersonaje, float& tiempoAnimacion, 
    valoresYoshi& rotsYoshi, bool pel = false)
{
    FRAME frame{};
    frame.posX = posicionPersonaje.x;
    frame.posY = posicionPersonaje.y;
    frame.posZ = posicionPersonaje.z;


    frame.rotaciones = rotsYoshi;

    if (pel) {
        frame.pelX = pelotaPosicion.x;
        frame.pelY = pelotaPosicion.y;
        frame.pelZ = pelotaPosicion.z;
    }
    else {
        frame.pelX = pelotaPP.x;
        frame.pelY = pelotaPP.y;
        frame.pelZ = pelotaPP.z;
    }
    
    frame.tiempo = tiempoAnimacion;

    KeyFrame.push_back(frame);

    printf("Frame guardado: %zu | Tiempo: %.2f\n",
        KeyFrame.size() - 1,
        frame.tiempo);
}

//Guarda animacion en archivo------------------------------------------------------------
bool saveAnimation(std::vector<FRAME>& KeyFrame)
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

bool loadAnimation(std::vector<FRAME>& KeyFrame)
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

void resetElements(std::vector<FRAME>& KeyFrame, glm::vec3& posicionPersonaje,int yoshi)
{
    if (KeyFrame.empty())
        return;

    // Posicion del cuerpo
    posicionPersonaje.x = KeyFrame[0].posX;
    posicionPersonaje.y = KeyFrame[0].posY;
    posicionPersonaje.z = KeyFrame[0].posZ;

    rotPel = KeyFrame[0].rotPel;
    // Rotaciones
    if (yoshi == 1) {
        pelotaPosicion.x = KeyFrame[0].pelX;
        pelotaPosicion.y = KeyFrame[0].pelY;
        pelotaPosicion.z = KeyFrame[0].pelZ;
        rotsYoshi = KeyFrame[0].rotaciones;
    }
    else if (yoshi == 2) {
        pelotaPP.x = KeyFrame[0].pelX;
        pelotaPP.y = KeyFrame[0].pelY;
        pelotaPP.z = KeyFrame[0].pelZ;
        rotsHuesitos = KeyFrame[0].rotaciones;
    }
    else if (yoshi == 3) {
        rotsYoshi = KeyFrame[0].rotaciones;
        rotsHuesitos = KeyFrame[0].rotaciones;
    }
}

valoresYoshi calcularIncrementos(valoresYoshi& actual,
    valoresYoshi& siguiente)
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

    inc.rotValPel = (siguiente.rotValPel - actual.rotValPel) / pasos;

    return inc;
}

void interpolation(std::vector<FRAME>& KeyFrame, int& playIndex)
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
    actual.incPelX = (siguiente.pelX -actual.pelX) / pasos;
    actual.incPelY = (siguiente.pelY - actual.pelY) / pasos;
    actual.incPelZ = (siguiente.pelZ - actual.pelZ) / pasos;
    actual.incPel = (siguiente.rotPel - actual.rotPel) / pasos;

    // Rotaciones
    actual.incrementos = calcularIncrementos(
        actual.rotaciones,
        siguiente.rotaciones
    );
}

void startAnimation(int& playIndex, std::vector<FRAME>& KeyFrame, glm::vec3& posicionPersonaje,
    int& pasoActual, double& tiempoInicio, double& tiempoUltimoPaso, bool& play, int yoshi)
{
    if (KeyFrame.size() < 2)
        return;

    playIndex = 0;
    pasoActual = 0;

    resetElements(KeyFrame,posicionPersonaje,yoshi);

    tiempoInicio = glfwGetTime();
    tiempoUltimoPaso = tiempoInicio;
    interpolation(KeyFrame,playIndex);
    play = true;
}

void startAnimation2(int& playIndex,int& playIndex2, 
    std::vector<FRAME>& KeyFrame, std::vector<FRAME>& KeyFrame2, 
    glm::vec3& posicionPersonaje, int& pasoActual, double& tiempoInicio, 
    double& tiempoUltimoPaso, bool& play, int yoshi)
{
    if (KeyFrame.size() < 2)
        return;

    playIndex = 0;
    pasoActual = 0;

    resetElements(KeyFrame, posicionPersonaje, yoshi);

    tiempoInicio = glfwGetTime();
    tiempoUltimoPaso = tiempoInicio;

    interpolation(KeyFrame, playIndex);
    interpolation(KeyFrame2, playIndex2);
    KeyFrame[playIndex].incPelX = KeyFrame2[playIndex2].incPelX;
    KeyFrame[playIndex].incPelY = KeyFrame2[playIndex2].incPelY;
    KeyFrame[playIndex].incPelZ = KeyFrame2[playIndex2].incPelZ;

    play = true;
}


void aplicarIncrementos(int& playIndex, std::vector<FRAME>& KeyFrame, 
    glm::vec3& posicionPersonaje, valoresYoshi& rotsYoshi)
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
    
    //Pelota
    pelotaPosicion += glm::vec3(f.incPelX, f.incPelY, f.incPelZ);
    rotPel += f.rotPel;
}

void playAnimation(bool& play,int& playIndex, std::vector<FRAME>& KeyFrame, 
    glm::vec3& posicionPersonaje, double& tiempoInicio, int& pasoActual, 
    valoresYoshi& rotsYoshi)
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
        aplicarIncrementos(playIndex,KeyFrame,posicionPersonaje, rotsYoshi);
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

        pelotaPosicion = glm::vec3(siguiente.pelX,siguiente.pelY,siguiente.pelZ);

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

        interpolation(KeyFrame,playIndex);
    }
}

inline void dibujarYoshiKeyframe(GLint modelLoc, Shader& shader, const PartesYoshi& p, 
    glm::mat4& base, glm::mat4& modelMano, int yoshi, Gorras& g)
{
    if (yoshi == 1) {
        glm::mat4 pelota(1);
        pelota = glm::translate(pelota,pelotaPosicion);
        pelota = glm::rotate(pelota, glm::radians(rotPel), glm::vec3(1.0f, 0.0f, 1.0f));
        pelota = glm::scale(pelota, glm::vec3(2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pelota));
        p.pelota->Draw(shader);

        glm::mat4 modelCuerpo = base;
        glm::mat4 model = base, modelTemp;

        // Cuerpo y cabeza (mismo pivote)
        modelCuerpo = model = glm::rotate(model, glm::radians(rotsYoshi.rotBody), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        p.cuerpo->Draw(shader);
        model = glm::rotate(model, glm::radians(rotsYoshi.rotHead), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        p.cabeza->Draw(shader);

        model = glm::translate(modelCuerpo, glm::vec3(-0.1f, 1.0f, 0.4f));
        model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotsYoshi.rotHead), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        g.quico->Draw(shader);


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
    else if (yoshi == 2)
    {

        glm::mat4 pelota(1);
        pelota = glm::translate(pelota, pelotaPosicion);
        pelota = glm::rotate(pelota, glm::radians(rotPel), glm::vec3(1.0f, 0.0f, 1.0f));
        pelota = glm::scale(pelota, glm::vec3(2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pelota));
        p.pelota->Draw(shader);

        glm::mat4 modelCuerpo = base;
        glm::mat4 model = base, modelTemp;

        //// CUERPO
        modelCuerpo = glm::rotate(model,glm::radians(rotsHuesitos.rotBody), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(
            modelLoc, 1, GL_FALSE, glm::value_ptr(modelCuerpo) );
        p.cuerpo->Draw(shader);
        // CABEZA
        model = glm::translate(
            modelCuerpo,
            glm::vec3(0.031647f, 0.39327f, 0.19935f));
        model = glm::rotate(model,glm::radians(rotsHuesitos.rotHead),glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(
        model,glm::vec3(0.0f, 0.35028f, 0.0f));
        glUniformMatrix4fv(
            modelLoc, 1, GL_FALSE, glm::value_ptr(model)
        );
        p.cabeza->Draw(shader);

        //Gorro
        model = glm::translate(modelCuerpo, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotsHuesitos.rotHead), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        g.donRamon->Draw(shader);

        // BRAZO DERECHO
        model = glm::translate(
            modelCuerpo,
            glm::vec3(-0.14468f, 0.22898f, 0.16496f)
        );

        model = glm::rotate(
            model,
            glm::radians(rotsHuesitos.rotArmD_Y),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        model = glm::rotate(
            model,
            glm::radians(rotsHuesitos.rotArmD_Z),
            glm::vec3(0.0f, 0.0f, 1.0f)
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
            glm::radians(rotsHuesitos.rotHandD),
            glm::vec3(1.0f, 0.0f, 0.0f)
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
            glm::radians(rotsHuesitos.rotArmI_Y),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        model = glm::rotate(
            model,
            glm::radians(rotsHuesitos.rotArmI_Z),
            glm::vec3(0.0f, 0.0f, 1.0f)
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
            glm::radians(rotsHuesitos.rotHandI),
            glm::vec3(1.0f, 0.0f, 0.0f)
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
            glm::radians(rotsHuesitos.rotLegD),
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
            glm::radians(rotsHuesitos.rotLegI),
            glm::vec3(1.0f, 0.0f, 0.0f)
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
    
}

void bateoAqui(float& tiempoAnimacion, glm::vec3& posicionPersonaje, std::vector<FRAME>& KeyFrame) {
    // 0 KEYFRAME 1 
    tiempoAnimacion = 0.0f;
    posicionPersonaje.x = -1.73735f;
    posicionPersonaje.y = 1.67006f;
    posicionPersonaje.z = -0.753044f;
    pelotaPosicion = glm::vec3(14.8709f, 1.84649f, -16.4633f);

    rotsYoshi = {
        0.0f,       // rotHead
        -51.0f,       // rotBody
        0.0f,       // rotArmD_Y
        0.0f,       // rotArmD_Z
        0.0f,       // rotArmI_Y
        0.0f,       // rotArmI_Z
        0.0f,       // rotLegD
        0.0f,       // rotLegI
        0.0f,       // rotHandD
        0.0f        // rotHandI
    };
    saveFrame(KeyFrame,posicionPersonaje,tiempoAnimacion,rotsYoshi,true);


    // 70 KEYFRAME 2 Gira cuerpo y junta las manos
    tiempoAnimacion = 0.2f;

    posicionPersonaje.x = -1.73735f;
    posicionPersonaje.y = 1.67006f;
    posicionPersonaje.z = -0.753044f;
    pelotaPosicion = glm::vec3(14.8709f, 1.84649f, -16.4633f);

    rotsYoshi = {
        42.5592f,
        -51.0f,
        110.0f,
        0.0f,
        -128.0f,
        0.0f,
        40.0f,
        -40.0f,
        110.0f,
        95.0f
    };
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsYoshi, true);


    // 120 KEYFRAME 3 Estira brazos
    tiempoAnimacion = 0.4f;
    posicionPersonaje.x = -1.73735f;
    posicionPersonaje.y = 1.67006f;
    posicionPersonaje.z = -0.753044f;
    pelotaPosicion = glm::vec3(14.8709f, 1.84649f, -16.4633f);

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
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsYoshi, true);


    //170 KEYFRAME 4 Gira para batear - pelota Choca
    tiempoAnimacion = 0.8f;
    posicionPersonaje.x = -1.73735f;
    posicionPersonaje.y = 1.67006f;
    posicionPersonaje.z = -0.753044f;
    pelotaPosicion = glm::vec3(0.0f, 1.67961f, 0.0f);

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
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsYoshi, true);


    //220 KEYFRAME 5 Salto y giro (momentum del bateo)
    tiempoAnimacion = 1.6f;
    posicionPersonaje.x = -1.73735f;
    posicionPersonaje.y = 2.26006f;
    posicionPersonaje.z = -0.753044f;
    pelotaPosicion = glm::vec3(60.6394f, 31.5427f, -72.2949f);

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
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsYoshi, true);


    // 250 KEYFRAME 6 vuelve al piso
    tiempoAnimacion = 2.4f;
    posicionPersonaje.x = -1.73735f;
    posicionPersonaje.y = 0.5f;
    posicionPersonaje.z = -0.753044f;
    pelotaPosicion = glm::vec3(88.7069f, 3.43961f, -105.757);

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
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsYoshi, true);
}

void lanzamiento(float& tiempoAnimacion, glm::vec3& posicionPersonaje, std::vector<FRAME>& KeyFrame)
{
    // KEYFRAME 1
    tiempoAnimacion = 0.0f;
    posicionHuesitos.y = 1.47702f;
    pelotaPosicion = glm::vec3(14.9834f, 1.54544f, -16.4595f);

    rotsHuesitos = {
        0.0f,       // rotHead
        0.0f,       // rotBody
        0.0f,        // rotArmD_Y
        0.0f,       // rotArmD_Z
        0.0f,        // rotArmI_Y
        0.0f,        // rotArmI_Z
        0.0f,        // rotLegD
        0.0f,        // rotLegI
        0.0f,        // rotHandD
        0.0f         // rotHandI
    };
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsHuesitos, true);


    // KEYFRAME 2
    tiempoAnimacion = 0.5f;
    posicionHuesitos.y = 2.59702f;
    pelotaPosicion = glm::vec3(15.1234f, 1.64544f, -16.5181f);
    rotsHuesitos = {
        -12.704f,
        178.0f,
        0.8903f,
        67.0f,
        3.9609f,
        0.0f,
        -180.0f,
        180.0f,
        -22.5556f,
        53.125f
    };
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsHuesitos, true);


    // KEYFRAME 3
    tiempoAnimacion = 0.8f;
    posicionHuesitos.y = 1.47702f;
    pelotaPosicion = glm::vec3(15.1834f, 1.54544f, -16.6595f);

    rotsHuesitos = {
        -36.2963f,
        362.98f,
        7.1221f,
        28.0f,
        12.2428f,
        0.0f,
        80.0f,
        0.0f,
        270.556f,
        170.0f
    };
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsHuesitos, true);


    // KEYFRAME 4
    tiempoAnimacion = 1.2f;
    posicionHuesitos.y = 1.47702f;
    pelotaPosicion = glm::vec3(15.1234f, 1.44544f, -16.8009f);

    rotsHuesitos = {
        -45.37f,
        426.798f,
        9.4591f,
        -11.0f,
        16.3162f,
        0.0f,
        80.0f,
        -41.0f,
        -80.5556f,
        232.422f
    };
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsHuesitos, true);


    // KEYFRAME 5
    tiempoAnimacion = 2.0f;
    posicionHuesitos.y = 1.47702f;
    pelotaPosicion = glm::vec3(14.9834f, 1.54544f, -16.8595f);

    rotsHuesitos = {
        -49.0f,
        459.0f,
        24.0371f,
        -11.0f,
        19.4444f,
        0.0f,
        -35.5741f,
        -2.55146f,
        -104.0f,
        286.875f
    };
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsHuesitos, true);


    // KEYFRAME 6
    tiempoAnimacion = 2.2f;
    posicionHuesitos.y = 1.47702f;
    pelotaPosicion = glm::vec3(14.8434f, 1.64544f, -16.8009f);

    rotsHuesitos = {
        350.0f,
        525.0f,
        121.0f,
        -11.0f,
        21.0f,
        0.0f,
        0.0f,
        48.0f,
        -87.0f,
        330.48f
    };
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsHuesitos, true);


    // KEYFRAME 7
    tiempoAnimacion = 2.8f;
    posicionHuesitos.y = 1.47702f;
    pelotaPosicion = glm::vec3(14.9834f, 1.54544f, -16.6595f);
    rotsHuesitos = {
        362.0f,
        724.0f,
        15.0f,
        -11.0f,
        21.0f,
        0.0f,
        0.0f,
        0.0f,
        -39.0f,
        340.0f
    };
    saveFrame(KeyFrame, posicionPersonaje, tiempoAnimacion, rotsHuesitos, true);
}