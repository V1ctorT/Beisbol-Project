/* 
-----------------------------------------------------------------------------------------
  OpenGL 3.3 Proyecto Final
  Beisbol

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
glm::vec3 pelotaPosicion(0.0f, 0.0f, 0.0f);
//Librerias para Carga de modelos y camara-----------------------------------------------
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
//Animaciones de publico y corredores----------------------------------------------------
#include "Publico.h"
glm::mat4 m(1);
glm::vec3 tra(0.0f,0.0f,0.0f);
#include "Corredores.h"
int estado = 0; //0 - Caminar, 1 - Bateo, 2 - Atrapar 3 - Lanzar
//Posicion pelota
glm::vec3 pelotaPP(0.0f, 0.0f, 0.0f);
bool lanzamientoDisparado = false;

Publico publico;
Corredores corredoresA, corredoresB;
Corredores* ajuste = &corredoresA;   // el que se mueve con el teclado
int noCorrA = 3;
int noCorrB = 3;

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
//0 yoshi, 1 tercera persona
//3 pelota 2 huesitos lanzador 
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
Camera  camera(glm::vec3(0.0f,0.0f,0.0f));
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
	glm::vec3(40.7534f, 46.5365f, 52.7199f),
	glm::vec3(-52.2116f,46.4115 ,-40.2842f ),
	glm::vec3(59.7414f, 40.5053f, -108.932f),

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
glm::vec3 posicionPersonaje(0.0f, 0.0f, 0.0f);
int corredor = -1;
glm::vec3 posicionHuesitos(15.547f, 3.47702f, -16.07f);

#include "Limites.h"
GLfloat rotPierna = 0.0f;
GLfloat rotacionBrazo = 0.0f;
GLfloat rotCabeza = 0.0f;

//Valores de rotaciones y posicion sin incrementos
struct valoresYoshi {
	float rotHead;
	float rotBody;
	float rotArmD_Y;
	float rotArmD_Z;
	float rotArmI_Y;
	float rotArmI_Z;
	float rotLegD;
	float rotLegI;
	float rotHandD;
	float rotHandI;
	float rotValPel;
};

//Animacion Keyframe Yoshi---------------------------------------------------------------
int playIndexYoshi = 0; //KeyFrame Actual
float tiempoAnimacionYoshi = 0.0f;
bool playYoshi = false; //Indica si esta ocurriendo la animacion
int pasoActualYoshi = 0; //Conteo de los pasos de interpolacioon
double tiempoInicioYoshi = 0.0f; //Momento de inicio de animacion
double tiempoUltimoPasoYoshi = 0.0f; //Momento del ultimo keyframe
bool teclaFAnterior = false; //Para accionar la animacion

//Animacion KeyFrame Huesitos------------------------------------------------------------
int playIndexHuesitos = 0; //KeyFrame Actual
float tiempoAnimacionHuesitos = 0.0f;
bool playHuesitos = false; //Indica si esta ocurriendo la animacion
int pasoActualHuesitos = 0; //Conteo de los pasos de interpolacioon
double tiempoInicioHuesitos = 0.0f; //Momento de inicio de animacion
double tiempoUltimoPasoHuesitos = 0.0f; //Momento del ultimo keyframe
bool teclaGAnterior = false; //Para accionar la animacion
//Quien se va a usar 1 - yoshi, 2 - huesitos
int yoshi = 1; 
bool secuenciaActiva = false;
bool yoshiIniciado = false;

//Variable global------------------------------------------------------------------------
valoresYoshi rotsYoshi = 
{
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f
};
valoresYoshi rotsHuesitos =
{
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f,0.0f,
	0.0f
};
float rotPel = 0.0f;

#include "KeyframeYoshis.h"
//Rutinas y Utilerias
#include "Pelota.h"
#include "Bateo.h"

//Caminar
GLfloat fasePaso = 0.0f;          // avanza solo mientras caminas
GLfloat intensidadCorrer = 0.0f;  // 0 = quieto, 1 = corriendo (transición suave)

//Toad
glm::vec3 posicionToad(3.0f, 0.43065f, -3.0f);

glm::vec3 Light1 = glm::vec3(0);

//Variables de Iluminacion--------------------------------------------------------------
int estadoLuces = 0;   // 0 apagado, 1 lámparas, 2 spotlights, 3 luz general
int ambiente = 2;
glm::vec3 colorLamparas(1.0f, 1.0f, 0.0f);

//Bate Actual
int noBate = -1;

//Luces Fiestaa
struct ConfigFiesta
{
	glm::vec3 postes[3] = {
	glm::vec3(70.768f,  7.6772f, -112.43f), //posición 1er poste
	glm::vec3(88.346f,  7.6772f, -100.52f), //posición 2do poste
	glm::vec3(106.8f,   7.6772f, -88.04f) //posición 3er poste
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

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

//Publico
//Posiciones Publico
glm::vec3 publicos[] = {
	glm::vec3(0.0f, 0.0f, 0.0f)
};

//Main Function--------------------------------------------------------------------------
int main()
{
	//Creacion del contexto de OpenGL y GLFW---------------------------------------------
	// Init GLFW
	glfwInit();
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

	//-----------------------------------------------------------------------------------
	//Carga de Modelos
	Model estadio((char*)"Modelos/Stadium/SMestadio.obj");

	//Personajes-------------------------------------------------------------------------
	Model yoshis((char*)"Modelos/YoshiOBJ/yoshi.obj"); //Completo
	Model huesittos2((char*)"Modelos/YoshiOBJ/Red Dry Bones/bloody_karon.obj"); //Completo

	//Pelotas----------------------------------------------------------------------------
	Model pelotaa((char*)"Modelos/Pelotas/pelota.obj");

	//Gorras-----------------------------------------------------------------------------
	Model quico((char*)"Modelos/Publico/Gorras/Quico_Gorra.obj");
	Model donRamon((char*)"Modelos/Publico/Gorras/Don_Ramon_Gorra.obj");
	Model referee((char*)"Modelos/Publico/Gorras/Referee.obj");
	Model amarillo((char*)"Modelos/Publico/Gorras/CascoAmarillo.obj");
	Model azul((char*)"Modelos/Publico/Gorras/CascoAzul.obj");
	Gorras gorras = {&quico,&donRamon,&referee, &amarillo, &azul};

	//Yoshi------------------------------------------------------------------------------
	Model yoshiH((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCabeza.obj");
	Model yoshiCuerpo((char*)"Modelos/YoshiOBJ/yoshi_Partes/YCuerpo.obj");
	Model yoL_hand((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoL_Hand.obj");
	Model yoBrazo_L((char*)"Modelos/YoshiOBJ/yoshi_Partes/YL_Brazo.obj");
	Model yoBrazo_R((char*)"Modelos/YoshiOBJ/yoshi_Partes/YD_Brazo.obj");
	Model yoD_hand((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoD_Hand.obj");
	Model yoPierna_L((char*)"Modelos/YoshiOBJ/yoshi_Partes/Y_LegI.obj");
	Model yoPierna_R((char*)"Modelos/YoshiOBJ/yoshi_Partes/YoLegDer.obj");
	PartesYoshi partes = { &yoshiCuerpo, &yoshiH, &yoBrazo_L, &yoL_hand,
					   &yoBrazo_R, &yoD_hand, &yoPierna_L, &yoPierna_R, &pelotaa };

	//Huesitos---------------------------------------------------------------------------
	Model hCabeza((char*)"Modelos/YoshiOBJ/huesitos_Partes/HCabeza.obj");
	Model hCuerpo((char*)"Modelos/YoshiOBJ/huesitos_Partes/HCuerpo.obj");
	Model hManoD((char*)"Modelos/YoshiOBJ/huesitos_Partes/HManoD.obj");
	Model hBrazoI((char*)"Modelos/YoshiOBJ/huesitos_Partes/HBrazoI.obj");
	Model hBrazoD((char*)"Modelos/YoshiOBJ/huesitos_Partes/HBrazoD.obj");
	Model hManoI((char*)"Modelos/YoshiOBJ/huesitos_Partes/HManoI.obj");
	Model hPataI((char*)"Modelos/YoshiOBJ/huesitos_Partes/HPataI.obj");
	Model hPataD((char*)"Modelos/YoshiOBJ/huesitos_Partes/HPataD.obj");
	PartesYoshi huesitos = {&hCuerpo,&hCabeza,&hBrazoI,&hManoI,&hBrazoD,
		&hManoD,&hPataI,& hPataD,& pelotaa };

	//Toad-------------------------------------------------------------------------------
	Model tBody((char*)"Modelos/Publico/Toad/Toad_Body.obj");
	Model tCabeza((char*)"Modelos/Publico/Toad/Toad_Head.obj");
	Model tManoD((char*)"Modelos/Publico/Toad/Toad_BArm.obj");
	Model tManoI((char*)"Modelos/Publico/Toad/Toad_LArm.obj");
	Model tPataD((char*)"Modelos/Publico/Toad/Toad_DLeg.obj");
	Model tPataI((char*)"Modelos/Publico/Toad/Toad_ILeg.obj");
	
	//Modelos de ambientacion y utileria-------------------------------------------------
	//Bates
	Model bateStel((char*)"Modelos/Bates/StellesBat/bate.obj");
	Model bateAl((char*)"Modelos/Bates/Aluminio/bateAl.obj");
	Model bateMadera((char*)"Modelos/Bates/Madera/MaderaBate.obj");
	Model bateHmmm((char*)"Modelos/Bates/bate/bateHmm.obj");
	Bates bates = { &bateStel ,&bateAl ,&bateMadera ,&bateHmmm };
	Model pelota((char*)"Modelos/Pelotas/pelota.obj");

	//Bloques de Publico-----------------------------------------------------------------
	// piso 1 (4.83f para el piso 2)
	const GLfloat YC = 1.0f;           // altura del Yoshi corriendo sobre el piso 1
	publico.init(partes);

	// ---- BLOQUE 1 ----
	publico.bloque4(glm::vec3(36.2f, 2.911f, 31.77f),
		glm::vec3(55.0f, 2.911f, 27.0f),25.0f);
	// ---- BLOQUE 2 ----
	publico.bloque4(glm::vec3(58.739f, 2.911f, 25.083f),
		glm::vec3(75.16f, 2.911f, 12.428f),35.0f);
	// ---- BLOQUE 3 ----
	publico.bloque4(glm::vec3(85.908f, 2.911f, -9.8168f),
		glm::vec3(90.346f, 2.911f, -28.475f),80.0f);
	// ---- BLOQUE 4 ----
	publico.bloque4(glm::vec3(90.3434f, 2.911f, -32.85f),
		glm::vec3(84.047f, 2.911f, -57.923f),100.0f);
	// ---- BLOQUE 5 ----
	publico.bloque4(glm::vec3(42.786f, 2.911f, -89.643f),
		glm::vec3(2.7034f, 2.911f, -82.71f),200.0f);
	// ---- BLOQUE 6 ----
	publico.bloque4(glm::vec3(-4.93f, 2.911f, -78.154f),
		glm::vec3(-29.234f, 2.911f, -45.706f),230.0f);
	// ---- BLOQUE 7 ----
	publico.bloque4(glm::vec3(-32.327f, 2.911f, -9.921f),
		glm::vec3(-21.9118f, 2.911f, 19.766f),300.0f);
	// ---- BLOQUE 8 ----
	publico.bloque4(glm::vec3(-18.508f, 2.911f, 22.85f),
		glm::vec3(10.161f, 2.911f, 33.105f),0.0f);

	// Banca 1 (frente a z ≈ 30, recorrido a lo largo de X)
	corredoresA.init(partes);
	corredoresA.centro = glm::vec3(22.5f, YC, 19.0f);
	corredoresA.rotPista = 0.0f;
	corredoresA.radioX = 8.0f;
	corredoresA.radioZ = 1.5f;
	corredoresA.exponente = 4.0f;          // 2 = elipse, 4-8 = más cuadrado
	corredoresA.agregar(noCorrA);

	// Banca 2 (frente a x ≈ -29.3, recorrido a lo largo de Z)
	noCorrB = 3;
	corredoresB.init(huesitos);
	corredoresB.centro = glm::vec3(-18.0f, YC, -22.5f);
	corredoresB.rotPista = 90.0f;
	corredoresB.radioX = 8.0f;
	corredoresB.radioZ = 1.5f;
	corredoresB.exponente = 4.0f;
	corredoresB.agregar(noCorrB);

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

	//Configuracion de luces tipo spootlight---------------------------------------------
	// Cono para los haces: punta en (0,0,0), base en y = -1 con radio 1
	std::vector<float> cono;
	const int SEG = 24; //Número de segmentos que formarán la base circular del cono
	for (int i = 0; i < SEG; i++)
	{
		float a0 = glm::two_pi<float>() * i / SEG;
		float a1 = glm::two_pi<float>() * (i + 1) / SEG; //Calcula el ángulo inicial y final de cada segmento
		cono.insert(cono.end(), { 0.0f, 0.0f, 0.0f,
								  cosf(a0), -1.0f, sinf(a0),
								  cosf(a1), -1.0f, sinf(a1) }); //Agrega tres vértices por cada triángulo
	}

	//Identificadores del VAO y VBO que almacenarán la geometría del cono
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

	//Animaciones KeyFrame----------------------------------------------------------------
	bateoAqui(tiempoAnimacionYoshi,posicionPersonaje,KeyFrame); //Carga la animacion yoshi
	//lanzamiento(tiempoAnimacionYoshi, posicionPersonaje, KeyFrame);
	lanzamiento(tiempoAnimacionHuesitos, posicionHuesitos, KeyFrameHuesitos); //Carga huesitos
	/*completo(tiempoAnimacionYoshi, posicionPersonaje, KeyFrame, KeyFrameHuesitos);*/

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

		//Matriz de proyeccion
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 500.0f);

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		ambientacion();
		//0 yoshi, 1 tercera persona
		//3 pelota 2 huesitos lanzador 
		switch (modoC) 
		{
			case 0://Primera Persona
				camera.PrimeraPersona(posicionPersonaje);
				break;
			case 1://Yoshi tercera persona
				camera.UpdatePivote(posicionPersonaje, 5.0f, 1.5f);
				break;
			case 2: //Pelota
				camera.UpdatePivote(pelotaPosicion, 5.0f, 1.5f);
				break;
			case 3: //Huesitos lanzador
				camera.UpdatePivote(posicionHuesitos, 5.0f, 1.5f);
				break;
			default: // Por si toma un valor inesperado
				camera.PrimeraPersona(posicionPersonaje);
				break;
		}

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// OpenGL options
		glEnable(GL_DEPTH_TEST);

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

		//Asigna el color de las lámparas si están encendidas
		//Si están apagadas, establece el color negro (sin iluminación)
		glm::vec3 lightColor = lamparasOn ? colorLamparas : glm::vec3(0.0f);

		glm::vec3 amb = ambient[ambiente];
		glm::vec3 dif = diffuse[ambiente];
		glm::vec3 spe = specular[ambiente];

		//Si está activada la iluminación general, reemplaza los valores
		//de iluminación por intensidades uniformes
		if (generalOn)
		{
			amb = glm::vec3(0.45f);
			dif = glm::vec3(0.55f);
			spe = glm::vec3(0.5f);
		}

		// Envía al shader la dirección de la luz direccional
		// y sus componentes ambiental, difusa y especular
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), direcciones[ambiente].x, direcciones[ambiente].y, direcciones[ambiente].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), amb.x, amb.y, amb.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), dif.x, dif.y, dif.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), spe.x, spe.y, spe.z);

		//Configuración de la primera luz puntual
		//Envía su posición, color y componentes de iluminación al shader
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), lightColor.x, lightColor.y, lightColor.z);
		//Define los factores de atenuación de la segunda luz puntual.
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.007f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.001f);

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

			//Calcula el ángulo de giro de cada reflector según el tiempo transcurrido,
			//la velocidad de movimiento y un desfase para separarlos entre sí
			GLfloat ang = tiempoActual * fiesta.velocidadGiro + i * 2.094f;

			//Calcula el punto hacia donde se dirige cada reflector,
			//realizando un movimiento circular alrededor del objetivo
			glm::vec3 punto = fiesta.objetivo + glm::vec3(cos(ang) * fiesta.radioBarrido, 0.0f,
				sin(ang) * fiesta.radioBarrido);

			// Calcula el vector normalizado que indica la dirección del reflector,
			// desde la posición del poste hasta el punto de destino
			glm::vec3 dirSpot = glm::normalize(punto - fiesta.postes[i]);

			glm::vec3 colorSpot = fiesta.colores[i] * fiesta.intensidad;
			//negro si están apagados
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
			//Define el ángulo interno de apertura del reflector en radianes
			glUniform1f(glGetUniformLocation(lightingShader.Program, (s + "cutOff").c_str()), glm::cos(glm::radians(fiesta.apertura)));
			//Define el ángulo externo del reflector, considerando el suavizado
			//para generar una transición gradual en los bordes del haz
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
		publico.dibujar(modelLoc, lightingShader, camera.GetPosition(), camera.GetFront(), 
			tiempoActual, gorras);
		/*
		Si gorro = 1
			Yoshi Gorra de kiko
			Huesitos Gorra azul don ramon
		sino 
			Yoshi Gorra azul don ramon
			Huesitos Gorra de kiko
		*/

		//Yoshi, personaje principal-----------------------------------------------------------
		// Correr solo cuando se presiona una tecla de movimiento
		bool moviendo = keys[GLFW_KEY_W] || keys[GLFW_KEY_A] || keys[GLFW_KEY_S] || keys[GLFW_KEY_D]
			|| keys[GLFW_KEY_UP] || keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT];
		GLfloat objetivoCorrer = moviendo ? 1.0f : 0.0f;
		intensidadCorrer += (objetivoCorrer - intensidadCorrer) * glm::min(1.0f, 10.0f * deltaTime);
		if (moviendo) fasePaso += deltaTime * 8.0f;

		GLfloat rotacionBrazo = sin(fasePaso) * 25.0f * intensidadCorrer;
		rotPierna = rotacionBrazo * 3.0f;

		// Configuración de posición y orientación del jugador
		glm::mat4 baseJugador(1.0f);
		baseJugador = glm::translate(baseJugador, posicionPersonaje + glm::vec3(0.0f, 1.159f, 0.0f));
		baseJugador = glm::rotate(baseJugador, glm::radians(-camera.GetYaw() + 90.0f + 200.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		// Inicialización de las matrices de transformación de las manos
		glm::mat4 modelManoYoshi(1.0f);
		glm::mat4 modelManoHuesitos(1.0f);
		
		// Registro y dibujo de los corredores de ambos equipos
		corredoresA.agregar(noCorrA);
		corredoresB.agregar(noCorrB);
		corredoresA.dibujar(modelLoc, lightingShader, tiempoActual, baseJugador, 1,gorras);
		corredoresB.dibujar(modelLoc, lightingShader, tiempoActual, baseJugador, 2,gorras);

		// Configuración de la posición del modelo de Huesitos
		glm::mat4 huesitoss(1.0f);
		huesitoss = glm::translate(huesitoss, posicionHuesitos);

		// =====================================================
		// HUESITOS: PRIMERO REALIZA EL LANZAMIENTO
		// =====================================================

		if (!playHuesitos)
		{
			// Dibujo jerárquico normal
			dibujarHuesitos(
				modelLoc, lightingShader,
				huesitos, huesitoss,
				rotCabeza, rotacionBrazo,
				rotacionBrazo, rotPierna,
				modelManoHuesitos, gorras, false
			);
		}
		else
		{
			// Dibujo mediante keyframes
			dibujarYoshiKeyframe(
				modelLoc, lightingShader,
				huesitos, huesitoss,
				modelManoHuesitos, 2, gorras
			);

			// Actualizar animación de Huesitos
			playAnimation(
				playHuesitos,
				playIndexHuesitos,
				KeyFrameHuesitos,
				posicionHuesitos,
				tiempoInicioHuesitos,
				pasoActualHuesitos,
				rotsHuesitos
			);
		}

		// =====================================================
		// YOSHI: COMIENZA CUANDO HUESITOS TERMINA
		// =====================================================

		// Detectar finalización de Huesitos e iniciar Yoshi
		if (secuenciaActiva && !playHuesitos && !yoshiIniciado)
		{
			std::cout << "Inicia Bateo de Yoshi" << endl;

			startAnimation(
				playIndexYoshi,
				KeyFrame,
				posicionPersonaje,
				pasoActualYoshi,
				tiempoInicioYoshi,
				tiempoUltimoPasoYoshi,
				playYoshi,
				1
			);

			yoshiIniciado = true;
		}


		// =====================================================
		// YOSHI: ANIMACIÓN DE BATEO
		// =====================================================

		if (!playYoshi)
		{
			// Dibujo jerárquico normal
			dibujarYoshi(
				modelLoc, lightingShader,
				partes, baseJugador,
				rotacionBrazo, 0.0f,
				rotacionBrazo, rotPierna,
				modelManoYoshi, gorras
			);
		}
		else
		{
			// Dibujo mediante keyframes
			dibujarYoshiKeyframe(
				modelLoc, lightingShader,
				partes, baseJugador,
				modelManoYoshi, 1, gorras
			);

			// Actualizar animación de Yoshi
			playAnimation(
				playYoshi,
				playIndexYoshi,
				KeyFrame,
				posicionPersonaje,
				tiempoInicioYoshi,
				pasoActualYoshi,
				rotsYoshi
			);
		}

		// =====================================================
		// FINALIZACIÓN DE LA SECUENCIA
		// =====================================================

		if (secuenciaActiva && yoshiIniciado && !playYoshi)
		{
			secuenciaActiva = false;
			std::cout << "Secuencia completa" << endl;
		}

		dibujarBates(modelLoc, lightingShader, modelManoYoshi, noBate, bates);
		
		glBindVertexArray(0);

		if (spotOn) //Animacion de luz spotlight-----------------------------------------
		{
			// Configuración del shader y envío de las matrices de cámara
			beamShader.Use();
			glUniformMatrix4fv(glGetUniformLocation(beamShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(beamShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			// Activación de la transparencia y configuración de profundidad
			// para representar los haces de luz sin ocultar otros objetos
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glDepthMask(GL_FALSE);

			// Dibujo de los tres haces de luz mediante el modelo de cono
			glBindVertexArray(coneVAO);
			for (int i = 0; i < 3; i++)
			{
				// Cálculo de la rotación y dimensiones del cono según la dirección del reflector
				glm::vec3 eje = glm::cross(glm::vec3(0, -1, 0), dirsHaz[i]);
				GLfloat ang = acos(glm::clamp(glm::dot(glm::vec3(0, -1, 0), dirsHaz[i]), -1.0f, 1.0f));
				GLfloat radio = largoHaz[i] * tan(glm::radians(fiesta.aperturaHaz));

				// Aplicación de transformaciones de posición, rotación y escala al cono
				glm::mat4 m(1.0f);
				m = glm::translate(m, fiesta.postes[i]);
				if (glm::length(eje) > 0.0001f)
					m = glm::rotate(m, ang, glm::normalize(eje));
				m = glm::scale(m, glm::vec3(radio, largoHaz[i], radio));

				// Envío de propiedades visuales del haz al shader y dibujo de sus triángulos
				glUniformMatrix4fv(glGetUniformLocation(beamShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(m));
				glUniform3f(glGetUniformLocation(beamShader.Program, "beamColor"),
					fiesta.colores[i].x, fiesta.colores[i].y, fiesta.colores[i].z);
				glUniform1f(glGetUniformLocation(beamShader.Program, "opacidad"), fiesta.opacidadHaz);
				glDrawArrays(GL_TRIANGLES, 0, SEG * 3);

			}

			// Liberación del VAO y restauración de la configuración de profundidad y transparencia
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}

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

//Lucees
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

// Impresión de las posiciones actuales de la cámara y del personaje en consola
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

	moverPersonaje(direccion,derecha);

	// Restablecimiento de la posición y orientación de la cámara al presionar R
	if(keys[GLFW_KEY_R])
	{
		camera.ProcessKeyboard(RESET, deltaTime);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
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
		modoC = (modoC + 1) % 4;
	}
	//Accionar el publico----------------------------------------------------------------
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		publico.alternar();
	}
	//Ajuste de Corredores---------------------------------------------------------------
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		ajuste = (ajuste == &corredoresA) ? &corredoresB : &corredoresA;

	//Imprime centro corredores----------------------------------------------------------
	if (key == GLFW_KEY_F)
	{
		if (action == GLFW_PRESS && !teclaFAnterior && !playYoshi)
		{
			std::cout << "centro: " << ajuste->centro.x << ", " << ajuste->centro.y << ", "
				<< ajuste->centro.z << "Inicia Bateo" << endl;
			estado = 1;
			startAnimation(playIndexYoshi,KeyFrame,posicionPersonaje,pasoActualYoshi,
				tiempoInicioYoshi,tiempoUltimoPasoYoshi,playYoshi,1);
			//estado = 0;
		}
		if (action == GLFW_PRESS)
			teclaFAnterior = true;

		if (action == GLFW_RELEASE)
			teclaFAnterior = false;
	}

	//Lanzamiento de la pelota-----------------------------------------------------------
	if(key == GLFW_KEY_G)
	{
		if (action == GLFW_PRESS && !teclaGAnterior && !secuenciaActiva)
		{
			std::cout << "Inicia Lanzamiento: " << endl;

			startAnimation(
				playIndexHuesitos,
				KeyFrameHuesitos,
				posicionHuesitos,
				pasoActualHuesitos,
				tiempoInicioHuesitos,
				tiempoUltimoPasoHuesitos,
				playHuesitos,
				2
			);

			secuenciaActiva = true;
			yoshiIniciado = false;
		}
	}

	if (key == GLFW_KEY_Q)
	{
		if (action == GLFW_PRESS && !teclaGAnterior && !playHuesitos && !teclaFAnterior && !playYoshi)
		{
			std::cout << "Inicia Animacion: " << endl;
			//Animacion completa
			startAnimation2(playIndexHuesitos,playIndexYoshi, KeyFrameHuesitos, KeyFrame,
				posicionHuesitos, pasoActualHuesitos,
				tiempoInicioHuesitos, tiempoUltimoPasoHuesitos, playHuesitos, 3);
			
		}
		if (action == GLFW_PRESS)
			teclaFAnterior = true;

		if (action == GLFW_RELEASE)
			teclaFAnterior = false;
			//estado = 0;
		if (action == GLFW_PRESS)
			teclaGAnterior = true;

		if (action == GLFW_RELEASE)
			teclaGAnterior = false;
	}
	
	//Zonas------------------------------------------------------------------------------
	if (keys[GLFW_KEY_ENTER] && action == GLFW_PRESS)
	{
		//Zona Para Lanzar
		if (posicionPersonaje.x >= 11.47f && posicionPersonaje.x <= 15.94f
			&& posicionPersonaje.z >= -15.758f && posicionPersonaje.z <= -11.374f)
		{
			lanzar = true;
		}
		corredor = dentroCorredores(
			glm::vec2(posicionPersonaje.x,posicionPersonaje.z));
		if (corredor == 1) {
			noCorrA += 1;
		}
		else if (corredor == 2)
		{
			noCorrB += 1;
		}
	}
	//Agarrar Bate-----------------------------------------------------------------------
	if (keys[GLFW_KEY_E] && action == GLFW_PRESS)
	{
		//camera.ProcessKeyboard(UP, deltaTime);
		if (noBate == -1)
		{
			int bateCercano = detectarBate(posicionPersonaje);

			if (bateCercano != -1)
			{
				noBate = bateCercano;

				printf("Agarraste el bate %d\n", noBate);
			}
		}
		else {
			soltarBate(noBate);
		}
	}
	if (keys[GLFW_KEY_0] && action == GLFW_PRESS) 
	{
		estado = 0;
		soltarBate(noBate);
		noBate = -1;
	}
	//Animacion de Bateo
	
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	// Cálculo del desplazamiento del mouse en ambos ejes
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset, pelotaPosicion,true, modoC);
}

void mainMovement(GLint modelLoc, Model& modelo, Shader& shader, glm::mat4 model) 
{

	// Activación del shader y aplicación de las transformaciones del modelo
	shader.Use();
	model = glm::translate(model, posicionPersonaje);
	model = glm::rotate(model, glm::radians(-camera.GetYaw() + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	modelo.Draw(shader);
}