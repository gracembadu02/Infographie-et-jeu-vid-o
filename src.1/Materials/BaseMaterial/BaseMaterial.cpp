
#include "BaseMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

BaseMaterial::BaseMaterial(std::string name) : MaterialGL(name) {

    vp = new GLProgram(MaterialPath + "BaseMaterial/Main-VS.glsl", GL_VERTEX_SHADER);
    fp = new GLProgram(MaterialPath + "BaseMaterial/Main-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fp, GL_FRAGMENT_SHADER_BIT);

    l_View = glGetUniformLocation(vp->getId(), "View");
    l_Proj = glGetUniformLocation(vp->getId(), "Proj");
    l_Model = glGetUniformLocation(vp->getId(), "Model");

    // deformation
    l_Temps = glGetUniformLocation(vp->getId(), "Temps");

}
BaseMaterial::~BaseMaterial() {}

void BaseMaterial::render(Node *o) {

    m_ProgramPipeline->bind();

    o->drawGeometry(GL_TRIANGLES);
    m_ProgramPipeline->release();
}

void BaseMaterial::animate(Node *o, const float elapsedTime)
{
    //Premier Affichage
    //obtenir la matrice model
    glm::mat4 gl_model = o->frame()->getModelMatrix();

    //obtenir la matrice view
    glm::mat4 gl_view = Scene::getInstance()->camera()->getViewMatrix();

    //obtenir la maatrice proj
    glm::mat4 gl_proj = Scene::getInstance()->camera()->getProjectionMatrix();

    //transmet au progrma vs a l'adresse l_Modle une seule matrice de valeur gl_model
    glProgramUniformMatrix4fv(vp->getId(),l_Model, 1, GL_FALSE, glm::value_ptr(gl_model));
    glProgramUniformMatrix4fv(vp->getId(), l_View, 1, GL_FALSE, glm::value_ptr(gl_view));
    glProgramUniformMatrix4fv(vp->getId(), l_Proj, 1, GL_FALSE, glm::value_ptr(gl_proj));

    //Deformation
    glProgramUniform1f(vp->getId(), l_Temps,elapsedTime);

}



































/**********************************************
    TP 2 - A completer
    Calculer et Transmettre les matrices Model View et Proj au shaders
    - Utilisez glm::value_ptr(mat) pour trouver le pointeur de la matrice mat a transmettre au GPU via la fonction glProgramUniform*()
    - Une matrice 4X4 se transmet grace a glProgramUniformMatrix4fv
    ***********************************************/