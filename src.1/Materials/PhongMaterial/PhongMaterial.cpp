#include "PhongMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

PhongMaterial::PhongMaterial(std::string name) : MaterialGL(name) {

    vp = new GLProgram(MaterialPath + "PhongMaterial/Main-PhongVS.glsl", GL_VERTEX_SHADER);
    fp = new GLProgram(MaterialPath + "PhongMaterial/Main-PhongFS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fp, GL_FRAGMENT_SHADER_BIT);

    l_View = glGetUniformLocation(vp->getId(), "View");
    l_Proj = glGetUniformLocation(vp->getId(), "Proj");
    l_Model = glGetUniformLocation(vp->getId(), "Model");

    //Eclairage diffus,ambiant, speculaire
    l_couleurObjet = glGetUniformLocation(fp->getId(), "Couleur_Objet");
    l_couleurLumiere = glGetUniformLocation(fp->getId(), "Couleur_Lumiere");
    l_couleurAmbiant = glGetUniformLocation(fp->getId(), "Couleur_Ambiant");

    l_ka = glGetUniformLocation(fp->getId(), "Ka");
    l_kd = glGetUniformLocation(fp->getId(), "Kd");
    l_ks = glGetUniformLocation(fp->getId(), "Ks");
  
    l_posLum = glGetUniformLocation(vp->getId(), "posLum"); // Independance lumiere objet
    l_posCam = glGetUniformLocation(vp->getId(), "posCam");  // Éclairage spéculaire
   
  
}
PhongMaterial::~PhongMaterial() {}

void PhongMaterial::render(Node *o) {
    m_ProgramPipeline->bind();

    o->drawGeometry(GL_TRIANGLES);
    m_ProgramPipeline->release();
}

void PhongMaterial::animate(Node *o, const float elapsedTime) {
   
  glm::mat4 mat_model = o->frame()->getModelMatrix(); 
  glm::mat4 mat_proj = Scene::getInstance()->camera()->getProjectionMatrix();
  glm::mat4 mat_view = Scene::getInstance()->camera()->getViewMatrix();

  //envoyer ces valeurs au vertex chaders
  glProgramUniformMatrix4fv(vp->getId(), l_Model, 1, GL_FALSE, glm::value_ptr(mat_model));
  glProgramUniformMatrix4fv(vp->getId(), l_View, 1, GL_FALSE, glm::value_ptr(mat_view));
  glProgramUniformMatrix4fv(vp->getId(), l_Proj, 1, GL_FALSE, glm::value_ptr(mat_proj));


  // Eclairage diffus et ambiant
  glm::vec3 couleurObj(0.8, 0.4, 0.5);
  glm::vec3 couleurLumiere(1.0, 1.0, 1.0);
  glm::vec3 couleurAmbiant(0.4, 0.8, 0.5);
  glProgramUniform3fv(fp->getId(), l_couleurObjet,1, glm::value_ptr(couleurObj));
  glProgramUniform3fv(fp->getId(), l_couleurLumiere, 1, glm::value_ptr(couleurLumiere));
  glProgramUniform3fv(fp->getId(), l_couleurAmbiant, 1, glm::value_ptr(couleurAmbiant));


  float ka = 0.4f;
  float kd = 0.5f;
  float ks = 0.6f;
  glProgramUniform1f(fp->getId(), l_ka, ka);
  glProgramUniform1f(fp->getId(), l_kd, kd);
  glProgramUniform1f(fp->getId(), l_ks, ks);
  
  //Independance lumiere objet
  //1.declarer le point p
  glm::vec3 P_lumiere(0.0, 0.0, 0.0);  
  //1.Recuperer le noeud lumiere depuis la classe Scene
  Node *noeudLumiere = Scene::getInstance()->getNode("Lumiere");
  Frame *repere_Lumiere = noeudLumiere->frame();
  Frame* repere_De_objet_o = o->frame();
  glm::vec3 pos_lumiere_object = repere_De_objet_o->convertPtFrom(P_lumiere, repere_Lumiere);

  //3.envoyer le point
  glProgramUniform3fv(vp->getId(), l_posLum, 1, glm::value_ptr(pos_lumiere_object));


  //Eclairage speculaire
  glm::vec3 p_camera(0.0, 0.0, 0.0); //point dans le repere camera
  Frame *repere_pCamera = Scene::getInstance()->camera()->frame();
  glm::vec3 pos_camera_object = repere_De_objet_o->convertPtFrom(p_camera, repere_pCamera);
  glProgramUniform3fv(vp->getId(), l_posCam, 1, glm::value_ptr(pos_camera_object));


}




  /*
glProgramUniform[nombre][type](program, location, count, valeur);
 f = float ,i = int , ui = unsigned int, fv = float vector (tableau), Matrix = pour les matrices
 glProgramUniform Matrix 4 f v = "OpenGL Program Uniform MATRIX 4x4 FLOAT VECTOR"

 glProgramUniform 3 f v = "OpenGL Program Uniform VECTOR 3 FLOAT VECTOR"

 glProgramUniform 1 f = "OpenGL Program Uniform SINGLE FLOAT"

*/