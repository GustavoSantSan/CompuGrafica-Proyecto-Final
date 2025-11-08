#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
  angulo = 0.0f;   // estado actual del ángulo de órbita
  altura = 5.0f;   // altura sobre el punto central
	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
  tiempoAltura = 0.0f;

  targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
  distanceFromTarget = 5.0f; // distancia inicial
  angleAroundTarget = 0.0f;
	update();
}



void Camera::followTarget(glm::vec3 modelPosition)
{
    targetPosition = modelPosition;

    float horizontalDistance = distanceFromTarget * cos(glm::radians(pitch));
    float verticalDistance = distanceFromTarget * sin(glm::radians(pitch));

    float offsetX = horizontalDistance * sin(glm::radians(yaw + angleAroundTarget));
    float offsetZ = horizontalDistance * cos(glm::radians(yaw + angleAroundTarget));

    position.x = targetPosition.x - offsetX;
    position.z = targetPosition.z - offsetZ;
    position.y = targetPosition.y + verticalDistance;

    update();
}




void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
    float velocity = 0.8f * deltaTime;

    // Modo cámara aérea
    if (pitch < -80.0f) { 
        glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));

        if (keys[GLFW_KEY_A]) position += forward * velocity;
        if (keys[GLFW_KEY_D]) position -= forward * velocity;
        if (keys[GLFW_KEY_S]) position -= right * velocity;
        if (keys[GLFW_KEY_W]) position += right * velocity;

        // No permitir moverse por debajo del plano
        if (position.y < 10.0f) position.y = 10.0f;
    }
}


void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    if (isTopDown) {
        yaw += xChange * 0.08;  
        position.y -= yChange * 1.5;  
        if (position.y < 10.0f) position.y = 10.0f;
        if (position.y > 700.0f) position.y = 700.0f;
        return;
   }

	xChange *= turnSpeed;
	yChange *= turnSpeed;

  angleAroundTarget -= xChange;
  pitch -= yChange;             

  if (pitch > 60.0f) pitch = 60.0f;
  if (pitch < -100.0f) pitch = -100.0f;
	update();
}






void Camera::orbitAround(glm::vec3 centro, float radio, float radioZ, float velocidad, float deltaTime,float alturaMin, float alturaMax, bool eliptica)
{
    // Acumular ángulo y tiempo
    angulo += velocidad * deltaTime;
    tiempoAltura += deltaTime;

    // Movimiento elíptico opcional
    //float factorElipse = eliptica ? 0.6f : 1.0f; // eje menor si elíptica=true
    float factorElipse = eliptica ? radioZ : radio;
    float x = centro.x + radio * cos(glm::radians(angulo));
    float z = centro.z + (factorElipse) * sin(glm::radians(angulo));

    // Oscilación vertical suave entre alturaMin y alturaMax
    float amplitud = (alturaMax - alturaMin) / 2.0f;
    float medio = (alturaMax + alturaMin) / 2.0f;
    float frecuencia = 0.01f; // velocidad de oscilación (puedes ajustar)
    float y = medio + amplitud * sin(frecuencia * tiempoAltura);

    // Actualizar posición y objetivo
    position = glm::vec3(x, y, z);
    targetPosition = centro;
    update();
}


glm::mat4 Camera::calculateViewMatrix()
{
  if (isStatic) {
      return glm::lookAt(position, targetPosition, worldUp);
  }
  else if (isTopDown) {
        // Mira hacia el suelo (vector hacia abajo)
        glm::vec3 lookDir = glm::vec3(0.0f, -1.0f, 0.0f);
        return glm::lookAt(position, position + lookDir, glm::vec3(0.0f, 0.0f, -1.0f));
  }
  else {
        // Cámara de seguimiento normal
        return glm::lookAt(position, targetPosition, up);
  }
}


glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	//return glm::normalize(front);
  return glm::normalize(glm::vec3(front.x, 0.0f, front.z));
}


void Camera::update() // SE modificó por completo
{
    float actualYaw = yaw + angleAroundTarget;

    front.x = sin(glm::radians(actualYaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(actualYaw)) * cos(glm::radians(pitch));

    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}


