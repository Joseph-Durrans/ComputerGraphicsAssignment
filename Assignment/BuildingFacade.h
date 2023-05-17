#pragma once

#include <Common/Facade.h>

namespace Assignment
{
	class BuildingFacade : public Common::Facade
	{
	public:
		BuildingFacade(std::string sName, osg::Node* pAsset, osg::Matrixf m = osg::Matrixf::identity(), bool bVisible = true);
		virtual ~BuildingFacade();

		osg::MatrixTransform* translate();
		osg::MatrixTransform* rotate();
		osg::MatrixTransform* scale();

		void enableNames(bool bEnable);
	protected:
		osg::MatrixTransform* m_pRotate;
		osg::MatrixTransform* m_pScale;

		osg::Switch* m_pNames;
	};
}