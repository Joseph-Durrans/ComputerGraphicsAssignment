#include "InteractionHandler.h"

#include <iostream>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>


InteractionHandler::InteractionHandler(KEY_FUNC* pFunc) : m_pFunc(pFunc)
{
}

InteractionHandler::~InteractionHandler()
{
}

bool InteractionHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*)
{
	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::KEYDOWN:
		std::cout << "The " << ea.getKey() << "was pressed at window position [" << ea.getWindowX() << ", " << ea.getWindowY() << "]" << std::endl;

		if (m_pFunc)
		{
			m_pFunc(ea.getKey());
		}
		else
		{
			std::cout << "There is no specific help for this application" << std::endl;
		}


		//    		return true;
	}

	return false;
}
