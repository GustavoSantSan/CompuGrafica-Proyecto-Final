#include "Window.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

Window::Window() {
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;

  articulacion1 = 0.0f;
  articulacion2 = 0.0f;
  articulacion3 = 0.0f;
  estaAbriendoI = false;
  estaCerrandoI = false;
  lanzarDado = false;
  dadoSubiendo = false;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;

  recorrido1 = true;
  recorrido2 = false;
  recorrido3 = false;
  numRecorrido3 = 0;

  dragonite = false;
  aterrizar = false;

  mesprit = false;

  galactic = false;

  kirby1 = false;
  kirby2 = false;

  faro = true;
  ring1 = true;
  ring2 = true;
  piramide = true;

  pelear = false;

	muevex = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise() {
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "PracticaXX:Nombre de la practica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
  return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

  static float paso = 2.0;
  if (theWindow->articulacion1 == 0.0f) {
	  paso = 2.0f;
  }
  else if (theWindow->articulacion1 == 84.0f) {
	  paso = -2.0f;
  }

  if (key == GLFW_KEY_X && GLFW_PRESS == action)
	  theWindow->mesprit = true;

  if (key == GLFW_KEY_H && GLFW_PRESS == action) {
	  theWindow->kirby1 = !theWindow->kirby1;
  }


  if (key == GLFW_KEY_Z && GLFW_PRESS == action)
	  theWindow->galactic = !theWindow->galactic; 

  if (key == GLFW_KEY_B && GLFW_PRESS == action)
	  theWindow->dragonite = true;

  if (key == GLFW_KEY_N && GLFW_PRESS == action)
	  theWindow->aterrizar = true;

  if (key == GLFW_KEY_F && GLFW_PRESS == action)
	  theWindow->faro = !theWindow->faro;

  if (key == GLFW_KEY_G && GLFW_PRESS == action)
	  theWindow->ring1 = !theWindow->ring1;

  if (key == GLFW_KEY_Y && GLFW_PRESS == action)
	  theWindow->ring2 = !theWindow->ring2;

  if (key == GLFW_KEY_U && GLFW_PRESS == action)
	  theWindow->piramide = !theWindow->piramide;

  if (key == GLFW_KEY_O  && GLFW_PRESS == action) {
      theWindow->estaAbriendoI = true;
  }
  if (key == GLFW_KEY_C  && GLFW_PRESS == action) {
      theWindow->estaCerrandoI = true;
  }

  if (key == GLFW_KEY_K  && GLFW_PRESS == action) {
      theWindow->estaCerrandoI = false;
      theWindow->estaAbriendoI = false;
  }


  if (key == GLFW_KEY_L && GLFW_PRESS == action) {
     theWindow->lanzarDado = theWindow->dadoSubiendo = true;
  }


if (key == GLFW_KEY_E && GLFW_PRESS == action){
   theWindow->recorrido1 = true;
   theWindow->recorrido2 = false;
   theWindow->recorrido3 = false;
}
if (key == GLFW_KEY_R && GLFW_PRESS == action){
   theWindow->recorrido1 = false;
   theWindow->recorrido2 = true;
   theWindow->recorrido3 = false;
}
if (key == GLFW_KEY_T && GLFW_PRESS == action){
   theWindow->recorrido1 = false;
   theWindow->recorrido2 = false;
   theWindow->recorrido3 = true;
}

if (key == GLFW_KEY_Q && GLFW_PRESS == action) {
    theWindow->numRecorrido3++;
}

if (key == GLFW_KEY_V && GLFW_PRESS == action)
	theWindow->pelear = !theWindow->pelear;



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}



void Window::handleMovement(GLfloat deltaTime, glm::vec3 dir, glm::vec3& modelPosition, GLfloat *modelYaw) {
  const float moveSpeed = 0.25f;     // velocidad movimiento
  const float rotateSpeed = 0.25f;   // suavidad de rotación
  const float epsDeg = 0.5f;

  float currentYaw = *modelYaw;

  // 1. Normalizar dirección de cámara al plano XZ
  glm::vec3 dirXZ = glm::normalize(glm::vec3(dir.x, 0.0f, dir.z));
  if (glm::length(dirXZ) < 1e-6f) dirXZ = glm::vec3(0.0f, 0.0f, 1.0f);

  // 2. Vector lateral (derecha)
  glm::vec3 rightXZ = glm::normalize(glm::cross(dirXZ, glm::vec3(0.0f, 1.0f, 0.0f)));

  // 3. Movimiento según teclas
  glm::vec3 moveDir(0.0f);
  bool forwardPressed  = keys[GLFW_KEY_W];
  bool backwardPressed = keys[GLFW_KEY_S];
  bool rightPressed    = keys[GLFW_KEY_D];
  bool leftPressed     = keys[GLFW_KEY_A];

  if (forwardPressed)  moveDir += dirXZ;
  if (backwardPressed) moveDir -= dirXZ;
  if (rightPressed)    moveDir += rightXZ;
  if (leftPressed)     moveDir -= rightXZ;

  // 4. Si hay movimiento
  if (glm::length(moveDir) > 1e-6f)
  {
    moveDir = glm::normalize(moveDir);

    // 🔹 SOLO girar al avanzar (W)
    if (forwardPressed || rightPressed || leftPressed || backwardPressed) {
        float targetYaw = glm::degrees(atan2(moveDir.z, -moveDir.x));

        float diff = targetYaw - currentYaw;
        while (diff > 180.0f) diff -= 360.0f;
        while (diff < -180.0f) diff += 360.0f;

        if (fabs(diff) > epsDeg) {
            float t = glm::clamp(rotateSpeed * deltaTime, 0.0f, 1.0f);
            currentYaw = glm::mix(currentYaw, currentYaw + diff, t);
        }
    }

    // Mover personaje en la dirección deseada
    modelPosition += moveDir * (moveSpeed * deltaTime);
    caminando = true;
  }
  else
  {
    caminando = false;
  }
  *modelYaw = currentYaw;
}





void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}

