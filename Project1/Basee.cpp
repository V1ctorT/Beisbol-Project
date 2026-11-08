/* 
-----------------------------------------------------------------------------------------
  OpenGL 3.3 Proyecto Final
  Beisbol -
  
  Funciones Principales
  Bateo de la Pelota

  Modo Espectador desde el Estadio

  Minijuegos de Practica

Creado por:
	Guadalupe Isabela De la Fuente Flores
	Victor Manuel Torres Martinez
-----------------------------------------------------------------------------------------
*/

//Librerias de OpenGL y GLFW ------------------------------------------------------------
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// Other Libs
#include "stb_image.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//Load Models
#include "SOIL2/SOIL2.h"

//Librerias para Carga de modelos y camara-----------------------------------------------
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "Publico.h"
#include "Corredores.h"
Publico publico;
Corredores corredoresA, corredoresB;
Corredores* ajuste = &corredoresA;   // el que se mueve con el teclado

// Function prototypes-------------------------------------------------------------------
//Entradas del teclado y mouse
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);

//Funciones para de camara y movimientos
void DoMovement();
void ambientacion();

//Reajustar espacio de Renderizado
void resize(GLFWwindow *window, int width, int height);
void imprimirPosicion(glm::vec3 posiciones[], int size);

//Limite de movimiento de la camara
void limiteCamara();
int modoC = 0;

//Rutinas
void rutLanzar();
bool lanzar = false;

GLfloat cameraDistance = 5.0f;
GLfloat cameraHeight = 2.0f;

void mainMovement(GLint modelLoc, Model& modelo, Shader& shader, glm::mat4 model);

// Window dimensions---------------------------------------------------------------------
const GLuint WIDTH = 1080, HEIGHT = 720;
int SCREEN_WIDTH = WIDTH, SCREEN_HEIGHT = HEIGHT;

// Camera--------------------------------------------------------------------------------
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);


// Posiciones de Las Luces---------------------------------------------------------------
glm::vec3 pointLightPositions[] = {
	//Lampara1
	glm::vec3(73.534f, 49.302f, 20.756f),

	glm::vec3(-65.747f,46.411f ,27.069f ),

	glm::vec3(-35.122f, 40.505f, -100.63f),


	glm::vec3(73.534f, 46.536f, 20.756f),
	glm::vec3(73.534f, 43.771f, 20.756f),

	glm::vec3(71.347f, 49.302f, 22.178f),
	glm::vec3(71.347f, 46.536f, 22.178f),
	glm::vec3(71.347f, 43.771f, 22.178f),

	glm::vec3(69.144f, 49.302f, 23.600f),
	glm::vec3(69.144f, 46.536f, 23.600f),
	glm::vec3(69.144f, 43.771f, 23.600f),

	glm::vec3(67.441f, 49.302f, 25.163f),
	glm::vec3(67.441f, 46.536f, 25.163f),
	glm::vec3(67.441f, 43.771f, 25.163f),

	glm::vec3(65.753f, 49.302f, 27.100f),
	glm::vec3(65.753f, 46.536f, 27.100f),
	glm::vec3(65.753f, 43.771f, 27.100f),

	glm::vec3(64.050f, 49.302f, 29.053f),
	glm::vec3(64.050f, 46.536f, 29.053f),
	glm::vec3(64.050f, 43.771f, 29.053f),

	glm::vec3(62.378f, 49.302f, 30.991f),
	glm::vec3(62.378f, 46.536f, 30.991f),
	glm::vec3(62.378f, 43.771f, 30.991f),

	glm::vec3(61.066f, 49.302f, 32.944f),
	glm::vec3(61.066f, 46.536f, 32.944f),
	glm::vec3(61.066f, 43.771f, 32.944f),

	glm::vec3(59.987f, 49.302f, 35.288f),
	glm::vec3(59.987f, 46.536f, 35.288f),
	glm::vec3(59.987f, 43.771f, 35.288f),

	glm::vec3(58.909f, 49.302f, 37.647f),
	glm::vec3(58.909f, 46.536f, 37.647f),
	glm::vec3(58.909f, 43.771f, 37.647f)
};

//Cubo Unitario para los modelos---------------------------------------------------------
float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

//Posicion del Personaje Principal------------------------------------------------------
glm::vec3 posicionPersonaje(0.0f, 0.5f, 0.0f);
GLfloat rotPierna = 0.0f;

int estadoLuces = 0;   // 0 apagado, 1 lámparas, 2 spotlights, 3 luz general

glm::vec3 colorLamparas(1.0f, 1.0f, 0.0f);

struct ConfigFiesta
{
	glm::vec3 postes[3] = {
	glm::vec3(70.768f,  7.6772f, -112.43f),
	glm::vec3(88.346f,  7.6772f, -100.52f),
	glm::vec3(106.8f,   7.6772f, -88.04f)
	};
	glm::vec3 colores[3] = {
		glm::vec3(1.0f, 0.1f, 0.1f),   // poste 1: rojo
		glm::vec3(0.1f, 1.0f, 0.2f),   // poste 2: verde
		glm::vec3(0.2f, 0.3f, 1.0f)    // poste 3: azul
	};
	glm::vec3 objetivo = glm::vec3(23.0f, 2.911f, -17.0f); // centro del campo
	GLfloat radioBarrido = 20.0f;
	GLfloat velocidadGiro = 1.0f;
	GLfloat intensidad = 1.5f;
	GLfloat suavizado = 6.0f;
	GLfloat apertura = 10.0f;
	GLfloat linear = 0.002f;
	GLfloat quadratic = 0.00002f;
	GLfloat opacidadHaz = 0.3f;
	GLfloat aperturaHaz = 3.0f;
} fiesta;

//Variables de Iluminacion--------------------------------------------------------------
int ambiente = 2;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

//Estructura Basica de Modelos
struct modeloPartes
{
	Model Cabeza;
	Model Cuerpo;
	Model BrazoDer;
	Model BrazoIzq;
	Model ManoDer;
	Model ManoIzq;
	Model PiernaIzq;
	Model PiernaDer;
};

GLfloat fasePaso = 0.0f;          // avanza solo mientras caminas
GLfloat intensidadCorrer = 0.0f;  // 0 = quieto, 1 = corriendo (transición suave)

//Main Function--------------------------------------------------------------------------
int main()
{
	//Creacion del contexto de OpenGL y GLFW---------------------------------------------
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Beisbol Proyecto", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(window, resize);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//Ocultar el cursor del mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	//Carga de Shaders ------------------------------------------------------------------
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader beamShader("Shader/beam.vs", "Shader/beam.frag");
	
	Model Piso((char*)"ModelsEjemplos/piso.obj");

	//-----------------------------------------------------------------------------------
	//Carga de Modelos
	
	//Model pisooo((char*)"Modelos/Estadio/PisoBase.obj");
	//Model base((char*)"Modelos/Bases/Base.obj");
	//Model base2((char*)"Modelos/Bases/Base2.obj");
	//Model base0((char*)"Modelos/Bases/BaseOg.obj");
	Model estadio((char*)"Modelos/Stadium/SMestadio.obj");


	//Personajes-------------------------------------------------------------------------
	Model yoshis((char*)"Modelos/YoshiOBJ/yoshi.obj");
	Model huesittos2((char*)"Modelos/YoshiOBJ/Red Dry Bones/bloody_karon.obj");

	Model yoshiH((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCabeza.obj");
	Model yoshiCuerpo((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCuerpo.obj");
	Model yoL_hand((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoL_Hand.obj");
	Model yoBrazo_L((char*)"Modelos/YoshiOBJ/yoshi_Partes/YL_Brazo.obj");
	Model yoBrazo_R((char*)"Modelos/YoshiOBJ/yoshi_Partes/YD_Brazo.obj");
	Model yoD_hand((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoD_Hand.obj");
	Model yoPierna_L((char*)"Modelos/YoshiOBJ/yoshi_Partes/Y_LegI.obj");
	Model yoPierna_R((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoLegDer.obj");

	PartesYoshi partes = { &yoshiCuerpo, &yoshiH, &yoBrazo_L, &yoL_hand,
					   &yoBrazo_R, &yoD_hand, &yoPierna_L, &yoPierna_R };
         // piso 1 (usa 4.83f para el piso 2)
	const GLfloat YC = 1.0f;           // altura del Yoshi corriendo sobre el piso 1

	publico.init(partes);
	
	// ---- BLOQUE 1 ----
	publico.bloque4(glm::vec3(36.2f, 2.911f, 31.77f),
		glm::vec3(55.0f, 2.911f, 27.0f),
		25.0f);
	// ---- BLOQUE 2 ----
	publico.bloque4(glm::vec3(58.739f, 2.911f, 25.083f),
		glm::vec3(75.16f, 2.911f, 12.428f),
		35.0f);
	// ---- BLOQUE 3 ----
	publico.bloque4(glm::vec3(85.908f, 2.911f, -9.8168f),
		glm::vec3(90.346f, 2.911f, -28.475f),
		80.0f);
	// ---- BLOQUE 4 ----
	publico.bloque4(glm::vec3(90.3434f, 2.911f, -32.85f),
		glm::vec3(84.047f, 2.911f, -57.923f),
		100.0f);
	// ---- BLOQUE 5 ----
	publico.bloque4(glm::vec3(42.786f, 2.911f, -89.643f),
		glm::vec3(2.7034f, 2.911f, -82.71f),
		200.0f);
	// ---- BLOQUE 6 ----
	publico.bloque4(glm::vec3(-4.93f, 2.911f, -78.154f),
		glm::vec3(-29.234f, 2.911f, -45.706f),
		230.0f);
	// ---- BLOQUE 7 ----
	publico.bloque4(glm::vec3(-32.327f, 2.911f, -9.921f),
		glm::vec3(-21.9118f, 2.911f, 19.766f),
		300.0f);
	// ---- BLOQUE 8 ----
	publico.bloque4(glm::vec3(-18.508f, 2.911f, 22.85f),
		glm::vec3(10.161f, 2.911f, 33.105f),
		0.0f);

	// Banca 1 (frente a z ≈ 30, recorrido a lo largo de X)
	corredoresA.init(partes);
	corredoresA.centro = glm::vec3(22.5f, YC, 19.0f);
	corredoresA.rotPista = 0.0f;
	corredoresA.radioX = 8.0f;
	corredoresA.radioZ = 1.5f;
	corredoresA.exponente = 4.0f;          // 2 = elipse, 4-8 = más cuadrado
	corredoresA.agregar(3);

	// Banca 2 (frente a x ≈ -29.3, recorrido a lo largo de Z)
	corredoresB.init(partes);
	corredoresB.centro = glm::vec3(-18.0f, YC, -22.5f);
	corredoresB.rotPista = 90.0f;
	corredoresB.radioX = 8.0f;
	corredoresB.radioZ = 1.5f;
	corredoresB.exponente = 4.0f;
	corredoresB.agregar(3);


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Cono para los haces: punta en (0,0,0), base en y = -1 con radio 1
	std::vector<float> cono;
	const int SEG = 24;
	for (int i = 0; i < SEG; i++)
	{
		float a0 = glm::two_pi<float>() * i / SEG;
		float a1 = glm::two_pi<float>() * (i + 1) / SEG;
		cono.insert(cono.end(), { 0.0f, 0.0f, 0.0f,
								  cosf(a0), -1.0f, sinf(a0),
								  cosf(a1), -1.0f, sinf(a1) });
	}
	GLuint coneVAO, coneVBO;
	glGenVertexArrays(1, &coneVAO);
	glGenBuffers(1, &coneVBO);
	glBindVertexArray(coneVAO);
	glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
	glBufferData(GL_ARRAY_BUFFER, cono.size() * sizeof(float), cono.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);



	//Intervalos de Tiempo
	GLfloat tiempoAnterior = glfwGetTime();
	GLfloat intervalo = 0.7f;
	
	// Game loop-------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		
		//Intervalos de tiempo
		GLfloat tiempoActual = glfwGetTime();

		if (tiempoActual - tiempoAnterior >= intervalo)
		{
			tiempoAnterior = tiempoActual;
			//Imprime en pantalla posiciones
			//imprimirPosicion(pointLightPositions, 4);
		}

		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 500.0f);

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		ambientacion();
		if (modoC == 1) {
			camera.UpdatePivote(posicionPersonaje, 5.0f, 1.5f);
		}
		else
		{
			camera.PrimeraPersona(posicionPersonaje);
		}

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);
		
		
		//Load Model
	

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light-------------------------------------------------------------

		//Luces para cada momento del dia
		// Direccion de la luz
		glm::vec3 direcciones[] = {
			glm::vec3(-0.3f, -0.8f,  0.2f),  // Noche
			glm::vec3(0.4f, -0.5f, -0.7f),  // Tarde
			glm::vec3(-0.2f, -1.0f, -0.3f)   // Dia
		};

		// Luz ambiental
		glm::vec3 ambient[] = {
			glm::vec3(0.08f, 0.09f, 0.15f),
			glm::vec3(0.20f, 0.13f, 0.08f),
			glm::vec3(0.30f, 0.30f, 0.30f)
		};

		// Luz difusa
		glm::vec3 diffuse[] = {
			glm::vec3(0.18f, 0.22f, 0.38f),
			glm::vec3(0.95f, 0.55f, 0.30f),
			glm::vec3(0.85f, 0.82f, 0.75f)
		};

		// Luz especular
		glm::vec3 specular[] = {
			glm::vec3(0.25f, 0.28f, 0.40f),
			glm::vec3(0.75f, 0.45f, 0.25f),
			glm::vec3(0.90f, 0.90f, 0.85f)
		};

		bool lamparasOn = (estadoLuces == 1);
		bool spotOn = (estadoLuces == 2);
		bool generalOn = (estadoLuces == 3);

		glm::vec3 lightColor = lamparasOn ? colorLamparas : glm::vec3(0.0f);

		glm::vec3 amb = ambient[ambiente];
		glm::vec3 dif = diffuse[ambiente];
		glm::vec3 spe = specular[ambiente];

		if (generalOn)
		{
			amb = glm::vec3(0.45f);
			dif = glm::vec3(0.55f);
			spe = glm::vec3(0.5f);
		}

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), direcciones[ambiente].x, direcciones[ambiente].y, direcciones[ambiente].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), amb.x, amb.y, amb.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), dif.x, dif.y, dif.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), spe.x, spe.y, spe.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), lightColor.x, lightColor.y, lightColor.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.001f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), lightColor.x, lightColor.y, lightColor.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.001f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), lightColor.x, lightColor.y, lightColor.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.001f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

		glm::vec3 dirsHaz[3];
		GLfloat largoHaz[3];

		// SpotLights de fiesta (3 postes)
		for (int i = 0; i < 3; i++)
		{
			std::string s = "spotLights[" + std::to_string(i) + "].";

			GLfloat ang = tiempoActual * fiesta.velocidadGiro + i * 2.094f;
			glm::vec3 punto = fiesta.objetivo + glm::vec3(cos(ang) * fiesta.radioBarrido, 0.0f,
				sin(ang) * fiesta.radioBarrido);
			glm::vec3 dirSpot = glm::normalize(punto - fiesta.postes[i]);

			glm::vec3 colorSpot = fiesta.colores[i] * fiesta.intensidad;
			if (!spotOn) colorSpot = glm::vec3(0.0f);

			dirsHaz[i] = dirSpot;
			largoHaz[i] = glm::length(punto - fiesta.postes[i]);

			glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "position").c_str()), fiesta.postes[i].x, fiesta.postes[i].y, fiesta.postes[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "direction").c_str()), dirSpot.x, dirSpot.y, dirSpot.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "ambient").c_str()), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "diffuse").c_str()), colorSpot.x, colorSpot.y, colorSpot.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (s + "specular").c_str()), colorSpot.x, colorSpot.y, colorSpot.z);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "linear").c_str()), fiesta.linear);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "quadratic").c_str()), fiesta.quadratic);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "cutOff").c_str()), glm::cos(glm::radians(fiesta.apertura)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "outerCutOff").c_str()), glm::cos(glm::radians(fiesta.apertura + fiesta.suavizado)));
		}

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);
		glm::mat4 modelTemp(1);
		glm::mat4 modelCuerpo(1);

		//Carga de escena----------------------------------------------------------------------
        view = camera.GetViewMatrix();	
		
		//Escenario Principal------------------------------------------------------------------
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		estadio.Draw(lightingShader);


		publico.actualizar(deltaTime);
		publico.dibujar(modelLoc, lightingShader, camera.GetPosition(), camera.GetFront(), tiempoActual);

		corredoresA.dibujar(modelLoc, lightingShader, tiempoActual);
		corredoresB.dibujar(modelLoc, lightingShader, tiempoActual);

		//Yoshi, personaje principal-----------------------------------------------------------
		// Correr solo cuando se presiona una tecla de movimiento
		bool moviendo = keys[GLFW_KEY_W] || keys[GLFW_KEY_A] || keys[GLFW_KEY_S] || keys[GLFW_KEY_D]
			|| keys[GLFW_KEY_UP] || keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT];

		GLfloat objetivoCorrer = moviendo ? 1.0f : 0.0f;
		intensidadCorrer += (objetivoCorrer - intensidadCorrer) * glm::min(1.0f, 10.0f * deltaTime);
		if (moviendo)
			fasePaso += deltaTime * 8.0f;

		GLfloat rotacionBrazo = sin(fasePaso) * 25.0f * intensidadCorrer;
		rotPierna = rotacionBrazo * 3.0f;

		glm::mat4 baseJugador(1.0f);
		baseJugador = glm::translate(baseJugador, posicionPersonaje + glm::vec3(0.0f, 1.159f, 0.0f));
		baseJugador = glm::rotate(baseJugador, glm::radians(-camera.GetYaw() + 90.0f + 200.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		dibujarYoshi(modelLoc, lightingShader, partes, baseJugador,
			rotacionBrazo, 0.0f, rotacionBrazo, rotPierna);

		if (lanzar) {

		}

		if (spotOn)
		{
			beamShader.Use();
			glUniformMatrix4fv(glGetUniformLocation(beamShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(beamShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);   // suma de luz
			glDepthMask(GL_FALSE);               // no tapa lo que está detrás

			glBindVertexArray(coneVAO);
			for (int i = 0; i < 3; i++)
			{
				// girar el cono (que apunta a -Y) hacia dirsHaz[i]
				glm::vec3 eje = glm::cross(glm::vec3(0, -1, 0), dirsHaz[i]);
				GLfloat ang = acos(glm::clamp(glm::dot(glm::vec3(0, -1, 0), dirsHaz[i]), -1.0f, 1.0f));

				GLfloat radio = largoHaz[i] * tan(glm::radians(fiesta.aperturaHaz));

				glm::mat4 m(1.0f);
				m = glm::translate(m, fiesta.postes[i]);
				if (glm::length(eje) > 0.0001f)
					m = glm::rotate(m, ang, glm::normalize(eje));
				m = glm::scale(m, glm::vec3(radio, largoHaz[i], radio));


				glUniformMatrix4fv(glGetUniformLocation(beamShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(m));
				glUniform3f(glGetUniformLocation(beamShader.Program, "beamColor"),
					fiesta.colores[i].x, fiesta.colores[i].y, fiesta.colores[i].z);
				glUniform1f(glGetUniformLocation(beamShader.Program, "opacidad"), fiesta.opacidadHaz);
				glDrawArrays(GL_TRIANGLES, 0, SEG * 3);

			}
			glBindVertexArray(0);

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}

		glBindVertexArray(0);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 30; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

void ambientacion() {
	//Ambiente
	if (keys[GLFW_KEY_1])
	{
		ambiente = 0;
	}
	if (keys[GLFW_KEY_2])
	{
		ambiente = 1;
	}
	if (keys[GLFW_KEY_3])
	{
		ambiente = 2;
	}
}

void resize(GLFWwindow* window, int width, int height)
{
	SCREEN_HEIGHT = height;
	SCREEN_WIDTH = width;
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void imprimirPosicion(glm::vec3 posiciones[], int size)
{
	system("cls");
	for (int i = 0; i < size; i++)
	{
		std::cout << "Posicion Camara" << camera.GetPosition().x << ", " << camera.GetPosition().y << ", " << camera.GetPosition().z << ", " << camera.GetYaw() << "\n" << std::endl;
		std::cout << "Posicion Personaje" << posicionPersonaje.x << ", " << posicionPersonaje.y << ", " << posicionPersonaje.z <<"\n" << std::endl;
	}
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls--------------------------------------------------------------

	glm::vec3 direccion = camera.GetFront();
	direccion.y = 0.0f;
	direccion = glm::normalize(direccion);
	glm::vec3 derecha = glm::normalize(glm::cross(direccion, glm::vec3(0.0f, 1.0f, 0.0f)));
	GLfloat velocity = 5.0f * deltaTime;

	if(keys[GLFW_KEY_R])
	{
		camera.ProcessKeyboard(RESET, deltaTime);
	}
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		posicionPersonaje += direccion * velocity;
		//camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		posicionPersonaje -= direccion * velocity;
		//camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		posicionPersonaje -= derecha * velocity;
		//camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		posicionPersonaje += derecha * velocity;
		//camera.ProcessKeyboard(RIGHT, deltaTime);


	}
	if (keys[GLFW_KEY_Q] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(DOWN, deltaTime);

	}

	if (keys[GLFW_KEY_E] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(UP, deltaTime);

	}
	if (keys[GLFW_KEY_T] )
	{

	}

	if (keys[GLFW_KEY_Y] )
	{

	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// Encender y apagar luz con SPACE---------------------------------------------------
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		estadoLuces = (estadoLuces + 1) % 4;
	}
	//Cambiar de modo de camara con C----------------------------------------------------
	if (keys[GLFW_KEY_C] && action == GLFW_PRESS)
	{
		if (modoC == 0)
		{
			modoC = 1;
		}else if (modoC == 1)
			{
				modoC = 0;
			}	
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		publico.alternar();
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		ajuste = (ajuste == &corredoresA) ? &corredoresB : &corredoresA;

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		std::cout << "centro: " << ajuste->centro.x << ", " << ajuste->centro.y << ", "
		<< ajuste->centro.z << std::endl;

	//Zonas
	if (keys[GLFW_KEY_ENTER] && action == GLFW_PRESS)
	{
		//Zona Para Lanzar
		if (posicionPersonaje.x >= 11.47f && posicionPersonaje.x <= 15.94f 
			&& posicionPersonaje.z >= -15.758f && posicionPersonaje.z <= -11.374f)
		{
			lanzar = true;
		}
	}

}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset, true, modoC, posicionPersonaje);
}


void mainMovement(GLint modelLoc, Model& modelo, Shader& shader, glm::mat4 model) 
{

	shader.Use();
	model = glm::translate(model, posicionPersonaje);
	model = glm::rotate(model, glm::radians(-camera.GetYaw() + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	modelo.Draw(shader);
}
