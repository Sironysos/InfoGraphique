#include <Viewer.hpp>
#include <ShaderProgram.hpp>

#include <lighting/LightedMeshRenderable.hpp>
#include <lighting/LightedCubeRenderable.hpp>
#include <lighting/LightedCylinderRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <lighting/PointLightRenderable.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <FrameRenderable.hpp>
#include <texturing/CubeMapRenderable.hpp>
#include <FrameRenderable.hpp>
#include <GeometricTransformation.hpp>
#include <Utils.hpp>

#include <iostream>

void initialize_scene( Viewer& viewer )
{
    /* //Default shader
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

    //Define a transformation
    glm::mat4 globalTransformation, localTransformation;

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,-1.0,-1.0));
    glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.3,0.3,0.1), d_specular(0.3,0.3,0.1);
    //glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.0,0.0,0.0), d_specular(0.0,0.0,0.0);
    glm::vec3 lightPosition(0.0,5.0,8.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    directionalLight->setGlobalTransform(getTranslationMatrix(lightPosition) * directionalLight->getGlobalTransform());
    
    //Add a renderable to display the light and control it via mouse/key event
    DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight);
    localTransformation = getScaleMatrix(0.5);
    directionalLightRenderable->setLocalTransform(localTransformation);
    viewer.addDirectionalLight(directionalLight);
    viewer.addRenderable(directionalLightRenderable);

    //Define a point light
    glm::vec3 p_position(0.0,0.0,0.0), p_ambient(0.0,0.0,0.0), p_diffuse(0.0,0.0,0.0), p_specular(0.0,0.0,0.0);
    float p_constant=0.0, p_linear=0.0, p_quadratic=0.0;

    p_position = glm::vec3(-8, 5.0, 5.0);
    p_ambient = glm::vec3(0.0,0.0,0.0);
    p_diffuse = glm::vec3(1.0,0.0,0.0);
    p_specular = glm::vec3(1.0,0.0,0.0);
    p_constant=1.0;
    p_linear=5e-1;
    p_quadratic=0;
    PointLightPtr pointLight1 = std::make_shared<PointLight>(p_position, p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
    PointLightRenderablePtr pointLightRenderable1 = std::make_shared<PointLightRenderable>(flatShader, pointLight1);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    pointLightRenderable1->setLocalTransform(localTransformation);
    viewer.addPointLight(pointLight1);
    viewer.addRenderable(pointLightRenderable1);

    p_position = glm::vec3(8, 5.0, 5.0);
    p_ambient = glm::vec3(0.0,0.0,0.0);
    p_diffuse = glm::vec3(0.0,0.0,1.0);
    p_specular = glm::vec3(0.0,0.0,1.0);
    p_constant=1.0;
    p_linear=5e-1;
    p_quadratic=0;
    PointLightPtr pointLight2 = std::make_shared<PointLight>(p_position, p_ambient, p_diffuse, p_specular, p_constant, p_linear, p_quadratic);
    PointLightRenderablePtr pointLightRenderable2 = std::make_shared<PointLightRenderable>(flatShader, pointLight2);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    pointLightRenderable2->setLocalTransform(localTransformation);
    viewer.addPointLight(pointLight2);
    viewer.addRenderable(pointLightRenderable2);

    //Define a spot light
    glm::vec3 s_position(0.0,5.0,-8.0), s_spotDirection = glm::normalize(glm::vec3(0.0,-1.0,1.0));
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.5,0.5,0.5), s_specular(0.5,0.5,0.5);
    float s_constant=1.0, s_linear=0.0, s_quadratic=0.0;
    float s_innerCutOff=std::cos(glm::radians(20.0f)), s_outerCutOff=std::cos(glm::radians(40.0f));
    SpotLightPtr spotLight = std::make_shared<SpotLight>(s_position, s_spotDirection,
                                                         s_ambient, s_diffuse, s_specular,
                                                         s_constant, s_linear, s_quadratic,
                                                         s_innerCutOff, s_outerCutOff);
    SpotLightRenderablePtr spotLightRenderable = std::make_shared<SpotLightRenderable>(flatShader, spotLight);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    spotLightRenderable->setLocalTransform(localTransformation);
    viewer.addSpotLight(spotLight);
    viewer.addRenderable(spotLightRenderable);

    //Define materials
    glm::vec3 mAmbient(0.0), mDiffuse(0.0), mSpecular(0.0);
    float mShininess=0.0;
    MaterialPtr myMaterial = std::make_shared<Material>(mAmbient, mDiffuse, mSpecular, mShininess);

    //Lighted Cube
    MaterialPtr pearl = Material::Pearl();
    LightedCubeRenderablePtr ground = std::make_shared<LightedCubeRenderable>(phongShader, false, pearl);
    globalTransformation = glm::translate( glm::mat4(1.0), glm::vec3(0.0,-1.2,0.0) );
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(20.0,0.2,20.0));
    ground->setGlobalTransform(globalTransformation);
    ground->setLocalTransform(localTransformation);
    viewer.addRenderable( ground ); */
    //Position the camera
    
    viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(5, 5, 5 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram(flatShader);
    viewer.addShaderProgram(cubeMapShader);
    viewer.addShaderProgram(phongShader);
    
    std::string cubemap_dir = "../../sfmlGraphicsPipeline/textures/skybox2";
    auto cubemap = std::make_shared<CubeMapRenderable>(cubeMapShader, cubemap_dir);

    viewer.addRenderable(cubemap);

    auto frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);
    frame->setLocalTransform(getScaleMatrix(10,10,10));

    auto mat = std::make_shared<Material>(glm::vec3(0), glm::vec3(1), glm::vec3(0), 100.0f);

    auto lighted_cube = std::make_shared<LightedCubeRenderable>(phongShader, false, mat);
    lighted_cube->setLocalTransform(getScaleMatrix(5,.2,5)*getTranslationMatrix(0,-5,0));   
    viewer.addRenderable(lighted_cube);
    
    //Define a transformation
    glm::mat4 globalTransformation, localTransformation;

    //Lighted Mesh 1
    std::string suzanne_path = "./../../sfmlGraphicsPipeline/meshes/suzanne.obj";
    MaterialPtr bronze = Material::Bronze();
    LightedMeshRenderablePtr suzanne1 = std::make_shared<LightedMeshRenderable>(phongShader, suzanne_path, bronze);
    globalTransformation = glm::translate( glm::mat4(1.0), glm::vec3(-2.0,0.0,0.0) );
    localTransformation = glm::mat4(1.0);
    suzanne1->setGlobalTransform(globalTransformation);
    suzanne1->setLocalTransform(localTransformation);
    viewer.addRenderable( suzanne1 );

    //Lighted Mesh 2
    MaterialPtr emerald = Material::Emerald();
    LightedMeshRenderablePtr suzanne2 = std::make_shared<LightedMeshRenderable>(phongShader, suzanne_path, emerald);
    globalTransformation = glm::translate( glm::mat4(1.0), glm::vec3(2.0,0.0,0.0) );
    localTransformation = glm::mat4(1.0);
    suzanne2->setGlobalTransform(globalTransformation);
    suzanne2->setLocalTransform(localTransformation);
    viewer.addRenderable( suzanne2 );

    //Moving lights
    glm::vec3 red(0.9,0.3,0.4), green(0.3,0.9,0.4), blue(0.4,0.3,0.9);
    { // Moving PointLight
        auto point_light = std::make_shared<PointLight>(glm::vec3(3,5,3), glm::vec3(0), red, glm::vec3(0), 1, 0, 0);
        viewer.addPointLight(point_light);

        point_light->addGlobalTransformKeyframe(getTranslationMatrix(3,1,3), 0);
        point_light->addGlobalTransformKeyframe(getTranslationMatrix(3,1,-3), 3);
        point_light->addGlobalTransformKeyframe(getTranslationMatrix(-3,1,-3), 6);
        point_light->addGlobalTransformKeyframe(getTranslationMatrix(-3,1,3), 9);
        point_light->addGlobalTransformKeyframe(getTranslationMatrix(3,1,3), 12);

        auto point_light_renderable = std::make_shared<PointLightRenderable>(flatShader, point_light);
        point_light_renderable->setLocalTransform(getScaleMatrix(0.3));
        viewer.addRenderable(point_light_renderable);
    }
    
    { // Moving DirectionalLight
        auto dir_light = std::make_shared<DirectionalLight>(glm::vec3(1,0,0), glm::vec3(0), green, glm::vec3(0));
        viewer.addDirectionalLight(dir_light);
        
        dir_light->addGlobalTransformKeyframe(getRotationMatrix( 0 * 2 * M_PI, 0, 1, 0)    , 0);
        dir_light->addGlobalTransformKeyframe(getRotationMatrix( 0.25 * 2 * M_PI, 0, 1, 0) , 6);
        dir_light->addGlobalTransformKeyframe(getRotationMatrix( 0.5 * 2 * M_PI, 0, 1, 0)  , 12);
        dir_light->addGlobalTransformKeyframe(getRotationMatrix( 0.75 * 2 * M_PI, 0, 1, 0) , 18);
        dir_light->addGlobalTransformKeyframe(getRotationMatrix( 1 * 2 * M_PI, 0, 1, 0)    , 24);

        auto dir_light_renderable = std::make_shared<DirectionalLightRenderable>(flatShader, dir_light);
        dir_light_renderable->setLocalTransform(getTranslationMatrix(0, 1.5, 3) * getScaleMatrix(0.5));

        viewer.addRenderable(dir_light_renderable);
    }
    
    { // Moving SpotLight
        auto spot_light = std::make_shared<SpotLight>(glm::vec3(3,5,3), glm::vec3(-1,-1,-1), glm::vec3(0), blue, glm::vec3(0), 1, 0, 0, 0.98, 0.92);
        viewer.addSpotLight(spot_light);

        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(3,5,3), glm::vec3(0), Light::base_forward), 0);
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(-3,5,3), glm::vec3(0), Light::base_forward), 3);
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(-3,5,-3), glm::vec3(0), Light::base_forward), 6);
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(3,5,-3), glm::vec3(0), Light::base_forward), 9);
        spot_light->addGlobalTransformKeyframe(lookAtModel(glm::vec3(3,5,3), glm::vec3(0), Light::base_forward), 12);

        auto spot_light_renderable = std::make_shared<SpotLightRenderable>(flatShader, spot_light);
        viewer.addRenderable(spot_light_renderable);
    }


    //viewer.startAnimation();
    viewer.startAnimation();
    //viewer.setAnimationLoop(true, 4.0);
}

int main() 
{
	Viewer viewer(1280,720, glm::vec4(0.8,0.8,0.8,1));
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
