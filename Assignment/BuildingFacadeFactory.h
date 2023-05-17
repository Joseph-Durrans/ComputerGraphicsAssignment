#pragma once

#include <Common\FacadeFactoryA.h>

namespace Assignment
{
	class BuildingFacadeFactory : public Common::FacadeFactoryA
	{
	public:
		BuildingFacadeFactory();
		virtual ~BuildingFacadeFactory();

		// Inherited via FacadeFactoryA
		virtual Common::Facade* create(std::string sName, osg::Node* pModelRoot, osg::Matrixf rTransform, bool bVisible) override;
	};
}
