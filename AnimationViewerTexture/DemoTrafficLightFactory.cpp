#include "DemoTrafficLight.h"
#include "DemoTrafficLightFactory.h"

AnimationViewerTexture::DemoTrafficLightFactory::DemoTrafficLightFactory()
{
}

AnimationViewerTexture::DemoTrafficLightFactory::~DemoTrafficLightFactory()
{
}

Common::Facade* AnimationViewerTexture::DemoTrafficLightFactory::create(std::string sName, osg::Node* pModelRoot, osg::Matrixf rTransform, bool bVisible)
{
	return new DemoTrafficLight(sName, pModelRoot, rTransform, bVisible);
}
