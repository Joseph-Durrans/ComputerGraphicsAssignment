#pragma once

#include <Windows.h>
#include <osg/Node>

namespace Assignment
{
	class LightControl
	{
	public:



		enum LightState
		{
			STOP,
			READY,
			GO,
			SLOW,
			ALL,
			NONE,
		};

		virtual void setState(LightState eState) = 0;
		LightState getState();
		virtual osg::Node* rootNode()=0;

	protected:
		LightState m_eState;
	};
}
