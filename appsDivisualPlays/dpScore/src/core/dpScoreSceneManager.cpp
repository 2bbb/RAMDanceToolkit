//
//  dpScoreSceneManager.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreSceneManager.h"

DP_SCORE_NAMESPACE_BEGIN

SceneManager::SceneManager() :
mUpdateAll(true),
mSceneId(0)
{
    mScenes.clear();
    
    mTabBar = new ofxUITabBar();
    mTabBar->setPosition(20.f, 40.f);
    mTabBar->addToggle("Update All", &mUpdateAll);
    mTabBar->addSpacer();
}

SceneManager::~SceneManager()
{
    if (mTabBar) {
        delete mTabBar;
        mTabBar = NULL;
    }
    
    for (auto& scene : mScenes) scene->shutDown();
}

void SceneManager::add(SceneBase::Ptr scene)
{
    if (mScenes.empty() == false) {
        const string className = scene->getName();
        if (findScene(className) != mScenes.end())
            ofxThrowExceptionf(ofxException,
                               "class %s is already exists",
                               className.c_str());
    }
    scene->initialize();
    
    if (scene->getUICanvas() != NULL) {
        mTabBar->addCanvas(scene->getUICanvas());
    }
    
    scene->setId(mScenes.size());
    mScenes.push_back(scene);
}

void SceneManager::next()
{
    if (mScenes.empty())
        ofxThrowException(ofxException, "mScenes is empty");
    ++mSceneId %= mScenes.size();
    change();
}

void SceneManager::prev()
{
    if (mScenes.empty())
        ofxThrowException(ofxException, "mScenes is empty");
    --mSceneId;
    if (mSceneId<0) mSceneId = mScenes.size()-1;
    change();
}

void SceneManager::change(int index)
{
    if (index<0 || index>=mScenes.size())
        ofxThrowExceptionf(ofxException, "%d is out of range", index);
    mSceneId = index;
    change();
}

void SceneManager::change(const string& name)
{
    auto it = findScene(name);
    if (it == mScenes.end())
        ofxThrowExceptionf(ofxException,
                           "no scene named %s found",
                           name.c_str());
    mSceneId = (*it)->getId();
    change();
}

void SceneManager::change()
{
    if (mCurrentScene) mCurrentScene->exit();
    mCurrentScene = mScenes.at(mSceneId);
    mCurrentScene->enter();
    
    ofLogNotice() << "changed scene: " << mSceneId << ", " << mCurrentScene->getName();
}

void SceneManager::update(ofxEventMessage& m)
{
    if (mUpdateAll) {
        for (auto& scene : mScenes) scene->update(m);
    }
    else if (mCurrentScene) {
        mCurrentScene->update(m);
    }
}

void SceneManager::draw()
{
    if (mCurrentScene) mCurrentScene->draw();
}

ofPtr<SceneBase> SceneManager::getCurrent()
{
    return mCurrentScene;
}

void SceneManager::setUpdateAll(bool update)
{
    mUpdateAll = update;
}

void SceneManager::keyPressed(int key)
{
    if (mCurrentScene) mCurrentScene->keyPressed(key);
}

void SceneManager::keyReleased(int key)
{
    if (mCurrentScene) mCurrentScene->keyReleased(key);
}

void SceneManager::mouseMoved(int x, int y)
{
    if (mCurrentScene) mCurrentScene->mouseMoved(x, y);
}

void SceneManager::mouseDragged(int x, int y, int button)
{
    if (mCurrentScene) mCurrentScene->mouseDragged(x, y, button);
}

void SceneManager::mousePressed(int x, int y, int button)
{
    if (mCurrentScene) mCurrentScene->mousePressed(x, y, button);
}

void SceneManager::mouseReleased(int x, int y, int button)
{
    if (mCurrentScene) mCurrentScene->mouseReleased(x, y, button);
}

void SceneManager::windowResized(int w, int h)
{
    if (mCurrentScene) mCurrentScene->windowResized(w, h);
}

void SceneManager::dragEvent(ofDragInfo dragInfo)
{
    if (mCurrentScene) mCurrentScene->dragEvent(dragInfo);
}

void SceneManager::gotMessage(ofMessage msg)
{
    if (mCurrentScene) mCurrentScene->gotMessage(msg);
}

void SceneManager::guiEvent(ofxUIEventArgs &e)
{
    
}

SceneManager::SceneVec::iterator
SceneManager::findScene(const string& name)
{
    return find_if(mScenes.begin(),
                   mScenes.end(),
                   [&](const SceneBase::Ptr& rhs)
                   {
                       return name == rhs->getName();
                   });
}

DP_SCORE_NAMESPACE_END