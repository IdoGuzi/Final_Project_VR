#include <string>
#include <HeadSetDriver.h>
#include <driverlog.h>

HeadSetDriver::HeadSetDriver(MpuReader mr) {
	*this->IMU = mr;

	this->m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
	this->m_ulPropertyContainer = vr::k_ulInvalidPropertyContainer;

	m_flIPD = vr::VRSettings()->GetFloat(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_IPD_Float);

	char buf[1024];
	vr::VRSettings()->GetString(k_pch_VR_Section, k_pch_VR_SerialNumber_String, buf, sizeof(buf));
	m_sSerialNumber = buf;

	vr::VRSettings()->GetString(k_pch_VR_Section, k_pch_VR_ModelNumber_String, buf, sizeof(buf));
	m_sModelNumber = buf;

	m_nWindowX = vr::VRSettings()->GetInt32(k_pch_Display_Section, k_pch_Display_WindowX_Int32);
	m_nWindowY = vr::VRSettings()->GetInt32(k_pch_Display_Section, k_pch_Display_WindowY_Int32);
	m_nWindowWidth = vr::VRSettings()->GetInt32(k_pch_Display_Section, k_pch_Display_WindowWidth_Int32);
	m_nWindowHeight = vr::VRSettings()->GetInt32(k_pch_Display_Section, k_pch_Display_WindowHeight_Int32);
	m_nRenderWidth = vr::VRSettings()->GetInt32(k_pch_Display_Section, k_pch_Display_RenderWidth_Int32);
	m_nRenderHeight = vr::VRSettings()->GetInt32(k_pch_Display_Section, k_pch_Display_RenderHeight_Int32);
	m_flSecondsFromVsyncToPhotons = vr::VRSettings()->GetFloat(k_pch_Display_Section, k_pch_Display_SecondsFromVsyncToPhotons_Float);
	m_flDisplayFrequency = vr::VRSettings()->GetFloat(k_pch_Display_Section, k_pch_Display_DisplayFrequency_Float);
}

vr::EVRInitError HeadSetDriver::Activate(uint32_t unObjectId) {
	m_unObjectId = unObjectId;
	m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_unObjectId);


	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ModelNumber_String, m_sModelNumber.c_str());
	vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_RenderModelName_String, m_sModelNumber.c_str());
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_UserIpdMeters_Float, m_flIPD);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_UserHeadToEyeDepthMeters_Float, 0.f);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_DisplayFrequency_Float, m_flDisplayFrequency);
	vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_SecondsFromVsyncToPhotons_Float, m_flSecondsFromVsyncToPhotons);

	// return a constant that's not 0 (invalid) or 1 (reserved for Oculus)
	vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, vr::Prop_CurrentUniverseId_Uint64, 2);

	// avoid "not fullscreen" warnings from vrmonitor
	vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, vr::Prop_IsOnDesktop_Bool, false);

	this->IMU->readFromMpu();
	Sleep(3000);
	if (!this->IMU->isAvailable()) {
		DriverLog("Driver activation failed: IMU isn't available\n");
		return vr::VRInitError_Init_VRServiceStartupFailed;
	}

	return vr::VRInitError_None;
}

void HeadSetDriver::Deactivate() {
	this->m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
}

void HeadSetDriver::EnterStandby() {
	//leave empty
}

void* HeadSetDriver::GetComponent(const char* pchComponentNameAndVersion) {
	if (!_stricmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version)){
		return (vr::IVRDisplayComponent*)this;
	}
	// override this to add a component to a driver
	return NULL;
}

void HeadSetDriver::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) {
	if (unResponseBufferSize >= 1)
		pchResponseBuffer[0] = 0;
}

vr::DriverPose_t HeadSetDriver::GetPose() {
	if (IMU->readFromMpu()) {

	}
	auto dr = IMU->getQuaternion();
	auto ac = IMU->getAcceleration();
	vr::DriverPose_t pos;
	pos.deviceIsConnected = true;
	pos.poseIsValid = true;
	pos.shouldApplyHeadModel = true;
	pos.willDriftInYaw = true;

	pos.poseTimeOffset = vr::VRSettings()->GetFloat(k_pch_VR_Section, k_pch_VR_PoseTimeOffset_Float);
	pos.qDriverFromHeadRotation = { dr[0],dr[1],dr[2],dr[3] };
	pos.qWorldFromDriverRotation = { dr[0],dr[1],dr[2],dr[3] };
	pos.qRotation = { dr[0],dr[1],dr[2],dr[3] };
	pos.result = vr::TrackingResult_Running_OK;
	pos.vecPosition[0] = 0.0;
	pos.vecPosition[1] = 0.0;
	pos.vecPosition[2] = 0.0;
	//pos.vecAcceleration[0] = ac[0];
	//pos.vecAcceleration[1] = ac[1];
	//pos.vecAcceleration[2] = ac[2];

	return pos;
}



void HeadSetDriver::runframe() {
	if (this->m_unObjectId != vr::k_unTrackedDeviceIndexInvalid) {
		vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_unObjectId, this->GetPose(), sizeof(vr::DriverPose_t));
	}
}


//******* display commponent function *******
void HeadSetDriver::GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) {
	*pnX = m_nWindowX;
	*pnY = m_nWindowY;
	*pnWidth = m_nWindowWidth;
	*pnHeight = m_nWindowHeight;
}

bool HeadSetDriver::IsDisplayOnDesktop() {
	return true;
}
bool HeadSetDriver::IsDisplayRealDisplay() {
	return true;
}
void HeadSetDriver::GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight) {
	*pnHeight = this->m_nRenderHeight;
	*pnWidth = this->m_nRenderWidth;
}
void HeadSetDriver::GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) {
	*pnY = 0;
	*pnWidth = m_nWindowWidth / 2;
	*pnHeight = m_nWindowHeight;

	if (eEye == vr::Eye_Left)
	{
		*pnX = 0;
	}else{
		*pnX = m_nWindowWidth / 2;
	}
}
void HeadSetDriver::GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom) {
	*pfLeft = -1.0;
	*pfRight = 1.0;
	*pfTop = -1.0;
	*pfBottom = 1.0;
}
vr::DistortionCoordinates_t HeadSetDriver::ComputeDistortion(vr::EVREye eEye, float fU, float fV) {
	vr::DistortionCoordinates_t coordinates;
	coordinates.rfBlue[0] = fU;
	coordinates.rfBlue[1] = fV;
	coordinates.rfGreen[0] = fU;
	coordinates.rfGreen[1] = fV;
	coordinates.rfRed[0] = fU;
	coordinates.rfRed[1] = fV;
	return coordinates;
}



