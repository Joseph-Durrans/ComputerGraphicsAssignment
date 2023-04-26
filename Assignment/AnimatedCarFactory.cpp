#include "AnimatedCarFactory.h"
#include "AnimatedCar.h"

Assignment::AnimatedCarFactory::AnimatedCarFactory()
{
}

Assignment::AnimatedCarFactory::~AnimatedCarFactory()
{
}

Common::Facade* Assignment::AnimatedCarFactory::create(std::string sName, osg::Node* pModelRoot, osg::Matrixf rTransform, bool bVisible)
{
    return new AnimatedCar(sName, pModelRoot, rTransform, bVisible);
}
