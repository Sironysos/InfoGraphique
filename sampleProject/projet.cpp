#include <Viewer.hpp>
#include <ShaderProgram.hpp>
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


    /* const std::string pillar_path = "../../models3D/train2.obj";
    MeshRenderablePtr pillar = std::make_shared<MeshRenderable>(flatShader, pillar_path);
    viewer.addRenderable(pillar);*/


	//Frame
    /* FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame); */

    //Animated train
    const std::string train_path = "../../models3D/traingMalaaade.obj";
    MeshRenderablePtr train = std::make_shared<MeshRenderable>(flatShader, train_path);
    train->setGlobalTransform(glm::mat4(1.0));
    //turn the train
    train->setLocalTransform(getRotationMatrix(M_PI*0.5, glm::vec3(0,1,0))*getRotationMatrix(M_PI*1.5, glm::vec3(1,0,0))*getTranslationMatrix(0,0,-1));

    viewer.addRenderable(train);

    // Keyframes on parent transformation
    train->addGlobalTransformKeyframe(getRotationMatrix(0.0, glm::vec3(0,1,0)),0.0);
    train->addGlobalTransformKeyframe(getRotationMatrix(M_PI*0.25, glm::vec3(0,1,0)),2.0);
    /* train->addGlobalTransformKeyframe(getRotationMatrix(M_PI*0.5, glm::vec3(0,1,0)), 2.0);
    train->addGlobalTransformKeyframe(getRotationMatrix(M_PI*0.25, glm::vec3(0,1,0)), 3.0); */
    train->addGlobalTransformKeyframe(getRotationMatrix(M_PI*0.0, glm::vec3(0,1,0)), 4.0);
    train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI*0.25, glm::vec3(0,1,0)), 6.0);
    /* train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI*0.5, glm::vec3(0,1,0)), 6.0);
    train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI*0.25, glm::vec3(0,1,0)), 7.0); */
    train->addGlobalTransformKeyframe(getRotationMatrix(M_PI*0.0, glm::vec3(0,1,0)), 8.0);

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