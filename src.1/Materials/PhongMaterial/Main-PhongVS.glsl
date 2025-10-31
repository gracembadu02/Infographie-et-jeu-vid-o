#version 460
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

//Independance lumiere objet
uniform vec3 posLum;

//Éclairage spéculaire
uniform vec3 posCam;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 normale;

out vec3 frag_L;
out vec3 frag_V;
out vec3 frag_N;


void main() 
{    
  
    
 //Eclairage diffus et ambiant
 //1.calcul du vecteur representnt la direction de la lumiere ayant une position fixe
 vec3 L = (posLum - Position);

 //vecteur direction V qui pointe du sommet actuel  vers la position de la caméra
 vec3 V = (posCam - Position);

 //position finale de chaque sommet dans l'espace image
 gl_Position = Proj * View * Model * vec4(Position, 1.0);
 
 frag_L = L;
 frag_V = V;
 frag_N = normale;
  
}                                                                             




