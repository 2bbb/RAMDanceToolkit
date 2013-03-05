#include "ramControlPanel.h"

#include "ramBaseScene.h"
#include "ramControllable.h"

#include "ramActorsScene.h"

#ifdef RAM_GUI_SYSTEM_OFXUI

struct ButtonEventListener
{
	ofxUIButton *button;
	ofEvent<ofEventArgs> evt;
	
	ButtonEventListener(ofxUIButton *button) : button(button){}
	~ButtonEventListener() { delete button; }
	
	void handle(ofxUIEventArgs &e)
	{
		if (e.widget != button
			&& !button->getValue()) return;
		
		static ofEventArgs args;
		ofNotifyEvent(evt, args);
	}
};

ramPreferencesTab& ramControlPanel::getPreferencesTab() { return preferencesTab; }
ofxUITabbedCanvas& ramControlPanel::getSceneTabs() { return mSceneTabs; }
void ramControlPanel::save(const string& path) { getSceneTabs().saveSettings(path); }
void ramControlPanel::load(const string& path) { getSceneTabs().loadSettings(path); }
ofxUICanvasPlus* ramControlPanel::getCurrentUIContext() { return current_panel; }

ofEvent<ofEventArgs>& ramControlPanel::addButton(const string& name)
{
	ofxUIButton *button = current_panel->addButton(name, false, 30, 30);
	
	// FIXME: memory leak
	ButtonEventListener *e = new ButtonEventListener(button);
	ofAddListener(current_panel->newGUIEvent, e, &ButtonEventListener::handle);
	return e->evt;
}


ramControlPanel *ramControlPanel::_instance = NULL;

ramControlPanel& ramControlPanel::instance()
{
	if (_instance == NULL)
	{
		_instance = new ramControlPanel();
	}
	return *_instance;
}

ramControlPanel::ramControlPanel()
:kDim(16)
,kXInit(OFX_UI_GLOBAL_WIDGET_SPACING)
,kLength(320 - kXInit)
{
}

ramControlPanel::~ramControlPanel()
{
}

void ramControlPanel::setup()
{
	/// Event hooks
	// -------------------------------------
	ofAddListener(ofEvents().update, this, &ramControlPanel::update);
	
	addPanel(playbackTab);
	addPanel(presetTab);
	addPanel(preferencesTab);
	
	ofAddListener(mSceneTabs.newGUIEvent, this, &ramControlPanel::guiEvent);
	
	mSceneTabs.addSpacer();
}

void ramControlPanel::update(ofEventArgs &e)
{
	if(!ofGetMousePressed())
	{
		bool hover = mSceneTabs.isHit(ofGetMouseX(), ofGetMouseY());
		ramCameraManager::instance().setEnableInteractiveCamera(!hover);
	}
}

//

void ramControlPanel::addPanel(ramBaseScene* control, bool enableable)
{
	ofxUITab *panel = new ofxUITab(control->getName(), enableable);
	current_panel = panel;	
	control->setupControlPanel();
	getSceneTabs().add(panel);
	scenes.push_back(enableable ? control : NULL);
	panel->autoSizeToFitWidgets();
}

void ramControlPanel::addPanel(ofxUITab& tab)
{
	scenes.push_back(NULL);
	getSceneTabs().add(&tab);
}

void ramControlPanel::addSection(const string& name)
{
	current_panel->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_MEDIUM));
	current_panel->addSpacer(kLength, 2);
	current_panel->autoSizeToFitWidgets();
}

void ramControlPanel::addSeparator()
{
	current_panel->addSpacer(kLength, 2);
	current_panel->autoSizeToFitWidgets();
}

void ramControlPanel::addLabel(const string& content)
{
	current_panel->addWidgetDown(new ofxUILabel(content, OFX_UI_FONT_MEDIUM));
	current_panel->autoSizeToFitWidgets();
}

void ramControlPanel::addToggle(const string& name, bool *value)
{
	current_panel->addToggle(name, value, 30, 30);
	current_panel->autoSizeToFitWidgets();
}

void ramControlPanel::addMultiToggle(const string& name, const vector<string>& content, int *value)
{
	assert(false);
}

struct RadioGroupListener
{
	ofxUIRadio *o;
	int *value;
	
	RadioGroupListener(ofxUIRadio *o, int *value) : o(o), value(value)
	{
		o->getToggles().at(*value)->setValue(true);
	}
	
	void handle(ofxUIEventArgs &e)
	{
		if (e.widget->getParent() != o) return;
		
		vector<ofxUIToggle *> t = o->getToggles();
		for (int i = 0; i < t.size(); i++)
		{
			if (t[i]->getValue())
			{
				*value = i;
				break;
			}
		}
	}
};

ofxUIRadio* ramControlPanel::addRadioGroup(const string& name, const vector<string>& content, int *value)
{
	ofxUIRadio *o = current_panel->addRadio(name, content, OFX_UI_ORIENTATION_VERTICAL, kDim, kDim);
	
	// FIXME: memory leak
	RadioGroupListener *e = new RadioGroupListener(o, value);
	ofAddListener(current_panel->newGUIEvent, e, &RadioGroupListener::handle);
	current_panel->autoSizeToFitWidgets();
	return o;
}

void ramControlPanel::addDropdown(const string& name, const vector<string>& content, int *value)
{
	assert(false);
}

void ramControlPanel::addSlider(const string& name, float min_value, float max_value, float *value)
{
	current_panel->addSlider(name, min_value, max_value, value, kLength, kDim);
	current_panel->autoSizeToFitWidgets();
}

struct ColorSelectorListener
{
	ofxUIToggle* toggle;
	ofFloatColor *value;
	ofFloatColor color;
	
	ColorSelectorListener(ofxUIToggle* toggle, ofFloatColor *value) : toggle(toggle), value(value), color(*value) {}
	
	void handle(ofxUIEventArgs &e)
	{
		if (toggle->getValue())
		{
			color = *value;
			value->a = 1;
		}
		else
		{
			*value = color;
			value->a = 0;
		}
	}
};

void ramControlPanel::addColorSelector(const string& name, ofFloatColor *value)
{
	current_panel->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_MEDIUM));
	
	ofxUIToggle* toggle = current_panel->addToggle("", true, 26, 26);
	current_panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	
	// FIXME: memory leak
	ColorSelectorListener *e = new ColorSelectorListener(toggle, value);
	ofAddListener(current_panel->newGUIEvent, e, &ColorSelectorListener::handle);
	
	current_panel->addSlider("R", 0, 1, &value->r, 90, kDim);
	current_panel->addSlider("G", 0, 1, &value->g, 90, kDim);
	current_panel->addSlider("B", 0, 1, &value->b, 90, kDim);
	current_panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	current_panel->autoSizeToFitWidgets();
}

void ramControlPanel::remove(const string& name)
{
	assert(false);
}

void ramControlPanel::guiEvent(ofxUIEventArgs &e)
{
	for(int i = 0; i < scenes.size(); i++) 
	{
		if(scenes[i] != NULL)
		{
			// bomisutaro: this is a weak connection, it would be better for ramScene to extend ofxUITab
			scenes[i]->setEnabled(mSceneTabs.at(i)->getEnabled());
		}
	}
}

#endif