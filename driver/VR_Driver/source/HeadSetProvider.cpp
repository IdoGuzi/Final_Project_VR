#include <iostream>
#include <string>
#include <HeadSetProvider.h>
#include <HeadSetDriver.h>
#include <MpuReader.h>
#include <driverlog.h>

EVRInitError HeadsetProvider::Init(IVRDriverContext* pDriverContext) {
	vr::EVRInitError eError = vr::InitServerDriverContext(pDriverContext);
	if (eError != vr::VRInitError_None) {
		DriverLog("DIY_VR failed when creating hmd provider\n");
		return eError;
	}
	char port[256];
	vr::VRSettings()->GetString(k_pch_VR_Section, k_pch_VR_Port_String,port,sizeof(port));
	this->deviceDriver = new HeadSetDriver(MpuReader(std::string(port)));
	vr::VRServerDriverHost()->TrackedDeviceAdded(this->deviceDriver->getSerial().c_str(),
												 vr::TrackedDeviceClass_HMD,
												 this->deviceDriver
												);

	return vr::VRInitError_None;
}

void HeadsetProvider::Cleanup() {
	delete this->deviceDriver;
	this->deviceDriver = nullptr;
}

void HeadsetProvider::RunFrame() {
	this->deviceDriver->runframe();
}


bool HeadsetProvider::ShouldBlockStandbyMode() {
	return false;
}