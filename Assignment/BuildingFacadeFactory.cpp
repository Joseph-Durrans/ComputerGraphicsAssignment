#include "BuildingFacadeFactory.h"
#include "BuildingFacade.h"

Assignment::BuildingFacadeFactory::BuildingFacadeFactory()
{
}

Assignment::BuildingFacadeFactory::~BuildingFacadeFactory()
{
}

Common::Facade* Assignment::BuildingFacadeFactory::create(std::string sName, osg::Node* pModelRoot, osg::Matrixf rTransform, bool bVisible)
{
	return new BuildingFacade(sName, pModelRoot, rTransform, bVisible);
}
