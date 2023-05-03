#include <Windows.h>
#include <osg/Geode>

#include "LightControl.h"
#include "ControllableTrafficLightFacade.h"

#include <Common/NodeFinderT.h>
#include <iostream>

osg::Material* Assignment::ControllableTrafficLightFacade::sm_pRedOn = 0;
osg::Material* Assignment::ControllableTrafficLightFacade::sm_pGreenOn = 0;
osg::Material* Assignment::ControllableTrafficLightFacade::sm_pAmberOn = 0;
osg::Material* Assignment::ControllableTrafficLightFacade::sm_pRedOff = 0;
osg::Material* Assignment::ControllableTrafficLightFacade::sm_pGreenOff = 0;
osg::Material* Assignment::ControllableTrafficLightFacade::sm_pAmberOff = 0;


Assignment::ControllableTrafficLightFacade::ControllableTrafficLightFacade(std::string sname, osg::Node* pAsset, osg::Matrixf m, bool bVisible): TrafficLightFacade(sname, pAsset, m, bVisible), m_pAmber(0), m_pRed(0), m_pGreen(0)
{
	if (m_pCollisionTargetSwitch)
	{
		m_pTransformation->addChild(m_pCollisionTargetSwitch);
		m_pCollisionTargetSwitch->getParent(0)->removeChild(m_pCollisionTargetSwitch);
	}

	// set the position of the collision target - in this case to the local (0,0,0)
	m_pCollisionTarget->setMatrix(osg::Matrixf::identity());

	if (!sm_pRedOn)
	{
		sm_pRedOn = new osg::Material();
		sm_pRedOn->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.2f, 0.0f, 0.0f, 1.0f));
		sm_pRedOn->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.7f, 0.0f, 0.0f, 1.0f));
		sm_pRedOn->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.6f, 0.0f, 0.0f, 1.0f));
	}
	sm_pRedOn->ref();

	if (!sm_pRedOff)
	{
		sm_pRedOff = new osg::Material();
		sm_pRedOff->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.1f, 0.0f, 0.0f, 1.0f));
		sm_pRedOff->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.2f, 0.0f, 0.0f, 1.0f));
		sm_pRedOff->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.1f, 0.0f, 0.0f, 1.0f));
	}
	sm_pRedOff->ref();

	if (!sm_pGreenOn)
	{
		sm_pGreenOn = new osg::Material();
		sm_pGreenOn->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.0f, 2.0f, 0.0f, 1.0f));
		sm_pGreenOn->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.0f, 7.0f, 0.0f, 1.0f));
		sm_pGreenOn->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.0f, 6.0f, 0.0f, 1.0f));
	}
	sm_pGreenOn->ref();

	if (!sm_pGreenOff)
	{
		sm_pGreenOff = new osg::Material();
		sm_pGreenOff->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.0f, 0.1f, 0.0f, 1.0f));
		sm_pGreenOff->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.0f, 0.2f, 0.0f, 1.0f));
		sm_pGreenOff->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.0f, 0.1f, 0.0f, 1.0f));
	}
	sm_pGreenOff->ref();

	if (!sm_pAmberOn)
	{
		sm_pAmberOn = new osg::Material();
		sm_pAmberOn->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.2f, 2.0f, 0.0f, 1.0f));
		sm_pAmberOn->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.7f, 7.0f, 0.0f, 1.0f));
		sm_pAmberOn->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.6f, 6.0f, 0.0f, 1.0f));
	}
	sm_pAmberOn->ref();

	if (!sm_pAmberOff)
	{
		sm_pAmberOff = new osg::Material();
		sm_pAmberOff->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.1f, 0.1f, 0.0f, 1.0f));
		sm_pAmberOff->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.2f, 0.2f, 0.0f, 1.0f));
		sm_pAmberOff->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4f(0.1f, 0.1f, 0.0f, 1.0f));
	}
	sm_pAmberOff->ref();

	if (m_pAsset)
	{
		Common::NodeFinderT<osg::Geode> amberFinder("trafficLight::AmberLamp-GEODE");
		Common::NodeFinderT<osg::Geode> redFinder("trafficLight::RedLamp-GEODE");
		Common::NodeFinderT<osg::Geode> greenFinder("trafficLight::GreenLamp-GEODE");

		if (m_pAmber = amberFinder.find(m_pRoot)) m_pAmber->ref();
		if (m_pRed = redFinder.find(m_pRoot)) m_pRed->ref();
		if (m_pGreen = greenFinder.find(m_pRoot)) m_pGreen->ref();
	}
	setState(STOP);

}

Assignment::ControllableTrafficLightFacade::~ControllableTrafficLightFacade()
{
	if (m_pAmber) m_pAmber->unref();
	if (m_pGreen) m_pGreen->unref();
	if (m_pRed) m_pRed->unref();
	if (sm_pRedOn) sm_pRedOn->unref();
	if (sm_pAmberOn) sm_pAmberOn->unref();
	if (sm_pGreenOn) sm_pGreenOn->unref();
	if (sm_pRedOff) sm_pRedOff->unref();
	if (sm_pAmberOff) sm_pAmberOff->unref();
	if (sm_pGreenOff) sm_pGreenOff->unref();
}

void Assignment::ControllableTrafficLightFacade::setState(LightControl::LightState eState)
{
	m_eState = eState;
	switch (eState)
	{
	case LightControl::STOP:
		m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOn);
		m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOff);
		m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOff);
		break;
	case LightControl::READY:
		m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOn);
		m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOn);
		m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOff);
		break;
	case LightControl::GO:
		m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOff);
		m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOff);
		m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOn);
		break;
	case LightControl::SLOW:
		m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOff);
		m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOn);
		m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOff);
		break;
	case LightControl::ALL:
		m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOn);
		m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOn);
		m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOn);
		break;
	case LightControl::NONE:
		m_pRed->getOrCreateStateSet()->setAttributeAndModes(sm_pRedOff);
		m_pAmber->getOrCreateStateSet()->setAttributeAndModes(sm_pAmberOff);
		m_pGreen->getOrCreateStateSet()->setAttributeAndModes(sm_pGreenOff);
		break;
	}
}

osg::Node* Assignment::ControllableTrafficLightFacade::rootNode()
{
	return m_pRoot;
}

osg::Vec3f Assignment::ControllableTrafficLightFacade::getFacadeCollisionPoint()
{
	// currently this is calculating the world position target omn every frame. Ideally, because this is a static object,
	// this position could be calculated in the constructor, stored as a member variable and returned here without repeating the calculation

	osg::Vec3f t, s;
	osg::Quat r, sr;

	// get the path, from the position target to the root, and decompose the resultant matrix to get the world position of the collision target
	osg::computeLocalToWorld(m_pCollisionTarget->getParentalNodePaths(0)[0]).decompose(t, r, s, sr);

	return t;
}