    
#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <FrameRenderable.hpp>
#include <Utils.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <Io.hpp>

void initialize_scene( Viewer& viewer )
{
    // In this scene, we will see the trolley problem: 
    // a rail with one penguin on the left, three on the current rail,
    // and one with a lever on his hand

    //TODO scene 2 :
    // trolley problem

    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");

    //Textured shader
    //    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram( texShader );

    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram(phongShader);



    //add the black walls to do the eyes opening transition
    const std::string wall_path = "../../models3D/eye.obj";
    MeshRenderablePtr wall1 = std::make_shared<MeshRenderable>(flatShader, wall_path);
    wall1->setGlobalTransform(getTranslationMatrix(0, 3, -4.8));
    viewer.addRenderable(wall1);

    MeshRenderablePtr wall2 = std::make_shared<MeshRenderable>(flatShader, wall_path);
    wall2->setGlobalTransform(getTranslationMatrix(0, -3, -4.8)*getRotationMatrix(M_PI*1,0,0,1));
    viewer.addRenderable(wall2);



    // Keyframe animation for the wall movement
    wall1->addGlobalTransformKeyframe(getTranslationMatrix(0, 3, -4.8), 0.0);
    wall1->addGlobalTransformKeyframe(getTranslationMatrix(0, 3.1, -4.8), 0.5);
    wall1->addGlobalTransformKeyframe(getTranslationMatrix(0, 3, -4.8), 1.0);
    wall1->addGlobalTransformKeyframe(getTranslationMatrix(0, 3.2, -4.8), 2.0);
    wall1->addGlobalTransformKeyframe(getTranslationMatrix(0, 3, -4.8), 3.0);
    wall1->addGlobalTransformKeyframe(getTranslationMatrix(0, 5, -4.8), 5.0);

    // Keyframe animation for the wall movement
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 0.0);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3.1, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 0.5);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 1.0);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3.2, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 2.0);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 3.0);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -5, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 5.0);
}

int main() 
{
    //glm::vec4 background_color(0.0,0.0,0.0,1);
	glm::vec4 background_color(0.8,0.8,0.8,1);
	Viewer viewer(1280,720, background_color);
	initialize_scene(viewer);
	viewer.startAnimation();

	while( viewer.isRunning() )
	{
		viewer.handleEvent();
		viewer.animate();
		viewer.draw();
		viewer.display();
	}	

	return EXIT_SUCCESS;
}