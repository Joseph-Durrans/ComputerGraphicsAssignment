#include "DemoAnimatedCarFactory.h"
#include "DemoAnimatedCar.h"

AnimationViewerTexture::DemoAnimatedCarFactory::DemoAnimatedCarFactory()
{
}

AnimationViewerTexture::DemoAnimatedCarFactory::~DemoAnimatedCarFactory()
{
}

Common::Facade* AnimationViewerTexture::DemoAnimatedCarFactory::create(std::string sName, osg::Node* pModelRoot, osg::Matrixf rTransform, bool bVisible)
{
    return new DemoAnimatedCar(sName, pModelRoot, rTransform, bVisible);
}
