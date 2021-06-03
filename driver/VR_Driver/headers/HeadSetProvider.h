#pragma once

#include <openvr_driver.h>
#include <driverlog.h>
#include <HeadSetDriver.h>
#include <ControllerDriver.h>


class HeadSetProvider : public vr::IServerTrackedDeviceProvider{
private:
	HeadSetDriver* m_pNullHmdLatest = nullptr;
	CSampleControllerDriver* m_pController = nullptr;
public:
	virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext);
	virtual void Cleanup();
	virtual const char* const* GetInterfaceVersions() { return vr::k_InterfaceVersions; }
	virtual void RunFrame();
	virtual bool ShouldBlockStandbyMode() { return false; }
	virtual void EnterStandby() {}
	virtual void LeaveStandby() {}
};