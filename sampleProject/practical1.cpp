#include <Viewer.hpp>
#include <glm/glm.hpp>
#include "./../include/ShaderProgram.hpp"
#include "./../include/FrameRenderable.hpp"
#include "./../include/CubeRenderable.hpp"

#define SCR_WIDTH 1024
#define SCR_HEIGHT 768

int main(int argc, char *argv[])
{
	// Stage 1: Create the window and its OpenGL context
	glm::vec4 background_color = glm::vec4(0.8, 0.8, 0.8, 1.0);
	Viewer viewer(SCR_WIDTH, SCR_HEIGHT, background_color);

	// // Path to the vertex shader glsl code
	// std::string vShader = "./../../sfmlGraphicsPipeline/shaders/defaultVertex.glsl";
	// // Path to the fragment shader glsl code
	// std::string fShader = "./../../sfmlGraphicsPipeline/shaders/defaultFragment.glsl";
	// // Compile and link the shaders into a program
	// ShaderProgramPtr defaultShader = std::make_shared<ShaderProgram>(vShader, fShader);
	// // Add the shader program to the Viewer
	// viewer.addShaderProgram(defaultShader);

	// Path to the vertex shader glsl code
	std::string vShader = "./../../sfmlGraphicsPipeline/shaders/flatVertex.glsl";
	// Path to the fragment shader glsl code
	std::string fShader = "./../../sfmlGraphicsPipeline/shaders/flatFragment.glsl";
	// Compile and link the shaders into a program
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(vShader, fShader);
	// Add the shader program to the Viewer
	viewer.addShaderProgram(flatShader);

	CubeRenderablePtr cube = std::make_shared<CubeRenderable>(flatShader);
	viewer.addRenderable(cube);

	// Shader program instantiation
	// ...
	// When instantiating a renderable ,
	// you must specify the shader program used to draw it .
	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);

	// Stage 3: Our program loop
	while (viewer.isRunning())
	{
		viewer.handleEvent(); // user interactivity (keyboard/mouse)
		viewer.draw();		  // rasterization (write in framebuffer)
		viewer.display();	  // refresh window
	}

	return EXIT_SUCCESS;
}
