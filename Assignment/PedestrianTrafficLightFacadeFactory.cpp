#include "PedestrianTrafficLightFacade.h"
#include "PedestrianTrafficLightFacadeFactory.h"

Assignment::PedestrianTrafficLightFacadeFactory::PedestrianTrafficLightFacadeFactory()
{
}

Assignment::PedestrianTrafficLightFacadeFactory::~PedestrianTrafficLightFacadeFactory()
{
}

Common::Facade* Assignment::PedestrianTrafficLightFacadeFactory::create(std::string sName, osg::Node* pModelRoot, osg::Matrixf rTransform, bool bVisible)
{
	return new PedestrianTrafficLightFacade(sName, pModelRoot, rTransform, bVisible);
}
