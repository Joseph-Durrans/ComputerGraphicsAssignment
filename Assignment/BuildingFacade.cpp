#include <Common/Facade.h>
#include "BuildingFacade.h"
#include <Common/NodeFinderT.h>

Assignment::BuildingFacade::BuildingFacade(std::string sName, osg::Node* pAsset, osg::Matrixf m, bool bVisible) : Facade(sName, pAsset, m, bVisible), m_pRotate(new osg::MatrixTransform), m_pScale(new osg::MatrixTransform)
{
	if (m_pAsset && m_pRoot && m_pTransformation)
	{
		m_pRotate->ref();
		m_pScale->ref();

		m_pTransformation->setName("Translation");
		m_pRotate->setName("Rotation");
		m_pScale->setName("Scale");

		// build new transform structure
		m_pTransformation->removeChild(m_pAsset);
		m_pTransformation->addChild(m_pRotate);
		m_pRotate->addChild(m_pScale);
		m_pScale->addChild(m_pAsset);

		// find switches
		Common::NodeFinderT<osg::Switch> idSwitchFinder("IDSwitch");

		m_pNames = idSwitchFinder.find(m_pAsset);

		if (m_pNames) m_pNames->ref();
	}
}

Assignment::BuildingFacade::~BuildingFacade()
{
	if (m_pRotate) m_pRotate->unref();
	if (m_pScale) m_pScale->unref();
	if (m_pNames) m_pNames->unref();
}

osg::MatrixTransform* Assignment::BuildingFacade::translate()
{
	return m_pTransformation;
}

osg::MatrixTransform* Assignment::BuildingFacade::rotate()
{
	return m_pRotate;
}

osg::MatrixTransform* Assignment::BuildingFacade::scale()
{
	return m_pScale;
}

void Assignment::BuildingFacade::enableNames(bool bEnable)
{
	if (m_pNames)
	{
		if (bEnable) m_pNames->setAllChildrenOn();
		else m_pNames->setAllChildrenOff();
	}
}
