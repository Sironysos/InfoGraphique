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

	// Add light to the scene
	glm::vec3 red(0.9,0.3,0.4), green(0.3,0.9,0.4), blue(0.4,0.3,0.9);
	glm::vec3 white(1,1,1);
	glm::vec3 maladWhite(1,0.933,0.64);
	{ // Moving SpotLight
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(0,3,-8), glm::vec3(0,-0.2,-0.1), glm::vec3(0), maladWhite, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(flatShader, spot_light);
        viewer.addRenderable(spot_light_renderable);
    }


	const std::string traing_path = "../../models3D/traingMalaaade.obj";
    LightedMeshRenderablePtr traing = std::make_shared<LightedMeshRenderable>(phongShader, traing_path, Material::GreenRubber());
	viewer.addRenderable(traing);

    auto lighted_cube = std::make_shared<LightedCubeRenderable>(phongShader, false, mat);

	HierarchicalRenderable::addChild(traing, lighted_cube);

	traing->setGlobalTransform(glm::mat4(1.0)*getRotationMatrix(-M_PI*0.55, glm::vec3(1,0,0))*getTranslationMatrix(glm::vec3(0,0,-2)));

	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*0.0, glm::vec3(0,0,1)),0.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/6, glm::vec3(0,0,1)),1.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/3, glm::vec3(0,0,1)),2.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/2, glm::vec3(0,0,1)),3.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*2/3, glm::vec3(0,0,1)),4.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*5/6, glm::vec3(0,0,1)),5.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*2/3, glm::vec3(0,0,1)),6.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/2, glm::vec3(0,0,1)),7.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/3, glm::vec3(0,0,1)),8.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI/6, glm::vec3(0,0,1)),9.0/0.3);
	traing->addLocalTransformKeyframe(getRotationMatrix(M_PI*0.0, glm::vec3(0,0,1)),10.0/0.3);
	
	lighted_cube->setGlobalTransform(glm::mat4(1.0)*getRotationMatrix(-M_PI*0.55, glm::vec3(1,0,0))*getTranslationMatrix(glm::vec3(0,0,-2)));
    lighted_cube->setLocalTransform(getScaleMatrix(50,.2,50)*getTranslationMatrix(0,0,0));



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