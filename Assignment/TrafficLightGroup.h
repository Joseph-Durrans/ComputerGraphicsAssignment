#pragma once

#include <windows.h>

#include <TrafficSystem/TrafficLightFacade.h>
#include <osg/Group>
#include "LightControl.h"

namespace Assignment {


	typedef std::list<LightControl*> GroupLights;

	class TrafficLightGroup
	{
	public:	
		TrafficLightGroup();
		virtual ~TrafficLightGroup();

		void addLight(LightControl* pCTF);

		osg::Group* root();

		void setState(LightControl::LightState eState);

	protected:
		osg::Group* m_pRoot;
		GroupLights m_lLights;
	};

}
