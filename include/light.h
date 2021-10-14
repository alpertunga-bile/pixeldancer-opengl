#pragma once

#include "shader.h"

namespace pixeldancer
{
	class Light
	{
	public:
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
	};

	class DirectionalLight : Light
	{
	public:
		DirectionalLight(
			glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec4 ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 
			glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
		)
		{
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			direction = dir;
		}

		void setInShader(Shader& shader, const std::string class_name = "light")
		{
			shader.load();

			shader.setVec4(class_name + ".ambient", ambient);
			shader.setVec4(class_name + ".diffuse", diffuse);
			shader.setVec4(class_name + ".specular", specular);

			shader.setVec3(class_name + ".direction", direction);
		}

	public:
		glm::vec3 direction;
	};

	class PositionalLight : Light
	{
	public:
		PositionalLight(
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec4 ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
		)
		{
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			position = pos;
		}

		void setInShader(Shader& shader, const std::string class_name = "light")
		{
			shader.load();

			shader.setVec4(class_name + ".ambient", ambient);
			shader.setVec4(class_name + ".diffuse", diffuse);
			shader.setVec4(class_name + ".specular", specular);

			shader.setVec3(class_name + ".position", position);
		}

	public:
		glm::vec3 position;
	};

	class SpotLight : Light
	{
	public:
		SpotLight(
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f),
			float cut = 20.0f, float exponent = 10.0f,
			glm::vec4 ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
			)
		{
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			position = pos;
			direction = dir;
			cutOff = cut;
			this->exponent = exponent;
		}

		void setInShader(Shader& shader, const std::string class_name = "light")
		{
			shader.load();

			shader.setVec4(class_name + ".ambient", ambient);
			shader.setVec4(class_name + ".diffuse", diffuse);
			shader.setVec4(class_name + ".specular", specular);

			shader.setVec3(class_name + ".position", position);
			shader.setVec3(class_name + ".direction", direction);
			shader.setFloat(class_name + ".cutOff", cutOff);
			shader.setFloat(class_name + ".exponent", exponent);
		}

	public:
		glm::vec3 position;
		glm::vec3 direction;
		float cutOff;
		float exponent;
	};
}