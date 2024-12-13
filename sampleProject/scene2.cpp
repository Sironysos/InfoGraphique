    
#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <FrameRenderable.hpp>
#include <Utils.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <Io.hpp>
#include <texturing/CubeMapRenderable.hpp>
#include <texturing/TexturedCubeRenderable.hpp>

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


    
    glm::vec3 dir = glm::normalize(glm::vec3(-1,-1,-1));
    glm::vec3 ambient = glm::vec3(0,0,0);
    glm::vec3 diffuse = glm::vec3(1,1,1);
    glm::vec3 specular = glm::vec3(1,1,1);
    DirectionalLightPtr light1 = std::make_shared<DirectionalLight>(dir, ambient, diffuse, specular);
    dir = glm::normalize(glm::vec3(1,-1,1));
    ambient = glm::vec3(0,0,0);
    diffuse = glm::vec3(1,0.9,0.9);
    specular = glm::vec3(1,0.9,0.9);
    DirectionalLightPtr light2 = std::make_shared<DirectionalLight>(dir, ambient, diffuse, specular);
    dir = glm::normalize(glm::vec3(0,1,0));
    ambient = glm::vec3(0,0,0);
    diffuse = glm::vec3(0.5,0.3,0.3);
    specular = glm::vec3(0.5,0.3,0.3);
    DirectionalLightPtr light3 = std::make_shared<DirectionalLight>(dir, ambient, diffuse, specular);

    viewer.addDirectionalLight(light1);
    viewer.addDirectionalLight(light2);
    viewer.addDirectionalLight(light3);

    glm::vec3 white(1,1,1);

    { // SpotLight
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(0,4,-8), glm::vec3(0,4,-0.2), glm::vec3(0), white, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(phongShader, spot_light);
        viewer.addRenderable(spot_light_renderable);
    }

    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    viewer.addShaderProgram(cubeMapShader);

    std::string cubemap_dir = "../../models3D/ciel";
    auto cubemap = std::make_shared<CubeMapRenderable>(cubeMapShader, cubemap_dir);

    viewer.addRenderable(cubemap);
    
    


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
    wall1->addGlobalTransformKeyframe(getTranslationMatrix(0, 5, -4.8), 30);

    // Keyframe animation for the wall movement
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 0.0);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3.1, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 0.5);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 1.0);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3.2, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 2.0);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -3, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 3.0);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -5, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 5.0);
    wall2->addGlobalTransformKeyframe(getTranslationMatrix(0, -5, -4.8)*getRotationMatrix(M_PI*1,0,0,1), 30);

    auto tcube = std::make_shared<TexturedCubeRenderable>(texShader, "../../models3D/sable.jpg");
    viewer.addRenderable(tcube);
    tcube->setGlobalTransform(getTranslationMatrix(0,-5,0)*getScaleMatrix(1000,1,1000));
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