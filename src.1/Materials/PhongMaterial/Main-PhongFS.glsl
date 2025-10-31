#version 460

layout(location = 0) out vec4 Color;

//Eclairage diffus et ambiant
uniform vec3 Couleur_Objet;
uniform vec3 Couleur_Ambiant;
uniform vec3 Couleur_Lumiere;

uniform float Kd;
uniform float Ka;
uniform float Ks;

in vec3 frag_L;
in vec3 frag_V;
in vec3 frag_N;

void main() 
{
  vec3 l= normalize( frag_L );
  vec3 v = normalize(frag_V);
  vec3 n = normalize(frag_N);

 float intensite_diffuse = Kd * max(dot(n , l), 0.0); //max est utilisée pour eviter les contributions qui arrivent derriere 
 
 vec3 terme_diffus = Couleur_Objet * intensite_diffuse *  Couleur_Lumiere;

 vec3 terme_ambiant =  Couleur_Ambiant * Ka;

 //direction de reflexion ideale 
 // reflect() prend la direction INCIDENTE (-L) et calcule la direction de réflexion selon N
 vec3 R = reflect(- l, n);

 int s = 128;
 float facteur_scalaire =  pow(max(dot(R,v), 0.0) ,s);
 float intenste_speculaire = Ks * facteur_scalaire;
 vec3 terme_speculaire = intenste_speculaire * Couleur_Lumiere;

 vec3 couleur_finale= terme_ambiant + terme_diffus + terme_speculaire;

  Color = vec4(couleur_finale, 1.0);
} 