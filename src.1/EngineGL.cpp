    
#include "EngineGL.h"
#include "Scene.h"
#include "Materials/RotationMaterial/RotationMaterial.h"
#include "Materials/PhongMaterial/PhongMaterial.h"


#include "Materials/BaseMaterial/BaseMaterial.h"

#include "Texture2D.h"

bool EngineGL::init() {
    LOG_INFO << "Initializing Scene" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, m_Width, m_Height);
    setClearColor(glm::vec4(0.5, 0.5, 0.5, 1.0));

    // TP 2 *******************************************

    // Création d'un materiau de Base
    //BaseMaterial *material = new BaseMaterial("IMN401-TP2");
    PhongMaterial *material_Du_Phong = new PhongMaterial("IMN401-TP2");

    // d'un objet, méthode détaillée
    Node *bunny = scene->getNode("Bunny");
    bunny->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Bunny.obj"));
    bunny->frame()->scale(glm::vec3(30.0));
    bunny->setMaterial(material_Du_Phong);
    scene->getSceneNode()->adopt(bunny);

  //Animations et autres objets
    RotationMaterial *r = new RotationMaterial(glm::radians(15.0/60));
    A_node = Scene::getInstance()->getNode("Noeud A"); // 1.creer un noeud vide A
    A_node->setMaterial(r);                 // assigner un materiau
    bunny->adopt(A_node);


  //Une lumière indépendante
     BaseMaterial *materialLumiere= new BaseMaterial("Materiau-Lumiere");
     noeudLumiere = Scene::getInstance()->getNode("Lumiere");                                    // crée ou recupere le noeud Lumiere
      noeudLumiere->setModel(Scene::getInstance()->m_Models.get<ModelGL>(ObjPath + "Sphere.obj"));  //assigner la goemetrie sphere au noeudLumiere
      noeudLumiere->frame()->scale(glm::vec3(0.8));
      noeudLumiere->setMaterial(materialLumiere);  //assigner un materiau 
      noeudLumiere->frame()->translate(glm::vec3(3.0, 1.0, 2.0));   //Positionner la lumière quelque part dans la scène
      A_node->adopt(noeudLumiere);


    
    PhongMaterial *material_Du_Sol = new PhongMaterial("IMN401-TP2"); 
    noeudSol = Scene::getInstance()->getNode("Sol"); 
    noeudSol->setModel(Scene::getInstance()->m_Models.get<ModelGL>(ObjPath + "wall.obj")); // assigner la goemetrie sphere au noeudLumiere
    noeudSol->frame()->scale(glm::vec3(10.0));
    noeudSol->setMaterial(material_Du_Sol); 
    noeudSol->frame()->translate(glm::vec3(0.0, -5.0/10.0, 0.0));
    scene->getSceneNode()->adopt(noeudSol);
    

      
    /*
    // 2.Creer d'un materiau pour l'Objet L
      BaseMaterial *sphereMaterial = new BaseMaterial("IMN401-TP2");
      //creation de l"objet objet L
      L_sphere = Scene::getInstance()->getNode("Sphere");
      L_sphere->setModel(Scene::getInstance()->m_Models.get<ModelGL>(ObjPath + "Sphere.obj"));
      L_sphere->frame()->scale(glm::vec3(0.2));
      L_sphere->setMaterial(sphereMaterial);

   //3.effectuer une translation de 0.2 le long de l'axe X du noeud L_sphere
      L_sphere->frame()->translate(glm::vec3(1.9, 0.0, 0.0));
   
  // 4.attacher le noeud L_sphere au repere A
    A_node->adopt(L_sphere);
    */

    setupEngine();
    return (true);
}

void EngineGL::render() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (unsigned int i = 0; i < allNodes->nodes.size(); i++)
        allNodes->nodes[i]->render();
}

void EngineGL::animate(const float elapsedTime) {
    // Animate each node


    for (unsigned int i = 0; i < allNodes->nodes.size(); i++) 
    {
        allNodes->nodes[i]->animate(elapsedTime);
    }

    
    // Update Camera Buffer
    scene->camera()->updateBuffer();  
    
}

void EngineGL::onWindowResize(int w, int h) {
    m_Width = w;
    m_Height = h;
    glViewport(0, 0, w, h);
    float ratio = (float)w / (float)h;

    scene->resizeViewport(w, h);
    scene->camera()->setPerspectiveProjection(glm::radians(45.0f), ratio, 1.0f, 2000.0f);
}

void EngineGL::setClearColor(glm::vec4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void EngineGL::displayInterface() {
    if (myFBO) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("FBOs")) {
                ImGui::MenuItem(myFBO->getName().c_str(), NULL, &(myFBO->show_interface));
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        myFBO->displayInterface();
    }
}

// Message callbck error for getting OpenGL problems
// All credits to https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions/blob/master/README.md#gltexture
void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const *message, void const *user_param) {
    auto const src_str = [source]() {
        switch (source) {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        default: return "";
        }
    }();

    auto const type_str = [type]() {
        switch (type) {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        default: return "";
        }
    }();

    auto const severity_str = [severity]() {
        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        default: return "";
        }
    }();
    LOG_INFO << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

EngineGL::EngineGL(int width, int height) {
    m_Width = width;
    m_Height = height;

    myFBO = NULL;

    scene = Scene::getInstance();
    scene->resizeViewport(m_Width, m_Height);
}

EngineGL::~EngineGL() {}

void EngineGL::setupEngine() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(message_callback, nullptr);

    this->allNodes = new NodeCollector();
    allNodes->collect(scene->getRoot());

    LOG_INFO << "initialisation complete" << std::endl;
}