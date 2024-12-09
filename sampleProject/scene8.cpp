#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <MeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include "./../sfmlGraphicsPipeline/include/texturing/TexturedMeshRenderable.hpp"
#include <Utils.hpp>
#include <Io.hpp>

#include <lighting/LightedMeshRenderable.hpp>
#include <lighting/Light.hpp>

void initialize_scene( Viewer& viewer )
{
    // In this scene, we will start in the mouth of one of the penguins,
    // then we will dezoom and see the other two penguins on the rail, sorry but relieved
    
    // Create a shader program
	ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");

    // Add the shader program to the viewer
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

	//Frame
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(phongShader);
    viewer.addRenderable(frame);

    //Rusty train

    //Final train
    const std::string train_path = "../../models3D/rustyTrain.obj";
    const std::string mtl_basepath = "../../models3D/";

    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<std::vector<unsigned int>> all_indices;
    std::vector<MaterialPtr> materials;

    read_obj_with_materials(train_path, mtl_basepath, all_positions, all_normals, all_texcoords, materials);

    int n_object = materials.size();
    std::vector<glm::vec4> colors;

    LightedMeshRenderablePtr train;

    train = std::make_shared<LightedMeshRenderable>(phongShader, all_positions[0], all_normals[0], colors, materials[0]);
    for (int i = 1 ; i < n_object ; ++i){
        LightedMeshRenderablePtr part = std::make_shared<LightedMeshRenderable>(
        phongShader, all_positions[i], all_normals[i], colors, materials[i]);
        HierarchicalRenderable::addChild(train, part);
        
    }
    
    train->setGlobalTransform(glm::mat4(1.0));
    //scale the train
    train->setLocalTransform(getScaleMatrix(1,1,1));
    viewer.addRenderable(train);

    /* //Animated box
    const std::string box_path = "../../models3D/caisse.obj";
    TexturedMeshRenderablePtr box = std::make_shared<TexturedMeshRenderable>(phongShader, box_path, "../../textures/cardboard.jpg");
    box->setGlobalTransform(glm::mat4(1.0));
    
    viewer.addRenderable(box);

    // Keyframes on parent transformation
    box->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0),0.0);
    box->addGlobalTransformKeyframe(getTranslationMatrix(0,0,-4.25),8.0); */

    viewer.startAnimation();

}

int main() 
{
    glm::vec4 background_color(0.8,0.8,0.8,1);
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