#include "RotationMaterial.h"

RotationMaterial::RotationMaterial( float Pangle) {
    angle = Pangle;
}

void RotationMaterial::render(Node *o) {}

void RotationMaterial::animate(Node *o, const float elapsedTime) {
    //  float angleChangee = glm::radians(0.5f);

    o->frame()->rotate(glm::vec3(0.0, 1.0, 0.0) ,angle);
}