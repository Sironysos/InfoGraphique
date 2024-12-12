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
    // In this scene, we will see the penguin panic next to his lever
    
    
    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
    viewer.addShaderProgram( flatShader );

    //Define a shader that encode an illumination model
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>( "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl", 
                                                                    "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram( phongShader );

    //Add a 3D frame to the viewer
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Textured shader
    //    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram( texShader );

    /* //Lights
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
    viewer.addDirectionalLight(light3); */

    // Add light to the scene
	glm::vec3 red(0.9,0.3,0.4), green(0.3,0.9,0.4), blue(0.4,0.3,0.9);
	glm::vec3 white(1,1,1);
	glm::vec3 leafGreen(0,1,0);
	{ // SpotLight
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(0,3,2), glm::vec3(0,-1,-3), glm::vec3(0), white, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(phongShader, spot_light);
        viewer.addRenderable(spot_light_renderable);
    }

    /* // Textured cactus
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 2 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );
    std::string penguin_mesh_path = "../../models3D/cactus/source/untitled.obj";
    std::string penguin_texture_path = "../../models3D/cactus/textures/Amigo_03.png";

    std::string penguin_mesh_path2 = "../../models3D/cactus/source/cap.obj";

    std::vector<std::vector<glm::vec3>> all_positions1;
    std::vector<std::vector<glm::vec3>> all_normals1;
    std::vector<std::vector<glm::vec2>> all_texcoords1;
    std::vector<std::vector<unsigned int>> all_indices1;
    std::vector<MaterialPtr> materials1;

    std::vector<std::vector<glm::vec3>> all_positions2;
    std::vector<std::vector<glm::vec3>> all_normals2;
    std::vector<std::vector<glm::vec2>> all_texcoords2;
    std::vector<std::vector<unsigned int>> all_indices2;
    std::vector<MaterialPtr> materials2;

    read_obj_with_materials(penguin_mesh_path, "../../models3D/cactus/source/", all_positions1, all_normals1, all_texcoords1, materials1);
    TexturedLightedMeshRenderablePtr penguin = std::make_shared<TexturedLightedMeshRenderable>(texShader, penguin_mesh_path, materials1[0], penguin_texture_path);
    
    //penguin->setGlobalTransform(getRotationMatrix(M_PI * 0.5, glm::vec3(0, 0, 1)));
    viewer.addRenderable(penguin);

    read_obj_with_materials(penguin_mesh_path2, "../../models3D/cactus/source/", all_positions2, all_normals2, all_texcoords2, materials2);
    TexturedLightedMeshRenderablePtr penguin2 = std::make_shared<TexturedLightedMeshRenderable>(texShader, penguin_mesh_path2, materials2[0], penguin_texture_path);
    
    viewer.addRenderable(penguin2); */

    // Add the penguin
    /* const std::vector<std::string> penguin_paths = {
        "../../models3D/penguinEileen/beakBot.obj",
        "../../models3D/penguinEileen/beakTop.obj",
        "../../models3D/penguinEileen/body.obj",
        "../../models3D/penguinEileen/eyes.obj",
        "../../models3D/penguinEileen/footRight.obj",
        "../../models3D/penguinEileen/footLeft.obj",
        "../../models3D/penguinEileen/wingRight.obj",
        "../../models3D/penguinEileen/wingLeft.obj"
    };
    const std::vector<std::string> penguin_texture_paths = {
        "../../models3D/penguinEileen/penguinBodyTex.png",
        "../../models3D/penguinEileen/sad_eyes.png"
    }; */

    /* int i = 0;
    
    for (const auto& carl : penguin_paths ) {
        std::vector<std::vector<glm::vec3>> all_positions;
        std::vector<std::vector<glm::vec3>> all_normals;
        std::vector<std::vector<glm::vec2>> all_texcoords;
        std::vector<std::vector<unsigned int>> all_indices;
        std::vector<MaterialPtr> materials;
        read_obj_with_materials(carl, "../../models3D/penguinEileen/", all_positions, all_normals, all_texcoords, materials);
        if (i == 3) {
            TexturedLightedMeshRenderablePtr penguin = std::make_shared<TexturedLightedMeshRenderable>(texShader, carl, materials[0], penguin_texture_paths[1]);
            viewer.addRenderable(penguin);
        }else {
            TexturedLightedMeshRenderablePtr penguin = std::make_shared<TexturedLightedMeshRenderable>(texShader, carl, materials[0], penguin_texture_paths[0]);
            viewer.addRenderable(penguin);
        }
        i++;
    } */

    /* const std::string beakBot_path = "../../models3D/penguinEileen/beakBot.obj";
    const std::string beakTop_path = "../../models3D/penguinEileen/beakTop.obj";
    const std::string eyes_path = "../../models3D/penguinEileen/eyes.obj";
    const std::string footR_path = "../../models3D/penguinEileen/footRight.obj";
    const std::string footL_path = "../../models3D/penguinEileen/footLeft.obj";
    const std::string wingR_path = "../../models3D/penguinEileen/wingRight.obj";
    const std::string wingL_path = "../../models3D/penguinEileen/wingLeft.obj";
    std::string eyes_texture_path = "../../models3D/penguinEileen/sad_eyes.png"; */

    /* const std::string body_path = "../../models3D/penguinEileen/body.obj";
    std::string body_texture_path = "../../models3D/penguinEileen/penguinBodyTex.png";
    

    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<MaterialPtr> materials;
    read_obj_with_materials(body_path, "../../models3D/penguinEileen/", all_positions, all_normals, all_texcoords, materials);
    TexturedLightedMeshRenderablePtr penguin = std::make_shared<TexturedLightedMeshRenderable>(texShader,body_path, materials[0], body_texture_path);
    viewer.addRenderable(penguin); */
    
    
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
    
    


    //penguin->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getTranslationMatrix(1,1,0));

    // Place the parts of the penguin based on the originParts positions
    glm::vec3 originParts[] = {
        glm::vec3(0, 0.98319, 0), // BODY

        glm::vec3(0.74143, 0.95372, 0), // WINGS - Left
        glm::vec3(-0.74143, 0.95372, 0), // WINGS - Right

        glm::vec3(0, 1.25055, 0.658344), // EYES

        glm::vec3(0, 1.11317, 0.626099), // BEAK - Top
        glm::vec3(0, 0.983957, 0.626099), // BEAK - Bottom

        glm::vec3(0.368973, -0.859886, -0.117778), // FEET - Left
        glm::vec3(-0.368973, -0.859886, -0.117778), // FEET - Right
    };

    // Set transforms for each part
    penguin->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[0])); // BODY

    wingL->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[1])); // Left Wing
    wingR->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[2])); // Right Wing

    eyes->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[3])); // EYES

    beakTop->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[4])); // BEAK - Top
    beakBot->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[5])); // BEAK - Bottom

    footL->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[6])); // Left Foot
    footR->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[7])); // Right Foot


    viewer.addRenderable(beakBot);
    viewer.addRenderable(beakTop);
    viewer.addRenderable(eyes);
    viewer.addRenderable(footR);
    viewer.addRenderable(footL);
    viewer.addRenderable(wingR);
    viewer.addRenderable(wingL);
    viewer.addRenderable(penguin);



    
    //penguin->setGlobalTransform(getRotationMatrix(M_PI * 0.5, glm::vec3(0, 0, 1)));

    //lever

    const std::string leverB_path = "../../models3D/lever/leverBody.obj";
    LightedMeshRenderablePtr leverB = std::make_shared<LightedMeshRenderable>(phongShader, leverB_path, Material::bodyLever());
    leverB->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));
    viewer.addRenderable(leverB);

    const std::string lever_path = "../../models3D/lever/leverLever.obj";

    // Create the lever object with a material and shader
    LightedMeshRenderablePtr lever = std::make_shared<LightedMeshRenderable>(phongShader, lever_path, Material::Lever());

    // Initial position and orientation of the lever
    lever->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1));

    // Add keyframes for lever animation
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1), 0.0);
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1), 4.0);

    viewer.addRenderable(lever);


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