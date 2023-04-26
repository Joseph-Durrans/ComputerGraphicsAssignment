#include "ControllableTrafficLightFacade.h"

#include "TrafficLightGroup.h"


void Assignment::TrafficLightGroup::addLight(LightControl* pCTL)
{

	if(pCTL)
	{
		m_pRoot->addChild(pCTL->rootNode());
		m_lLights.push_back(pCTL);
	}
}

osg::Group* Assignment::TrafficLightGroup::root()
{
	return m_pRoot;
}

void Assignment::TrafficLightGroup::setState(LightControl::LightState eState)
{
	for (GroupLights::iterator it = m_lLights.begin(); it != m_lLights.end(); it++)
		(*it)->setState(eState);
}

Assignment::TrafficLightGroup::TrafficLightGroup(): m_pRoot(new osg::Group())
{
}

Assignment::TrafficLightGroup::~TrafficLightGroup()
{
}




