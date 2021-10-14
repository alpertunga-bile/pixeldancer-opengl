#pragma once

#include "helper.h"

namespace pixeldancer
{
	class ShapeFactory
	{
	public:
		ShapeFactory()
		{
			planeVAO = 0;
			cubeVAO = 0;
			quadVAO = 0;
			sphereVAO = 0;
			gridVAO = 0;
			skyboxVAO = 0;
		}

		void renderPlane()
		{
			if (planeVAO == 0)
			{
				float planeVertices[48] = {
					// positions            // normals            // texcoords
					-10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
					-10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
					 10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,

					 10.0f, 0.0f, -10.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
					-10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
					 10.0f, 0.0f,  10.0f,  0.0f, 1.0f, 0.0f,    1.0f, 1.0f
				};

				glGenVertexArrays(1, &planeVAO);
				glGenBuffers(1, &planeVBO);

				// fill buffer
				glBindVertexArray(planeVAO);
				glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices[0], GL_STATIC_DRAW);

				// link vertex attributes
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}

			// render plane
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		void renderCube(bool instance = false, int number_instance = 10)
		{
			if (cubeVAO == 0)
			{
				float cube_vertices_texture_normal[288] = {
					// back face
					-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
					 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
					 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
					 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
					-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
					-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
					// front face
					-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
					 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
					 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
					 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
					-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
					-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
					// left face
					-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
					-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // top-left
					-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
					-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
					-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // bottom-right
					-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
					// right face
					 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // top-left
					 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // bottom-right
					 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
					 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // bottom-right
					 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // top-left
					 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
					// bottom face
					-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-right
					 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-left
					 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-left
					 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-left
					-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
					-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-right
					// top face
					-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
					 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
					 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
					 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
					-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
					-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left 
				};

				glGenVertexArrays(1, &cubeVAO);
				glGenBuffers(1, &cubeVBO);

				glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_texture_normal), &cube_vertices_texture_normal[0], GL_STATIC_DRAW);

				glBindVertexArray(cubeVAO);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}

			// render cube
			glBindVertexArray(cubeVAO);
			if (!instance)
				glDrawArrays(GL_TRIANGLES, 0, 36);
			else
				glDrawArraysInstanced(GL_TRIANGLES, 0, 36, number_instance);
			glBindVertexArray(0);
		}

		void renderSkybox()
		{
			if (skyboxVAO == 0)
			{
				float skybox_vertices[180] = {
					-1.0f, 1.0f, -1.0f,		1.00f, 0.66f,
					-1.0f, -1.0f, -1.0f,	1.00f, 0.33f,
					 1.0f, -1.0f, -1.0f,	0.75f, 0.33f,
					 1.0f, -1.0f, -1.0f,	0.75f, 0.33f,
					 1.0f, 1.0f, -1.0f,		0.75f, 0.66f,
					-1.0f, 1.0f, -1.0f,		1.00f, 0.66f,
					 1.0f, -1.0f, -1.0f,	0.75f, 0.33f,
					 1.0f, -1.0f, 1.0f,		0.50f, 0.33f,
					 1.0f, 1.0f, -1.0f,		0.75f, 0.66f,
					 1.0f, -1.0f, 1.0f,		0.50f, 0.33f,
					 1.0f, 1.0f, 1.0f,		0.50f, 0.66f,
					 1.0f, 1.0f, -1.0f,		0.75f, 0.66f,
					 1.0f, -1.0f, 1.0f,		0.50f, 0.33f,
					-1.0f, -1.0f, 1.0f,		0.25f, 0.33f,
					 1.0f, 1.0f, 1.0f,		0.50f, 0.66f,
					-1.0f, -1.0f, 1.0f,		0.25f, 0.33f,
					-1.0f, 1.0f, 1.0f,		0.25f, 0.66f,
					 1.0f, 1.0f, 1.0f,		0.50f, 0.66f,
					-1.0f, -1.0f, 1.0f,		0.25f, 0.33f,
					-1.0f, -1.0f, -1.0f,	0.00f, 0.33f,
					-1.0f, 1.0f, 1.0f,		0.25f, 0.66f,
					-1.0f, -1.0f, -1.0f,	0.00f, 0.33f,
					-1.0f, 1.0f, -1.0f,		0.00f, 0.66f,
					-1.0f, 1.0f, 1.0f,		0.25f, 0.66f,
					-1.0f, -1.0f, 1.0f,		0.25f, 0.33f,
					 1.0f, -1.0f, 1.0f,		0.50f, 0.33f,
					 1.0f, -1.0f, -1.0f,	0.50f, 0.00f,
					 1.0f, -1.0f, -1.0f,	0.50f, 0.00f,
				    -1.0f, -1.0f, -1.0f,	0.25f, 0.00f,
					-1.0f, -1.0f, 1.0f,		0.25f, 0.33f,
					-1.0f, 1.0f, -1.0f,		0.25f, 1.00f,
					 1.0f, 1.0f, -1.0f,		0.50f, 1.00f,
					 1.0f, 1.0f, 1.0f,		0.50f, 0.66f,
					 1.0f, 1.0f, 1.0f,		0.50f, 0.66f,
					-1.0f, 1.0f, 1.0f,		0.25f, 0.66f,
					-1.0f, 1.0f, -1.0f,		0.25f, 1.00f
				};

				glGenVertexArrays(1, &skyboxVAO);
				glGenBuffers(1, &skyboxVBO);

				glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices[0], GL_STATIC_DRAW);

				glBindVertexArray(skyboxVAO);

				glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}

			// render cube
			glBindVertexArray(skyboxVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		void renderQuad()
		{
			if (quadVAO == 0)
			{
				float quadVertices[] = {
					// positions        // texture Coords
					-1.0f,  1.0f, 0.3f,  0.0f, 1.0f,
					-1.0f, -1.0f, 0.3f,  0.0f, 0.0f,
					 1.0f, -1.0f, 0.3f,  1.0f, 0.0f,
					 
					 1.0f, -1.0f, 0.3f,  1.0f, 0.0f,
					 1.0f,  1.0f, 0.3f,  1.0f, 1.0f,
					-1.0f,  1.0f, 0.3f,  0.0f, 1.0f
				};
				// setup plane VAO
				glGenVertexArrays(1, &quadVAO);
				glGenBuffers(1, &quadVBO);
				glBindVertexArray(quadVAO);
				glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			}
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		void renderSphere()
		{
			if (sphereVAO == 0)
			{
				glGenVertexArrays(1, &sphereVAO);

				unsigned int vbo, ebo;
				glGenBuffers(1, &vbo);
				glGenBuffers(1, &ebo);

				std::vector<glm::vec3> positions;
				std::vector<glm::vec2> uv;
				std::vector<glm::vec3> normals;
				std::vector<unsigned int> indices;

				const unsigned int X_SEGMENTS = 64;
				const unsigned int Y_SEGMENTS = 64;
				const float PI = 3.14159265359;
				for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
				{
					for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
					{
						float xSegment = (float)x / (float)X_SEGMENTS;
						float ySegment = (float)y / (float)Y_SEGMENTS;
						float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
						float yPos = std::cos(ySegment * PI);
						float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

						positions.push_back(glm::vec3(xPos, yPos, zPos));
						uv.push_back(glm::vec2(xSegment, ySegment));
						normals.push_back(glm::vec3(xPos, yPos, zPos));
					}
				}

				bool oddRow = false;
				for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
				{
					if (!oddRow) // even rows: y == 0, y == 2; and so on
					{
						for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
						{
							indices.push_back(y * (X_SEGMENTS + 1) + x);
							indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
						}
					}
					else
					{
						for (int x = X_SEGMENTS; x >= 0; --x)
						{
							indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
							indices.push_back(y * (X_SEGMENTS + 1) + x);
						}
					}
					oddRow = !oddRow;
				}
				indexCount = indices.size();

				std::vector<float> data;
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					data.push_back(positions[i].x);
					data.push_back(positions[i].y);
					data.push_back(positions[i].z);
					if (uv.size() > 0)
					{
						data.push_back(uv[i].x);
						data.push_back(uv[i].y);
					}
					if (normals.size() > 0)
					{
						data.push_back(normals[i].x);
						data.push_back(normals[i].y);
						data.push_back(normals[i].z);
					}
				}
				glBindVertexArray(sphereVAO);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
				float stride = (3 + 2 + 3) * sizeof(float);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
			}

			glBindVertexArray(sphereVAO);
			glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
		}

		void renderGrid(int size = 100, bool bigger = false, int scale = 10)
		{
			if (gridVAO == 0)
			{
				glGenVertexArrays(1, &gridVAO);

				unsigned int vbo, ebo;
				glGenBuffers(1, &vbo);
				glGenBuffers(1, &ebo);

				std::vector<glm::vec3> positions;
				std::vector<glm::vec2> uv;
				std::vector<glm::vec3> normals;
				std::vector<unsigned int> indices;

				const unsigned int X_SEGMENTS = size;
				const unsigned int Y_SEGMENTS = size;

				float xSegment, ySegment, xPos, yPos = 0.0f, zPos;
				for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
				{
					for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
					{
						xSegment = (float)x / (float)(X_SEGMENTS);
						ySegment = (float)y / (float)(Y_SEGMENTS);
						
						xPos = xSegment;
						zPos = ySegment;

						positions.push_back(glm::vec3(xPos, yPos, zPos));
						uv.push_back(glm::vec2(xSegment, ySegment));
						normals.push_back(glm::vec3(xPos, 1.0f, zPos));
					}
				}

				bool oddRow = false;
				for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
				{
					if (!oddRow) // even rows: y == 0, y == 2; and so on
					{
						for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
						{
							indices.push_back(y * (X_SEGMENTS + 1) + x);
							indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
						}
					}
					else
					{
						for (int x = X_SEGMENTS; x >= 0; --x)
						{
							indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
							indices.push_back(y * (X_SEGMENTS + 1) + x);
						}
					}
					oddRow = !oddRow;
				}
				indexCount = indices.size();

				std::vector<float> data;
				for (unsigned int i = 0; i < positions.size(); ++i)
				{
					data.push_back(positions[i].x);
					data.push_back(positions[i].y);
					data.push_back(positions[i].z);
					if (uv.size() > 0)
					{
						data.push_back(uv[i].x);
						data.push_back(uv[i].y);
					}
					if (normals.size() > 0)
					{
						data.push_back(normals[i].x);
						data.push_back(normals[i].y);
						data.push_back(normals[i].z);
					}
				}
				glBindVertexArray(gridVAO);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
				float stride = (3 + 2 + 3) * sizeof(float);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
			}

			glBindVertexArray(gridVAO);
			glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
		}

		~ShapeFactory()
		{
			if (planeVAO != 0)
			{
				glDeleteVertexArrays(1, &planeVAO);
				glDeleteBuffers(1, &planeVBO);
			}

			if (cubeVAO != 0)
			{
				glDeleteVertexArrays(1, &cubeVAO);
				glDeleteBuffers(1, &cubeVBO);
			}

			if (skyboxVAO != 0)
			{
				glDeleteVertexArrays(1, &skyboxVAO);
				glDeleteBuffers(1, &skyboxVBO);
			}

			if (quadVAO != 0)
			{
				glDeleteVertexArrays(1, &quadVAO);
				glDeleteBuffers(1, &quadVBO);
			}

			if (sphereVAO != 0)
				glDeleteVertexArrays(1, &sphereVAO);

			if (gridVAO != 0)
				glDeleteVertexArrays(1, &gridVAO);
		}

	private:
		unsigned int planeVAO, planeVBO;
		unsigned int cubeVAO, cubeVBO;
		unsigned int skyboxVAO, skyboxVBO;
		unsigned int quadVAO, quadVBO;
		unsigned int sphereVAO;
		unsigned int gridVAO;
		unsigned int indexCount;
	};
}