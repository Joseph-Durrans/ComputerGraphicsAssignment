#pragma once

#include <windows.h>
#include <osgGA/GUIEventHandler>

namespace Assignment {
	class EventHandler : public osgGA::GUIEventHandler
	{
	public:
		EventHandler();
		virtual ~EventHandler();

		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*);

	};
}

