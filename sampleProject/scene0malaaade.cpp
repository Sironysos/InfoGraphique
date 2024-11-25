#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <CylinderMeshRenderable.hpp>
#include <MeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>
#include <Utils.hpp>

void initialize_scene( Viewer& viewer )
{
    // Create a shader program
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");

    // Add the shader program to the viewer
    viewer.addShaderProgram( flatShader );

	const std::string traing_path = "../../models3D/traingMalaaade.obj";
    MeshRenderablePtr traing = std::make_shared<MeshRenderable>(flatShader, traing_path);
	viewer.addRenderable(traing);
	traing->setGlobalTransform(glm::mat4(1.0)*getRotationMatrix(-M_PI*0.55, glm::vec3(1,0,0))*getTranslationMatrix(glm::vec3(0,0,-2)));

	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*0.0, glm::vec3(0,0,1)),0.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/6, glm::vec3(0,0,1)),1.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/3, glm::vec3(0,0,1)),2.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/2, glm::vec3(0,0,1)),3.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*2/3, glm::vec3(0,0,1)),4.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*5/6, glm::vec3(0,0,1)),5.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*2/3, glm::vec3(0,0,1)),6.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/2, glm::vec3(0,0,1)),7.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/3, glm::vec3(0,0,1)),8.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/6, glm::vec3(0,0,1)),9.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*0.0, glm::vec3(0,0,1)),10.0/0.3);
	
	viewer.startAnimation();



}

int main() 
{
    glm::vec4 background_color(0.0,0.0,0.0,1);
	Viewer viewer(1280,720, background_color);
	initialize_scene(viewer);

	while( viewer.isRunning() )
	{
		viewer.handleEvent();
		viewer.animate();
		viewer.draw();
		viewer.display();
	}	

	return EXIT_SUCCESS;
}