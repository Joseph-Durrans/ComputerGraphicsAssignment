#pragma once

#include <windows.h>
#include <osg/Material>

#include "LightControl.h"

#include <TrafficSystem/TrafficLightFacade.h>

namespace Assignment
{
	class ControllableTrafficLightFacade: public TrafficSystem::TrafficLightFacade, public LightControl
	{
	public:
		ControllableTrafficLightFacade(std::string sname, osg::Node* pAsset, osg::Matrixf m, bool bVisible);
		virtual ~ControllableTrafficLightFacade();

		virtual osg::Vec3f getFacadeCollisionPoint() override;

		virtual void setState(LightControl::LightState eState);
	protected:
		static osg::Material* sm_pRedOn;
		static osg::Material* sm_pGreenOn;
		static osg::Material* sm_pAmberOn;
		static osg::Material* sm_pRedOff;
		static osg::Material* sm_pGreenOff;
		static osg::Material* sm_pAmberOff;

		osg::Geode* m_pAmber;
		osg::Geode* m_pRed;
		osg::Geode* m_pGreen;

		osg::Vec3f m_WorldCollisionTarget;


		// Inherited via LightControl
		virtual osg::Node* rootNode() override;
	};
}
