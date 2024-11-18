#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <FrameRenderable.hpp>
#include <HierarchicalRenderable.hpp>
#include <CylinderMeshRenderable.hpp>
#include <MeshRenderable.hpp>
#include <SphereMeshRenderable.hpp>
#include <Utils.hpp>

void initialize_scene( Viewer& viewer )
{
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");

	viewer.addShaderProgram( flatShader );
	
	//Frame
	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);

	std::shared_ptr<CylinderMeshRenderable> c1 = std::make_shared<CylinderMeshRenderable>(flatShader, false, 20, true);
	std::shared_ptr<CylinderMeshRenderable> c2 = std::make_shared<CylinderMeshRenderable>(flatShader, false, 20, true);
	std::shared_ptr<CylinderMeshRenderable> c3 = std::make_shared<CylinderMeshRenderable>(flatShader, false, 20, true);
	std::shared_ptr<CylinderMeshRenderable> c4 = std::make_shared<CylinderMeshRenderable>(flatShader, false, 20, true);
	std::shared_ptr<CylinderMeshRenderable> c5 = std::make_shared<CylinderMeshRenderable>(flatShader, false, 20, true);
	std::shared_ptr<CylinderMeshRenderable> c6 = std::make_shared<CylinderMeshRenderable>(flatShader, false, 20, true);
	std::shared_ptr<CylinderMeshRenderable> c7 = std::make_shared<CylinderMeshRenderable>(flatShader, false, 20, true);

	HierarchicalRenderable::addChild(c1,c2);
	HierarchicalRenderable::addChild(c1,c3);
	HierarchicalRenderable::addChild(c2,c4);
	HierarchicalRenderable::addChild(c2,c5);
	HierarchicalRenderable::addChild(c3,c6);
	HierarchicalRenderable::addChild(c3,c7);

	/*
	glm::mat4 scale_matrix = getScaleMatrix(0.2,0.2,1.0);

	c1->setLocalTransform(scale_matrix);
	c2->setLocalTransform(scale_matrix);
	c3->setLocalTransform(scale_matrix);
	c4->setLocalTransform(scale_matrix);
	c5->setLocalTransform(scale_matrix);
	c6->setLocalTransform(scale_matrix);
	c7->setLocalTransform(scale_matrix);
	*/
	c1->setGlobalTransform(getRotationMatrix(M_PI*0.5,0,0,1));
	c1->setGlobalTransform(getRotationMatrix(-M_PI*0.5,1,0,0));
	c2->setGlobalTransform(getTranslationMatrix(0,0,10)*getRotationMatrix(M_PI*0.15, 1,0,0));
	c3->setGlobalTransform(getTranslationMatrix(0,0,10)*getRotationMatrix(-M_PI*0.15, 1,0,0));
	c4->setGlobalTransform(getTranslationMatrix(0,0,10)*getRotationMatrix(M_PI*0.15, 1,0,0));
	c5->setGlobalTransform(getTranslationMatrix(0,0,10)*getRotationMatrix(-M_PI*0.15, 1,0,0));
	c6->setGlobalTransform(getTranslationMatrix(0,0,10)*getRotationMatrix(M_PI*0.15, 1,0,0));
	c7->setGlobalTransform(getTranslationMatrix(0,0,10)*getRotationMatrix(-M_PI*0.15, 1,0,0));
	

	viewer.addRenderable(c1);
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
