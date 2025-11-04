#pragma once

#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw3.h>
#include <vector>


struct CameraPoint {
    glm::vec3 centro;
    float radioX;
    float radioZ;
    float velocidad;
    float alturaInicial;
    float alturaFinal;
};


class Camera {
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();


  void followTarget(glm::vec3 modelPosition);
  // float getYaw() const { return yaw + angleAroundTarget; }
  bool isTopDown = false;
  bool isStatic = false;

  //void setPosition(const glm::vec3& pos);
  //void setFrontFromTarget(const glm::vec3& target); // actualiza front/right/up mirando target
  //void lookAt(const glm::vec3& target); // convenience: set front to target-position
  void orbitAround(glm::vec3 centro, float radio, float radioZ, float velocidad, float deltaTime,float alturaMin, float alturaMax, bool eliptica);

  //void scrollControl(GLfloat yOffset);


	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;
  float tiempoAltura; // para variar la altura suavemente con el tiempo

  float angulo;   // estado actual del ángulo de órbita
  float altura;   // altura sobre el punto central
  glm::vec3 targetPosition;  // posición del modelo
  GLfloat distanceFromTarget; // distancia de la cámara al modelo
  GLfloat angleAroundTarget;  // ángulo alrededor del modelo

	void update();
};

