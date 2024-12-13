#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <texturing/CubeMapRenderable.hpp>
#include <Io.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <texturing/TexturedCubeRenderable.hpp>
#include <lighting/SpotLightRenderable.hpp>

void initialize_scene( Viewer& viewer )
{
    // In this scene, we will see the train on rails going right in front of us, in our face

    //Shaders
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);
    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    viewer.addShaderProgram(cubeMapShader);
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>( "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl", 
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

    std::string cubemap_dir = "../../models3D/ciel";
    auto cubemap = std::make_shared<CubeMapRenderable>(cubeMapShader, cubemap_dir);
    viewer.addRenderable(cubemap);

	//Rusty train
	const std::string traing_path = "../../models3D/un_oldTrain.obj";
    const std::string train_texture_path = "../../models3D/metal.jpg";

    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<MaterialPtr> materials;

    read_obj_with_materials(traing_path, "../../models3D/", all_positions, all_normals, all_texcoords, materials);

    TexturedLightedMeshRenderablePtr train = std::make_shared<TexturedLightedMeshRenderable>(texShader, traing_path, materials[0], train_texture_path);
	
    viewer.addRenderable(train);
	train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI * 0.15, glm::vec3(1, 0, 0)) * getTranslationMatrix(glm::vec3(0, 0, 90)), 0.0); 
	train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI * 0.15, glm::vec3(1, 0, 0)) * getTranslationMatrix(glm::vec3(0, 0, -0)), 4.0);

    // Rail
    const std::string rail_path = "../../models3D/rail/rail.obj";
    std::string rail_texture_path = "../../models3D/rail/woodAndMetal.jpg";

    std::vector<std::vector<glm::vec3>> all_positions1;
    std::vector<std::vector<glm::vec3>> all_normals1;
    std::vector<std::vector<glm::vec2>> all_texcoords1;
    std::vector<MaterialPtr> materials1;

    read_obj_with_materials(rail_path, "../../models3D/rail/", all_positions1, all_normals1, all_texcoords1, materials1);
    
    TexturedLightedMeshRenderablePtr railo = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
    railo->setLocalTransform(getScaleMatrix(1,1,1)*getRotationMatrix(-M_PI * 0.15, glm::vec3(1, 0, 0))*getTranslationMatrix(0,-1,-3.6*2));
    
    for (int i = -1; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
        rail->setLocalTransform(getScaleMatrix(1,1,1)*getRotationMatrix(-M_PI * 0.15, glm::vec3(1, 0, 0))*getTranslationMatrix(0,-1,3.6*i));
        HierarchicalRenderable::addChild(railo, rail);
    }
    railo->setGlobalTransform(getScaleMatrix(1.6,1.6,1.6)*getTranslationMatrix(0,0,2));
    viewer.addRenderable(railo);

    auto tcube = std::make_shared<TexturedCubeRenderable>(texShader, "../../models3D/sable.jpg");
    viewer.addRenderable(tcube);
    tcube->setGlobalTransform(getRotationMatrix(-M_PI * 0.15, glm::vec3(1, 0, 0))*getTranslationMatrix(0,-5,0)*getScaleMatrix(1000,1,1000));
}

int main() 
{
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