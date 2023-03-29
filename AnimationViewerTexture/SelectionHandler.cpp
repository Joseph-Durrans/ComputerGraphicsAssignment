#include "SelectionHandler.h"

AnimationViewerTexture::SelectionHandler::SelectionHandler(): m_pFacade(0)
{
}

AnimationViewerTexture::SelectionHandler::SelectionHandler(Common::Facade* pFacade): m_pFacade(pFacade)
{
}

AnimationViewerTexture::SelectionHandler::~SelectionHandler()
{
}

osg::Object* AnimationViewerTexture::SelectionHandler::cloneType() const
{
    return new SelectionHandler();
}

osg::Object* AnimationViewerTexture::SelectionHandler::clone(const osg::CopyOp&) const
{
    return new SelectionHandler(m_pFacade);
}

const char* AnimationViewerTexture::SelectionHandler::libraryName() const
{
    return "None";
}

const char* AnimationViewerTexture::SelectionHandler::className() const
{
    return "SelectionHandler";
}

Common::Facade* AnimationViewerTexture::SelectionHandler::facade()
{
    return m_pFacade;
}
