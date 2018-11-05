/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
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

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

const int BOX_SIZE = 300;

//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);
    Ogre::Vector3 ballDirection;
    Ogre::Real bSpeed;

protected:
    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
    Ogre::SceneNode* ballNode;

private:
    bool processUnbufferedInput(const Ogre::FrameEvent& fe);
    void createBall();
    float RandomBall(float a, float b);
    void createWalls();
    void movement(const Ogre::FrameEvent& fe);
    Ogre::Entity* getBallEntity(Ogre::SceneNode* ballNode) {
        return static_cast<Ogre::Entity*>(ballNode->getAttachedObject(0));
    }
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
