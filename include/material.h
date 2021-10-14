#pragma once

#include "shader.h"

namespace pixeldancer
{
	class Material
	{
	public:
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		float shininess;
	};

	class Gold : Material
	{
	public:
		Gold()
		{
			ambient = glm::vec4(0.2473f, 0.1995f, 0.0745f, 1.0f);
			diffuse = glm::vec4(0.7516f, 0.6560f, 0.2265f, 1.0f);
			specular = glm::vec4(0.6283f, 0.5558f, 0.3661f, 1.0f);
			shininess = 51.2f;
		}

		void setInShader(Shader& shader, const std::string class_name = "material")
		{
			shader.load();

			shader.setVec4(class_name + ".ambient", ambient);
			shader.setVec4(class_name + ".diffuse", diffuse);
			shader.setVec4(class_name + ".specular", specular);
			shader.setFloat(class_name + ".shininess", shininess);
		}
	};

	class Jade : Material
	{
	public:
		Jade()
		{
			ambient = glm::vec4(0.1350f, 0.2225f, 0.1575f, 0.95f);
			diffuse = glm::vec4(0.54f, 0.89f, 0.63f, 0.95f);
			specular = glm::vec4(0.3162f, 0.3162f, 0.3162f, 0.95f);
			shininess = 12.8f;
		}

		void setInShader(Shader& shader, const std::string class_name = "material")
		{
			shader.load();

			shader.setVec4(class_name + ".ambient", ambient);
			shader.setVec4(class_name + ".diffuse", diffuse);
			shader.setVec4(class_name + ".specular", specular);
			shader.setFloat(class_name + ".shininess", shininess);
		}
	};

	class Pearl : Material
	{
	public:
		Pearl()
		{
			ambient = glm::vec4(0.25f, 0.2073f, 0.2073f, 0.922f);
			diffuse = glm::vec4(1.0f, 0.8290f, 0.8290f, 0.922f);
			specular = glm::vec4(0.2966f, 0.2966f, 0.2966f, 0.922f);
			shininess = 11.264f;
		}

		void setInShader(Shader& shader, const std::string class_name = "material")
		{
			shader.load();

			shader.setVec4(class_name + ".ambient", ambient);
			shader.setVec4(class_name + ".diffuse", diffuse);
			shader.setVec4(class_name + ".specular", specular);
			shader.setFloat(class_name + ".shininess", shininess);
		}
	};

	class Silver : Material
	{
	public:
		Silver()
		{
			ambient = glm::vec4(0.1923f, 0.1923f, 0.1923f, 1.0f);
			diffuse = glm::vec4(0.5075f, 0.5075f, 0.5075f, 1.0f);
			specular = glm::vec4(0.5083f, 0.5083f, 0.5083f, 1.0f);
			shininess = 51.2f;
		}

		void setInShader(Shader& shader, const std::string class_name = "material")
		{
			shader.load();

			shader.setVec4(class_name + ".ambient", ambient);
			shader.setVec4(class_name + ".diffuse", diffuse);
			shader.setVec4(class_name + ".specular", specular);
			shader.setFloat(class_name + ".shininess", shininess);
		}
	};
}