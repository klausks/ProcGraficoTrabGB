#pragma once

//Esta classe vai ir sofrendo altera��es durante o curso
//Aqui ela est� preparada apenas para sprites texturizados

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite
{
public:
	Sprite();
	~Sprite() {}

	void initialize(); //similar ao setupGeometry

	//Para setar os atributos 
	void setTexture(int texID);
	void setShader(Shader* shader) { this->shader = shader; shader->Use(); }
	void setPosition(glm::vec3 pos) { this->pos = pos; } //dica: fazer setX e setY individualmente
	void setDimension(glm::vec3 scale) { this->scale = scale; }
	void setAngle(float angle) { this->angle = angle; } //para 3D precisa angulo por eixo ou quaternion
	

	//Para controle direto da matriz de transforma��es
	void setRotation(float angle, glm::vec3 axis, bool reset = true);
	void setTranslation(glm::vec3 displacements, bool reset = true);
	void setScale(glm::vec3 scaleFactors, bool reset = true);
	
	//Chamados no ciclo do SceneManager
	void draw();
	void update();

	glm::vec3 pos, scale;
	glm::mat4 transform; //matriz de transforma��o

	float angle;
	Shader* shader; //ponteiro para o objeto de shader, para fazer a interface

	//TO DO: m�todos para anima��o

protected:
	//Atributos gerais
	GLuint VAO; //ID do buffer de geometria
	unsigned int texID; //identificador da textura

	//Atributos para controle da anima��o
	int nAnims, nFrames;
	int iAnim, iFrame; //indices da anima��o e frame atual

};

