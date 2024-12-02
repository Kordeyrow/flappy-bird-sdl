// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the BIRDENGINE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// BIRDENGINE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#include <dll/export_def.h>
#include <device/device_manager.h>
#include <user_interface/user_interface.h>

// This class is exported from the dll
class BIRDENGINE_API BirdEngine {
public:
	BirdEngine();
	~BirdEngine();
	void init();
	void run();
	UserInterface& get_user_interface();
private:
	DeviceManager device_manager;
	UserInterface user_interface;
};
