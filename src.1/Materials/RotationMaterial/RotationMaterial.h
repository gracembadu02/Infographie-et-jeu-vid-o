#include "EngineGL.h"
#include "Node.h"
#include "MaterialGL.h"

#ifndef _RotationMaterial_
#define _RotationMaterial_

class RotationMaterial : public MaterialGL{
public:
    RotationMaterial(float);
    virtual void  render(Node *o);

    virtual void animate(Node *o, const float elapsedTime);

    

protected:
    float angle;

};
#endif