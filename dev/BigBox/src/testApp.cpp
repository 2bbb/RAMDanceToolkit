#include "testApp.h"



#include "BigBox.h"
BigBox bigbox;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10000);

	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	bigbox.setup();
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	bigbox.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackgroundGradient( ofColor( 240 ), ofColor( 60 ) );
	bigbox.draw();
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(600., 50.);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	bigbox.drawActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	
}




#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}

