#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <FrameRenderable.hpp>
#include <HierarchicalRenderable.hpp>
#include <CylinderMeshRenderable.hpp>
#include <MeshRenderable.hpp>
#include <SphereMeshRenderable.hpp>

void initialize_scene( Viewer& viewer )
{
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");

	viewer.addShaderProgram( flatShader );
	
	//Frame
	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);

		ShaderProgramPtr parentProg = std::make_shared<ShaderProgram>("../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
		"../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
	ShaderProgramPtr childProg = std::make_shared<ShaderProgram>("../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
		"../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
	viewer.addShaderProgram(parentProg);
	viewer.addShaderProgram(childProg);

	std::shared_ptr<CylinderMeshRenderable> root = std::make_shared<CylinderMeshRenderable>(parentProg, false, 20, true);
	std::shared_ptr<CylinderMeshRenderable> child1 = std::make_shared<CylinderMeshRenderable>(childProg, false, 20, true);

	glm::mat4 rootGlobalTransform;
	root->setGlobalTransform(rootGlobalTransform);
	glm::mat4 child1GlobalTransform;
	child1->setGlobalTransform(child1GlobalTransform);
	glm::mat4 child1LocalTransform=getTranslationMatrix(0, 0, 10);
	child1LocalTransform = glm::rotate(child1LocalTransform, 0.5f, glm::vec3(1, 0, 0));
	child1->setLocalTransform(child1LocalTransform);
	HierarchicalRenderable::addChild(root, child1);
	viewer.addRenderable(root);

	

	

}

int main() 
{
	Viewer viewer(1280,720);
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
