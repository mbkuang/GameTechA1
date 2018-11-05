/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    // Create your scene here :)
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
    
    Ogre::Light * pointLight = mSceneMgr->createLight("PointLight");
    pointLight->setPosition(20, 200, 50);
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setDiffuseColour(Ogre::ColourValue::White);
    pointLight->setSpecularColour(Ogre::ColourValue::White);
    createWalls();
    createBall();

    mCamera->setPosition(0, 0, 800);
    mCamera->lookAt(0,0,0);
}

void TutorialApplication::createBall() {
    Ogre::Entity* ball = mSceneMgr->createEntity("Ball", "sphere.mesh");
    ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallNode");
    ball->setMaterialName("BallColor/CubeMap");
    ballNode->setScale(0.25, 0.25, 0.25);
    ball->setCastShadows(true);   //EC: Enable ball's shadow for depth
/*    Ogre::Real x_dir = RandomBall(-0.2, 0.2);
    Ogre::Real y_dir = RandomBall(-0.2, 0.2);
    Ogre::Real z_dir = RandomBall(-0.2, 0.2);*/

    bSpeed = RandomBall(0.1, 3.0);

    Ogre::Real x_dir = Ogre::Math::RangeRandom(-0.2, 0.2);
    Ogre::Real y_dir = Ogre::Math::RangeRandom(-0.2, 0.2);
    Ogre::Real z_dir = Ogre::Math::RangeRandom(-0.2, 0.2);

    ballDirection = Ogre::Vector3(x_dir, y_dir, z_dir);
    ballNode->attachObject(ball);
}

void TutorialApplication::createWalls() {
    //Create the plane template
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("Plane",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, BOX_SIZE, BOX_SIZE, 10, 10, true, 
        1, 1, 1, Ogre::Vector3::UNIT_Z);

    /*
        Y-axis: yaw() left and right
        X-axis: pitch() front and back
        Z-axis: roll() top and bottom
    */

    Ogre::Entity * left = mSceneMgr->createEntity("Plane");
    Ogre::SceneNode * leftNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    leftNode->setPosition(BOX_SIZE/-2, 0, 0);
    leftNode->roll(Ogre::Degree(270));
    leftNode->yaw(Ogre::Degree(90));
    left->setMaterialName("Examples/WaterStream");  //EC: Pretty textures
    leftNode->attachObject(left);

    Ogre::Entity * right = mSceneMgr->createEntity("Plane");
    Ogre::SceneNode * rightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    rightNode->setPosition(BOX_SIZE/2, 0, 0);
    rightNode->roll(Ogre::Degree(90));
    rightNode->yaw(Ogre::Degree(270));
    right->setMaterialName("Examples/WaterStream");
    rightNode->attachObject(right);

    Ogre::Entity * backEntity = mSceneMgr->createEntity("Plane");
    Ogre::SceneNode * backNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    backNode->setPosition(0, 0, BOX_SIZE/-2);
    backNode->pitch(Ogre::Degree(90));
    backEntity->setMaterialName("Examples/WaterStream");
    backNode->attachObject(backEntity);

    //Gives the entire room a watery look
    Ogre::Entity * frontWall = mSceneMgr->createEntity("Plane");
    Ogre::SceneNode * frontNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    frontNode->setPosition(0, 0, BOX_SIZE/2);
    frontNode->pitch(Ogre::Degree(270));
    frontWall->setMaterialName("Examples/WaterStream");
    frontNode->attachObject(frontWall);

    Ogre::Entity* bottom = mSceneMgr->createEntity("Plane");
    Ogre::SceneNode* bottomNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    bottomNode->setPosition(0, BOX_SIZE/-2, 0);
    bottomNode->roll(Ogre::Degree(0));
    bottom->setMaterialName("Examples/BeachStones");
    bottomNode->attachObject(bottom);

    Ogre::Entity* top = mSceneMgr->createEntity("Plane");
    Ogre::SceneNode * topNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    topNode->setPosition(0, BOX_SIZE/2, 0);
    topNode->roll(Ogre::Degree(180));
    top->setMaterialName("Examples/CloudySky");
    topNode->attachObject(top);
}

float TutorialApplication::RandomBall(float a, float b) {
    srand(time(0));
    float random = ((float) rand()/(float) RAND_MAX);
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    if(mWindow->isClosed())
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;

    movement(evt);

    return true;

}

void TutorialApplication::movement(const Ogre::FrameEvent& evt) {
    Ogre::Entity* ballEntity = getBallEntity(ballNode);
    Ogre::AxisAlignedBox ballHitBox = ballEntity->getWorldBoundingBox();
    
    Ogre::Vector3 normalVector = Ogre::Vector3::ZERO; //Normal force zero
    Ogre::Vector3 lowHitBox = ballHitBox.getMinimum(); //Bottom and left
    Ogre::Vector3 highHitBox = ballHitBox.getMaximum(); //Top and right

    if(lowHitBox.x <= BOX_SIZE/-2)
        normalVector.x = 1;
    if(lowHitBox.y <= BOX_SIZE/-2)
        normalVector.y = 1;
    if(lowHitBox.z <= BOX_SIZE/-2)
        normalVector.z = 1;

    if(highHitBox.x >= BOX_SIZE/2)
        normalVector.x = -1;
    if(highHitBox.y >= BOX_SIZE/2)
        normalVector.y = -1;
    if(highHitBox.z >= BOX_SIZE/2)
        normalVector.z = -1;

    normalVector.normalise();
    //Normal force will be (0, 0, 0) unless ball has just intersected wall
    ballDirection = ballDirection.reflect(normalVector);
    ballNode->translate(ballDirection * bSpeed, Ogre::Node::TS_LOCAL);
}
//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
