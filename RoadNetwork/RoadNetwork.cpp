#include <windows.h>
#include <iostream>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgDB/ReadFile>
#include <osgGA/DriveManipulator>

#include <Common/Facade.h>
#include <Common/Printer.h>
#include <Common/AssetLibrary.h>
#include <Common/FacadeManufactory.h>

#include <TrafficSystem/RoadFacadeFactory.h>
#include <TrafficSystem/RoadFacade.h>
#include <TrafficSystem/TrafficLightFacadeFactory.h>
#include <TrafficSystem/TrafficLightFacade.h>
#include <TrafficSystem/CarFacadeFactory.h>
#include <TrafficSystem/CarFacade.h>

#include <Assignment/Example.h>

#include <TrafficSystem/AnimationPointFinder.h>
#include <TrafficSystem/Collider.h>

// #include "ExampleInteractionHandler.h"

#include <Assignment/ControllableTrafficLightFacade.h>
#include <Assignment/ControllableTrafficLightFacadeFactory.h>

#include <Assignment/RoadTileLightsFacade.h>
#include <Assignment/RoadTileLightsFacadeFactory.h>

#include <Assignment/Geometry.h>
#include <Assignment/CubeGeometry.h>

#include <Common/NodeFinderT.h>


// have to add these to new library they are currently in demo
#include <Assignment/AnimatedCar.h>
#include <Assignment/AnimatedCarFactory.h>


osg::Group* g_pRoot;

const float TILE_SIZE = 472.0f;


float addControlPoint(std::string sTile, std::string sPoint, osg::AnimationPath* pPath, float fTime, float fSpeed, osg::Vec3f& rvLastPos, bool bFirst = false)
{
	//find the facade for the tile we want
	Common::Facade* pF = Common::Facade::findFacade(sTile);

	// setup finder to look for animation point
	Common::NodeFinderT<osg::MatrixTransform> finder(sPoint);


	// use finder to find point in the tile
	if (osg::MatrixTransform* pMT = finder.find(pF->root()))
	{
		// find all the routes between the found anaimation point and the scene root
		// remember there could be many as we have re-used this bit of geometry. We need the one that passes through the root node of this facade
		osg::NodePathList npl = pMT->getParentalNodePaths(0);

		// loop through all of the node paths we have found
		for (osg::NodePathList::iterator it = npl.begin(); it != npl.end(); it++)
		{
			// test to see of the current node path is the one that passes through the facade we are considering now
			if (std::find(it->begin(), it->end(), pF->root()) != it->end())
			{
				//we now have the correct route for the animation point we are using

				// calulate the local to world matrix for this route
				osg::Matrix m = osg::computeLocalToWorld(*it);

				//decompose the matrix to get the world translation	and rotation
				osg::Vec3f t, s;
				osg::Quat r, sr;
				m.decompose(t, r, s, sr);

				// if this is not the first control point calculate the time needed to move from the last position
				if (!bFirst)
				{
					float fDistance = (t - rvLastPos).length();
					fTime += fDistance / fSpeed;
				}

				// update the last position - remember this has been passed by reference so we can updated in this function
				rvLastPos = t;

				// add the control point to the animation path
				pPath->insert(fTime, osg::AnimationPath::ControlPoint(t, r));

				// return the updated time 
				return fTime;
			}
		}
	}
	return fTime;
}

void createRoadNetwork() {
	// Straight road Transformations
	osg::Matrixf mRS0, mRS1, mRS2, mRS3, mRS4, mRS5, mRS6;

	mRS1 = osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(TILE_SIZE, TILE_SIZE, 0.0f);
	mRS2 = osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(0.0f, TILE_SIZE * 3, 0.0f);
	mRS3 = osg::Matrixf::translate(TILE_SIZE, TILE_SIZE * 5, 0.0f);
	mRS4 = osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(-TILE_SIZE, TILE_SIZE * 3, 0.0f);
	mRS5 = osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(-TILE_SIZE, TILE_SIZE * 2, 0.0f);
	mRS6 = osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(-TILE_SIZE, TILE_SIZE, 0.0f);

	// Curved road Transformations
	osg::Matrixf mRC0, mRC1, mRC2, mRC3, mRC4, mRC5, mRC6, mRC7, mRC8, mRC9, mRC10;

	mRC0 = osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(TILE_SIZE, 0.0f, 0.0f);
	mRC1 = osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(TILE_SIZE * 2, TILE_SIZE * 2, 0.0f);
	mRC2 = osg::Matrixf::translate(TILE_SIZE, TILE_SIZE * 3, 0.0f);
	mRC3 = osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(TILE_SIZE * 3, TILE_SIZE * 3, 0.0f);
	mRC4 = osg::Matrixf::translate(0.0f, TILE_SIZE * 2, 0.0f);
	mRC5 = osg::Matrixf::rotate(osg::DegreesToRadians(180.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(TILE_SIZE * 3, TILE_SIZE * 4, 0.0f);
	mRC6 = osg::Matrixf::rotate(osg::DegreesToRadians(270.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(TILE_SIZE, TILE_SIZE * 4, 0.0f);
	mRC7 = osg::Matrixf::rotate(osg::DegreesToRadians(180.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(TILE_SIZE * 2, TILE_SIZE * 5, 0.0f);
	mRC8 = osg::Matrixf::rotate(osg::DegreesToRadians(270.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(0.0f, TILE_SIZE * 5, 0.0f);
	mRC9 = osg::Matrixf::translate(-TILE_SIZE, 0.0f, 0.0f);
	mRC10 = osg::Matrixf::rotate(osg::DegreesToRadians(270.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(-TILE_SIZE, TILE_SIZE * 4, 0.0f);

	// T junction Transformations
	osg::Matrixf mRT0, mRT1;

	mRT0 = osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0) * osg::Matrixf::translate(TILE_SIZE, TILE_SIZE * 2, 0.0f);
	mRT1 = osg::Matrixf::translate(0.0f, TILE_SIZE * 4, 0.0f);

	// X junction Transformations
	osg::Matrixf mRX0, mRX1;

	mRX0 = osg::Matrixf::translate(TILE_SIZE * 2, TILE_SIZE * 3, 0.0f);
	mRX1 = osg::Matrixf::translate(TILE_SIZE * 2, TILE_SIZE * 4, 0.0f);

	// Create straight roads
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadStraight0", Common::AssetLibrary::instance()->getAsset("Road-Straight"), mRS0, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadStraight1", Common::AssetLibrary::instance()->getAsset("Road-Straight"), mRS1, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadStraight2", Common::AssetLibrary::instance()->getAsset("Road-Straight"), mRS2, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadStraight3", Common::AssetLibrary::instance()->getAsset("Road-Straight"), mRS3, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadStraight4", Common::AssetLibrary::instance()->getAsset("Road-Straight"), mRS4, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadStraight5", Common::AssetLibrary::instance()->getAsset("Road-Straight"), mRS5, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadStraight6", Common::AssetLibrary::instance()->getAsset("Road-Straight"), mRS6, true)->root());

	// Create curved roads
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve0", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC0, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve1", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC1, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve2", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC2, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve3", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC3, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve4", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC4, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve5", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC5, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve6", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC6, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve7", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC7, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve8", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC8, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve9", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC9, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadCurve10", Common::AssetLibrary::instance()->getAsset("Road-Curve"), mRC10, true)->root());

	// Create T junctions
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadTJunction0", Common::AssetLibrary::instance()->getAsset("Road-TJunction"), mRT0, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTileLights", "RoadTJunction1", Common::AssetLibrary::instance()->getAsset("Road-TJunction"), mRT1, true)->root());

	// Create X junctions
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTile", "RoadXJunction0", Common::AssetLibrary::instance()->getAsset("Road-XJunction"), mRX0, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("RoadTileLights", "RoadXJunction1", Common::AssetLibrary::instance()->getAsset("Road-XJunction"), mRX1, true)->root());

	// Find t juction and create lights
	if (Assignment::RoadTileLightsFacade* pRL = dynamic_cast<Assignment::RoadTileLightsFacade*>(Common::Facade::findFacade("RoadTJunction1")))
	{
		osg::Matrix mL0, mL1, mL2, mL3;

		mL0 = osg::Matrixf::scale(0.03f, 0.03f, 0.03f) *
			osg::Matrixf::rotate(osg::DegreesToRadians(0.0f), 0.0f, 0.0f, 1.0f) *
			osg::Matrixf::translate(-180.0f, 180.0f, 0.0f);


		mL2 = osg::Matrixf::scale(0.03f, 0.03f, 0.03f) *
			osg::Matrixf::rotate(osg::DegreesToRadians(180.0f), 0.0f, 0.0f, 1.0f) *
			osg::Matrixf::translate(180.0f, -180.0f, 0.0f);

		mL3 = osg::Matrixf::scale(0.03f, 0.03f, 0.03f) *
			osg::Matrixf::rotate(osg::DegreesToRadians(-90.0f), 0.0f, 0.0f, 1.0f) *
			osg::Matrixf::translate(180.0f, 180.0f, 0.0f);

		pRL->addLight(dynamic_cast<Assignment::ControllableTrafficLightFacade*>(Common::FacadeManufactory::instance()->create("ControlledTrafficLight", "RoadTJunction1TrafficLight0", Common::AssetLibrary::instance()->cloneAsset("TrafficLight"), mL0, true)));
		pRL->addLight(dynamic_cast<Assignment::ControllableTrafficLightFacade*>(Common::FacadeManufactory::instance()->create("ControlledTrafficLight", "RoadTJunction1TrafficLight1", Common::AssetLibrary::instance()->cloneAsset("TrafficLight"), mL2, true)));
		pRL->addLight(dynamic_cast<Assignment::ControllableTrafficLightFacade*>(Common::FacadeManufactory::instance()->create("ControlledTrafficLight", "RoadTJunction1TrafficLight2", Common::AssetLibrary::instance()->cloneAsset("TrafficLight"), mL3, true)));
	}

	// Find x juction and create lights
	if (Assignment::RoadTileLightsFacade* pRL = dynamic_cast<Assignment::RoadTileLightsFacade*>(Common::Facade::findFacade("RoadXJunction1")))
	{
		osg::Matrix mL0, mL1, mL2, mL3;

		mL0 = osg::Matrixf::scale(0.03f, 0.03f, 0.03f) *
			osg::Matrixf::rotate(osg::DegreesToRadians(0.0f), 0.0f, 0.0f, 1.0f) *
			osg::Matrixf::translate(-180.0f, 180.0f, 0.0f);

		mL1 = osg::Matrixf::scale(0.03f, 0.03f, 0.03f) *
			osg::Matrixf::rotate(osg::DegreesToRadians(90.0f), 0.0f, 0.0f, 1.0f) *
			osg::Matrixf::translate(-180.0f, -180.0f, 0.0f);

		mL2 = osg::Matrixf::scale(0.03f, 0.03f, 0.03f) *
			osg::Matrixf::rotate(osg::DegreesToRadians(180.0f), 0.0f, 0.0f, 1.0f) *
			osg::Matrixf::translate(180.0f, -180.0f, 0.0f);

		mL3 = osg::Matrixf::scale(0.03f, 0.03f, 0.03f) *
			osg::Matrixf::rotate(osg::DegreesToRadians(-90.0f), 0.0f, 0.0f, 1.0f) *
			osg::Matrixf::translate(180.0f, 180.0f, 0.0f);

		pRL->addLight(dynamic_cast<Assignment::ControllableTrafficLightFacade*>(Common::FacadeManufactory::instance()->create("ControlledTrafficLight", "RoadXJunction1TrafficLight0", Common::AssetLibrary::instance()->cloneAsset("TrafficLight"), mL0, true)));
		pRL->addLight(dynamic_cast<Assignment::ControllableTrafficLightFacade*>(Common::FacadeManufactory::instance()->create("ControlledTrafficLight", "RoadXJunction1TrafficLight1", Common::AssetLibrary::instance()->cloneAsset("TrafficLight"), mL1, true)));
		pRL->addLight(dynamic_cast<Assignment::ControllableTrafficLightFacade*>(Common::FacadeManufactory::instance()->create("ControlledTrafficLight", "RoadXJunction1TrafficLight2", Common::AssetLibrary::instance()->cloneAsset("TrafficLight"), mL2, true)));
		pRL->addLight(dynamic_cast<Assignment::ControllableTrafficLightFacade*>(Common::FacadeManufactory::instance()->create("ControlledTrafficLight", "RoadXJunction1TrafficLight3", Common::AssetLibrary::instance()->cloneAsset("TrafficLight"), mL3, true)));
	}
}

void createAnimatedCars() {
	// Show tile names and animation point names
	for (Common::FacadeMap::iterator it = Common::Facade::facades().begin(); it != Common::Facade::facades().end(); it++)
	{
		if (TrafficSystem::RoadFacade* pRF = dynamic_cast<TrafficSystem::RoadFacade*>(it->second))
		{
			pRF->enableAnimationIDs(true);
			pRF->enableNames(true);
			pRF->enableAnimationPoints(true);

			// this bit adds the tile name to the billboards over the road tile
			osg::Billboard* pBB = new osg::Billboard();
			pBB->setNormal(osg::Vec3f(0.0f, 0.0f, 1.0f));
			osgText::Text* pT = new osgText::Text();
			pT->setText(it->first);

			pBB->addDrawable(pT);
			pBB->setPosition(0, osg::Vec3f(0.0f, 0.0f, 60.0f));
			pRF->scale()->addChild(pBB);
		}
	}

	osg::Matrixf mAC1, mAC2;

	mAC1 = osg::Matrixf::scale(40.0f, 40.0f, 40.0f) *
		osg::Matrix::rotate(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)) *
		osg::Matrixf::translate(0.0f, 0.0, 25.0f);

	mAC2 = osg::Matrixf::scale(1.0f, 1.0f, 1.0f) *
		osg::Matrix::rotate(osg::DegreesToRadians(90.0f), osg::Vec3f(1.0f, 0.0f, 0.0f)) *
		osg::Matrix::rotate(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)) *

		osg::Matrixf::translate(0.0f, 0.0, 25.0f);

	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("AnimatedCar", "AnimatedCar0", Common::AssetLibrary::instance()->getAsset("Car-Delta"), mAC1, true)->root());
	g_pRoot->addChild(Common::FacadeManufactory::instance()->create("AnimatedCar", "AnimatedCar1", Common::AssetLibrary::instance()->getAsset("Car-Stratos"), mAC2, true)->root());

	if (Assignment::AnimatedCar* pAC = dynamic_cast<Assignment::AnimatedCar*>(Common::Facade::findFacade("AnimatedCar0"))) {
		osg::AnimationPath* pPath = new osg::AnimationPath();
		float fSpeed = 250.0f;
		float fTime = 0.0f;
		osg::Vec3f vLastPos;

		fTime = addControlPoint("RoadStraight0", "2", pPath, fTime, fSpeed, vLastPos, true);
		fTime = addControlPoint("RoadStraight0", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve0", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve0", "1", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve0", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight1", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight1", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadTJunction0", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadTJunction0", "3", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadTJunction0", "4", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve1", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve1", "1", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve1", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction0", "8", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction0", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction1", "8", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction1", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve7", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve7", "1", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve7", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight3", "5", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight3", "3", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve8", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve8", "1", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve8", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadTJunction1", "5", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadTJunction1", "6", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadTJunction1", "7", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve10", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve10", "1", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve10", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight4", "5", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight4", "3", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight5", "5", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight5", "3", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight6", "5", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadStraight6", "3", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve9", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve9", "1", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve9", "2", pPath, fTime, fSpeed, vLastPos);

		pAC->setAnimationPath(pPath);
	}

	if (Assignment::AnimatedCar* pAC = dynamic_cast<Assignment::AnimatedCar*>(Common::Facade::findFacade("AnimatedCar1"))) {
		osg::AnimationPath* pPath = new osg::AnimationPath();
		float fSpeed = 800.0f;
		float fTime = 0.0f;
		osg::Vec3f vLastPos;

		fTime = addControlPoint("RoadCurve3", "3", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve3", "4", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve3", "5", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction0", "11", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction0", "13", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction0", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction1", "8", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction1", "9", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction1", "7", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve6", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve6", "1", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve6", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve2", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve2", "1", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve2", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction0", "0", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction0", "1", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction0", "2", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction1", "8", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction1", "15", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadXJunction1", "10", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve5", "3", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve5", "4", pPath, fTime, fSpeed, vLastPos);
		fTime = addControlPoint("RoadCurve5", "5", pPath, fTime, fSpeed, vLastPos);

		pAC->setAnimationPath(pPath);
	}
}

int main()
{
	// Declare Viewer
	osgViewer::Viewer viewer;

	// Initialise Scene Tree
	g_pRoot = new osg::Group();
	g_pRoot->ref();

	// Initialise FacadeManuFactory if it doesnt already exist
	Common::FacadeManufactory::start();

	// Get FacadeManuFactory instance and add new factory if it doesnt already exist
	Common::FacadeManufactory::instance()->addFactory("RoadTile", new TrafficSystem::RoadFacadeFactory());
	Common::FacadeManufactory::instance()->addFactory("RoadTileLights", new Assignment::RoadTileLightsFacadeFactory());
	Common::FacadeManufactory::instance()->addFactory("TrafficLight", new TrafficSystem::TrafficLightFacadeFactory());
	Common::FacadeManufactory::instance()->addFactory("ControlledTrafficLight", new Assignment::ControllableTrafficLightFacadeFactory());
	Common::FacadeManufactory::instance()->addFactory("AnimatedCar", new Assignment::AnimatedCarFactory());


	// Initialise AssetLibrary if it doesnt already exist
	Common::AssetLibrary::start();

	// Get AssetLibrary instance and load new asset if it doesnt already exist
	Common::AssetLibrary::instance()->loadAsset("Road-Straight", "../../Data/roadStraight.osgb");
	Common::AssetLibrary::instance()->loadAsset("Road-TJunction", "../../Data/roadTJunction.osgb");
	Common::AssetLibrary::instance()->loadAsset("Road-XJunction", "../../Data/roadXJUnction.osgb");
	Common::AssetLibrary::instance()->loadAsset("Road-Curve", "../../Data/roadCurve.osgb");
	Common::AssetLibrary::instance()->loadAsset("TrafficLight", "../../Data/raaTrafficLight.osgb");
	Common::AssetLibrary::instance()->loadAsset("Car-Dumptruck", "../../OpenSceneGraph-Data/dumptruck.osgt");
	Common::AssetLibrary::instance()->loadAsset("Car-Delta", "../../Data/Lancia-Delta.obj");
	Common::AssetLibrary::instance()->loadAsset("Car-Stratos", "../../Data/Lancia-Stratos/source/lshfg4.fbx");

	createRoadNetwork();

	createAnimatedCars();

	// Initialise window Traits 
	osg::GraphicsContext::Traits* pTraits = new osg::GraphicsContext::Traits();
	pTraits->x = 20;
	pTraits->y = 20;
	pTraits->width = 1920;
	pTraits->height = 1080;
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	pTraits->sharedContext = 0;
	pTraits->readDISPLAY();
	pTraits->setUndefinedScreenDetailsToDefaultScreen();

	// Initialise GraphicsContext with window Traits
	osg::GraphicsContext* pGraphicsContext = osg::GraphicsContext::createGraphicsContext(pTraits);

	// Pointer to viewer Camera
	osg::Camera* pCamera = viewer.getCamera();

	// Set Camera properties with window Traits and GraphicsContext
	pCamera->setGraphicsContext(pGraphicsContext);
	pCamera->setViewport(new osg::Viewport(0, 0, pTraits->width, pTraits->height));
	GLenum buffer = pTraits->doubleBuffer ? GL_BACK : GL_FRONT;
	pCamera->setDrawBuffer(buffer);
	pCamera->setReadBuffer(buffer);
	viewer.setCamera(pCamera);

	// add event handlers
	//viewer.addEventHandler(new osgViewer::ThreadingHandler);
	//viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
	//viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	//viewer.addEventHandler(new osgViewer::StatsHandler);
	//viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);
	//viewer.addEventHandler(new osgViewer::LODScaleHandler);
	//viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);
	//viewer.addEventHandler(new ExampleInteractionHandler(keyFunction));

	// Set the Scene to render
	viewer.setSceneData(g_pRoot);

	// Present the window
	viewer.realize();

	// Start rendering loop
	return viewer.run();
}