#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <Io.hpp>

void initialize_scene( Viewer& viewer )
{
    //Shaders
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);

    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>( "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram(phongShader);

    // Add light to the scene
    glm::vec3 white(1,1,1);
    { // SpotLight
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(0,4,8), glm::vec3(0,0.2,0.2), glm::vec3(0), white, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(phongShader, spot_light);
        viewer.addRenderable(spot_light_renderable);
    }

    //Camera movement
    viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(0, 1, 3), glm::vec3(0, -0.5, 0), glm::vec3( 0, 1, 0 ) ) );

    const std::string box_path = "../../models3D/caisse.obj";
    std::string box_texture_path = "../../models3D/crate.jpg";

    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<MaterialPtr> materials;

    read_obj_with_materials(box_path, "../../models3D/", all_positions, all_normals, all_texcoords, materials);

    TexturedLightedMeshRenderablePtr box = std::make_shared<TexturedLightedMeshRenderable>(texShader, box_path, materials[0], box_texture_path);
    box->setGlobalTransform(getTranslationMatrix(0, -1, -1));

    // Keyframe animation for the box movement
    box->addGlobalTransformKeyframe(getTranslationMatrix(0, -1, -1), 0.0);
    box->addGlobalTransformKeyframe(getTranslationMatrix(0, -1, -1)*getTranslationMatrix(0, 0, 1), 40.0);
    viewer.addRenderable(box);
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