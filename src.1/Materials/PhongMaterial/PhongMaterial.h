
#ifndef _PhongMaterial_
#define _PhongMaterial_

#include "MaterialGL.h"

class PhongMaterial : public MaterialGL {
public:
    PhongMaterial(std::string name = "");

    ~PhongMaterial();

    virtual void render(Node *o);

    virtual void animate(Node *o, const float elapsedTime);

    virtual void displayInterface(){};

protected:
    GLProgram *vp;
    GLProgram *fp;

    GLuint l_View, l_Proj, l_Model; // location of uniforms

    //Eclairage diffus et ambiant
    GLuint l_ks, l_kd, l_ka; //parametres pour le modele phong 
    
    //Independance lumiere objet
    GLuint l_posLum;  //position 
     
    //Éclairage spéculaire
    GLuint l_posCam; // position de la camera
    GLuint l_couleurLumiere ,l_couleurObjet, l_couleurAmbiant;
};

#endif