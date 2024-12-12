
    #include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <CylinderMeshRenderable.hpp>
#include <MeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>
#include <texturing/TexturedMeshRenderable.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <texturing/CubeMapRenderable.hpp>
#include <Io.hpp>
#include <Utils.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <lighting/LightedCubeRenderable.hpp>

void initialize_scene( Viewer& viewer )
{
    // In this scene, we will see the pengouin pushing the lever back in place
    // then we dezoom to see the pannel "a train can hide another"
    // then we dezoom to see the train on the rail, and the penguins on the side
    // then the train passes over the penguins, from left to right
    
    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
    viewer.addShaderProgram( flatShader );

    //Define a shader that encode an illumination model
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>( "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl", 
                                                                    "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram( phongShader );

    /* //Add a 3D frame to the viewer
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame); */

    //Textured shader
    //    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram( texShader );

    // Add light to the scene
	glm::vec3 red(0.9,0.3,0.4), green(0.3,0.9,0.4), blue(0.4,0.3,0.9);
	glm::vec3 white(1,1,1);
	glm::vec3 leafGreen(0,1,0);
	{ // SpotLight
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(0,3,2), glm::vec3(0,-1,-1), glm::vec3(0), white, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(phongShader, spot_light);
        viewer.addRenderable(spot_light_renderable);
    }

    //TODO : add a spotlight following the pannel
   /* { // SpotLight
        // Set the initial Spotlight position inside the box
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(0,3,2), glm::vec3(0,-1,-3), glm::vec3(0), white, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(phongShader, spot_light);
        viewer.addRenderable(spot_light_renderable);

        spot_light_renderable->setLocalTransform(getScaleMatrix(0.1, 0.1, 0.1));

        // Animate the spotlight by adding keyframes for its position*
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(0,3,2), glm::vec3(0,0,0), Light::base_forward), 0);
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(0,3,2), glm::vec3(0,0,0), Light::base_forward), 3);
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(0,3,-5), glm::vec3(0,0,0), Light::base_forward), 10.0);
    } */

    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 2 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );
    std::string penguin_mesh_path = "../../models3D/penguinEileen/bodyPingoinobj.obj";
    std::string penguin_texture_path = "../../models3D/penguinEileen/pinpoin.PNG";


    std::vector<std::vector<glm::vec3>> all_positions1;
    std::vector<std::vector<glm::vec3>> all_normals1;
    std::vector<std::vector<glm::vec2>> all_texcoords1;
    std::vector<MaterialPtr> materials1;


    read_obj_with_materials(penguin_mesh_path, "../../models3D/penguinEileen/", all_positions1, all_normals1, all_texcoords1, materials1);
    TexturedLightedMeshRenderablePtr penguin = std::make_shared<TexturedLightedMeshRenderable>(texShader, penguin_mesh_path, materials1[0], penguin_texture_path);
    
    const std::string beakBot_path = "../../models3D/penguinEileen/beakBot.obj";
    const std::string beakTop_path = "../../models3D/penguinEileen/beakTop.obj";
    const std::string eyes_path = "../../models3D/penguinEileen/eyes.obj";
    const std::string footR_path = "../../models3D/penguinEileen/footRight.obj";
    const std::string footL_path = "../../models3D/penguinEileen/footLeft.obj";
    const std::string wingR_path = "../../models3D/penguinEileen/wingRight.obj";
    const std::string wingL_path = "../../models3D/penguinEileen/wingLeft.obj";

    LightedMeshRenderablePtr beakBot = std::make_shared<LightedMeshRenderable>(phongShader, beakBot_path, Material::Gold());
    //beakBot->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));

    LightedMeshRenderablePtr beakTop = std::make_shared<LightedMeshRenderable>(phongShader, beakTop_path, Material::Gold());
    //beakTop->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));

    LightedMeshRenderablePtr eyes = std::make_shared<LightedMeshRenderable>(phongShader, eyes_path, Material::Pearl());
    //eyes->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));

    LightedMeshRenderablePtr footR = std::make_shared<LightedMeshRenderable>(phongShader, footR_path, Material::Gold());
    //footR->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));

    LightedMeshRenderablePtr footL = std::make_shared<LightedMeshRenderable>(phongShader, footL_path, Material::Gold());
    //footL->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));

    LightedMeshRenderablePtr wingR = std::make_shared<LightedMeshRenderable>(phongShader, wingR_path, Material::Pearl());
    //wingR->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));

    LightedMeshRenderablePtr wingL = std::make_shared<LightedMeshRenderable>(phongShader, wingL_path, Material::Pearl());
    //wingL->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));
    
    HierarchicalRenderable::addChild(penguin, beakBot);
    HierarchicalRenderable::addChild(penguin, beakTop);
    HierarchicalRenderable::addChild(penguin, eyes);
    HierarchicalRenderable::addChild(penguin, footR);
    HierarchicalRenderable::addChild(penguin, footL);
    HierarchicalRenderable::addChild(penguin, wingL);


    // Place the parts of the penguin based on the originParts positions
    glm::vec3 originParts[] = {
        glm::vec3(0, 0, 0), // BODY

        glm::vec3(0.74143, 0.95372, 0), // WINGS - Left
        glm::vec3(-0.74143, 0.95372, 0), // WINGS - Right

        glm::vec3(0, 1.25055, 0.658344), // EYES

        glm::vec3(0, 1.11317, 0.626099), // BEAK - Top
        glm::vec3(0, 0.983957, 0.626099), // BEAK - Bottom

        glm::vec3(0.368973, -0.859886, -0.117778), // FEET - Left
        glm::vec3(-0.368973, -0.859886, -0.117778), // FEET - Right
    };

    // Set transforms for each part
    penguin->setLocalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[0])* getTranslationMatrix(0.5,0.9,-0.8)); // BODY

    wingL->setLocalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[1])* getTranslationMatrix(0.5,0.9,-0.8)); // Left Wing
    wingR->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[2])* getTranslationMatrix(0.5,0.9,-0.8)); // Right Wing

    eyes->setLocalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[3])* getTranslationMatrix(0.5,0.9,-0.8)); // EYES

    beakBot->setLocalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[5])* getTranslationMatrix(0.5,0.9,-0.8)); // BEAK - Bottom
    beakTop->setLocalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[4])* getTranslationMatrix(0.5,0.9,-0.8)); // BEAK - Top

    footL->setLocalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[6])* getTranslationMatrix(0.5,0.9,-0.8)); // Left Foot
    footR->setLocalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[7])* getTranslationMatrix(0.5,0.9,-0.8)); // Right Foot

    //Animate de right wing
    wingR->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[2])* getTranslationMatrix(0.5,0.9,-0.8), 3.0);
    wingR->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[2])* getTranslationMatrix(0.5,0.9,-0.8)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 0, 1))*getTranslationMatrix(glm::vec3(0, 0, 0.5)), 0.5);
    wingR->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[2])* getTranslationMatrix(0.5,0.9,-0.8)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 0, 1))*getTranslationMatrix(glm::vec3(0, 0, 0.5)), 0.0);
    //unzoom
    wingR->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[2])* getTranslationMatrix(0.5,0.9,-0.8)*getTranslationMatrix(0,0,-10), 10.0);

    penguin->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0),0.0);
    penguin->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0),3.0);
    penguin->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0)*getTranslationMatrix(0,0,-5),10.0);

    viewer.addRenderable(wingR);
    viewer.addRenderable(penguin);

    //lever

    const std::string leverB_path = "../../models3D/lever/leverBody.obj";
    LightedMeshRenderablePtr leverB = std::make_shared<LightedMeshRenderable>(phongShader, leverB_path, Material::bodyLever());
    leverB->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));
    // Add keyframes for lever animation
    leverB->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1), 0.0);
    leverB->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1), 3.0);
    leverB->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1)*getTranslationMatrix(-10,0,0), 10.0);
    viewer.addRenderable(leverB);
    


    const std::string lever_path = "../../models3D/lever/leverLever.obj";

    // Create the lever object with a material and shader
    LightedMeshRenderablePtr lever = std::make_shared<LightedMeshRenderable>(phongShader, lever_path, Material::Lever());

    // Initial position and orientation of the lever
    lever->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1));

    // Add keyframes for lever animation
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1), 3.0);
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1), 2.5);
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1)*getRotationMatrix(M_PI * 0.15, glm::vec3(1, 0, 0)), 1);
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1)*getRotationMatrix(M_PI * 0.15, glm::vec3(1, 0, 0)), 0.0);
    //unzoom
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1)*getTranslationMatrix(-10,0,0), 10.0);


    viewer.addRenderable(lever);


    //panneau
    //TODO : les textures marchent pas
    const std::string pannel_path = "../../models3D/pano/pannal.obj";
    LightedMeshRenderablePtr pannel = std::make_shared<LightedMeshRenderable>(phongShader, pannel_path, Material::WhiteRubber());
    pannel->setGlobalTransform(getScaleMatrix(0.1,0.1,0.1));

    const std::string poto_path = "../../models3D/pano/poto.obj";
    LightedMeshRenderablePtr poto = std::make_shared<LightedMeshRenderable>(phongShader, poto_path, Material::Copper());
    pannel->setGlobalTransform(getScaleMatrix(0.1,0.1,0.1));

    HierarchicalRenderable::addChild(pannel, poto);

    pannel->addGlobalTransformKeyframe(getScaleMatrix(0.1,0.1,0.1)*getTranslationMatrix(0,0,35),0.0);
    pannel->addGlobalTransformKeyframe(getScaleMatrix(0.1,0.1,0.1)*getTranslationMatrix(0,0,35),3.0);
    pannel->addGlobalTransformKeyframe(getScaleMatrix(0.1,0.1,0.1)*getTranslationMatrix(0,0,-15),10.0);

    viewer.addRenderable(pannel);

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