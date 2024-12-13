#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <Io.hpp>

void initialize_scene( Viewer& viewer )
{
    // Shaders
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);

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
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(0,4,-8), glm::vec3(0,0.2,-0.2), glm::vec3(0), white, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(phongShader, spot_light);
        viewer.addRenderable(spot_light_renderable);
    }

	//Rusty train
	const std::string traing_path = "../../models3D/oldTrain.obj";
    const std::string train_texture_path = "../../models3D/rusty.jpg";

    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<MaterialPtr> materials;

    read_obj_with_materials(traing_path, "../../models3D/", all_positions, all_normals, all_texcoords, materials);

    TexturedLightedMeshRenderablePtr train = std::make_shared<TexturedLightedMeshRenderable>(texShader, traing_path, materials[0], train_texture_path);
	
    viewer.addRenderable(train);
	train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI * 0.15, glm::vec3(0, 1, 0)) * getTranslationMatrix(glm::vec3(0, 0, 50)), 0.0); 
	train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI * 0.15, glm::vec3(0, 1, 0)) * getTranslationMatrix(glm::vec3(0, 0, -40)), 4.0);

    // Rail
    const std::string rail_path = "../../models3D/rail/rail.obj";
    std::string rail_texture_path = "../../models3D/rail/woodAndMetal.jpg";

    std::vector<std::vector<glm::vec3>> all_positions1;
    std::vector<std::vector<glm::vec3>> all_normals1;
    std::vector<std::vector<glm::vec2>> all_texcoords1;
    std::vector<MaterialPtr> materials1;

    read_obj_with_materials(rail_path, "../../models3D/rail/", all_positions1, all_normals1, all_texcoords1, materials1);
    
    for (int i = -2; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
        rail->setGlobalTransform(getScaleMatrix(1.5,1.5,1.5)*getRotationMatrix(-M_PI * 0.15, glm::vec3(0, 1, 0))*getTranslationMatrix(0,-1.5,3.6*i));
        viewer.addRenderable(rail);
    }
}

int main() 
{
    glm::vec4 background_color(0.0,0.0,0.0,1);
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