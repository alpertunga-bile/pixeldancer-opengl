#pragma once

#include "shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace pixeldancer
{
	const int MAX_BONE_INFLUENCE = 4;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;

		int m_boneIds[MAX_BONE_INFLUENCE];
		float m_weights[MAX_BONE_INFLUENCE];
	};

	struct Texture
	{
		unsigned int id;
		std::string type;
		aiString path;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();
		}

		void draw(Shader& shader)
		{
			/*unsigned int diffuseNumber = 1;
			unsigned int specularNumber = 1;
			unsigned int normalNumber = 1;
			unsigned int heightNumber = 1;

			for (unsigned int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);

				std::string number;
				std::string name = textures[i].type;

				if (name == "texture_diffuse")
					number = std::to_string(diffuseNumber++);
				else if (name == "texture_specular")
					number = std::to_string(specularNumber++);
				else if (name == "texture_normal")
					number = std::to_string(normalNumber++);
				else if (name == "texture_height")
					number = std::to_string(heightNumber++);

				glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}*/

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// glActiveTexture(GL_TEXTURE0);
		}

		void cleanup()
		{
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);

			if (!vertices.empty())
				vertices.clear();
			if (!indices.empty())
				indices.clear();
			if (!textures.empty())
				textures.clear();
		}

	private:
		void setupMesh()
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// position buffer location = 0
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(0);

			// normal buffer location = 1
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			glEnableVertexAttribArray(1);

			// texture coordinates location = 2
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			glEnableVertexAttribArray(2);

			// tangent coordinates location = 3
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
			glEnableVertexAttribArray(3);

			// bitangent coordinates location = 4
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
			glEnableVertexAttribArray(4);

			glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_boneIds));
			glEnableVertexAttribArray(5);

			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_weights));
			glEnableVertexAttribArray(6);

			glBindVertexArray(0);
		}

	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		unsigned int VAO;

	private:
		unsigned int VBO, EBO;
	};
}