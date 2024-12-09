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
#include "./../sfmlGraphicsPipeline/include/texturing/TexturedMeshRenderable.hpp"

void initialize_scene( Viewer& viewer )
{
    // Create a shader program
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);

    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    viewer.addShaderProgram(cubeMapShader);

    // Define a shader that encodes an illumination model (Phong shading)
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram(phongShader);

    // Define a transformation
    glm::mat4 globalTransformation, localTransformation;

    // Define a material
    auto mat = std::make_shared<Material>(glm::vec3(0), glm::vec3(1), glm::vec3(0), 100.0f);

    // Define colors for lights
    glm::vec3 yellow(1, 1, 0);

    { // SpotLight
        // Set the initial Spotlight position inside the box
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(0, 2, 0), glm::vec3(0, -1, -0), glm::vec3(0), yellow, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(flatShader, spot_light);
        viewer.addRenderable(spot_light_renderable);

        // Animate the spotlight by adding keyframes for its position
        spot_light->addGlobalTransformKeyframe(getTranslationMatrix(0, 2, 0), 0.0);
        spot_light->addGlobalTransformKeyframe(getTranslationMatrix(0, 2, -4.25), 8.0);
    }

    const std::string box_path = "../../models3D/caisse.obj";
    LightedMeshRenderablePtr box = std::make_shared<LightedMeshRenderable>(phongShader, box_path, Material::GreenRubber());
    box->setGlobalTransform(glm::mat4(1.0f));

    viewer.addRenderable(box);

    // Keyframe animation for the box movement
    box->addGlobalTransformKeyframe(getTranslationMatrix(0, 0, 0), 0.0);
    box->addGlobalTransformKeyframe(getTranslationMatrix(0, 0, -4.25), 8.0);
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