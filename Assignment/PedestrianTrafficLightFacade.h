#pragma once

#include <windows.h>
#include <osg/Material>

#include <TrafficSystem/TrafficLightFacade.h>

#include "LightControl.h"

//#define WALK_SHADER

namespace Assignment {
	class PedestrianTrafficLightFacade : public TrafficSystem::TrafficLightFacade, public osg::Callback, public Assignment::LightControl
	{
	public:
		/* enum TLState
		{
			Stop,
			Go,
		}; */

		PedestrianTrafficLightFacade(std::string sName, osg::Node* pAsset, osg::Matrixf m, bool bVisible);
		virtual ~PedestrianTrafficLightFacade();

		virtual osg::Vec3f getFacadeCollisionPoint() override;

		void setState(LightState eState);
		osg::Node* rootNode();


		void toggleState();

#ifdef WALK_SHADER
		virtual bool run(osg::Object* object, osg::Object* data);
#endif

	protected:
		osg::MatrixTransform* m_pSelectTransform;
		//TLState m_eState;


	private:
		static osg::Material* sm_pStopMaterial;
		static osg::Material* sm_pGoMaterial;

		osg::Geode* m_pRedLamp;
		osg::Geode* m_pAmberLamp;
		osg::Geode* m_pGreenLamp;

#ifdef WALK_SHADER
		osg::Uniform* m_pWalkTimeUniform;
		osg::Uniform* m_pWalkStateUniform;
#else
		osg::Switch* m_pTextureSwitch;
#endif
	};
}

