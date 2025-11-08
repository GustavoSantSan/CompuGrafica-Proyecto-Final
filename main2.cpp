#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;

// SE USAN PARA EL LETRERO ----------------------------------------
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;


float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float dragonavance = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Camera camTopDown;
Camera pruebaCam;
Camera* activeCam;



Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture PokemonTexture;
Texture octaTexture;
Texture Pokemon2Texture;

Texture pastoTexture;
Texture asfaltoTexture;
Texture tierraTexture;
Texture arenaTexture;
Texture marTexture;
Texture transicionTexture;
Texture calleTexture;
Texture rejaTexture;





Model Dragonite_M;
Model Dragonite_Ala_Der_M;
Model Dragonite_Ala_Izq_M;
Model Dragonite_Brazo_Der_M;
Model Dragonite_Brazo_Izq_M;
Model Dragonite_Pierna_Der_M;
Model Dragonite_Pierna_Izq_M;


Model Tiamat_M;
Skybox skybox;

Model PuertaIzq_M;
Model PuertaDer_M;
Model Arco_M;
Model Letrero_M;

Model May_M;

Model GalacticNova;

Model Pasto;
Model Mansion;
Model Piramide;
Model PiramideQ;
Model KingBody;
Model KingLarm;
Model KingRarm;
Model KoraiB;
Model KoraiL;
Model KoraiA;
Model KoraiT;
Model Hawlucha;
Model Ring;

Model MayBody;
Model MayRightArm;
Model MayRightHand;
Model MayLeftArm;
Model MayLeftHand;
Model MayRightLeg;
Model MayRightFoot;
Model MayLeftLeg;
Model MayLeftFoot;

Model Faro;
Model Estadio;
Model EstadioA;
Model Torre;
Model Edificio;
Model Lampara;
Model LamparaSant;

Model Lago;
Model Boat;
Model Cueva;
Model MespritBody;
Model MespritHead;
Model MespritRArm;
Model MespritLArm;
Model MespritTail;
Model AzelfBody;
Model AzelfRArm;
Model AzelfLArm;
Model AzelfTail;
Model UxiefBody;
Model UxiefRArm;
Model UxiefLArm;
Model UxiefTail;
Model Temple;
Model KirbySant;
Model KirbyBocaChiquita;
Model KirbyBostezando;
Model KirbyRaro;
Model KirbyTriste;
Model KirbyEnojado;
Model KirbyDormido;
Model KirbyNormal;
Model KirbySorprendido;
Model KirbyMalvado;
Model KirbyFeliz;
Model KirbyAbsorbiendo;
Model KirbyLArm;
Model KirbyRArm;
Model KirbyRFoot;
Model KirbyLFoot;
Model Kirbys[12];
Model Casita1;
Model Casita2;
Model Casita3;
Model Casita4;
Model Arbol;
Model Antorcha;


Model Bosque;
Model SolB;
Model SolPD;
Model SolPT;
Model SolC;
Model LunaBody;
Model LunaWing;



//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
DirectionalLight NightLight;
//para declarar varias luces de tipo pointlight
PointLight pointLightsBlanco[MAX_POINT_LIGHTS];
PointLight pointLightsAzul[MAX_POINT_LIGHTS];
PointLight pointLightsNaranja[MAX_POINT_LIGHTS]; // DE TAMAÑO 3
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLightsAux[3];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	GLfloat letraVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.8f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		  0.2f, 0.8f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.2f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};


	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número
  //

  Mesh* obj8 = new Mesh();
  obj8->CreateMesh(letraVertices, numeroIndices, 32, 6);
  meshList.push_back(obj8);  // Solo una letra
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



//DAdo es el indice 8
void CrearOctaedro() {
    unsigned int octaedro_indices[] = {
        0,  1,  2,     // Cara 1
        3,  4,  5,     // Cara 2
        6,  7,  8,     // Cara 3
        9,  10, 11,    // Cara 4
        12, 13, 14,    // Cara 5
        15, 16, 17,    // Cara 6
        18, 19, 20,    // Cara 7
        21, 22, 23     // Cara 8
    };

    GLfloat octaedro_vertices[] = {
// Cara 1 (arriba - frente)
0.0f, 1.0f, 0.0f,   0.50f, 0.73f,   0.0f, -1.0f, 0.0f,
1.0f, 0.0f, 0.0f,   0.27f, 0.51f,  -1.0f,  0.0f, 0.0f,
0.0f, 0.0f, 1.0f,   0.73f, 0.51f,   0.0f,  0.0f, -1.0f,

// Cara 2 (arriba - izquierda)
0.0f, 1.0f, 0.0f,   0.48f, 0.73f,   0.0f, -1.0f, 0.0f,
0.0f, 0.0f, 1.0f,   0.25f, 0.52f,   0.0f,  0.0f, -1.0f,
-1.0f,0.0f, 0.0f,   0.05f, 0.73f,   1.0f,  0.0f, 0.0f,

// Cara 3 (arriba - atrás)
0.0f, 1.0f, 0.0f,   0.46f, 0.78f,   0.0f, -1.0f, 0.0f,
-1.0f,0.0f, 0.0f,   0.04f, 0.78f,   1.0f,  0.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.25f, 0.98f,   0.0f,  0.0f, 1.0f,

// Cara 4 (arriba - derecha)
0.0f, 1.0f, 0.0f,   0.52f, 0.73f,   0.0f, -1.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.98f, 0.73f,   0.0f,  0.0f, 1.0f,
1.0f, 0.0f, 0.0f,   0.75f, 0.52f,  -1.0f,  0.0f, 0.0f,

// Cara 5 (abajo - frente)
0.0f,-1.0f, 0.0f,   0.50f, 0.26f,   0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f,   0.26f, 0.49f,   0.0f, 0.0f, -1.0f,
1.0f, 0.0f, 0.0f,   0.73f, 0.49f,  -1.0f, 0.0f, 0.0f,

// Cara 6 (abajo - izquierda)
0.0f,-1.0f, 0.0f,   0.48f, 0.26f,   0.0f, 1.0f, 0.0f,
-1.0f,0.0f, 0.0f,   0.02f, 0.26f,   1.0f, 0.0f, 0.0f,
0.0f, 0.0f, 1.0f,   0.26f, 0.47f,   0.0f, 0.0f, -1.0f,

// Cara 7 (abajo - atrás)
0.0f,-1.0f, 0.0f,   0.48f, 0.23f,   0.0f, 1.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.25f, 0.02f,   0.0f, 0.0f, 1.0f,
-1.0f,0.0f, 0.0f,   0.06f, 0.23f,   1.0f, 0.0f, 0.0f,

// Cara 8 (abajo - derecha)
0.0f,-1.0f, 0.0f,   0.54f, 0.27f,   0.0f, 1.0f, 0.0f,
1.0f, 0.0f, 0.0f,   0.73f, 0.47f,  -1.0f, 0.0f, 0.0f,
0.0f, 0.0f,-1.0f,   0.97f, 0.27f,   0.0f, 0.0f, 1.0f 
    };
    Mesh* oct = new Mesh();
    oct->CreateMesh(octaedro_vertices, octaedro_indices, 24 * 8, 24);
    meshList.push_back(oct);
}

///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;



//NEW// Keyframes
float posXavion = 0.0, posYavion = 0.0, posZavion = 400.0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f;
float giroAvion = 0;

#define MAX_FRAMES 10 //Número de cuadros máximos
int i_max_steps = 90; //Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;		//Variable para GiroAvion
	float giroAvionInc;		//Variable para IncrementoGiroAvion
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 3;			//El número de cuadros guardados actualmente desde 0 para no sobreescribir
bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;
	//Se agregan nuevas líneas para guardar más variables si es necesario

	//no volatil,se requiere agregar una forma de escribir a un archivo para guardar los frames
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			//printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				//printf("Frame index= %d\n", FrameIndex);
				//printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolación del próximo cuadro
			{

				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

glm::vec3 modelPosition = glm::vec3(0.0f, 3.0f, 0.0f);
bool caminando = false;



int main() {
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearOctaedro();
	/*
	struct CameraPoint {
		glm::vec3 position;      // Dónde está la cámara
		float radioX;
		float velocidad;
		float radioZ;
		float alturaInicial;
		float alturaFinal;
	};
	*/
	std::vector<CameraPoint> puntos;


	std::vector<glm::vec3> centros;
	centros.push_back(glm::vec3(19.0794, 19.1999, 185.0)); // estadio
	centros.push_back(glm::vec3(249.541, 28.7997, 149.669)); // faro
	centros.push_back(glm::vec3(1.84984, 42.3999, -221.884)); // piramide
	centros.push_back(glm::vec3(-197.27, 10.4, -223.666)); // mesprit
	centros.push_back(glm::vec3(189.952, 0, -250.333)); // uxie
	centros.push_back(glm::vec3(-264.682, 10, -120.476)); // kirby

	std::vector<float> radiosX;
	radiosX.push_back(50.0f);
	radiosX.push_back(50.0f);
	radiosX.push_back(100.0f);
	radiosX.push_back(90.0f);
	radiosX.push_back(5.0f);
	radiosX.push_back(30.0f);

	std::vector<float> radiosZ;
	radiosZ.push_back(50.0f);
	radiosZ.push_back(50.0f);
	radiosZ.push_back(100.0f);
	radiosZ.push_back(40.0f);
	radiosZ.push_back(5.0f);
	radiosZ.push_back(30.0f);


	// --- Crear CameraPoints a partir de esas posiciones ---
	for (int i = 0; i < centros.size(); i++) {
		CameraPoint p;
		p.centro = centros[i];      // centro de la órbita
		p.radioX = radiosX[i];       // por ejemplo, 10 unidades
		p.radioZ = radiosZ[i];
		p.velocidad = 0.4f;   // grados por segundo
		p.alturaInicial = 5.0f;       // altura sobre el centro
		p.alturaFinal = 30.0f;
		puntos.push_back(p);
	}


	pruebaCam = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);
	pruebaCam.isStatic = true;


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);
	activeCam = &camera; // empieza en modo tercera persona


	camTopDown = Camera(glm::vec3(0.0f, 60.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -89.0f, 20.0f, 0.5f);
	camTopDown.isTopDown = true;






	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	pastoTexture = Texture("Textures/pasto_prehispanic.png");
	pastoTexture.LoadTextureA();
	calleTexture = Texture("Textures/calle.png");
	calleTexture.LoadTextureA();
	tierraTexture = Texture("Textures/tierra.png");
	tierraTexture.LoadTextureA();
	arenaTexture = Texture("Textures/Arena.png");
	arenaTexture.LoadTextureA();
	asfaltoTexture = Texture("Textures/asfalto.png");
	asfaltoTexture.LoadTextureA();
	marTexture = Texture("Textures/mar.png");
	marTexture.LoadTextureA();
	transicionTexture = Texture("Textures/transicion.png");
	transicionTexture.LoadTextureA();
	rejaTexture = Texture("Textures/reja.png");
	rejaTexture.LoadTextureA();

	dirtTexture = Texture("Textures/cemento.png");
	dirtTexture.LoadTextureA();

	octaTexture = Texture("Textures/octaedroNumeros.png"); // -----------------------
	octaTexture.LoadTextureA();
	Pokemon2Texture = Texture("Textures/POKEMONLETRAS.png");
	Pokemon2Texture.LoadTextureA();

	
	Dragonite_M = Model();
	Dragonite_M.LoadModel("Models/CuerpoDragonite.obj");

	Dragonite_Ala_Der_M = Model();
	Dragonite_Ala_Izq_M = Model();
	Dragonite_Ala_Izq_M.LoadModel("Models/AlaDerechaDragonite.obj");
	Dragonite_Ala_Der_M.LoadModel("Models/AlaIzquierdaDragonite.obj");
	Dragonite_Brazo_Der_M = Model();
	Dragonite_Brazo_Der_M.LoadModel("Models/BrazoDerechoDragonite.obj");
	Dragonite_Brazo_Izq_M = Model();
	Dragonite_Brazo_Izq_M.LoadModel("Models/BrazoIzquierdoDragonite.obj");
	Dragonite_Pierna_Der_M = Model();
	Dragonite_Pierna_Der_M.LoadModel("Models/PataDerechaDragonite.obj");
	Dragonite_Pierna_Izq_M = Model();
	Dragonite_Pierna_Izq_M.LoadModel("Models/PataIzquierdaDragonite.obj");


	GalacticNova = Model();
	GalacticNova.LoadModel("Models/GalacticNova.obj");


	PuertaIzq_M = Model();
	PuertaDer_M = Model();
	Arco_M = Model();
	Letrero_M = Model();
	PuertaIzq_M.LoadModel("Models/puerta izq.obj");
	PuertaDer_M.LoadModel("Models/puerta der.obj");
	Arco_M.LoadModel("Models/Arco2.obj");
	Letrero_M.LoadModel("Models/letrero.obj");


	Mansion = Model();
	Mansion.LoadModel("Models/Luigi's Mansion.obj");
	Piramide = Model();
	Piramide.LoadModel("Models/Piramide.obj");
	PiramideQ = Model();
	PiramideQ.LoadModel("Models/PiramideQ.obj");
	KingBody = Model();
	KingBody.LoadModel("Models/KingBody.obj");
	KingLarm = Model();
	KingLarm.LoadModel("Models/KingLarm.obj");
	KingRarm = Model();
	KingRarm.LoadModel("Models/KingRarm.obj");
	KoraiB = Model();
	KoraiB.LoadModel("Models/KoraiB.obj");
	KoraiL = Model();
	KoraiL.LoadModel("Models/KoraiLeg.obj");
	KoraiA = Model();
	KoraiA.LoadModel("Models/KoraiArm.obj");
	KoraiT = Model();
	KoraiT.LoadModel("Models/KoraiTail.obj");
	Hawlucha = Model();
	Hawlucha.LoadModel("Models/Hawlucha.obj");
	Ring = Model();
	Ring.LoadModel("Models/BoxingRing.obj");

	MayBody = Model();
	MayBody.LoadModel("Models/MayBody.obj");
	MayRightArm = Model();
	MayRightArm.LoadModel("Models/MayRightArm.obj");
	MayRightHand = Model();
	MayRightHand.LoadModel("Models/MayRightHand.obj");
	MayLeftArm = Model();
	MayLeftArm.LoadModel("Models/MayLeftArm.obj");
	MayLeftHand = Model();
	MayLeftHand.LoadModel("Models/MayLeftHand.obj");
	MayRightLeg = Model();
	MayRightLeg.LoadModel("Models/MayRightLeg.obj");
	MayRightFoot = Model();
	MayRightFoot.LoadModel("Models/MayRightFoot.obj");
	MayLeftLeg = Model();
	MayLeftLeg.LoadModel("Models/MayLeftLeg.obj");
	MayLeftFoot = Model();
	MayLeftFoot.LoadModel("Models/MayLeftFoot.obj");

	Faro = Model();
	Faro.LoadModel("Models/faro.obj");
	Estadio = Model();
	Estadio.LoadModel("Models/Stadium.obj");
	EstadioA = Model();
	EstadioA.LoadModel("Models/StadiumA.obj");
	Torre = Model();
	Torre.LoadModel("Models/Torre.obj");
	Edificio = Model();
	Edificio.LoadModel("Models/Edificio.obj");
	Lampara = Model();
	Lampara.LoadModel("Models/Street_Lamp.obj");
	LamparaSant = Model();
	LamparaSant.LoadModel("Models/lampara.obj");

	Lago = Model();
	Lago.LoadModel("Models/LakeLandscape.obj");
	Cueva = Model();
	Cueva.LoadModel("Models/building_base_mdl.obj");
	Casita1 = Model();
	Casita1.LoadModel("Models/root_hut_fire.obj");
	Casita2 = Model();
	Casita2.LoadModel("Models/root_hut_water.obj");
	Casita3 = Model();
	Casita3.LoadModel("Models/House (Green).obj");
	Casita4 = Model();
	Casita4.LoadModel("Models/House (Yellow).obj");
	Antorcha = Model();
	Antorcha.LoadModel("Models/Torch.obj");

	Boat = Model();
	Boat.LoadModel("Models/Boat.obj");
	MespritBody = Model();
	MespritBody.LoadModel("Models/MespritCuerpo.obj");
	MespritHead = Model();
	MespritHead.LoadModel("Models/MespritCabeza.obj");
	MespritRArm = Model();
	MespritRArm.LoadModel("Models/MespritBrazoDerecho.obj");
	MespritLArm = Model();
	MespritLArm.LoadModel("Models/MespritBrazoIzquierdo.obj");
	MespritTail = Model();
	MespritTail.LoadModel("Models/MespritCola.obj");
	AzelfBody = Model();
	AzelfBody.LoadModel("Models/AzelfCuerpo.obj");
	AzelfRArm = Model();
	AzelfRArm.LoadModel("Models/AzelfBrazoDerecho.obj");
	AzelfLArm = Model();
	AzelfLArm.LoadModel("Models/AzelfBrazoIzquierdo.obj");
	AzelfTail = Model();
	AzelfTail.LoadModel("Models/AzelfCola.obj");
	UxiefBody = Model();
	UxiefBody.LoadModel("Models/UxieCuerpo.obj");
	UxiefRArm = Model();
	UxiefRArm.LoadModel("Models/UxieBrazoDerecho.obj");
	UxiefLArm = Model();
	UxiefLArm.LoadModel("Models/UxieBrazoIzquierdo.obj");
	UxiefTail = Model();
	UxiefTail.LoadModel("Models/UxieCola.obj");
	Arbol = Model();
	Arbol.LoadModel("Models/Arbol.obj");
	Temple = Model();
	Temple.LoadModel("Models/Aztec Temple - Atlas Final.obj");
	KirbySant = Model();
	KirbySant.LoadModel("Models/kirbySantana.obj");
	KirbyBocaChiquita = Model();
	KirbyBocaChiquita.LoadModel("Models/kirbyBocaChiquita.obj");
	KirbyBostezando = Model();
	KirbyBostezando.LoadModel("Models/kirbyBostezando.obj");
	KirbyRaro = Model();
	KirbyRaro.LoadModel("Models/kirbyRaro.obj");
	KirbyTriste = Model();
	KirbyTriste.LoadModel("Models/kirbyTriste.obj");
	KirbyEnojado = Model();
	KirbyEnojado.LoadModel("Models/kirbyEnojado.obj");
	KirbyDormido = Model();
	KirbyDormido.LoadModel("Models/kirbyDormido.obj");
	KirbyFeliz = Model();
	KirbyFeliz.LoadModel("Models/kirbyFeliz.obj");
	KirbySorprendido = Model();
	KirbySorprendido.LoadModel("Models/kirbySorprendido.obj");
	KirbyNormal = Model();
	KirbyNormal.LoadModel("Models/kirbyNormal.obj");
	KirbyMalvado = Model();
	KirbyMalvado.LoadModel("Models/kirbyMalvado.obj");
	KirbyAbsorbiendo = Model();
	KirbyAbsorbiendo.LoadModel("Models/kirbyAbsorbiendo.obj");

	KirbyLArm = Model();
	KirbyLArm.LoadModel("Models/KirbyBrazoIzquierdo.obj");
	KirbyRArm = Model();
	KirbyRArm.LoadModel("Models/KirbyBrazoDerecho.obj");
	KirbyLFoot = Model();
	KirbyLFoot.LoadModel("Models/KirbyPieIzquierdo.obj");
	KirbyRFoot = Model();
	KirbyRFoot.LoadModel("Models/KirbyPieDerecho.obj");

Bosque = Model();
Bosque.LoadModel("Models/Bosque.obj");
	
SolB = Model();
SolB.LoadModel("Models/SolBody.obj");
SolPD = Model();
SolPD.LoadModel("Models/SolPataD.obj");
SolPT = Model();
SolPT.LoadModel("Models/SolPataT.obj");
SolC = Model();
SolC.LoadModel("Models/SolCola.obj");
LunaBody = Model();
LunaBody.LoadModel("Models/LunaBody.obj");
LunaWing = Model();
LunaWing.LoadModel("Models/LunaWing.obj");



glm::mat4 modelauxBP(1.0);

glm::vec3 posicionesCuerpo[] = {
		glm::vec3(-20.0f, 2.0f, -20.0),
		glm::vec3(0.05f, 0.55f, 0.18),
		glm::vec3(-0.03f, -0.66f, 0.29),
		glm::vec3(0.05f, 0.55f, -0.18),
		glm::vec3(-0.03f, -0.68f, -0.29),
		glm::vec3(0.0f, -0.55f, 0.18),
		glm::vec3(-0.03f, -0.89f, 0.0),
		glm::vec3(0.05f, -0.55f, -0.18),
		glm::vec3(-0.03f, -0.89f, -0.0)
};

GLfloat rotacionesCuerpo[] = {
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
};

int contador = 0;
bool avanzar = true;
bool detenerse = false;






	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/rightDia.png");
	skyboxFaces.push_back("Textures/Skybox/leftDia.png");
	skyboxFaces.push_back("Textures/Skybox/bottomDia.png");
	skyboxFaces.push_back("Textures/Skybox/topDia.png");
	skyboxFaces.push_back("Textures/Skybox/frontDia.png");
	skyboxFaces.push_back("Textures/Skybox/backDia.png");

	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/right.png");
	skyboxFaces2.push_back("Textures/Skybox/left.png");
	skyboxFaces2.push_back("Textures/Skybox/bottom.png");
	skyboxFaces2.push_back("Textures/Skybox/top.png");
	skyboxFaces2.push_back("Textures/Skybox/front.png");
	skyboxFaces2.push_back("Textures/Skybox/back.png");

	skybox = Skybox(skyboxFaces);






	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);



//luz direccional, sólo 1 y siempre debe de existir
mainLight = DirectionalLight(0.6f, 0.4f, 0.3f,
	0.8f, 0.8f,
	0.0f, -0.0f, -0.0f);

NightLight = DirectionalLight(1.0f, 1.0f, 1.0f,
	0.09f, 0.09f,
	0.0f, -0.0f, -0.0f);
/*
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
*/
bool DayNight = true;
GLfloat rotarSobreMapa = 0.0;
GLfloat trasladSobreMapa = 0.0;
GLfloat muevePata = 22.5;
bool muevePataB = true;
GLfloat dirLight = -1.0;

//Variables animación pelea
GLfloat movPelea = 0.0;
GLfloat rotPelea = 0.0;
GLfloat caePelea = 0.0;

GLfloat movCuerdas = -5.0;
GLfloat rotCuerdas = 0.0;
bool girando = true;
bool avanzando = true;

//Variables animación Dragonite
float rotDrag = 0.0;

//Variable para cambiar de kirby lol

int cambiaKirby = 0, contadorKirby = 0;
Kirbys[0] = KirbyAbsorbiendo;
Kirbys[1] = KirbyBostezando;
Kirbys[2] = KirbyDormido;
Kirbys[3] = KirbyEnojado;
Kirbys[4] = KirbyFeliz;
Kirbys[5] = KirbyMalvado;
Kirbys[6] = KirbyAbsorbiendo;
Kirbys[7] = KirbyNormal;
Kirbys[8] = KirbyRaro;
Kirbys[9] = KirbySant;
Kirbys[10] = KirbySorprendido;
Kirbys[11] = KirbyTriste;

GLfloat brazos = 0.0;

//contador de luces puntuales
unsigned int pointLightCount = 0;

// Luz lampara ....------- -------------------------------------------------------
pointLightsAzul[0] = PointLight(0.4f, 1.0f, 1.0f,
	12.0f, 10.0f,
	-1.0f, 2.0f, 0.0f,
	0.3f, 0.2f, 0.1f);
pointLightCount++;

pointLightsBlanco[0] = PointLight(1.0f, 1.0f, 1.0f,
	12.0f, 10.0f,
	-1.0f, 2.0f, 0.0f,
	0.3f, 0.2f, 0.1f);
pointLightCount++;

pointLightsAzul[1] = PointLight(0.4f, 1.0f, 1.0f,
	12.0f, 10.0f,
	-1.0f, 2.0f, 0.0f,
	0.3f, 0.2f, 0.1f);
pointLightCount++;

pointLightsBlanco[1] = PointLight(1.0f, 1.0f, 1.0f,
	12.0f, 10.0f,
	-1.0f, 2.0f, 0.0f,
	0.3f, 0.2f, 0.1f);
pointLightCount++;

pointLightsAzul[2] = PointLight(0.4f, 1.0f, 1.0f,
	12.0f, 10.0f,
	-1.0f, 2.0f, 0.0f,
	0.3f, 0.2f, 0.1f);
pointLightCount++;

pointLightsBlanco[2] = PointLight(1.0f, 1.0f, 1.0f,
	12.0f, 10.0f,
	-1.0f, 2.0f, 0.0f,
	0.3f, 0.2f, 0.1f);
pointLightCount++;

pointLightsNaranja[0] = PointLight(0.9686f, 0.5961f, 0.0392f,
	12.0f, 10.0f,
	-1.0f, 2.0f, 0.0f,
	0.3f, 0.2f, 0.1f);
pointLightCount++;

pointLightsNaranja[1] = PointLight(0.9686f, 0.5961f, 0.0392f,
	12.0f, 10.0f,
	-1.0f, 2.0f, 0.0f,
	0.3f, 0.2f, 0.1f);
pointLightCount++;

pointLightsNaranja[2] = PointLight(0.9686f, 0.5961f, 0.0392f,
	12.0f, 10.0f,
	-1.0f, 2.0f, 0.0f,
	0.3f, 0.2f, 0.1f);
pointLightCount++;

int luces[] = { 0, 0, 0, 0 };

	unsigned int spotLightCount = 0;

	//luz fija
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		34.0f, 29.5f, 171.0,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		20.0f, 50.0f, 185.0,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		15.0f, 2.0f,
		250.0f, 111.0f, 150.0f,
		-1.0f, -0.0f, -0.0f,
		1.0f, 0.05f, 0.01f,
		100.0f);
	spotLightCount++;

	// PIRAMIDE
	spotLights[3] = SpotLight(1.0f, 0.0f, 0.0f,
		20.0f, 2.0f,
		4.18119, 100.7999, -215.087,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.05f, 0.01f,
		450.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.5f; // Se aumentó de 0.01f a 0.5f
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;



// Variables que se agregan en ejercicio de clase 
	avanza = true;
	bool dragonAvanza = true;

  int contadorNum = 0;
	bool esUno = true;





	glm::vec3 lowerLight(0.0f,0.0f,0.0f);
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);



// Para puertas
  GLfloat auxiliar = 0.0f;


// Para letras
		GLfloat toffsetletracambiau = 0.0;
		GLfloat toffsetletrav = 0.0;

    GLfloat coordenadasLetrasU[] = {
    //          P      R     O     Y     E     C     T    O
         0.8f, 0.6f, 0.8f, 0.4f, 0.2f, 0.8f, 0.4f, 0.6f, 0.4f, 0.8f,

    //   C      G      E    I    H    C
        0.4f, 0.2f, 0.8f, 0.0f, 0.6f, 0.4f
    }; 
    
    GLfloat coordenadasLetrasV[] = {
    //         P      R     O     Y     E     C     T    O
      0.2f, 0.4f, 0.4f, 0.4f, 0.2f, 0.0f, 0.0f, 0.2f, 0.4f, 0.2f,

    //   C      G      E    I    H    C
        0.0f, 0.8f, 0.0f, 0.6f, 0.8f, 0.0f
    };
int abba = 0;
int abba2 = 0;



// DAdo
bool dadoCayendo = false;

// Inicializar semilla una sola vez (por ejemplo, al iniciar el programa)
srand(static_cast<unsigned int>(time(nullptr)));

// Generar número aleatorio entero entre 0 y N-1
int randomInt = rand() % 8;  // Ejemplo: N = 10 → valores [0..7]
int distanciaQueSubiraElDado = (rand() % 26) + 20;  // Ejemplo: N = 10 → valores [0..25]
int loly = distanciaQueSubiraElDado;
int lolx = (rand() % 26) + 10;
int lolz = (rand() % 26) + 10;

GLfloat angulos[] = {125.6f, 125.6, 125.6f, 125.6f,
54.74f, 54.74f, 54.74f, 54.74f 
  };

  glm::vec3 ejes[] = {
    glm::vec3(1.0f * 1/glm::sqrt(2), 0.0f, -1.0f * 1/glm::sqrt(2)),

    glm::vec3(-1.0f * 1/glm::sqrt(2), 0.0f, -1.0f * 1/glm::sqrt(2)),

    glm::vec3(-1.0f * 1/glm::sqrt(2), 0.0f, 1.0f * 1/glm::sqrt(2)),

    glm::vec3(1.0f * 1/glm::sqrt(2), 0.0f, 1.0f * 1/glm::sqrt(2)),

    glm::vec3(1.0f * 1/glm::sqrt(2), 0.0f, -1.0f * 1/glm::sqrt(2)),

    glm::vec3(-1.0f * 1/glm::sqrt(2), 0.0f, 1.0f * 1/glm::sqrt(2)),

    glm::vec3(-1.0f * 1/glm::sqrt(2), 0.0f, -1.0f * 1/glm::sqrt(2)),

    glm::vec3(1.0f * 1/glm::sqrt(2), 0.0f, 1.0f * 1/glm::sqrt(2)),
  };

GLfloat posicionX = 0.0f;
GLfloat posicionZ = 0.0f;
GLfloat posicionZAcumulada = 0.0f;

GLfloat posicionXAcumulada = 0.0f;


int signo1 = (rand() % 2 == 0) ? 1 : -1;
int signo2 = (rand() % 2 == 0) ? 1 : -1;
int contadoRoy = 0;

float gravedad = 4.0f;
bool dadoRodando = false;


    bool inicializado = false;
        float gravedadInicial;


glm::vec3 modelPosition = glm::vec3(0.0f, 3.0f, 0.0f); 
float angle = 0.0f;
float modelYaw = 0.0f; // rotación actual del personaje (en grados)





bool cinematicMode = false;
bool cinematicStarted = false;
bool ePressed = false;


int pruebaSkybox = 0;

//Variables animacion kirby
GLfloat kirbyMov = 0.0, kirbyRot = 0.0, kirbyMov1 = 0.0;
bool kirbyinicio = false, kirbyVuelta = false;

// DRAGONITE
float speed = 0.03f;
float radius = 45.0f;
float x = 0.0f;
float z = 0.0f;

bool subiendo1 = true;
float w = 0.0f;
bool subiendo2 = true;
float u = 0.0f;
glm::vec3 tangent;
float facingAngle = 0.0f;
float walkBob = 0.0f;
bool volar = false;
bool correr = true;
bool transicion = false;
glm::mat4 modelDragonite(1.0);

float valor = 0.0f;
float velocidad = 0.0f;
float aceleracion = 0.003f; // cuánto acelera por segundo
bool subiendo = true;
float anguloSaludo = 0.0f;
bool alternarSaludo = true;

//Animación keyframes
//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ
KeyFrame[0].movAvion_x = 0.0f; //-400.0f;
KeyFrame[0].movAvion_y = 0.0f; //400.0f;
KeyFrame[0].giroAvion = 0.0;


KeyFrame[1].movAvion_x = 0.0f;
KeyFrame[1].movAvion_y = 1200.0f;
KeyFrame[1].giroAvion = 0.0;
FrameIndex = 10;



KeyFrame[2].movAvion_x = 0.0f;
KeyFrame[2].movAvion_y = 0.0f;
KeyFrame[2].giroAvion = 0.0f;



KeyFrame[3].movAvion_x = 0.0f;
KeyFrame[3].movAvion_y = 0.0f;
KeyFrame[3].giroAvion = -90.0f;




KeyFrame[4].movAvion_x = 1200.0f;
KeyFrame[4].movAvion_y = 0.0f;
KeyFrame[4].giroAvion = -90.0;



KeyFrame[5].movAvion_x = 1200.0f;
KeyFrame[5].movAvion_y = 0.0f;
KeyFrame[5].giroAvion = -180;


KeyFrame[6].movAvion_x = 1200.0f;
KeyFrame[6].movAvion_y = 1200.0f;
KeyFrame[6].giroAvion = -180;



KeyFrame[7].movAvion_x = 1200.0f;
KeyFrame[7].movAvion_y = 1200.0f;
KeyFrame[7].giroAvion = -270;



KeyFrame[8].movAvion_x = 0.0f;
KeyFrame[8].movAvion_y = 1200.0f;
KeyFrame[8].giroAvion = -270;




KeyFrame[9].movAvion_x = 0.0f;
KeyFrame[9].movAvion_y = 1200.0f;
KeyFrame[9].giroAvion = -360;


glm::vec3 posGalactic;

glm::vec4 posLuzLampara = glm::vec4(-1.0f, 2.0f, 0.0f, 1.0f);

glm::vec3 posFinal;


float move = 0.0;
bool moveK = true;

float angleFaro  = 0;

int numSpotOn = 0;

glm::mat4 estadioAux;

	while (!mainWindow.getShouldClose()) {
		
    GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;


		if (deltaTime > 2)      //Arreglando el primer bug lol
			deltaTime = 1;	// Olvidenlo no jaló



		angulovaria += 3.14f*deltaTime; // Cambiamos de 0.5 a 3.14f


// EJERCICIO 3 DE CLASE. DRAGON
		//dragonavance en el eje X
		if (dragonAvanza) {
			if (dragonavance > -20.0f) {
				dragonavance -= 0.1f * deltaTime;
			} else {
				dragonAvanza = false;
			}
		} 
    else {
			if (dragonavance < 0.0f) {
				dragonavance += 0.1f * deltaTime;
			} else {
				dragonAvanza = true;
			}
		}

pruebaSkybox++;


		//Recibir eventos del usuario
		glfwPollEvents();
    // ---------------------------------------------------------------------------------------------
    if (mainWindow.getRecorrido2()) {
      activeCam = &camTopDown;
		  camTopDown.keyControl(mainWindow.getsKeys(), deltaTime);
      activeCam->mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
     // std::cout << activeCam->getCameraPosition().x << ", " << activeCam->getCameraPosition().y << ", " << activeCam->getCameraPosition().z << "\n";
    }
		// ---------------------------------------------------------------------------------------------
    //activeCam->mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
    // ---------------------------------------------------------------------------------------------

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(activeCam->calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

if (mainWindow.getRecorrido1()) {
  activeCam = &camera;
  activeCam->mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
  mainWindow.handleMovement(deltaTime, camera.getCameraDirection(), modelPosition, &modelYaw);
  camera.followTarget(modelPosition + glm::vec3(0.0f, 3.0f, 0.0f));
}








if (mainWindow.getRecorrido3()) {
  activeCam = &pruebaCam;
  activeCam->orbitAround(puntos[mainWindow.getNumRecorrido3() % centros.size()].centro, puntos[mainWindow.getNumRecorrido3() % centros.size()].radioX, puntos[mainWindow.getNumRecorrido3() % centros.size()].radioZ, puntos[mainWindow.getNumRecorrido3() % centros.size()].velocidad, deltaTime, puntos[mainWindow.getNumRecorrido3() % centros.size()].alturaInicial, puntos[mainWindow.getNumRecorrido3() % centros.size()].alturaFinal, true);
}



    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr( activeCam->calculateViewMatrix()   ));
    glUniform3f(uniformEyePosition, activeCam->getCameraPosition().x, activeCam->getCameraPosition().y, activeCam->getCameraPosition().z);
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//lowerLight = activeCam->getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, activeCam->getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

		numSpotOn = 0;

		if (modelPosition.z >= -33) {
			if (mainWindow.getFaro() == true) {
				spotLightsAux[numSpotOn] = spotLights[0];
				numSpotOn++;
			}
			if (mainWindow.getRing1() == true) {
				spotLightsAux[numSpotOn] = spotLights[1];
				numSpotOn++;
			}
			if (mainWindow.getRing2() == true) {
				spotLightsAux[numSpotOn] = spotLights[2];
				numSpotOn++;
			}
			
			shaderList[0].SetSpotLights(spotLightsAux, numSpotOn);
			
		}
		if (modelPosition.z < -33) {
			if (mainWindow.getPiramide() == true) {
				spotLightsAux[numSpotOn] = spotLights[3];
				numSpotOn++;
			}
			shaderList[0].SetSpotLights(spotLightsAux, numSpotOn);
		}


		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
    
   

    //PISO 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, -2.0f, -190.0f));
		model = glm::scale(model, glm::vec3(41.2f, 1.0f, 14.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pastoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

	//PISO 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, -2.1f, -125.0f));
		model = glm::scale(model, glm::vec3(41.2f, 1.0f, 16.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		transicionTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

	//PISO 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, -2.15f, -85.0f));
		model = glm::scale(model, glm::vec3(41.2f, 1.0f, 16.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tierraTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

	//PISO 5
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, -2.17f, -0.0f));
		model = glm::scale(model, glm::vec3(41.2f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		asfaltoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


	//PISO 6
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(350.0f, -2.07f, 188.0f)); // 2.07
		model = glm::scale(model, glm::vec3(13.2f, 1.0f, 11.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		arenaTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

	//PISO 7
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, -4.9f, -190.0f));
		model = glm::scale(model, glm::vec3(80.2f, 1.0f, 80.2f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		marTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

/*model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -3.0f, -100.0));
model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
torch.RenderModel();*/

		if (!play)
			play = true;



		/*
		if (mainWindow.getGalactic() == true) {
			play = true;
		  //Keyframes
		  //playIndex = 0;
		  //i_curr_steps = i_max_steps;
		  //interpolation();
		  mainWindow.setGalactic(false);
		}
		*/
		animate();


		model = glm::mat4(1.0);
		posGalactic = glm::vec3(-600 + movAvion_x, 100.0f + 5 * sin(angulovaria), 600 + movAvion_y);

		model = glm::translate(model, posGalactic);
		//model = glm::translate(model, glm::vec3(-400.0f, 0.0f, -400.0));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GalacticNova.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(295.0f, -15.0f, -85.0));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.45f, 0.35f, 0.45f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Mansion.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -2.0f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
PiramideQ.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, 2.0f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbySant.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(5.0f, 2.0f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyBocaChiquita.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(10.0f, 2.0f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyBostezando.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-5.0f, 2.0f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyRaro.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyTriste.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-15.0f, 2.0f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyEnojado.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(15.0f, 2.0f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyDormido.RenderModel();


model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(20.0f, 2.0f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyFeliz.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, 2.0f, -210.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbySorprendido.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-5.0f, 2.0f, -210.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyNormal.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(5.0f, 2.0f, -210.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyMalvado.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(10.0f, 2.0f, -210.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
KirbyAbsorbiendo.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -2.0f, -190.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(4.0f, 3.0f, 4.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Temple.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-250.0f, -1.18f, -220.0)); // -1.18
model = glm::scale(model, glm::vec3(50.0f, 40.0f, 50.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Lago.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-265.0f, -0.6f, -220.0));
model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Boat.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-250.0f, -1.18f, -220.0));
model = glm::scale(model, glm::vec3(50.0f, 40.0f, 50.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Arbol.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-265.0f, -2.0f, -120.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Casita1.RenderModel();

for (int i = 0; i < 6; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-295.0f + 100 * i, -2.3f, -150.0));
	model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Antorcha.RenderModel();
	if (luces[3] == 0 && i == 0) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsNaranja[0].SetPos(posFinal);
	}

	if (luces[3] == 0 && i == 1) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsNaranja[1].SetPos(posFinal);
	}

	if (luces[3] == 0 && i == 2) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsNaranja[2].SetPos(posFinal);
	}

	if (luces[3] == 1 && i == 3) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsNaranja[0].SetPos(posFinal);
	}

	if (luces[3] == 1 && i == 4) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsNaranja[1].SetPos(posFinal);
	}

	if (luces[3] == 1 && i == 5) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsNaranja[2].SetPos(posFinal);
	}
}






if (mainWindow.getKirby1()) {
	
	

	if (kirbyVuelta == false) {
		if (kirbyMov >= 0 && kirbyMov <= 20) {
			kirbyMov += 0.5 * deltaTime;
		}
		if (kirbyMov >= 20) {
			if (kirbyRot >= 0 && kirbyRot <= 90)
				kirbyRot += 1.5 * deltaTime;
		}

		if (kirbyRot >= 90) {
			if (kirbyMov1 >= 0 && kirbyMov1 <= 145) {
				kirbyMov1 += 0.5 * deltaTime;
			}
		}

		if (kirbyMov1 >= 145) {
			if (kirbyRot >= 90 && kirbyRot <= 180)
				kirbyRot += 1.5 * deltaTime;
			if (kirbyRot >= 180) {
				kirbyVuelta = true;
			}
		}
		
	}

	if (kirbyRot >= 180 && kirbyMov >= 0) {
		if (kirbyMov >= 0) {
			kirbyMov -= 0.5 * deltaTime;
		}

	}else if (kirbyVuelta) {
		if (kirbyRot <= 182 && kirbyRot >= 0)
			kirbyRot -= 1.5 * deltaTime;
	}
}
else {
	kirbyMov = 0;
	kirbyRot = 0;
	kirbyMov1 = 0;
	kirbyVuelta = false;
	
}
if (kirbyinicio == false) {

	if (moveK) {
		if (move >= 0 && move >= 22.5)
			moveK = !moveK;
		move += 2.8 * deltaTime;

	}
	else {
		if (move <= 0 && move <= -22.5)
			moveK = !moveK;
		move -= 2.8 * deltaTime;

	}

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-265.0f + kirbyMov1, 0.6f, -120.0 - kirbyMov));
	model = glm::rotate(model, -kirbyRot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (contadorKirby == 0) {
		KirbySant.RenderModel();

	}
	else if (contadorKirby % 128 == 0) {
		cambiaKirby = rand() % 11;
	}
	Kirbys[cambiaKirby].RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(3.5f, 0.0f, -0.0));
	model = glm::rotate(model, move * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KirbyRArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-3.5f, 0.0f, -0.0));
	model = glm::rotate(model, -move * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KirbyLArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(2.3f, -3.0f, -0.0));
	model = glm::rotate(model, move * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KirbyLFoot.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-2.3f, -3.0f, 0.0));
	model = glm::rotate(model,-move * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KirbyRFoot.RenderModel();
}
else {

	if (moveK) {
		if (move >= 0 && move >= 22.5)
			moveK = !moveK;
		move += 2.8 * deltaTime;

	}
	else {
		if (move <= 0 && move <= -22.5)
			moveK = !moveK;
		move -= 2.8 * deltaTime;

	}

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-145.0f - kirbyMov1, 0.6f, -120.0 - kirbyMov));
	model = glm::rotate(model, kirbyRot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	if (contadorKirby == 0) {
		KirbySant.RenderModel();

	}
	else if (contadorKirby % 128 == 0) {
		cambiaKirby = rand() % 11;
	}
	Kirbys[cambiaKirby].RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(3.5f, 0.0f, -0.0));
	model = glm::rotate(model, move * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KirbyRArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-3.5f, 0.0f, -0.0));
	model = glm::rotate(model, -move * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KirbyLArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(2.3f, -3.0f, -0.0));
	model = glm::rotate(model, move * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KirbyLFoot.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-2.3f, -3.0f, 0.0));
	model = glm::rotate(model, -move * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KirbyRFoot.RenderModel();
}

static int vueltasMesprit = 0;
static bool mespritSubiendo;
mespritSubiendo = mainWindow.getMesprit();
static bool mespritVolando = false;
static float mespritAltura = 0.0f;
static float mespritVelocidad = 0.1f;
static float mespritAngulo = 0.001f;
static bool mespritBajando = false;

if (mespritSubiendo) {
	mespritAltura += mespritVelocidad * deltaTime;
	mespritAngulo += mespritAngulo * deltaTime;

	if (mespritAngulo >= 90.0f)
		mespritAngulo = 90.0f;
	if (mespritAltura >= 10.0f) {
		mespritAltura = 10.0f;
		mespritSubiendo = false;
		mainWindow.setMesprit(false);
		mespritVolando = true;
	}
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-265.0f, 1.3f + mespritAltura, -220.0));
	model = glm::rotate(model, (180 + mespritAngulo) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritBody.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.02f, 0.41f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritHead.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.12f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritRArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.12f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritLArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.05f, -0.1f, -0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritTail.RenderModel();
}

else if (mespritVolando) {
	// Variables
	static float t = M_PI / 2.0f;
	float velocidad = 0.05f;
	float a = 20.0f;
	static float signo = 1.0f;
	static float yPrev = 0.0f;
	static float zPrev = 0.0f;

	// Actualiza el parámetro
	t += velocidad * deltaTime;

	// --- Posición actual ---
	float denom = 1.0f + pow(sin(t), 2);
	float y = (a * cos(t)) / denom;
	float z = (a * cos(t) * sin(t)) / denom;



	// --- Posición un poquito adelante (para estimar la dirección) ---
	float dt = 0.01f;
	float denom2 = 1.0f + pow(sin(t + dt), 2);
	float y2 = (a * cos(t + dt)) / denom2;
	float z2 = (a * cos(t + dt) * sin(t + dt)) / denom2;

	// --- Vector tangente (dirección del movimiento) ---
	float dy = y2 - y;
	float dz = z2 - z;

	// --- Ángulo en radianes ---
	float angulo = atan2(dz, dy); // dirección de vuelo
	// Diferencias
	float dy2 = y - yPrev;
	float dz2 = z - zPrev;

	// Evitar que cambie de signo por ruido pequeño
	float umbral = 0.0001f;
	if (fabs(dy2) > umbral || fabs(dz2) > umbral) {
		signo = (dy2 > 0.0f) ? -1.0f : 1.0f; // detecta dirección principal del vuelo
	}
	float targetSigno = (dy2 > 0.0f) ? -1.0f : 1.0f;
	signo = glm::mix(signo, targetSigno, 0.01f * deltaTime); // suaviza la transición

	// Actualiza posición anterior
	yPrev = y;
	zPrev = z;


	if (t > 2 * M_PI) t -= 2 * M_PI;
	float tCentro1 = M_PI / 2.0f;
	float tCentro2 = 3.0f * M_PI / 2.0f;

	// cuando pasa cerca del centro
	if (fabs(t - tCentro1) < 0.01f || fabs(t - tCentro2) < 0.01f) {
		//std::cout << "roy" << x << z << "\t" << vueltasMesprit << std::endl;
		vueltasMesprit++;
	}
	if (vueltasMesprit == 10) {
		mespritVolando = false;
		mespritBajando = true;
	}
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-265.0f, 1.3f + mespritAltura + z, -220.0 + y));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, signo * 30 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
	model = glm::rotate(model, -angulo, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritBody.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.02f, 0.41f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritHead.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.12f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritRArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.12f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritLArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.05f, -0.1f, -0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritTail.RenderModel();





}
else if (mespritBajando) {
	mespritAltura -= mespritVelocidad * deltaTime;
	mespritAngulo -= mespritAngulo * deltaTime;

	if (mespritAngulo <= 0.0f)
		mespritAngulo = 0.0f;
	if (mespritAltura <= 0.0f) {
		mespritAltura = 0.0f;
		vueltasMesprit = 0;
		mespritBajando = false;
	}
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-265.0f, 1.3f + mespritAltura, -220.0));
	model = glm::rotate(model, (180 + mespritAngulo) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritBody.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.02f, 0.41f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritHead.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.12f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritRArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.12f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritLArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.05f, -0.1f, -0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritTail.RenderModel();





}
else {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-265.0f, 1.3f + 0.5f * sin(glm::radians(angulovaria)), -220.0));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritBody.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.02f, 0.41f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritHead.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.12f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritRArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.12f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritLArm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.05f, -0.1f, -0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	MespritTail.RenderModel();
}
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-120.0f, -1.18f, -220.0)); // -1.18
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(50.0f, 40.0f, 70.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Lago.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-120.0f, -1.18f, -220.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(50.0f, 40.0f, 70.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Arbol.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-120.0f, -2.0f, -120.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Casita2.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-155.0f, -0.6f, -190.0));
model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Boat.RenderModel();

static glm::vec3 waypoints[4] = {
	{-155.025, 5.0f, -168.595},
	{-86.0435f, 5.0f, -160.001f},
	{-89.8674f, 5.0f, -277.082f},
	{-151.8f, 5.0f, -278.248f}
};

static int currentSegment = 0;
static float t = 0.0f;

float speed1 = 0.015f * deltaTime;
float waveFreq = 4.0f;
float waveAmp = 3.0f;
float heightAmp = 1.5f;

t += speed1;
if (t > 1.0f) {
	t = 0.0f;
	currentSegment = (currentSegment + 1) % 4;
}

glm::vec3 p0 = waypoints[currentSegment];
glm::vec3 p1 = waypoints[(currentSegment + 1) % 4];

glm::vec3 basePos = glm::mix(p0, p1, t);

glm::vec3 dir = glm::normalize(p1 - p0);
glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0))); // perpendicular

float wave = sin(t * glm::two_pi<float>() * waveFreq);       // valor de la onda
glm::vec3 offset = right * wave * waveAmp;                   // desplazamiento lateral
offset.y = sin(t * glm::two_pi<float>() * waveFreq * 0.5f) * heightAmp; // vertical

glm::vec3 position = basePos + offset;
// --- ORIENTACIÓN ---
glm::vec3 baseDir = glm::normalize(p1 - p0);                 // rumbo principal
glm::vec3 localDir = glm::normalize(baseDir + right * wave * 0.2f); // leve desviación

// Calcular ángulos
float yaw = atan2(localDir.x, localDir.z);  // rotación horizontal
float pitch = -asin(localDir.y);            // inclinación vertical
float roll = -wave * 0.0f;
yaw += glm::radians(90.0f);

model = glm::mat4(1.0f);
model = glm::translate(model, position);
model = glm::rotate(model, 45 * toRadians, -right);
model = glm::rotate(model, yaw, glm::vec3(0, 1, 0));   // rumbo
model = glm::rotate(model, pitch, glm::vec3(1, 0, 0));   // inclinación vertical
model = glm::rotate(model, roll, glm::vec3(0, 0, 1));   // inclinación lateral (efecto de vuelo)
model = glm::scale(model, glm::vec3(3.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
AzelfBody.RenderModel();

model = modelaux;
model = glm::translate(model, glm::vec3(0.02f, 0.17f, 0.0));
//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
AzelfRArm.RenderModel();

model = modelaux;
model = glm::translate(model, glm::vec3(0.02f, 0.17f, 0.0));
//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
AzelfLArm.RenderModel();

model = modelaux;
model = glm::translate(model, glm::vec3(0.05f, -0.19f, -0.0));
//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
AzelfTail.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(180.0f, -1.18f, -220.0));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(80.0f, 40.0f, 80.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Lago.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(180.0f, -1.18f, -220.0));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(80.0f, 40.0f, 80.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Arbol.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(80.0f, -1.18f, -120.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Casita3.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(150.0f, 7.5f, -110.0));
model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Casita4.RenderModel();


model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(180.0f, -0.6f, -200.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Boat.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(190.0f, -0.6f, -250.0));
model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Cueva.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(190.0f, 2.0f + 0.5f * sin(glm::radians(angulovaria)), -250.0));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
UxiefBody.RenderModel();

model = modelaux;
model = glm::translate(model, glm::vec3(-0.02f, 0.08f, 0.0));
//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
UxiefRArm.RenderModel();

model = modelaux;
model = glm::translate(model, glm::vec3(-0.02f, 0.08f, 0.0));
//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
UxiefLArm.RenderModel();

model = modelaux;
model = glm::translate(model, glm::vec3(0.05f, -0.18f, -0.0));
//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
UxiefTail.RenderModel();


//////////////////////////////BOSQUE

for (int i = 0; i < 4; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-220 + i*130, -2.0f, 0.0));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f, 6.0f, 5.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Bosque.RenderModel();
}

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(280.0f, -2.0f, 20.0));
model = glm::scale(model, glm::vec3(5.0f, 6.0f, 3.5f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Bosque.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(417.47, -2.0, -91.9289));
model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(5.0f, 6.0f, 3.5f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Bosque.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(344.547, -2.0, -200.415));
model = glm::scale(model, glm::vec3(5.0f, 6.0f, 3.5f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Bosque.RenderModel();

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(344.547, -2.0, -280.415));
model = glm::scale(model, glm::vec3(5.0f, 6.0f, 3.5f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Bosque.RenderModel();


model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(20.0f, -0.0f, 185.0));
estadioAux = model;
model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(6.01f, 8.01f, 6.01f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
EstadioA.RenderModel();

model = estadioAux;
//model = glm::translate(model, glm::vec3(20.0f, -0.0f, 150.0));
model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(6.0f, 8.0f, 6.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Estadio.RenderModel();

model = estadioAux;
model = glm::translate(model, glm::vec3(-65.0f, -1.0f, 0.0));
model = glm::scale(model, glm::vec3(0.7f, 0.5f, 0.7f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Ring.RenderModel();

if (avanzando) {
	if (brazos >= 22.5)
		avanzando = !avanzando;
	brazos += 0.8 * deltaTime;

}
else {
	if (brazos <= -22.5)
		avanzando = !avanzando;
	brazos -= 0.8 * deltaTime;

}

if (mainWindow.getPelea()) {
	if (movPelea >= 0 && movPelea <= 8)
		movPelea += 0.1 * deltaTime;
	if (movPelea >= 2) {
		if (rotPelea >= 0 && rotPelea <= 90) 
			rotPelea += 1.7 * deltaTime;
	}
	if (movPelea >= 7) {
		if(caePelea <= 8)
			caePelea += 0.2 * deltaTime;
	}


	model = estadioAux;
	model = glm::translate(model, glm::vec3(10.0f - movPelea * 1.1, 2.4f + (movPelea * movPelea / 4) - (caePelea * caePelea / 4), 10.0 - movPelea * 1.1));
	model = glm::rotate(model, 40 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, -rotPelea * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.6f, 2.6f, 2.6f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KingBody.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.5f, 1.0f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KingLarm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.5f, 1.0f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KingRarm.RenderModel();

	model = estadioAux;
	model = glm::translate(model, glm::vec3(-10.0f + movPelea * 1.1, 6.0f + (movPelea * movPelea / 4) - (caePelea * caePelea / 3.4), -10.0 + movPelea * 1.1));
	model = glm::rotate(model, 220 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, -rotPelea * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiB.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, 0.35f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiA.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.3f, 0.35f, 0.0));
	model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiA.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.15f, -0.50f, 0.0));
	model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiL.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.15f, -0.50f, 0.0));
	model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0));
	model = glm::rotate(model, 160 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiL.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, -0.50f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiT.RenderModel();

	model = estadioAux;
	model = glm::translate(model, glm::vec3(14.0f - movPelea * 1.3, 9.5f + (movPelea * movPelea / 5) - (caePelea * caePelea / 3.3), -14.0 + movPelea * 1.3));
	model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotPelea * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Hawlucha.RenderModel();

	movCuerdas = 0;
	rotCuerdas = 0;
}
else{
	//std::cout << movCuerdas << "Roy\n";
	
	if (girando) {
		if (movCuerdas <= 0) {
			if (deltaTime > 30)
				deltaTime = 1.5;
			movCuerdas += 0.5 * deltaTime;
			//std::cout << deltaTime << "Sant\n";
			if (movCuerdas >= 0) {
				girando = !girando;
			}
		}
		if (movCuerdas >= -5 && movCuerdas <= 0) {
			if (rotCuerdas >= 0 && rotCuerdas <= 180) {
				rotCuerdas += 20.5 * deltaTime;
				if (rotCuerdas >= 180) {
					rotCuerdas = 181;
				}
			}
		}
	}
	else {
		if (movCuerdas >= -20) {
			movCuerdas -= 0.5 * deltaTime;
			if (movCuerdas <= -20) {
				girando = !girando;
			}
		}

		if (movCuerdas <= -15 && movCuerdas >= -20) {
			if (rotCuerdas >= 180 && rotCuerdas <= 360) {
				rotCuerdas += 20.5 * deltaTime;
				if (rotCuerdas >= 360) {
					rotCuerdas = 1;
				}
			}
		}
	}

	model = estadioAux;
	model = glm::translate(model, glm::vec3(10.0f , 2.4f , 10.0 + movCuerdas));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotCuerdas * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.6f, 2.6f, 2.6f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KingBody.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.5f, 1.0f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KingLarm.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.5f, 1.0f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KingRarm.RenderModel();

	model = estadioAux;
	model = glm::translate(model, glm::vec3(-10.0f , 6.0f , -10.0 - movCuerdas));
	//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotCuerdas * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiB.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, 0.35f, 0.0));
	model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, brazos * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiA.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.3f, 0.35f, 0.0));
	model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0));
	model = glm::rotate(model, 160 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, 22.5f - brazos * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiA.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.15f, -0.50f, 0.0));
	model = glm::rotate(model, -brazos * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiL.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.15f, -0.50f, 0.0));
	model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0));
	model = glm::rotate(model, 160 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, brazos * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiL.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, -0.50f, 0.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	KoraiT.RenderModel();

	model = estadioAux;
	model = glm::translate(model, glm::vec3(14.0f , 9.5f, -14.0));
	model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Hawlucha.RenderModel();

	movPelea = 0;
	rotPelea = 0;
	caePelea = 0;
}

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(250.0f, -2.0f, 150.0));
model = glm::scale(model, glm::vec3(10.6f, 10.6f, 10.6f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Faro.RenderModel();

angleFaro -= 0.03 * deltaTime;
static float xFaro = cos(angleFaro);
static float zFaro = sin(angleFaro);
xFaro = cos(angleFaro);
zFaro = sin(angleFaro);
spotLights[2].SetFlash(glm::vec3(250, 111, 150), glm::vec3(xFaro, 0, zFaro));

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-220.0f, -2.0f, 150.0));
model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
Torre.RenderModel();


for (int i = 0; i < 2; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-130.0f, 15.0f, 100.0 + i * 150));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.8f, 0.5f, 0.8f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Edificio.RenderModel();
}

for (int i = 0; i < 2; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(170.0f, 15.0f, 80.0 + i * 150));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 90  * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.8f, 0.5f, 0.8f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Edificio.RenderModel();
}

for (int i = 0; i < 3; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-210.0f + 50 * i, -2.5f, 210.0));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Lampara.RenderModel();
	if (luces[0] == 0 && i == 0) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsBlanco[0].SetPos(posFinal);
	}

	if (luces[1] == 0 && i == 1) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsBlanco[1].SetPos(posFinal);
	}

	if (luces[2] == 0 && i == 2) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsBlanco[2].SetPos(posFinal);
	}
}

for (int i = 0; i < 3; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-210.0f + 50 * i, -2.0f, 140.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.7f, 3.7f, 3.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LamparaSant.RenderModel();

	if (luces[0] == 1 && i == 0) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsAzul[0].SetPos(posFinal);
	}

	if (luces[1] == 1 && i == 1) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsAzul[1].SetPos(posFinal);
	}

	if (luces[2] == 1 && i == 2) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsAzul[2].SetPos(posFinal);
	}

}

for (int i = 0; i < 3; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(140.0f, -2.5f, 90.0 + i * 30));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Lampara.RenderModel();
	if (luces[0] == 2 && i == 0) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsBlanco[0].SetPos(posFinal);
	}

	if (luces[1] == 2 && i == 1) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsBlanco[1].SetPos(posFinal);
	}

	if (luces[2] == 2 && i == 2) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsBlanco[2].SetPos(posFinal);
	}

}

for (int i = 0; i < 3; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(140.0f, -2.0f, 230.0 + i * 30));
	model = glm::scale(model, glm::vec3(3.7f, 3.7f, 3.7f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LamparaSant.RenderModel();

	if (luces[0] == 3 && i == 0) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsAzul[0].SetPos(posFinal);
	}

	if (luces[1] == 3 && i == 1) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsAzul[1].SetPos(posFinal);
	}

	if (luces[2] == 3 && i == 2) {
		posFinal = glm::vec3(model * posLuzLampara);
		pointLightsAzul[2].SetPos(posFinal);
	}
}


luces[0]++;
luces[1]++;
luces[2]++;
luces[3]++;

if (luces[0] == 4) {
	luces[0] = 0;
	luces[1] = 0;
	luces[2] = 0;
}
if (luces[3] == 2) {
	luces[3] = 0;
}



		
if (mainWindow.getCaminando()) {
	
	if (rotacionesCuerpo[0] <= 25 && avanzar == true) {
		rotacionesCuerpo[0] += 0.8 * deltaTime;
		rotacionesCuerpo[1] += 1.2 * deltaTime;
		rotacionesCuerpo[2] -= 0.8 * deltaTime;
		rotacionesCuerpo[3] -= 1.2 * deltaTime;
		rotacionesCuerpo[4] -= 0.8 * deltaTime;
		rotacionesCuerpo[5] -= 0.8 * deltaTime;
		rotacionesCuerpo[6] += 0.8 * deltaTime;
		rotacionesCuerpo[7] += 0.8 * deltaTime;
		if (rotacionesCuerpo[0] >= 25) {
			avanzar = false;
		}
	}
	else if (rotacionesCuerpo[0] >= -25 && avanzar == false) {
		rotacionesCuerpo[0] -= 0.8 * deltaTime;
		rotacionesCuerpo[1] -= 1.2 * deltaTime;
		rotacionesCuerpo[2] += 0.8 * deltaTime;
		rotacionesCuerpo[3] += 1.2 * deltaTime;
		rotacionesCuerpo[4] += 0.8 * deltaTime;
		rotacionesCuerpo[5] += 0.8 * deltaTime;
		rotacionesCuerpo[6] -= 0.8 * deltaTime;
		rotacionesCuerpo[7] -= 0.8 * deltaTime;
		if (rotacionesCuerpo[0] <= -25) {
			avanzar = true;
		}
	}
}
else {
	if (contadoRoy % 256 == 0) {
		detenerse = !detenerse;
	}
	if (detenerse == false) {
		if (rotacionesCuerpo[0] < 1) {
			rotacionesCuerpo[0] += 1.3 * deltaTime;
			rotacionesCuerpo[1] += 1.7 * deltaTime;
			rotacionesCuerpo[2] -= 1.3 * deltaTime;
			rotacionesCuerpo[3] -= 1.7 * deltaTime;
			rotacionesCuerpo[4] -= 1.3 * deltaTime;
			rotacionesCuerpo[5] -= 1.3 * deltaTime;
			rotacionesCuerpo[6] += 1.3 * deltaTime;
			rotacionesCuerpo[7] += 1.3 * deltaTime;
		}
		else if (rotacionesCuerpo[0] > 1) {
			rotacionesCuerpo[0] -= 1.3 * deltaTime;
			rotacionesCuerpo[1] -= 1.7 * deltaTime;
			rotacionesCuerpo[2] += 1.3 * deltaTime;
			rotacionesCuerpo[3] += 1.7 * deltaTime;
			rotacionesCuerpo[4] += 1.3 * deltaTime;
			rotacionesCuerpo[5] += 1.3 * deltaTime;
			rotacionesCuerpo[6] -= 1.3 * deltaTime;
			rotacionesCuerpo[7] -= 1.3 * deltaTime;
		}
		if (rotacionesCuerpo[0] > 0.9 && rotacionesCuerpo[0] <1.1)
			detenerse = true;
	}
	avanzar = true;
}


model = glm::mat4(1.0);
model = glm::translate(model, modelPosition + glm::vec3(0.0f, 2.0f, 0.0f));
model = glm::rotate(model, glm::radians(modelYaw), glm::vec3(0.0f, 1.0f, 0.0f));
//model = glm::translate(model, posicionesCuerpo[0]);
//model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
modelaux = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
MayBody.RenderModel();

model = modelaux;
model = glm::translate(model, posicionesCuerpo[1]);
model = glm::rotate(model, rotacionesCuerpo[0] * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
modelauxBP = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
MayRightArm.RenderModel();

model = modelauxBP;
model = glm::translate(model, posicionesCuerpo[2]);
model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
//model = glm::translate(model, posicionesCuerpo[2]);
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
model = glm::rotate(model, rotacionesCuerpo[1] * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
MayRightHand.RenderModel();

model = modelaux;
model = glm::translate(model, posicionesCuerpo[3]);
model = glm::rotate(model, rotacionesCuerpo[2] * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
modelauxBP = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
MayLeftArm.RenderModel();

model = modelauxBP;
model = glm::translate(model, posicionesCuerpo[4]);
model = glm::rotate(model, -35 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, rotacionesCuerpo[3] * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
MayLeftHand.RenderModel();

model = modelaux;
model = glm::translate(model, posicionesCuerpo[5]);
model = glm::rotate(model, rotacionesCuerpo[4] * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
modelauxBP = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
MayRightLeg.RenderModel();

model = modelauxBP;
model = glm::translate(model, posicionesCuerpo[6]);
model = glm::rotate(model, rotacionesCuerpo[5] * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
MayRightFoot.RenderModel();

model = modelaux;
model = glm::translate(model, posicionesCuerpo[7]);
model = glm::rotate(model, rotacionesCuerpo[6] * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
modelauxBP = model;
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
MayLeftLeg.RenderModel();

model = modelauxBP;
model = glm::translate(model, posicionesCuerpo[8]);
model = glm::rotate(model, rotacionesCuerpo[7] * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
MayLeftFoot.RenderModel();







if (DayNight) {
	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(nullptr, 0);
	if (muevePataB) {
		if (muevePata >= 22.5)
			muevePataB = !muevePataB;
		muevePata += 0.5 * deltaTime;

	}
	else {
		if (muevePata <= -22.5)
			muevePataB = !muevePataB;
		muevePata -= 0.5 * deltaTime;

	}

	model = glm::mat4(1.0);
	if (rotarSobreMapa == 0)
		model = glm::translate(model, glm::vec3(0.0, -2.0f, 310.0f - trasladSobreMapa));

	rotarSobreMapa -= 0.05 * deltaTime;
	trasladSobreMapa += 0.5 * deltaTime;
	dirLight += 0.001 * deltaTime;
	if (rotarSobreMapa <= -90) {
		rotarSobreMapa = 0;
		trasladSobreMapa = 0;
		dirLight = -1.0;
		DayNight = !DayNight;
	  skybox = Skybox(skyboxFaces2);
	}
	mainLight.SetFlash(glm::vec3(0.0, -1.0, -dirLight));
	model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotarSobreMapa * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0, 350.0f, 350 - trasladSobreMapa));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SolB.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-3.5f, 0.0f, -9.5f));
	model = glm::rotate(model, muevePata * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SolPD.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(4.5f, 0.0f, -9.5f));
	model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, -muevePata * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SolPD.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 7.0f));
	model = glm::rotate(model, -muevePata * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SolPT.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(2.5f, 0.0f, 7.0f));
	model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, muevePata * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SolPT.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.0f, 3.0f, 10.0f));
	model = glm::rotate(model, -muevePata * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	SolC.RenderModel();
}
else {
	shaderList[0].SetDirectionalLight(&NightLight);
	if (modelPosition.z >= -33) {
		if (luces[0] == 0 || luces[0] == 2)
			shaderList[0].SetPointLights(pointLightsAzul, 3);
		if (luces[0] == 1 || luces[0] == 3)
			shaderList[0].SetPointLights(pointLightsBlanco, 3);
	}
	if (modelPosition.z < -33) {
		shaderList[0].SetPointLights(pointLightsNaranja, 3);
	}
	if (muevePataB) {
		if (muevePata >= 22.5)
			muevePataB = !muevePataB;
		muevePata += 0.5 * deltaTime;

	}
	else {
		if (muevePata <= -22.5)
			muevePataB = !muevePataB;
		muevePata -= 0.5 * deltaTime;

	}

	dirLight += 0.05 * deltaTime;
	model = glm::mat4(1.0);
	if (rotarSobreMapa == 0)
		model = glm::translate(model, glm::vec3(0.0, -2.0f, 310.0f - trasladSobreMapa));

	rotarSobreMapa -= 0.05 * deltaTime;
	trasladSobreMapa += 0.5 * deltaTime;
	dirLight += 0.001 * deltaTime;
	if (rotarSobreMapa <= -90) {
		rotarSobreMapa = 0;
		trasladSobreMapa = 0;
		dirLight = -1.0;
		DayNight = !DayNight;
    skybox = Skybox(skyboxFaces);
}
	mainLight.SetFlash(glm::vec3(0.0, -1.0, -dirLight));
	model = glm::rotate(model, 45 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotarSobreMapa * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0, 350.0f, 350 - trasladSobreMapa));
	model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LunaBody.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.7f, 0.0f, -0.0f));
	model = glm::rotate(model, muevePata * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LunaWing.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.7f, 0.0f, -0.0f));
	model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, muevePata * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	LunaWing.RenderModel();
}


static int lolazo = 0;

if (correr) {
	angle -= speed * deltaTime;
	// Ecuacion de la circunferencia
	x = radius * cos(angle);
	z = radius * sin(angle);

	if (subiendo1)
		w += deltaTime * 15.5f;
	else
		w -= deltaTime * 15.5f;

	if (subiendo2)
		u += deltaTime * 10.5f;
	else
		u -= deltaTime * 10.5f;

	if (u >= 75.0f) {
		u = 75.0f;
		subiendo2 = false;
	}
	if (u <= -75.0f) {
		u = -75.0f;
		subiendo2 = true;
	}
	// Si alcanza los límites, invierte la dirección
	if (w >= 70.0f) {
		w = 70.0f;
		subiendo1 = false;
	}
	if (w <= -20.0f) {
		w = -20.0f;
		subiendo1 = true;
	}

	// Vector tangente a la trayectoria
	tangent = glm::normalize(glm::vec3(-sin(angle), 0.0f, cos(angle)));
	// Calcular ángulo de orientación a partir del tangente
	facingAngle = atan2(tangent.z, tangent.x);
	walkBob = 5.0f * sin(angle);//angulovaria += 3.14f*deltaTime;

	model = glm::mat4(1.0);
	//model = glm::translate(model, glm::vec3(250.0f + x, 15.25f + walkBob, 150.0f + z));
	model = glm::translate(model, glm::vec3(250.0f + x * 0.7, 6.0f + walkBob * 0.2, 150.0f + z * 0.7));
	model = glm::rotate(model, -facingAngle + 360 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // orientar tangencialmente
	//model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.5f, 1.48f, 0.53));
	model = glm::rotate(model, w * 0.2f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Ala_Izq_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.5f, 1.48f, -0.53));
	model = glm::rotate(model, -w * 0.2f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Ala_Der_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, 1.2f, -1.0));
	model = glm::rotate(model, -u * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Brazo_Izq_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, 1.2f, 1.0));
	model = glm::rotate(model, u * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Brazo_Der_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, -1.4f, -1.1));
	model = glm::rotate(model, u * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Pierna_Izq_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, -1.4f, 1.1));
	model = glm::rotate(model, -u * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Pierna_Der_M.RenderModel();
}
if (mainWindow.getDragonite()) {



	if (alternarSaludo)
		anguloSaludo -= deltaTime * 25.0f;
	else
		anguloSaludo += deltaTime * 25.0f;

	if (anguloSaludo >= 70.0f) {
		anguloSaludo = 70.0f;
		alternarSaludo = true;
	}
	if (anguloSaludo <= -40.0f) {
		anguloSaludo = -40.0f;
		alternarSaludo = false;
	}





	model = glm::mat4(1.0);
	//model = glm::translate(model, glm::vec3(250.0f + x, 15.25f + walkBob, 150.0f + z));
	model = glm::translate(model, glm::vec3(250.0f + x * 0.7, 6.0f + walkBob * 0.2, 150.0f + z * 0.7));
	model = glm::rotate(model, -facingAngle + 360 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // orientar tangencialmente
	//model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.5f, 1.48f, 0.53));
	model = glm::rotate(model, (w * 0.2f + anguloSaludo) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Ala_Izq_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.5f, 1.48f, -0.53));
	model = glm::rotate(model, (-w * 0.2f - anguloSaludo) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Ala_Der_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, 1.2f, -1.0));
	model = glm::rotate(model, (-u) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, -anguloSaludo * toRadians,  glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Brazo_Izq_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, 1.2f, 1.0));
	model = glm::rotate(model, u * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Brazo_Der_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, -1.4f, -1.1));
	model = glm::rotate(model, u * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Pierna_Izq_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, -1.4f, 1.1));
	model = glm::rotate(model, -u * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Pierna_Der_M.RenderModel();
	correr = false;
	lolazo++;
	if (lolazo % 500 == 0) {
		volar = true;
		mainWindow.setDragonite(false);
		transicion = true;
		modelDragonite = modelaux;
	}
}
else if (volar) {



	if (subiendo1)
		w += deltaTime * 15.5f;
	else
		w -= deltaTime * 15.5f;

	if (subiendo2)
		u += speed * deltaTime * 15.5f;
	else
		u -= speed * deltaTime * 15.5f;

	if (u >= 25.0f) {
		u = 25.0f;
		subiendo2 = false;
	}
	if (u <= -25.0f) {
		u = -25.0f;
		subiendo2 = true;
	}
	// Si alcanza los límites, invierte la dirección
	if (w >= 70.0f) {
		w = 70.0f;
		subiendo1 = false;
	}
	if (w <= -20.0f) {
		w = -20.0f;
		subiendo1 = true;
	}

	// Vector tangente a la trayectoria
	tangent = glm::normalize(glm::vec3(-sin(angle), 0.0f, cos(angle)));
	// Calcular ángulo de orientación a partir del tangente
	facingAngle = atan2(tangent.z, tangent.x);
	walkBob = 5.0f * sin(angle);//angulovaria += 3.14f*deltaTime;


	if (subiendo) {
		velocidad += aceleracion * deltaTime;     // acelera al subir
		valor += velocidad * deltaTime;

		// empieza a frenar cerca del tope
		if (valor > 80.0f)
			velocidad -= aceleracion * 2.0f * deltaTime;

		if (valor >= 110.0f) {
			valor = 110.0f;
			subiendo = false;
			velocidad = 0.0f; // reinicia la velocidad
		}
	}
	else {
		velocidad += aceleracion * deltaTime;     // acelera al bajar
		valor -= velocidad * deltaTime;

		// frena antes de llegar a 0
		if (valor < 20.0f)
			velocidad -= aceleracion * 2.0f * deltaTime;

		if (valor <= 0.0f) {
			valor = 0.0f;
			subiendo = true;
			velocidad = 0.0f;
		}
	}


	if (transicion) {
		model = modelDragonite;
		model = glm::translate(model, glm::vec3(250.0f + x, 10.0f /*+ walkBob*/ + valor, 150.0f + z));
		model = glm::rotate(model, -facingAngle + 360 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // orientar tangencialmente
		model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragonite_M.RenderModel();
		transicion = false;
	}
	else {
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(250.0f + x, 10.0f /*+ walkBob */ + valor, 150.0f + z));
		model = glm::rotate(model, -facingAngle + 360 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // orientar tangencialmente
		model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragonite_M.RenderModel();

	}

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.5f, 1.48f, 0.53));
	model = glm::rotate(model, w * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Ala_Izq_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.5f, 1.48f, -0.53));
	model = glm::rotate(model, -w * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Ala_Der_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, 1.2f, -1.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Brazo_Izq_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, 1.2f, 1.0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Brazo_Der_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, -1.4f, -1.1));
	//model = glm::rotate(model, u * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Pierna_Izq_M.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, -1.4f, 1.1));
	//model = glm::rotate(model, -u * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Dragonite_Pierna_Der_M.RenderModel();

	speed = 0.03f;
	radius = 45.0f;
	float altura = 6.0f;
	angle -= speed * deltaTime;
	// Ecuacion de la circunferencia
	x = radius * cos(angle);
	z = radius * sin(angle);


	lolazo++;
	if (mainWindow.getAterrizar()) {
		volar = false;
		mainWindow.setAterrizar(false);
		//mainWindow.setDragonite(false);
		 //transicion = true;
		 //modelDragonite = modelaux;
		correr = true;
		valor = 0.0f;
	}
}



/*
model = glm::mat4(1.0f);
model = glm::translate(model, modelPosition);
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
May_M.RenderModel();
*/






		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 // THE PRACTICE STARTS HERE --------------------------------------------------
		// Octaedro en Opengl
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f + posicionXAcumulada, 3.0f + mainWindow.getArticulacion2(), 0.0f + posicionZAcumulada));
    model = glm::rotate( model, /*randomInt * */angulos[randomInt] * toRadians, ejes[randomInt] );
    model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		octaTexture.UseTexture();
		meshList[8]->RenderMesh();


if ( mainWindow.getLanzarDado() ) {
  if ( mainWindow.getDadoSubiendo() ) {
    auxiliar = mainWindow.getArticulacion2();
    auxiliar += gravedad * deltaTime;
    mainWindow.setArticulacion2(auxiliar);
    if (gravedad > 0.15f)
        gravedad -= 0.15f;
    //if (contadoRoy % 2 == 0)
        randomInt = rand() % 8;
    if ( mainWindow.getArticulacion2() >= loly ) {
      mainWindow.setDadoSubiendo(false);
      dadoCayendo = true;
      gravedad = 0.0f;
    }
    signo1 = (rand() % 2 == 0) ? 1 : -1;
    signo2 = (rand() % 2 == 0) ? 1 : -1;
  }
  else if (dadoCayendo) {
    auxiliar = mainWindow.getArticulacion2();
    //if (contadoRoy % 2 == 0)
        randomInt = rand() % 8;
    gravedad += 0.15f;
    if (auxiliar - gravedad*deltaTime >= 0.0f) {
      auxiliar -= gravedad * deltaTime;
      mainWindow.setArticulacion2(auxiliar);
    }
    else {
      mainWindow.setArticulacion2(0.0f);
      dadoCayendo = false;
      dadoRodando = true;
      inicializado = false;
    }
  }
  else if (dadoRodando) {
      posicionXAcumulada += 0.25f * deltaTime * signo1;
      posicionX += 0.25f * deltaTime * signo1;
      posicionZAcumulada += 0.25f * deltaTime * signo2;
      posicionZ += 0.25f * deltaTime * signo2;
      //if(contadoRoy % 16 == 0)
      randomInt = rand() % 8;
      if (posicionX >= lolx || posicionZ >= lolz || posicionX <= -1*lolx || posicionZ <= -1*lolz) {
        posicionX = 0.0f;
        posicionZ = 0.0f;
        gravedad = 4.0f;
        dadoRodando = false;
      }
  }
  else {
    mainWindow.setLanzarDado(false);
  }
}



// ARCO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 7.3f, -20.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
    modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arco_M.RenderModel();

// PUERTA IZQUIERDA (Se abre por rotación)
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.1f, -2.73f, -1.57f));
		model = glm::rotate(model, mainWindow.getArticulacion1() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaIzq_M.RenderModel();

// PUERTA DERECHA (Se abre deslizando a la derecha)
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.1f, 0.35f, 1.46f + mainWindow.getArticulacion1()/84 ));
		//model = glm::rotate(model, mainWindow.getArticulacion1() * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaDer_M.RenderModel();

// PUERTAS
if (mainWindow.getEstaAbriendoI()) {
  if (mainWindow.getArticulacion1() < 84.0f) {
    auxiliar = mainWindow.getArticulacion1();
    auxiliar += 0.2f * deltaTime;
    mainWindow.setArticulacion1(auxiliar);
  }
  else {
    mainWindow.setEstaAbriendoI(false);
    // LOOP
    mainWindow.setEstaCerrandoI(true);
  }
}
else if (mainWindow.getEstaCerrandoI()) {
  if (mainWindow.getArticulacion1() > 0.0f) {
    auxiliar = mainWindow.getArticulacion1();
    auxiliar -= 0.2f * deltaTime;
    mainWindow.setArticulacion1(auxiliar);
  }
  else {
    mainWindow.setEstaCerrandoI(false);
    // LOOP
    mainWindow.setEstaAbriendoI(true);
  }
}

// LETRERO

for (int i = 0; i < 73; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-338.0f + 7.7 * i, -0.4f, 300.0));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(8.2f, 2.2f, 5.0f));

	//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	rejaTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[4]->RenderMesh();
}

for (int i = 0; i < 29; i++) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-342.0f , -0.4f, 296.0 - 7.7 * i));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(8.2f, 2.2f, 5.0f));

	//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	rejaTexture.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[4]->RenderMesh();
}
    model = modelaux;
    color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero_M.RenderModel();

		toffsetflechau += 0.003; // Se aumenta de 0.001 a 0.00
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = modelaux;
    model = glm::translate(model, glm::vec3(-1.9f, 3.6f, 0.0));
    model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(9.0f, 1.2f, 1.2f));

    glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Pokemon2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		

contadoRoy++;
loly = /*(rand() % 260)*/ + 30;
lolx = (rand() % 260) + 10;
lolz = (rand() % 260) + 10;

contadorKirby++;

		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}



