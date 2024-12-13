#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <Utils.hpp>
#include <Io.hpp>

#include <CylinderMeshRenderable.hpp>
#include <MeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>

#include <lighting/SpotLightRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <lighting/LightedCubeRenderable.hpp>

#include <texturing/CubeMapRenderable.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <texturing/TexturedCubeRenderable.hpp>

#include <dynamics/DynamicSystem.hpp>
#include <dynamics/EulerExplicitSolver.hpp>
#include <dynamics/DynamicSystemRenderable.hpp>
#include <dynamics/ConstantForceField.hpp>
#include <dynamics/ParticleRenderable.hpp>

void initialize_scene( Viewer& viewer )
{
    // In this scene, we will see the conductor's view of the train, 
    // first going straight (into the three penguins), then turning left (into the other penguin)
    // then turning red (particles)
    
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram( texShader );

	//Define a shader that encode an illumination model
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>( "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl", 
                                                                    "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    viewer.addShaderProgram( phongShader );

    //Particle Shader
    ShaderProgramPtr particleShader = std::make_shared<ShaderProgram>( "../../sfmlGraphicsPipeline/shaders/billboardVertex.glsl",
                                                                        "../../sfmlGraphicsPipeline/shaders/billboardFragment.glsl");

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    system->setSolver(solver);
    system->setDt(0.01);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    //It is also responsible for some of the key/mouse events
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);

    //Deactivate collision detection
    system->setCollisionsDetection(false);

    glm::vec3 px,pv;
    float pm, pr;

    {
        //Initialize a particle with position, velocity, mass and radius and add it to the system
        px = glm::vec3(0.0,1.0,0.0);
        pv = glm::vec3(3.0,0.0,0.0);
        pr = 0.1;
        pm = 1.0;
        ParticlePtr particle1 = std::make_shared<Particle>( px, pv, pm, pr);
        system->addParticle( particle1 );

        px = glm::vec3(0.0,1.5,0.0);
        pv = glm::vec3(6.0,0.0,0.0);
        pr = 0.1;
        pm = 1.0;
        ParticlePtr particle2 = std::make_shared<Particle>( px, pv, pm, pr);
        system->addParticle( particle2 );

        //Initialize a force field that apply to all the particles of the system to simulate gravity
        //Add it to the system as a force field
        ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(system->getParticles(), DynamicSystem::gravity );
        system->addForceField( gravityForceField );

        //Create a particleRenderable for each particle of the system
        //DynamicSystemRenderable act as a hierarchical renderable
        //This which allows to easily apply transformation on the visualiazation of a dynamicSystem
        ParticleRenderablePtr particleRenderable1 = std::make_shared<ParticleRenderable>(particleShader, particle1);
        HierarchicalRenderable::addChild( systemRenderable, particleRenderable1 );
        ParticleRenderablePtr particleRenderable2 = std::make_shared<ParticleRenderable>(particleShader, particle2);
        HierarchicalRenderable::addChild( systemRenderable, particleRenderable2 );
    }

    
    //Lights
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


    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    viewer.addShaderProgram(cubeMapShader);

    std::string cubemap_dir = "../../models3D/ciel";
    auto cubemap = std::make_shared<CubeMapRenderable>(cubeMapShader, cubemap_dir);
    viewer.addRenderable(cubemap);

    // Rails
    const std::string rail_path = "../../models3D/rail/rail.obj";
    std::string rail_texture_path = "../../models3D/rail/woodAndMetal.jpg";


    std::vector<std::vector<glm::vec3>> all_positions1;
    std::vector<std::vector<glm::vec3>> all_normals1;
    std::vector<std::vector<glm::vec2>> all_texcoords1;
    std::vector<MaterialPtr> materials1;


    read_obj_with_materials(rail_path, "../../models3D/rail/", all_positions1, all_normals1, all_texcoords1, materials1);
    
    //Rails avant ambranchement
    TexturedLightedMeshRenderablePtr railo = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
    railo->setLocalTransform(getScaleMatrix(1,1,1)*getTranslationMatrix(0,0,-3.6*2));
    
    for (int i = -20; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
        rail->setLocalTransform(getScaleMatrix(1,1,1)*getTranslationMatrix(0,0,-3.6*i));
        HierarchicalRenderable::addChild(railo, rail);
    }
    railo->setGlobalTransform(getScaleMatrix(2,2,2)*getRotationMatrix(M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-0.5,-2,-20));
    viewer.addRenderable(railo);

    //Rails gauche après ambranchement
    TexturedLightedMeshRenderablePtr railo2 = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
    railo2->setGlobalTransform(getScaleMatrix(2,2,2)*getRotationMatrix(-M_PI*0.35, glm::vec3(0,1,0))*getTranslationMatrix(1.5,-2,0));
    viewer.addRenderable(railo2);

    for (int i = 1; i < 25; i++) {
        TexturedLightedMeshRenderablePtr rail = std::make_shared<TexturedLightedMeshRenderable>(texShader, rail_path, materials1[0], rail_texture_path);
        rail->setLocalTransform(getScaleMatrix(1,1,1)*getTranslationMatrix(0,0,3.6*i));
        HierarchicalRenderable::addChild(railo2, rail);
    }

    auto tcube = std::make_shared<TexturedCubeRenderable>(texShader, "../../models3D/sable.jpg");
    viewer.addRenderable(tcube);
    tcube->setLocalTransform(getTranslationMatrix(0,-5,0)*getScaleMatrix(1000,1,1000));
    tcube->setGlobalTransform(getRotationMatrix(M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-40,0,-1));

    HierarchicalRenderable::addChild(tcube, railo2);
    HierarchicalRenderable::addChild(tcube, railo);

    //deplacement de la scène, pour illusion de mouvement du train
    tcube->addGlobalTransformKeyframe(getRotationMatrix(M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-40,0,-1), 0.0);
    tcube->addGlobalTransformKeyframe(getRotationMatrix(M_PI*0.5, glm::vec3(0,1,0))*getTranslationMatrix(-1,0,-1), 4.95);
    tcube->addGlobalTransformKeyframe(getRotationMatrix(M_PI*0.35, glm::vec3(0,1,0))*getTranslationMatrix(-1,0,-3), 5.05);
    tcube->addGlobalTransformKeyframe(getRotationMatrix(M_PI*0.35, glm::vec3(0,1,0))*getTranslationMatrix(30.6,0,-19), 10.0);



    //train
    const std::string traing_path = "../../models3D/un_oldTrain.obj";
    const std::string train_texture_path = "../../models3D/metal.jpg";
    std::vector<std::vector<glm::vec3>> all_positions;
    std::vector<std::vector<glm::vec3>> all_normals;
    std::vector<std::vector<glm::vec2>> all_texcoords;
    std::vector<MaterialPtr> materials;

    read_obj_with_materials(traing_path, "../../models3D/", all_positions, all_normals, all_texcoords, materials);
    TexturedLightedMeshRenderablePtr train = std::make_shared<TexturedLightedMeshRenderable>(texShader, traing_path, materials[0], train_texture_path);
    viewer.addRenderable(train);
    train->setGlobalTransform(getRotationMatrix(M_PI, glm::vec3(0,1,0))*getTranslationMatrix(0, 0, 10.5)*getScaleMatrix(1,1,1));

    viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(0, 0, -4), glm::vec3(0, -3, -1), glm::vec3( 0, 1, 0 ) ) );
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