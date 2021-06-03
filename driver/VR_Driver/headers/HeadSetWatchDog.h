#pragma once
#include <thread>
#include <openvr_driver.h>

#if defined( _WINDOWS )
#include <windows.h>
#endif


class HeadSetWatchDog : public vr::IVRWatchdogProvider{
public:
	HeadSetWatchDog(){
		m_pWatchdogThread = nullptr;
	}

	virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext);
	virtual void Cleanup();

private:
	std::thread* m_pWatchdogThread;
};

