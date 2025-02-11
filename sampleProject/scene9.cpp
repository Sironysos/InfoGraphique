
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
#include <texturing/TexturedCubeRenderable.hpp>



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

    //Textured shader
    //    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram( texShader );

    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    viewer.addShaderProgram(cubeMapShader);

    std::string cubemap_dir = "../../models3D/ciel";
    auto cubemap = std::make_shared<CubeMapRenderable>(cubeMapShader, cubemap_dir);
    viewer.addRenderable(cubemap);

    // Add light to the scene
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

    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 1, 2 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );
    
    auto tcube = std::make_shared<TexturedCubeRenderable>(texShader, "../../models3D/sable.jpg");
    viewer.addRenderable(tcube);
    tcube->setGlobalTransform(getTranslationMatrix(0,-0.5,0)*getScaleMatrix(60,1,60));

    tcube->addLocalTransformKeyframe(getTranslationMatrix(0,-0.5,0)*getScaleMatrix(60,1,60), 0.0);
    tcube->addLocalTransformKeyframe(getTranslationMatrix(0,-0.5,0)*getScaleMatrix(60,1,60), 10.0);

    
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
    wingR->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[2])* getTranslationMatrix(0.5,0.9,-0.8), 20.0);

    penguin->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0),0.0);
    penguin->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0),3.0);
    penguin->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0),20);

    viewer.addRenderable(wingR);
    viewer.addRenderable(penguin);

    //lever

    const std::string leverB_path = "../../models3D/lever/leverBody.obj";
    LightedMeshRenderablePtr leverB = std::make_shared<LightedMeshRenderable>(phongShader, leverB_path, Material::bodyLever());
    leverB->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1));
    // Add keyframes for lever animation
    leverB->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1), 0);
    leverB->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1), 3);
    leverB->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1), 20);
    viewer.addRenderable(leverB);

    const std::string lever_path = "../../models3D/lever/leverLever.obj";

    // Create the lever object with a material and shader
    LightedMeshRenderablePtr lever = std::make_shared<LightedMeshRenderable>(phongShader, lever_path, Material::Lever());

    // Initial position and orientation of the lever
    lever->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1));

    // Add keyframes for lever animation
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1), 3);
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1), 2.5);
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1)*getRotationMatrix(M_PI * 0.15, glm::vec3(1, 0, 0)), 1);
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0, 0, 1)*getRotationMatrix(M_PI * 0.15, glm::vec3(1, 0, 0)), 0);
    //unzoom
    lever->addGlobalTransformKeyframe(getScaleMatrix(0.5, 0.5, 0.5)*getRotationMatrix(-M_PI * 0.5, glm::vec3(0, 1, 0))*getTranslationMatrix(0,0,1), 20);
    viewer.addRenderable(lever);

    // Rails
    const std::string rail_path = "../../models3D/rail/rail.obj";
    std::string rail_texture_path = "../../models3D/rail/woodAndMetal.jpg";
    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<MaterialPtr> materials;

    read_obj_with_materials(rail_path, "../../models3D/rail/", all_positions, all_normals, all_texcoords, materials);
    

    TexturedLightedMeshRenderablePtr railo = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials[0], rail_texture_path);
    railo->setLocalTransform(getScaleMatrix(1,1,1)*getTranslationMatrix(0,0,-3.6*2));
    
    for (int i = -1; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials[0], rail_texture_path);
        rail->setLocalTransform(getScaleMatrix(1,1,1)*getTranslationMatrix(0,0,3.6*i));
        HierarchicalRenderable::addChild(railo, rail);
    }
    railo->setGlobalTransform(getScaleMatrix(1,1,1)*getRotationMatrix(M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-1,0,-20));
    viewer.addRenderable(railo);

    //embranchement
    TexturedLightedMeshRenderablePtr railo2 = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials[0], rail_texture_path);

    for (int i = 1; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials[0], rail_texture_path);
        rail->setLocalTransform(getScaleMatrix(1,1,1)*getTranslationMatrix(0,0,3.6*i));
        HierarchicalRenderable::addChild(railo2, rail);
    }

    railo2->setGlobalTransform(getScaleMatrix(1,1,1)*getRotationMatrix(M_PI*0.65, glm::vec3(0,1,0))*getTranslationMatrix(-1.45,0,2.5));
    viewer.addRenderable(railo2);


    //panneau
    //TODO : les textures marchent pas
    const std::string pannel_path = "../../models3D/pano/pannal.obj";
    const std::string poto_path = "../../models3D/pano/poto.obj";
    std::string pannel_texture_path = "../../models3D/blanc_noir.jpg";
    std::string poto_texture_path = "../../models3D/rail/woodAndMetal.jpg";
    std::vector<std::vector<glm::vec3>> all_positions2;
    std::vector<std::vector<glm::vec3>> all_normals2;
    std::vector<std::vector<glm::vec2>> all_texcoords2;
    std::vector<MaterialPtr> materials2;

    read_obj_with_materials(pannel_path, "../../models3D/pano/", all_positions2, all_normals2, all_texcoords2, materials2);
    

    TexturedLightedMeshRenderablePtr pannel = std::make_shared<TexturedLightedMeshRenderable>(texShader, pannel_path, materials2[0], pannel_texture_path);
    pannel->setGlobalTransform(getScaleMatrix(0.1,0.1,0.1));

  
    std::vector<std::vector<glm::vec3>> all_positions3;
    std::vector<std::vector<glm::vec3>> all_normals3;
    std::vector<std::vector<glm::vec2>> all_texcoords3;
    std::vector<MaterialPtr> materials3;

    read_obj_with_materials(poto_path, "../../models3D/pano/", all_positions3, all_normals3, all_texcoords3, materials3);
    

    TexturedLightedMeshRenderablePtr poto = std::make_shared<TexturedLightedMeshRenderable>(texShader, poto_path, materials3[0], poto_texture_path);

    HierarchicalRenderable::addChild(pannel, poto);

    pannel->addGlobalTransformKeyframe(getScaleMatrix(0.3,0.3,0.3)*getTranslationMatrix(-2,-1,20),0.0);
    pannel->addGlobalTransformKeyframe(getScaleMatrix(0.3,0.3,0.3)*getTranslationMatrix(-2,-1,20),20.0);
    viewer.addRenderable(pannel);

    //Penguin 1
    std::string penguin1_texture_path = "../../models3D/penguinEileen/papier.jpg";
    TexturedLightedMeshRenderablePtr penguin1 = std::make_shared<TexturedLightedMeshRenderable>(texShader, penguin_mesh_path, materials[0], penguin1_texture_path);
    LightedMeshRenderablePtr beakBot1 = std::make_shared<LightedMeshRenderable>(phongShader, beakBot_path, Material::Gold());
    LightedMeshRenderablePtr beakTop1 = std::make_shared<LightedMeshRenderable>(phongShader, beakTop_path, Material::Gold());
    LightedMeshRenderablePtr eyes1 = std::make_shared<LightedMeshRenderable>(phongShader, eyes_path, Material::Emerald());   
    LightedMeshRenderablePtr footR1 = std::make_shared<LightedMeshRenderable>(phongShader, footR_path, Material::Gold());  
    LightedMeshRenderablePtr footL1 = std::make_shared<LightedMeshRenderable>(phongShader, footL_path, Material::Gold());  
    LightedMeshRenderablePtr wingR1 = std::make_shared<LightedMeshRenderable>(phongShader, wingR_path, Material::Pearl()); 
    LightedMeshRenderablePtr wingL1 = std::make_shared<LightedMeshRenderable>(phongShader, wingL_path, Material::Pearl());
    
    // Set transforms for each part
    penguin1->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[0])); // BODY
    wingL1->setGlobalTransform(getTranslationMatrix(originParts[1])); // Left Wing
    wingR1->setGlobalTransform(getTranslationMatrix(originParts[2])); // Right Wing
    eyes1->setGlobalTransform(getTranslationMatrix(originParts[3])); // EYES
    beakBot1->setGlobalTransform(getTranslationMatrix(originParts[5])); // BEAK - Bottom
    beakTop1->setGlobalTransform(getTranslationMatrix(originParts[4])); // BEAK - Top
    footL1->setGlobalTransform(getTranslationMatrix(originParts[6])); // Left Foot
    footR1->setGlobalTransform(getTranslationMatrix(originParts[7])); // Right Foot

    HierarchicalRenderable::addChild(penguin1, beakBot1);
    HierarchicalRenderable::addChild(penguin1, beakTop1);
    HierarchicalRenderable::addChild(penguin1, eyes1);
    HierarchicalRenderable::addChild(penguin1, footR1);
    HierarchicalRenderable::addChild(penguin1, footL1);
    HierarchicalRenderable::addChild(penguin1, wingR1);
    HierarchicalRenderable::addChild(penguin1, wingL1);

    penguin1->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(8,-3,1));
    penguin1->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(8,-3,1),0);
    penguin1->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(8,-3,1),8);
    penguin1->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(8,-3,0.5)*getScaleMatrix(1,1,0.2),8.05);
    penguin1->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(8,-3,0.5)*getScaleMatrix(1,1,0.2),20);

    viewer.addRenderable(penguin1);

    //Penguin 2
    std::string penguin2_texture_path = "../../models3D/penguinEileen/herbe.png";
    TexturedLightedMeshRenderablePtr penguin2 = std::make_shared<TexturedLightedMeshRenderable>(texShader, penguin_mesh_path, materials[0], penguin2_texture_path);
    LightedMeshRenderablePtr beakBot2 = std::make_shared<LightedMeshRenderable>(phongShader, beakBot_path, Material::Copper());
    LightedMeshRenderablePtr beakTop2 = std::make_shared<LightedMeshRenderable>(phongShader, beakTop_path, Material::Copper());
    LightedMeshRenderablePtr eyes2 = std::make_shared<LightedMeshRenderable>(phongShader, eyes_path, Material::Copper());   
    LightedMeshRenderablePtr footR2 = std::make_shared<LightedMeshRenderable>(phongShader, footR_path, Material::Gold());  
    LightedMeshRenderablePtr footL2 = std::make_shared<LightedMeshRenderable>(phongShader, footL_path, Material::Gold());  
    LightedMeshRenderablePtr wingR2 = std::make_shared<LightedMeshRenderable>(phongShader, wingR_path, Material::Pearl()); 
    LightedMeshRenderablePtr wingL2 = std::make_shared<LightedMeshRenderable>(phongShader, wingL_path, Material::Pearl());
    
    // Set transforms for each part
    penguin2->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[0])); // BODY
    wingL2->setGlobalTransform(getTranslationMatrix(originParts[1])); // Left Wing
    wingR2->setGlobalTransform(getTranslationMatrix(originParts[2])); // Right Wing
    eyes2->setGlobalTransform(getTranslationMatrix(originParts[3])); // EYES
    beakBot2->setGlobalTransform(getTranslationMatrix(originParts[5])); // BEAK - Bottom
    beakTop2->setGlobalTransform(getTranslationMatrix(originParts[4])); // BEAK - Top
    footL2->setGlobalTransform(getTranslationMatrix(originParts[6])); // Left Foot
    footR2->setGlobalTransform(getTranslationMatrix(originParts[7])); // Right Foot

    HierarchicalRenderable::addChild(penguin2, beakBot2);
    HierarchicalRenderable::addChild(penguin2, beakTop2);
    HierarchicalRenderable::addChild(penguin2, eyes2);
    HierarchicalRenderable::addChild(penguin2, footR2);
    HierarchicalRenderable::addChild(penguin2, footL2);
    HierarchicalRenderable::addChild(penguin2, wingR2);
    HierarchicalRenderable::addChild(penguin2, wingL2);

    penguin2->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(10,-3,1));
    penguin2->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(10,-3,1),0);
    penguin2->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(10,-3,1),8.05);
    penguin2->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(10,-3,0.5)*getScaleMatrix(1,1,0.2),8.1);
    penguin2->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(10,-3,0.5)*getScaleMatrix(1,1,0.2),20);
    viewer.addRenderable(penguin2);

    //Penguin 3
    std::string penguin3_texture_path = "../../models3D/penguinEileen/gold.png";
    TexturedLightedMeshRenderablePtr penguin3 = std::make_shared<TexturedLightedMeshRenderable>(texShader, penguin_mesh_path, materials[0], penguin3_texture_path);
    LightedMeshRenderablePtr beakBot3 = std::make_shared<LightedMeshRenderable>(phongShader, beakBot_path, Material::Gold());
    LightedMeshRenderablePtr beakTop3 = std::make_shared<LightedMeshRenderable>(phongShader, beakTop_path, Material::Gold());
    LightedMeshRenderablePtr eyes3 = std::make_shared<LightedMeshRenderable>(phongShader, eyes_path, Material::Emerald());   
    LightedMeshRenderablePtr footR3 = std::make_shared<LightedMeshRenderable>(phongShader, footR_path, Material::Gold());  
    LightedMeshRenderablePtr footL3 = std::make_shared<LightedMeshRenderable>(phongShader, footL_path, Material::Gold());  
    LightedMeshRenderablePtr wingR3 = std::make_shared<LightedMeshRenderable>(phongShader, wingR_path, Material::Pearl()); 
    LightedMeshRenderablePtr wingL3 = std::make_shared<LightedMeshRenderable>(phongShader, wingL_path, Material::Pearl());
    
    // Set transforms for each part
    penguin3->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[0])); // BODY
    wingL3->setGlobalTransform(getTranslationMatrix(originParts[1])); // Left Wing
    wingR3->setGlobalTransform(getTranslationMatrix(originParts[2])); // Right Wing
    eyes3->setGlobalTransform(getTranslationMatrix(originParts[3])); // EYES
    beakBot3->setGlobalTransform(getTranslationMatrix(originParts[5])); // BEAK - Bottom
    beakTop3->setGlobalTransform(getTranslationMatrix(originParts[4])); // BEAK - Top
    footL3->setGlobalTransform(getTranslationMatrix(originParts[6])); // Left Foot
    footR3->setGlobalTransform(getTranslationMatrix(originParts[7])); // Right Foot

    HierarchicalRenderable::addChild(penguin3, beakBot3);
    HierarchicalRenderable::addChild(penguin3, beakTop3);
    HierarchicalRenderable::addChild(penguin3, eyes3);
    HierarchicalRenderable::addChild(penguin3, footR3);
    HierarchicalRenderable::addChild(penguin3, footL3);
    HierarchicalRenderable::addChild(penguin3, wingR3);
    HierarchicalRenderable::addChild(penguin3, wingL3);

    penguin3->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(12,-3,1));
    penguin3->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(12,-3,1),0);
    penguin3->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(12,-3,1),8.1);
    penguin3->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(12,-3,0.5)*getScaleMatrix(1,1,0.2),8.15);
    penguin3->addGlobalTransformKeyframe(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(12,-3,0.5)*getScaleMatrix(1,1,0.2),20);
    viewer.addRenderable(penguin3);

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
    penguin4->setGlobalTransform(getScaleMatrix(0.5, 0.5, 0.5) * getTranslationMatrix(originParts[0])); // BODY
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

    penguin4->setGlobalTransform(getScaleMatrix(0.5,0.5,0.5)*getRotationMatrix(M_PI*0.5, glm::vec3(-1,0,0))*getTranslationMatrix(8,0,0.5)*getRotationMatrix(M_PI*0.15, glm::vec3(0,0,1))*getScaleMatrix(1,1,0.2));
    viewer.addRenderable(penguin4);

    HierarchicalRenderable::addChild(tcube, pannel);
    HierarchicalRenderable::addChild(tcube, railo2);
    HierarchicalRenderable::addChild(tcube, railo);
    HierarchicalRenderable::addChild(tcube, lever);
    HierarchicalRenderable::addChild(tcube, leverB);
    HierarchicalRenderable::addChild(tcube, penguin);
    HierarchicalRenderable::addChild(tcube, penguin1);
    HierarchicalRenderable::addChild(tcube, penguin2);
    HierarchicalRenderable::addChild(tcube, penguin3);
    HierarchicalRenderable::addChild(tcube, penguin4);
    HierarchicalRenderable::addChild(tcube, wingR);

    tcube->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0),0.0);
    tcube->addGlobalTransformKeyframe(getTranslationMatrix(0,0,0),3.0);
    tcube->addGlobalTransformKeyframe(getTranslationMatrix(0,0,-5),6.0);
    tcube->addGlobalTransformKeyframe(getTranslationMatrix(0,0,-5),20.0);

    //train
    const std::string traing_path = "../../models3D/un_oldTrain.obj";
    const std::string train_texture_path = "../../models3D/metal.jpg";
    std::vector<std::vector<glm::vec3>> all_positionst;
    std::vector<std::vector<glm::vec3>> all_normalst;
    std::vector<std::vector<glm::vec2>> all_texcoordst;
    std::vector<MaterialPtr> materialst;

    read_obj_with_materials(traing_path, "../../models3D/", all_positionst, all_normalst, all_texcoordst, materialst);
    TexturedLightedMeshRenderablePtr train = std::make_shared<TexturedLightedMeshRenderable>(texShader, traing_path, materialst[0], train_texture_path);
    viewer.addRenderable(train);
    train->setGlobalTransform(getRotationMatrix(-M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(0, 0, -40));
    train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-5, 0, 120), 0);
    train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-5, 0, 120), 6);
    train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-5, 0, -120),10);
    train->addGlobalTransformKeyframe(getRotationMatrix(-M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-5, 0, -120),20);

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