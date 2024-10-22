#include "./../include/CubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

CubeRenderable::CubeRenderable(ShaderProgramPtr shaderProgram)
	: Renderable(shaderProgram), m_vBuffer(0), m_cBuffer(0)
{
	// Build the geometry : On cree les 12 triangles formant les 6 faces du cube
	m_positions = {
		//face 1
		glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1), glm::vec3(1, 1, 1),
		glm::vec3(-1, -1, 1), glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1),
		//face 2
		glm::vec3(-1, -1, -1), glm::vec3(1, -1, -1), glm::vec3(1, 1, -1),
		glm::vec3(-1, -1, -1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1),
		//face 3
		glm::vec3(-1, -1, 1), glm::vec3(-1, -1, -1), glm::vec3(1, -1, -1),
		glm::vec3(-1, -1, 1), glm::vec3(1, -1, -1), glm::vec3(1, -1, 1),
		//face 4
		glm::vec3(1, -1, -1), glm::vec3(1, -1, 1), glm::vec3(1, 1, -1),
		glm::vec3(1, -1, 1), glm::vec3(1, 1, -1), glm::vec3(1, 1, 1),
		//face 5
		glm::vec3(-1, -1, -1), glm::vec3(-1, -1, 1), glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, 1), glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1),
		//face 6
		glm::vec3(-1, 1, 1), glm::vec3(1, 1, -1), glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1),
	};

	m_colors = {
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(1, 0, 0, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
		glm::vec4(0, 0, 1, 1),
	};

	// Set the model matrix to identity
	m_model = glm::mat4(1.0);

	// Create buffers
	glGenBuffers(1, &m_vBuffer); // vertices
	glGenBuffers(1, &m_cBuffer); // colors

	// Activate buffer and send data to the graphics card
	glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW);
}

void CubeRenderable::do_draw()
{
	// Get the identifier ( location ) of the uniform modelMat in the shader program
	int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
	// Send the data corresponding to this identifier on the GPU
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_model));

	// Get the identifier of the attribute vPosition in the shader program
	int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
	// Activate the attribute array at this location
	glEnableVertexAttribArray(positionLocation);
	// Bind the position buffer on the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer);
	// Specify the location and the format of the vertex position attribute
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
	glEnableVertexAttribArray(colorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);


	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, m_positions.size());

	// Release the vertex attribute array
	glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(colorLocation);
}

CubeRenderable::~CubeRenderable()
{
	glcheck(glDeleteBuffers(1, &m_vBuffer));
	glcheck(glDeleteBuffers(1, &m_cBuffer));
}
