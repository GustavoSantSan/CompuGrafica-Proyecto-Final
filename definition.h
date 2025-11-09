/*
* definition.h
*Basado en el código creado por
* Created on: 06/09/2016
* Author: reynaldo Martell
*/

#ifndef HEADERS_DEFINITION_H_
#define HEADERS_DEFINITION_H_

#include <glm.hpp>
#include <glew.h>
#include <gtc/random.hpp>
#include <iostream>

// Estructura original (Posición + Color)
typedef struct _VertexColor {

	_VertexColor() {
	}

	_VertexColor(glm::vec3 position, glm::vec3 color) {
		this->position = position;
		this->color = color;
	}

	glm::vec3 position;
	glm::vec3 color;
} VertexColor;


// Estructura para iluminación y color (Posición + Color + Normal)
typedef struct _VertexLightColor {

	_VertexLightColor() {
	}

	_VertexLightColor(glm::vec3 position, glm::vec3 color, glm::vec3 normal) {
		this->position = position;
		this->color = color;
		this->normal = normal;
	}

	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
} VertexLightColor;


// ESTRUCTURA NECESARIA PARA EL TOROIDE Y EL SHADER DE LUZ/TEXTURA:
// Posición (Location 0), Coordenadas UV (Location 1) y Normal (Location 2)
typedef struct _VertexLightTexture {

	_VertexLightTexture() {
	}

	_VertexLightTexture(glm::vec3 position, glm::vec2 texture,
		glm::vec3 normal) {
		this->position = position;
		this->texture = texture;
		this->normal = normal;
	}

	glm::vec3 position;
	glm::vec2 texture; // Coordenadas U, V
	glm::vec3 normal;  // Vector normal para la iluminación
} VertexLightTexture;


// Enumeración para definir qué tipo de datos tiene un Mesh (por si se utiliza en algún constructor)
enum MODEL_MODE {
	VERTEX,
	VERTEX_COLOR,
	VERTEX_TEXTURE,       // Sólo Posición y UV
	VERTEX_LIGHT_COLOR,
	VERTEX_LIGHT_TEXTURE  // ¡El modo que usaremos!
};

#endif /* HEADERS_DEFINITION_H_ */