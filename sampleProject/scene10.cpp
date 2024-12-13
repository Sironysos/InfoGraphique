#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <Io.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <lighting/Light.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <texturing/CubeMapRenderable.hpp>
#include <texturing/TexturedCubeRenderable.hpp>

void initialize_scene( Viewer& viewer )
{
    // In this scene, we will see the pengouin with the lever will evil eyes.
    
    //Shaders
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>( "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl", 
                                                                    "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram(phongShader);
    
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);

    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    viewer.addShaderProgram(cubeMapShader);

    // Sky
    std::string cubemap_dir = "../../models3D/ciel";
    auto cubemap = std::make_shared<CubeMapRenderable>(cubeMapShader, cubemap_dir);
    viewer.addRenderable(cubemap);

    // Lights
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

    // Camera  
    viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(0, 1.5, 2), glm::vec3(0, 1, 0), glm::vec3( 0, 1, 0 ) ) );

    std::string penguin_mesh_path = "../../models3D/penguinEileen/bodyPingoinobj.obj";
    std::string penguin_texture_path = "../../models3D/penguinEileen/pinpoin.PNG";

    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<MaterialPtr> materials;

    read_obj_with_materials(penguin_mesh_path, "../../models3D/penguinEileen/", all_positions, all_normals, all_texcoords, materials);
    TexturedLightedMeshRenderablePtr penguin = std::make_shared<TexturedLightedMeshRenderable>(texShader, penguin_mesh_path, materials[0], penguin_texture_path);
    
    const std::string beakBot_path = "../../models3D/penguinEileen/beakBot.obj";
    const std::string beakTop_path = "../../models3D/penguinEileen/beakTop.obj";
    const std::string eyes_path = "../../models3D/penguinEileen/eyes.obj";
    const std::string footR_path = "../../models3D/penguinEileen/footRight.obj";
    const std::string footL_path = "../../models3D/penguinEileen/footLeft.obj";
    const std::string wingR_path = "../../models3D/penguinEileen/wingRight.obj";
    const std::string wingL_path = "../../models3D/penguinEileen/wingLeft.obj";

    LightedMeshRenderablePtr beakBot = std::make_shared<LightedMeshRenderable>(phongShader, beakBot_path, Material::Gold());
    LightedMeshRenderablePtr beakTop = std::make_shared<LightedMeshRenderable>(phongShader, beakTop_path, Material::Gold());
    LightedMeshRenderablePtr eyes = std::make_shared<LightedMeshRenderable>(phongShader, eyes_path, Material::Ruby());
    LightedMeshRenderablePtr footR = std::make_shared<LightedMeshRenderable>(phongShader, footR_path, Material::Gold());  
    LightedMeshRenderablePtr footL = std::make_shared<LightedMeshRenderable>(phongShader, footL_path, Material::Gold());  
    LightedMeshRenderablePtr wingR = std::make_shared<LightedMeshRenderable>(phongShader, wingR_path, Material::Pearl());  
    LightedMeshRenderablePtr wingL = std::make_shared<LightedMeshRenderable>(phongShader, wingL_path, Material::Pearl());

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
    penguin->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[0])* getTranslationMatrix(0.5,0.9,-0.8)); // BODY

    wingL->setGlobalTransform(getTranslationMatrix(originParts[1])); // Left Wing
    wingR->setGlobalTransform(getTranslationMatrix(originParts[2])); // Right Wing

    eyes->setGlobalTransform(getTranslationMatrix(originParts[3])); // EYES

    beakBot->setGlobalTransform(getTranslationMatrix(originParts[5])); // BEAK - Bottom
    beakTop->setGlobalTransform(getTranslationMatrix(originParts[4])); // BEAK - Top

    footL->setGlobalTransform(getTranslationMatrix(originParts[6])); // Left Foot
    footR->setGlobalTransform(getTranslationMatrix(originParts[7])); // Right Foot

    HierarchicalRenderable::addChild(penguin, beakBot);
    HierarchicalRenderable::addChild(penguin, beakTop);
    HierarchicalRenderable::addChild(penguin, eyes);
    HierarchicalRenderable::addChild(penguin, footR);
    HierarchicalRenderable::addChild(penguin, footL);
    HierarchicalRenderable::addChild(penguin, wingR);
    HierarchicalRenderable::addChild(penguin, wingL);

    viewer.addRenderable(penguin);

    //lever
    const std::string leverB_path = "../../models3D/lever/leverBody.obj";
    LightedMeshRenderablePtr leverB = std::make_shared<LightedMeshRenderable>(phongShader, leverB_path, Material::bodyLever());
    leverB->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));
    viewer.addRenderable(leverB);

    const std::string lever_path = "../../models3D/lever/leverLever.obj";
    LightedMeshRenderablePtr lever = std::make_shared<LightedMeshRenderable>(phongShader, lever_path, Material::Lever());
    lever->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1));

    // Add keyframes for lever animation
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1), 0);
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1), 4);

    viewer.addRenderable(lever);

    // Ground
    auto tcube = std::make_shared<TexturedCubeRenderable>(texShader, "../../models3D/sable.jpg");
    viewer.addRenderable(tcube);
    tcube->setLocalTransform(getTranslationMatrix(0,-0.5,0)*getScaleMatrix(60,1,60));

    // Rails
    const std::string rail_path = "../../models3D/rail/rail.obj";
    std::string rail_texture_path = "../../models3D/rail/woodAndMetal.jpg";

    std::vector<std::vector<glm::vec3>> all_positions1;
    std::vector<std::vector<glm::vec3>> all_normals1;
    std::vector<std::vector<glm::vec2>> all_texcoords1;
    std::vector<MaterialPtr> materials1;

    read_obj_with_materials(rail_path, "../../models3D/rail/", all_positions1, all_normals1, all_texcoords1, materials1);

    TexturedLightedMeshRenderablePtr railo = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
    railo->setLocalTransform(getTranslationMatrix(0,0,-3.6*2));
    
    for (int i = -1; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
        rail->setLocalTransform(getTranslationMatrix(0,0,3.6*i));
        HierarchicalRenderable::addChild(railo, rail);
    }
    railo->setGlobalTransform(getRotationMatrix(M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-1,0,-20));
    viewer.addRenderable(railo);

    //embranchement
    TexturedLightedMeshRenderablePtr railo2 = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);

    for (int i = 1; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
        rail->setLocalTransform(getTranslationMatrix(0,0,3.6*i));
        HierarchicalRenderable::addChild(railo2, rail);
    }

    railo2->setGlobalTransform(getRotationMatrix(M_PI*0.65, glm::vec3(0,1,0))*getTranslationMatrix(-1.45,0,2.5));
    viewer.addRenderable(railo2);

    //Penguin 4
    std::string penguin4_texture_path = "../../models3D/penguinEileen/pink.png";
    TexturedLightedMeshRenderablePtr penguin4 = std::make_shared<TexturedLightedMeshRenderable>(texShader, penguin_mesh_path, materials[0], penguin4_texture_path);
    LightedMeshRenderablePtr beakBot4 = std::make_shared<LightedMeshRenderable>(phongShader, beakBot_path, Material::Gold());
    LightedMeshRenderablePtr beakTop4 = std::make_shared<LightedMeshRenderable>(phongShader, beakTop_path, Material::Gold());
    LightedMeshRenderablePtr eyes4 = std::make_shared<LightedMeshRenderable>(phongShader, eyes_path, Material::WhiteRubber());   
    LightedMeshRenderablePtr footR4 = std::make_shared<LightedMeshRenderable>(phongShader, footR_path, Material::Gold());  
    LightedMeshRenderablePtr footL4 = std::make_shared<LightedMeshRenderable>(phongShader, footL_path, Material::Gold());  
    LightedMeshRenderablePtr wingR4 = std::make_shared<LightedMeshRenderable>(phongShader, wingR_path, Material::Pearl()); 
    LightedMeshRenderablePtr wingL4 = std::make_shared<LightedMeshRenderable>(phongShader, wingL_path, Material::Pearl());
    
    // Set transforms for each part
    penguin4->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[0])* getTranslationMatrix(0.5,0.9,-0.8)); // BODY
    wingL4->setGlobalTransform(getTranslationMatrix(originParts[1])); // Left Wing
    wingR4->setGlobalTransform(getTranslationMatrix(originParts[2])); // Right Wing
    eyes4->setGlobalTransform(getTranslationMatrix(originParts[3])); // EYES
    beakBot4->setGlobalTransform(getTranslationMatrix(originParts[5])); // BEAK - Bottom
    beakTop4->setGlobalTransform(getTranslationMatrix(originParts[4])); // BEAK - Top
    footR4->setGlobalTransform(getTranslationMatrix(originParts[7])); // Right Foot
    footL4->setGlobalTransform(getTranslationMatrix(originParts[6])); // Left Foot

    HierarchicalRenderable::addChild(penguin4, beakBot4);
    HierarchicalRenderable::addChild(penguin4, beakTop4);
    HierarchicalRenderable::addChild(penguin4, eyes4);
    HierarchicalRenderable::addChild(penguin4, footR4);
    HierarchicalRenderable::addChild(penguin4, footL4);
    HierarchicalRenderable::addChild(penguin4, wingR4);
    HierarchicalRenderable::addChild(penguin4, wingL4);

    penguin4->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI, glm::vec3(0,1,0))*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(4,5,-3)*getRotationMatrix(M_PI,0,0,1)*getTranslationMatrix(12,5.1,3.5)*getRotationMatrix(M_PI*0.15,0,0,1)*getScaleMatrix(1,1,0.2));
    viewer.addRenderable(penguin4);

    HierarchicalRenderable::addChild(tcube, penguin4);
    HierarchicalRenderable::addChild(tcube, penguin);
    HierarchicalRenderable::addChild(tcube, lever);
    HierarchicalRenderable::addChild(tcube, leverB);
    HierarchicalRenderable::addChild(tcube, railo);
    HierarchicalRenderable::addChild(tcube, railo2);

    tcube->addLocalTransformKeyframe(getTranslationMatrix(0,-0.5,0)*getScaleMatrix(60,1,60),0);
    tcube->addLocalTransformKeyframe(getTranslationMatrix(0,-0.5,0)*getScaleMatrix(60,1,60),10);
    tcube->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0),0);
    tcube->addGlobalTransformKeyframe(getTranslationMatrix(-0.25,0.5,1.5),5);
    tcube->addGlobalTransformKeyframe(getTranslationMatrix(-0.25,0.5,1.5),9);

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