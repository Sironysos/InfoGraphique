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
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram( texShader );

	//Define a shader that encode an illumination model
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>( "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl", 
                                                                    "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram( phongShader );

	auto mat = std::make_shared<Material>(glm::vec3(0), glm::vec3(1), glm::vec3(0), 100.0f);

    

	//Define a transformation
    glm::mat4 globalTransformation, localTransformation;

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
	// Keyframe animation for the wall movement
    train->addGlobalTransformKeyframe(getTranslationMatrix(230, 0, -4.5) * getRotationMatrix(M_PI * 0.5, glm::vec3(0, 1, 0))*getScaleMatrix(0.25,0.25,0.25), 0.0);
    train->addGlobalTransformKeyframe(getTranslationMatrix(-245, 0, -4.5) * getRotationMatrix(M_PI * 0.5, glm::vec3(0, 1, 0))*getScaleMatrix(0.25,0.25,0.25), 30.0);


    // Rail
    const std::string rail_path = "../../models3D/rail/rail.obj";
    std::string rail_texture_path = "../../models3D/rail/woodAndMetal.jpg";


    std::vector<std::vector<glm::vec3>> all_positions1;
    std::vector<std::vector<glm::vec3>> all_normals1;
    std::vector<std::vector<glm::vec2>> all_texcoords1;
    std::vector<MaterialPtr> materials1;


    read_obj_with_materials(rail_path, "../../models3D/rail/", all_positions1, all_normals1, all_texcoords1, materials1);
    
    
    for (int i = 0; i < 5; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
        rail->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,-1,3.6*i));
        viewer.addRenderable(rail);
        rail->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5,glm::vec3(0,1,0))*getTranslationMatrix(0, -1, 3.6*i-0.2), 0.0);
        rail->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5,glm::vec3(0,1,0))*getTranslationMatrix(6, -1, 3.6*i-0.2), 10.0);
        rail->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5,glm::vec3(0,1,0))*getTranslationMatrix(6, -1, 3.6*i-0.2), 30.0);

    }

    // Define colors for lights
    glm::vec3 yellow = glm::vec3(1, 1, 0);
    glm::vec3 white(1,1,1);

    { // SpotLight
        // Set the initial Spotlight position inside the box
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(0,3,-2), glm::vec3(0,-1,3), glm::vec3(0), white, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(phongShader, spot_light);
        viewer.addRenderable(spot_light_renderable);

        spot_light_renderable->setLocalTransform(getScaleMatrix(0.1, 0.1, 0.1));

        // Animate the spotlight by adding keyframes for its position*
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(0,5,-6), glm::vec3(0,0,0), Light::base_forward), 0);
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(0,5,-10.25), glm::vec3(0,0,-4.25), Light::base_forward), 10.0);
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(0,5,-10.25), glm::vec3(0,0,-4.25), Light::base_forward), 30.0);
    }

    const std::string box_path = "../../models3D/caisse.obj";
    std::string box_texture_path = "../../models3D/crate.jpg";

    std::vector<std::vector<glm::vec3>> all_positions2;
    std::vector<std::vector<glm::vec3>> all_normals2;
    std::vector<std::vector<glm::vec2>> all_texcoords2;
    std::vector<MaterialPtr> materials2;

    read_obj_with_materials(box_path, "../../models3D/", all_positions2, all_normals2, all_texcoords2, materials2);

    TexturedLightedMeshRenderablePtr box = std::make_shared<TexturedLightedMeshRenderable>(
        texShader, box_path, materials2[0], box_texture_path);
    box->setGlobalTransform(glm::mat4(1.0f));

    viewer.addRenderable(box);

    // Keyframe animation for the box movement
    box->addGlobalTransformKeyframe(getTranslationMatrix(0, 0, 0), 0.0);
    box->addGlobalTransformKeyframe(getTranslationMatrix(0, 0, -4.25), 10.0);
    box->addGlobalTransformKeyframe(getTranslationMatrix(0, 0, -4.25), 30.0);

    //add the train
    //TODO : add the train

    //add the black wall to do the transition
    const std::string wall_path = "../../models3D/mur.obj";
    MeshRenderablePtr wall = std::make_shared<MeshRenderable>(phongShader, wall_path);
    wall->setGlobalTransform(getRotationMatrix(M_PI*0, 0,1,0)*getScaleMatrix(0.1, 0.1, 0.1));
    viewer.addRenderable(wall);

    // Keyframe animation for the wall movement
    wall->addGlobalTransformKeyframe(getTranslationMatrix(240, 0, -4.5), 0.0);
    wall->addGlobalTransformKeyframe(getTranslationMatrix(-240, 0, -4.5), 30.0);
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