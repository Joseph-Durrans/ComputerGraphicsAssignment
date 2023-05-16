#pragma once

#include <Common/FacadeFactoryA.h>

namespace Assignment {
	class PedestrianTrafficLightFacadeFactory : public Common::FacadeFactoryA
	{
	public:
		PedestrianTrafficLightFacadeFactory();
		virtual ~PedestrianTrafficLightFacadeFactory();

		virtual Common::Facade* create(std::string sName, osg::Node* pModelRoot, osg::Matrixf rTransform, bool bVisible) override;
	};
}

