#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <CylinderMeshRenderable.hpp>
#include <MeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>
#include <Utils.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <texturing/CubeMapRenderable.hpp>
#include <lighting/LightedCubeRenderable.hpp>
#include <Io.hpp>

void initialize_scene( Viewer& viewer )
{
    // Create a shader program
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
	viewer.addShaderProgram( flatShader );

	ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
	viewer.addShaderProgram(cubeMapShader);

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
	const std::string traing_path = "../../models3D/rustyTrain.obj";
    const std::string mtl_basepath = "../../models3D/";


    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<std::vector<unsigned int>> all_indices;
    std::vector<MaterialPtr> materials;

    read_obj_with_materials(traing_path, mtl_basepath, all_positions, all_normals, all_texcoords, materials);

    int n_object = materials.size();
    std::vector<glm::vec4> colors;

    LightedMeshRenderablePtr train;

    train = std::make_shared<LightedMeshRenderable>(phongShader, all_positions[0], all_normals[0], colors, materials[0]);
    for (int i = 1 ; i < n_object ; ++i){
        LightedMeshRenderablePtr part = std::make_shared<LightedMeshRenderable>(
        phongShader, all_positions[i], all_normals[i], colors, materials[i]);
        HierarchicalRenderable::addChild(train, part);
        
    }
	
    viewer.addRenderable(train);
	train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI * 0.15, glm::vec3(0, 1, 0)) * getTranslationMatrix(glm::vec3(0, 0, 50)), 0.0); 
	train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI * 0.15, glm::vec3(0, 1, 0)) * getTranslationMatrix(glm::vec3(0, 0, -40)), 4.0);

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