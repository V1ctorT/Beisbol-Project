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
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH = WIDTH, SCREEN_HEIGHT = HEIGHT;

// Camera--------------------------------------------------------------------------------
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

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
GLfloat rotacionBrazo = 0.0f;
GLfloat rotCabeza = 0.0f;

glm::vec3 posicionHuesitos(15.547f,0.47702f,-16.07f);
glm::vec3 posicionToad(3.0f, 0.43065f, -3.0f);

glm::vec3 Light1 = glm::vec3(0);

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

//Publico
//void publicodf(GLint modelLoc, Model& modelo, Shader& shader, glm::mat4 model);
//void dibujarPublico();

//Posiciones Publico
glm::vec3 publico[] = {
	glm::vec3(0.0f, 0.0f, 0.0f)
};

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

	Model Dog((char*)"ModelsEjemplos/RedDog.obj");
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



	//Inicializamos yoshi Structura
	//modeloPartes Yooshi;

	////Modelo Yoshi
	//Yooshi.Cabeza = Model((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCabeza.obj");
	//Yooshi.Cuerpo = Model((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCuerpo.obj");
	//Yooshi.ManoIzq = Model((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoL_Hand.obj");
	//Yooshi.BrazoDer = Model((char*)"Modelos/YoshiOBJ/yoshi_Partes/YD_Brazo.obj");
	//Yooshi.BrazoIzq = Model((char*)"Modelos/YoshiOBJ/yoshi_Partes/YL_Brazo.obj");
	//Yooshi.ManoDer = Model((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoD_Hand.obj");
	//Yooshi.PiernaIzq = Model((char*)"Modelos/YoshiOBJ/yoshi_Partes/Y_LegI.obj");
	//Yooshi.PiernaDer = Model((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoLegDer.obj");
	//Yoshi------------------------------------------------------------------------------
	Model yoshiH((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCabeza.obj");
	Model yoshiCuerpo((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCuerpo.obj");
	Model yoL_hand((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoL_Hand.obj");
	Model yoBrazo_L((char*)"Modelos/YoshiOBJ/yoshi_Partes/YL_Brazo.obj");
	Model yoBrazo_R((char*)"Modelos/YoshiOBJ/yoshi_Partes/YD_Brazo.obj");
	Model yoD_hand((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoD_Hand.obj");
	Model yoPierna_L((char*)"Modelos/YoshiOBJ/yoshi_Partes/Y_LegI.obj");
	Model yoPierna_R((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoLegDer.obj");
	//Huesitos---------------------------------------------------------------------------
	Model hCabeza((char*)"Modelos/YoshiOBJ/huesitos_Partes/HCabeza.obj");
	Model hCuerpo((char*)"Modelos/YoshiOBJ/huesitos_Partes/HCuerpo.obj");
	Model hManoD((char*)"Modelos/YoshiOBJ/huesitos_Partes/HManoD.obj");
	Model hBrazoI((char*)"Modelos/YoshiOBJ/huesitos_Partes/HBrazoI.obj");
	Model hBrazoD((char*)"Modelos/YoshiOBJ/huesitos_Partes/HBrazoD.obj");
	Model hManoI((char*)"Modelos/YoshiOBJ/huesitos_Partes/HManoI.obj");
	Model hPataI((char*)"Modelos/YoshiOBJ/huesitos_Partes/HPataI.obj");
	Model hPataD((char*)"Modelos/YoshiOBJ/huesitos_Partes/HPataD.obj");
	//Toad-------------------------------------------------------------------------------
	Model tBody((char*)"Modelos/Publico/Toad/Toad_Body.obj");
	Model tCabeza((char*)"Modelos/Publico/Toad/Toad_Head.obj");
	Model tManoD((char*)"Modelos/Publico/Toad/Toad_BArm.obj");
	Model tManoI((char*)"Modelos/Publico/Toad/Toad_LArm.obj");
	Model tPataD((char*)"Modelos/Publico/Toad/Toad_DLeg.obj");
	Model tPataI((char*)"Modelos/Publico/Toad/Toad_ILeg.obj");


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

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//Intervalos de Tiempo
	GLfloat tiempoAnterior = glfwGetTime();
	GLfloat intervalo = 0.7f;

	//Proyeccion para la interfaz--------------------------------------------------------
	glm::mat4 projectionUI = glm::ortho(
		0.0f, 1920.0f,
		0.0f, 1080.0f
	);
	//Posiciones UVs---------------------------------------------------------------------
	float verticesUI[] = {
			// Posiciones (X, Y, Z)   // Coordenadas UV (S, T)
			0.0f, 1.0f, 0.0f,         0.0f, 1.0f, // Superior izquierda
			1.0f, 1.0f, 0.0f,         1.0f, 1.0f, // Superior derecha
			1.0f, 0.0f, 0.0f,         1.0f, 0.0f, // Inferior derecha
			0.0f, 0.0f, 0.0f,         0.0f, 0.0f  // Inferior izquierda
	};

	GLuint indicesUI[] = {
		0, 1, 3,
		1, 2, 3
	};
	//Atributos de imagen----------------------------------------------------------------
	GLuint VBO_UI, VAO_UI, EBO_UI;
	glGenVertexArrays(1, &VAO_UI);
	glGenBuffers(1, &VBO_UI);
	glGenBuffers(1, &EBO_UI);

	glBindVertexArray(VAO_UI);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UI);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesUI), verticesUI, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_UI);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesUI), indicesUI, GL_STATIC_DRAW);

	// Posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
	//	8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// Coordenadas de textura
	glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Carga de Imagen -------------------------------------------------------------------
		// Load textures
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Diffuse map
	image = stbi_load("Interfaz/Interfaz.png", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);


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
			glm::vec3(0.08f, 0.09f, 0.15f), //Noche
			glm::vec3(0.20f, 0.13f, 0.08f), //Tarde 
			glm::vec3(0.30f, 0.30f, 0.30f) //Dia
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

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), direcciones[ambiente].x, direcciones[ambiente].y, direcciones[ambiente].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), ambient[ambiente].x, ambient[ambiente].y, ambient[ambiente].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), diffuse[ambiente].x, diffuse[ambiente].y, diffuse[ambiente].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), specular[ambiente].x, specular[ambiente].y, specular[ambiente].z);


		// Point light 1
	    glm::vec3 lightColor;
		lightColor.x= abs(sin(glfwGetTime() *Light1.x));
		lightColor.y= abs(sin(glfwGetTime() *Light1.y));
		lightColor.z= sin(glfwGetTime() *Light1.z);

		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.001f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.001f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
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

		// SpotLight---------------------------------------------------------------------
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(0.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(0.0f)));

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

		
		/*
		Si gorro = 1
			Yoshi Gorra de kiko
			Huesitos Gorra azul don ramon
		sino 
			Yoshi Gorra azul don ramon
			Huesitos Gorra de kiko
		*/

		//Yoshi, personaje principal-----------------------------------------------------------
		model = glm::mat4(1);
		mainMovement(modelLoc, yoshis, lightingShader, model);
		//model = glm::translate(model, glm::vec3((sin(tiempoActual * 4.0f)) * 5.0f ,2.0f,0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));


		//Rotacion de Brazos
		rotacionBrazo = sin(tiempoActual * 4.0f) * 25.0f;
		rotCabeza = sin(tiempoActual * 4.0f) * 30.0f;

		//Cuerpo de Yoshi
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		yoshiCuerpo.Draw(lightingShader);

		//Cabeza de Yoshi Tienen el mismo lugar de pivote
		modelCuerpo = modelTemp = model;
		model = glm::rotate(model, glm::radians(rotCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		yoshiH.Draw(lightingShader);

		//Gorro
		// Model = translare 0,0.5,0
		//Gorro.Draw(lightingShader);

		//Brazo Izquierdo de Yoshi
		model = glm::translate(modelTemp, glm::vec3(-0.26443f, -0.12442f, -0.015195f));//pivote
		model = glm::rotate(model, glm::radians(rotacionBrazo), glm::vec3(0.0f, 1.0f, 0.0f));
		//-0.41714f, -0.12442f, -0.052828f Centro del modelo
		modelTemp = model = glm::translate(model, glm::vec3(-0.15271f, 0.0f, -0.037633f));//centro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		yoBrazo_L.Draw(lightingShader);

		//Mano Izquierda de Yoshi
		model = glm::translate(modelTemp, glm::vec3(-0.12210f,-0.00470f,-0.037183f));
		model = glm::rotate(model, glm::radians(rotacionBrazo), glm::vec3(1.0f, 0.0f, 0.0f));
		//-0.71842, -0.17623, -0.178 Centro del modelo
		model = glm::translate(model, glm::vec3(-0.17918f,-0.04711f,-0.087989f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		yoL_hand.Draw(lightingShader);

		model = modelCuerpo;

		//Brazo Derecho de Yoshi
		model = glm::translate(modelCuerpo, glm::vec3(0.23581f,-0.14962f,0.11884f));//pivote
		model = glm::rotate(model, glm::radians(rotacionBrazo), glm::vec3(0.0f, -1.0f, 0.0f));
		//0.36386f, -0.13042f,0.17549f Centro del modelo
		modelTemp = model = glm::translate(model, glm::vec3(0.12805f,0.01920f,0.05665f));//centro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		yoBrazo_R.Draw(lightingShader);

		//Mano Derecha de Yoshi
		model = glm::translate(modelTemp, glm::vec3(0.13382f, 0.00130f, 0.05046f)); //Pivote de la mano
		model = glm::rotate(model, glm::radians(rotacionBrazo), glm::vec3(1.0f, 0.0f, 0.0f));
		//0.22834f, -0.06276f, 0.02576f
		model = glm::translate(model, glm::vec3(0.22834f, -0.06276f, 0.02576f)); //Centro de la mano
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		yoD_hand.Draw(lightingShader);

		model = modelCuerpo;

		rotPierna = rotacionBrazo * 3.0f;
		//Piernas de Yoshi
		//Izquierda
		model = glm::translate(model, glm::vec3(-0.25734f, -0.70411f, 0.066808f));
		model = glm::rotate(model, glm::radians(rotPierna), glm::vec3(1.0f, 0.0f, 0.0f));
		//-0.27725, -1.0203, -0.024045
		model = glm::translate(model, glm::vec3(-0.01991f, -0.31619f, -0.090853f)); //Centro de la pierna
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		yoPierna_L.Draw(lightingShader);

		//Derecha
		model = modelCuerpo;
		model = glm::translate(model, glm::vec3(0.17779f,-0.68234f,0.21343f));
		model = glm::rotate(model, glm::radians(-rotPierna), glm::vec3(1.0f, 0.0f, 0.0f));
		//0.24268, -1.018, 0.13401 
		model = glm::translate(model, glm::vec3(0.06489f, -0.33566f, -0.07942f)); //Centro de la pierna
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		yoPierna_R.Draw(lightingShader);

		if (lanzar) {

		}

		//Huesitos-----------------------------------------------------------------------
		model = glm::mat4(1);
		model = glm::translate(model, posicionHuesitos); //Posicion inicial

		//Cuerpo
		//0.0f,0.68843f,0.0f
		modelCuerpo = model = glm::translate(model, glm::vec3(0.0f,0.68843f,0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hCuerpo.Draw(lightingShader);

		//Cabeza
		model = glm::translate(modelCuerpo, glm::vec3(0.031647f,0.39327f,0.19935));//Pivote
		model = glm::rotate(model, glm::radians(rotCabeza), glm::vec3(0.0f,1.0f,0.0f));
		//Centro 0.042193,1.432,0.082021
		model = glm::translate(model,glm::vec3(0.0f, 0.35028f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hCabeza.Draw(lightingShader);

		//Brazo Derecho
		model = glm::mat4(1);
		model = translate(modelCuerpo, glm::vec3(-0.14468f,0.22898f,0.16496f));//Pivote
		model = glm::rotate(model, glm::radians(rotacionBrazo), glm::vec3(0.0f, 1.0f, 0.0f));
		//Centro -0.32961,0.91783,-0.063774
		modelTemp = model = glm::translate(model, glm::vec3(-0.17352f,0.000422f,0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hBrazoD.Draw(lightingShader);

		//Mano Derecha
		model = translate(modelTemp, glm::vec3(0.0f, 0.000422f, 0.0f));//Pivote
		model = rotate(model, glm::radians(rotacionBrazo), glm::vec3(1.0f, 1.0f, 0.0f));
		//Centro -0.60577,0.91575,0.003662
		model = glm::translate(model,glm::vec3(-0.27616f, -0.00208f, 0.067436f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hManoD.Draw(lightingShader);

		//Brazo Izquierdo
		model = glm::mat4(1);
		model = translate(modelCuerpo, glm::vec3(0.18415f, 0.22898f, 0.11287f));//Pivote
		model = glm::rotate(model, glm::radians(rotacionBrazo), glm::vec3(0.0f, 1.0f, 0.0f));
		//Centro 0.34683,0.17093,0.91783
		modelTemp = model = glm::translate(model, glm::vec3(0.17352f, 0.000422f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hBrazoI.Draw(lightingShader);
		
		//Mano Izquierda
		model = translate(modelTemp, glm::vec3(0.0756f, 0.0f, 0.0f));//Pivote
		model = rotate(model, glm::radians(rotacionBrazo), glm::vec3(1.0f, 1.0f, 0.0f));
		//Centro 0.60577,0.91575,0.003662
		model = glm::translate(model, glm::vec3(0.24735f, -0.010089f, -0.001561f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hManoI.Draw(lightingShader);

		//Piernas
		//Derecha
		model = glm::mat4(1);
		//0.0f,0.68843f,0.0f a -0.17231,0.63268,0.043567
		model = translate(modelCuerpo, glm::vec3(-0.17231f,-0.05575f,0.04356f));
		model = rotate(model, glm::radians(rotPierna), glm::vec3(1.0f, 0.0f, 0.0f));
		//Centro 0.60577,0.91575,0.003662
		model = glm::translate(model, glm::vec3(0.0f, -0.38253f, 0.075f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hPataD.Draw(lightingShader);

		//Izquierda
		model = glm::mat4(1);
		//0.0f,0.68843f,0.0f a 0.17747,0.63268,-0.011843
		model = translate(modelCuerpo, glm::vec3(0.17747f, -0.05575f, -0.011843f));
		model = rotate(model, glm::radians(rotPierna), glm::vec3(-1.0f, 0.0f, 0.0f));
		//Centro 0.60577,0.91575,0.003662
		model = glm::translate(model, glm::vec3(0.0f, -0.32620f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hPataD.Draw(lightingShader);

		//Toad Referee-------------------------------------------------------------------
		model = glm::mat4(1);
		modelCuerpo = model = translate(model, posicionToad);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tBody.Draw(lightingShader);

		//Cabeza
		model = translate(modelCuerpo, glm::vec3(0.0f,0.5323f,0.0f));
		model = rotate(model, glm::radians(rotCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tCabeza.Draw(lightingShader);

		//Patas
		//Derecha
		model = translate(modelCuerpo, glm::vec3(-0.23112f,0.18446f,0.094799f));
		model = rotate(model, glm::radians(rotPierna), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tPataD.Draw(lightingShader);
		//Izquierda
		model = translate(modelCuerpo, glm::vec3(0.23112f, 0.18446f, -0.094799f));
		model = rotate(model, glm::radians(rotPierna), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tPataI.Draw(lightingShader);

		//Brazos
		//Derecho
		model = translate(modelCuerpo, glm::vec3(-0.16343f, 0.46262f, 0.0f));
		model = rotate(model, glm::radians(rotacionBrazo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tManoD.Draw(lightingShader);
		//Izquierda
		model = translate(modelCuerpo, glm::vec3(0.16343f, 0.46262f, 0.0f));
		model = rotate(model, glm::radians(rotacionBrazo), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tManoI.Draw(lightingShader);

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

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO_UI);
	glDeleteVertexArrays(1, &VAO_UI);
	glDeleteBuffers(1, &VBO_UI);
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
		active = !active;

		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0.0f);
		}
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

//Animaciones
/*
	Correr -\
	
	Lanzar Pelota

	Recoger Pelota

	Isa
	Publico J Bandera que acciona el publico
		Salten J
		Aplaudan J


*/