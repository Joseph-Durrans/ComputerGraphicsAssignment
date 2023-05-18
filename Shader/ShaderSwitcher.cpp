#include "ShaderSwitcher.h"
#include "ShaderAttacher.h"
#include "ShaderUnit.h"

#include <iostream>
#include <osg/Node>


Shader::ShaderSwitcher::ShaderSwitcher(osg::Node* pRoot) : m_pRoot(pRoot), m_sCurrentShader("")
{
	
}

Shader::ShaderSwitcher::~ShaderSwitcher()
{
	
}

bool Shader::ShaderSwitcher::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*)
{
	if(ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN)
	{
		if(ea.getKey()=='h')
		{
			std::cout << std::endl;
			std::cout << "Viewer help - shader options" << std::endl;
			std::cout << "\tp - per pixel lighting shader" << std::endl;
			std::cout << "\td - per vertex lighting shader" << std::endl;
			std::cout << "\tr - reload and rebuild shaders (see shader dir)" << std::endl;
			std::cout << "\t1 - toggle ambient lighting on/off" << std::endl;
			std::cout << "\t2 - toggle diffuse lighting on/off" << std::endl;
			std::cout << "\t3 - toggle specular lighting on/off" << std::endl;
			std::cout << "\t4 - toggle texture on/off" << std::endl;
			std::cout << "\t5 - toggle psuedo bump mapping on/off (per pixel only)" << std::endl;
		}
		else if (ea.getKey() == 'p')
		{
			m_sCurrentShader = "perPixel";
			ShaderAttacher sa(m_pRoot, "perPixel", aa.asView()->getCamera());
		}
		else if (ea.getKey() == 'd')
		{
			m_sCurrentShader = "default";
			ShaderAttacher sa(m_pRoot, "default", aa.asView()->getCamera());
		}
		else if (ea.getKey() == 'r')
		{
			if (m_sCurrentShader.length())
				ShaderUnit::rebuildShader(m_sCurrentShader);
		}
		else if (ea.getKey() == '1')
		{
			if (m_sCurrentShader.length())
				ShaderUnit::toggleStateControl(m_sCurrentShader, "ambient");
		}
		else if (ea.getKey() == '2')
		{
			if (m_sCurrentShader.length())
				ShaderUnit::toggleStateControl(m_sCurrentShader, "diffuse");
		}
		else if (ea.getKey() == '3')
		{
			if (m_sCurrentShader.length())
				ShaderUnit::toggleStateControl(m_sCurrentShader, "specular");
		}
		else if (ea.getKey() == '4')
		{
			if (m_sCurrentShader.length())
				ShaderUnit::toggleStateControl(m_sCurrentShader, "texture");
		}
		else if (ea.getKey() == '5')
		{
			if (m_sCurrentShader.length())
				ShaderUnit::toggleStateControl(m_sCurrentShader, "bump");
		}

	}
	return false;
}


