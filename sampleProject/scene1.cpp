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
    // In this scene, we will zoom into a hole of the box, 
    // and then ther will be a train that will pass by from left to right, on rails
    // and a black rectangle will appear next to the train

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

    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<MaterialPtr> materials;

    read_obj_with_materials(box_path, "../../models3D/", all_positions, all_normals, all_texcoords, materials);

    TexturedLightedMeshRenderablePtr box = std::make_shared<TexturedLightedMeshRenderable>(
        texShader, box_path, materials[0], box_texture_path);
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
    MeshRenderablePtr wall = std::make_shared<MeshRenderable>(flatShader, wall_path);
    wall->setModelMatrix(getScaleMatrix(0.001,0.001,0.001)*getRotationMatrix(M_PI*0.25, 0,1,0));
    //TODO : ça scale rien du tout là ^
    viewer.addRenderable(wall);

    // Keyframe animation for the wall movement
    wall->addGlobalTransformKeyframe(getTranslationMatrix(240, 0, -4.5), 0.0);
    wall->addGlobalTransformKeyframe(getTranslationMatrix(-240, 0, -4.5), 30.0);
}

int main() 
{
    glm::vec4 background_color(0.0,0.0,0.0,1);
	//glm::vec4 background_color(0.8,0.8,0.8,1);
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