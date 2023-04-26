#pragma once

#include <Common/FacadeFactoryA.h>


namespace Assignment {
	class AnimatedCarFactory : public Common::FacadeFactoryA
	{
	public:
		AnimatedCarFactory();
		virtual ~AnimatedCarFactory();


		// Inherited via FacadeFactoryA
		virtual Common::Facade* create(std::string sName, osg::Node* pModelRoot, osg::Matrixf rTransform, bool bVisible) override;

	};
}

