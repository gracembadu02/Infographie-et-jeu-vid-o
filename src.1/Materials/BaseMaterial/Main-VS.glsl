#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout(location = 0) in vec3 Position;

//visualiser la normale
out vec3 couleur;
layout(location = 2) in vec3 Normale;

//Déformation de chaque position du sommet lo long de la normale;
uniform float Temps;

void main() 
{       
    // Calculer la valeur de déformation
     //float facteur_scalaire = 0.1 * sin((-0.1) * Temps);

    // Déformer la position en la déplaçant le long de la normale
     //vec3 valeur_Deformee_longNormale = Position + facteur_scalaire * Normale;  

    //position finale de chaque sommet dans l'espace image
    gl_Position = Proj * View * Model * vec4(Position, 1.0);
    
    couleur = abs(Normale);
}