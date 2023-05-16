#include "SelectionHandler.h"

Assignment::SelectionHandler::SelectionHandler() : m_pFacade(0)
{
}

Assignment::SelectionHandler::SelectionHandler(Common::Facade* pFacade) : m_pFacade(pFacade)
{
}

Assignment::SelectionHandler::~SelectionHandler()
{
}

osg::Object* Assignment::SelectionHandler::cloneType() const
{
    return new SelectionHandler();
}

osg::Object* Assignment::SelectionHandler::clone(const osg::CopyOp&) const
{
    return new SelectionHandler(m_pFacade);
}

const char* Assignment::SelectionHandler::libraryName() const
{
    return "None";
}

const char* Assignment::SelectionHandler::className() const
{
    return "SelectionHandler";
}

Common::Facade* Assignment::SelectionHandler::facade()
{
    return m_pFacade;
}
