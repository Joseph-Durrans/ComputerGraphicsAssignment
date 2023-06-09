#include <windows.h>
#include <iostream>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>
#include <osgGA/NodeTrackerManipulator>

#include "SelectionHandler.h"
#include "PedestrianTrafficLightFacade.h"
#include "EventHandler.h"

#include "AnimatedCar.h"

Assignment::EventHandler::EventHandler()
{

}

Assignment::EventHandler::~EventHandler()
{

}

bool Assignment::EventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*)
{
    // detect mouse press and setup intersection test
    if (ea.getEventType() & osgGA::GUIEventAdapter::PUSH && ea.getButtonMask() & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
    {
        // build an intersector based on the mouse position
        osgUtil::LineSegmentIntersector* pRay = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::PROJECTION, ea.getXnormalized(), ea.getYnormalized());
        osgUtil::IntersectionVisitor visitor(pRay);

        //apply the intersection visitor to the scene
        aa.asView()->getCamera()->accept(visitor);

        if (pRay->containsIntersections()) // if a hit was detected
        {
            //get the first (nearest) hit
            osgUtil::LineSegmentIntersector::Intersection intersection = pRay->getFirstIntersection();

            if (intersection.drawable) // check the hit is a drawable
            {
                // search up the drawable's path to find the root of the facade
                // this will be the node we attached the user data to
                for (osg::NodePath::iterator it = intersection.nodePath.begin(); it != intersection.nodePath.end(); it++)
                {
                    // if a selection habalder is found
                    if (SelectionHandler* pSH = dynamic_cast<SelectionHandler*>((*it)->getUserData()))
                    {
                        //test to see what type of object facade it is and apply functionallity
                        if (AnimatedCar* pAC = dynamic_cast<AnimatedCar*>(pSH->facade()))
                        {
                            std::cout << "Clicked car" << std::endl;
                        }
                        else if (Assignment::PedestrianTrafficLightFacade* pTL = dynamic_cast<Assignment::PedestrianTrafficLightFacade*>(pSH->facade()))
                        {
                            pTL->toggleState();
                        }
                    }
                }
            }
        }
    }

    return false;
}


