#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <FrameRenderable.hpp>
#include <Utils.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <texturing/CubeMapRenderable.hpp>
#include <Io.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <texturing/TexturedCubeRenderable.hpp>

void initialize_scene( Viewer& viewer )
{
    // In this scene, we will see the train on rails going right in front of us, in our face

    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram( texShader );

	//Define a shader that encode an illumination model
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>( "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl", 
                                                                    "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram( phongShader );
    
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


    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    viewer.addShaderProgram(cubeMapShader);

    std::string cubemap_dir = "../../models3D/ciel";
    auto cubemap = std::make_shared<CubeMapRenderable>(cubeMapShader, cubemap_dir);
    viewer.addRenderable(cubemap);

    //add the black walls to do the eyes opening transition
    const std::string wall_path = "../../models3D/eye.obj";
    MeshRenderablePtr wall1 = std::make_shared<MeshRenderable>(phongShader, wall_path);
    wall1->setGlobalTransform(getTranslationMatrix(0, 3, -4.8));
    viewer.addRenderable(wall1);

    MeshRenderablePtr wall2 = std::make_shared<MeshRenderable>(phongShader, wall_path);
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


    // Rails
    const std::string rail_path = "../../models3D/rail/rail.obj";
    std::string rail_texture_path = "../../models3D/rail/woodAndMetal.jpg";


    std::vector<std::vector<glm::vec3>> all_positions1;
    std::vector<std::vector<glm::vec3>> all_normals1;
    std::vector<std::vector<glm::vec2>> all_texcoords1;
    std::vector<MaterialPtr> materials1;


    read_obj_with_materials(rail_path, "../../models3D/rail/", all_positions1, all_normals1, all_texcoords1, materials1);
    
    //Rails droits
    TexturedLightedMeshRenderablePtr railo = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
    railo->setLocalTransform(getScaleMatrix(1,1,1)*getTranslationMatrix(0,0,-3.6*2));
    
    for (int i = -1; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
        rail->setLocalTransform(getScaleMatrix(1,1,1)*getTranslationMatrix(0,0,3.6*i));
        HierarchicalRenderable::addChild(railo, rail);
    }
    railo->setGlobalTransform(getScaleMatrix(2,2,2)*getRotationMatrix(M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-0.5,-2,-20));
    viewer.addRenderable(railo);

    //embranchement
    TexturedLightedMeshRenderablePtr railo2 = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
    railo2->setGlobalTransform(getScaleMatrix(2,2,2)*getRotationMatrix(-M_PI*0.35, glm::vec3(0,1,0))*getTranslationMatrix(1.5,-2,0));
    viewer.addRenderable(railo2);

    for (int i = 1; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
        rail->setLocalTransform(getScaleMatrix(1,1,1)*getTranslationMatrix(0,0,3.6*i));
        HierarchicalRenderable::addChild(railo2, rail);
    }

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
