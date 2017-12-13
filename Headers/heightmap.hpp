#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include <shader.hpp>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#include <stb_image.h>

struct Vertex {
	// position
	glm::vec3 Position;
	// texCoords
	glm::vec2 TexCoords;
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Heightmap
{
public:
	//heightmap attributes
	int width, height;

	// VAO for heightmap
	unsigned int VAO;

	// pointer to data - data is an array so can be accessed by data[x]. 
	//       - this is an uint8 array (so values range from 0-255)
	unsigned char *data;

	// heightmap vertices
	std::vector<Vertex> vertices;
	// indices for EBO
	std::vector<unsigned int> indices;


	// constructor
	Heightmap(const char* heightmapPath)
	{
		// load heightmap data
		load_heightmap(heightmapPath);

		// create heightmap verts from the image data - (you have to write this)
		create_heightmap();

		// free image data
		stbi_image_free(data);

		// create_indices - create the indices array (you have to write this)
		create_indices();

		// setup the VBO, VAO, and EBO and send the information to OpenGL (you need to write this)
		setup_heightmap();
	}

	// render the heightmap mesh (you need to write this)
	void Draw(Shader shader, unsigned int textureID)
	{
		// You must:
		// -  active proper texture unit before binding
		// -  bind the texture
		// -  draw mesh (using GL_TRIANGLES is the most reliable way)


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// draw mesh
		shader.use();


		glBindVertexArray(VAO);

		glm::mat4 model;

		shader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


		// always good practice to set everything back to defaults once configured.
		//glActiveTexture(GL_TEXTURE0);
	}

	float findHeight(int x, int y) {
		int temp = x + y*width;
		return (temp>=0 || temp < vertices.size()) ? vertices[temp].Position.y+2.0f : 0;
	}

private:

	unsigned int VBO, EBO;

	// Load the image data
	void load_heightmap(const char* heightmapPath)
	{
		int nrChannels;
		data = stbi_load(heightmapPath, &width, &height, &nrChannels, 0);
		if (!data)
		{
			std::cout << "Failed to load heightmap" << std::endl;
		}
	}


	// Make Vertex:  take x and y position return a new vertex for that position which includes 
	//  the position and the texture coordinates

	Vertex make_vertex(int x, int y)
	{
		Vertex v;
		//XYZ coords
		v.Position.x = ((float)(x) / (float)width - 0.5f)*200.0f;
		v.Position.y = (float)data[x + y*width] / 5.0f-50.0f;
		v.Position.z = ((float)(y) / (float)height - 0.5f)*200.0f;

		//Texture Coords
		v.TexCoords.x = (float)(x) / (float)width;
		v.TexCoords.y = (float)(y) / (float)height;
		return v;
	}


	// convert heightmap to floats, set position and texture vertices using the subfunction make_vertex
	void create_heightmap()
	{
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				vertices.push_back(make_vertex(x, y));
			}
		}
	}


	// create the indicies array for the EBO (so what indicies correspond with triangles to for rendering)
	void create_indices()
	{
		for (unsigned int x = 0; x < width-1; x++) {
			for (unsigned int y = 0; y < height-1; y++) {
				unsigned int temp = x + y*width;
				//first triangle
				indices.push_back(temp);
				indices.push_back(temp+1);
				indices.push_back(temp+1+width);
				//second triangle
				indices.push_back(temp);
				indices.push_back(temp + width);
				indices.push_back(temp + 1 + width);
			}
		}
		
	}


	// create buffers/arrays for the VAO, VBO, and EBO 
	// Notes
	//  -  sizeof(Vertex) returns the size of the vertex
	//  -  to get the pointer the underlying array, you can use "&vertices[0]"
	void setup_heightmap()
	{

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// vertex positions

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		// vertex texture coords

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(1);

	}

};
#endif