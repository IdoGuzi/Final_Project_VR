#include <thread>
#include <vector>
#include <algorithm>
#include <MpuReader.h>


MpuReader::MpuReader(std::string port) {
	this->readThread = false;
	this->available = false;
	this->port = port;
	*this->zero = std::vector<float>{ 1, 0, 0, 0 };
	*this->quaternion = std::vector<float>{ 1, 0, 0, 0 };
	*this->acceleration = std::vector<float>{ 0,0,0 };
}

bool MpuReader::isAvailable() {
	if (this->readThread && this->available) return true;
	return false;
}


std::vector<float> MpuReader::getZero() {
	return *this->zero;
}
void MpuReader::setZero() {
	*this->zero = getQuaternion();
}


std::vector<float> MpuReader::getQuaternion() {
	return *this->quaternion;
}

void MpuReader::setQuaternion(std::string data) {
	int i = 0;
	size_t index = data.find(',');
	while (index >= 0) {
		auto temp = data.substr(0, index);
		this->quaternion->at(i++) = std::stof(temp);
		data = data.substr(index + 1);
		index = data.find(',');
	}
}

std::vector<float> MpuReader::getAcceleration() {
	return *this->acceleration;
}

void MpuReader::setAcceleration(std::string data) {
	int i = 0;
	size_t index = data.find(',');
	while (index >= 0) {
		auto temp = data.substr(0, index);
		this->acceleration->at(i++) = std::stof(temp);
		data = data.substr(index + 1);
		index = data.find(',');
	}
}


bool MpuReader::readFromMpu() {
	if (this->readThread) {
		return true;
	}
	std::thread t(&MpuReader::read,this);
	return false;
}






void MpuReader::read() {
	this->readThread = true;
	//https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
	LPCSTR p = TEXT("COM3");
	HANDLE fd = CreateFileA(p,
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);

	if (fd == INVALID_HANDLE_VALUE) {
		this->available = this->readThread = false;
		return;
	}
	this->available = true;

	//rate speed of communication
	DCB state;
	bool succ = GetCommState(fd, &state);
	if (!succ) {
		this->available = this->readThread = false;
		return;
	}
	state.BaudRate = CBR_115200;

	succ = SetCommState(fd, &state);
	if (!succ) {
		this->available = this->readThread = false;
		return;
	}

	while (true) {
		if (!this->available || !this->readThread) {
			this->available = this->readThread = true;
		}
		std::string line = readline(fd);
		if (line.empty()) {
			this->available = this->readThread = false;
			return;
		}
		if (std::count(line.begin(), line.end(), ',') == 3) {
			this->setQuaternion(line);
		} else {
			//if not quaternion data try again until it is.
			continue;
		}
		line = readline(fd);
		if (line.empty()) {
			this->available = this->readThread = false;
			return;
		}
		if (std::count(line.begin(), line.end(), ',') == 2) {
			this->setAcceleration(line);
		}
		else {
			//if not acceleration data try again until it is.
			continue;
		}
	}
	this->available = this->readThread = false;
}







std::string MpuReader::readline(HANDLE fd) {
	std::string out = "";
	char c = NULL;
	DWORD byteRead = 0;
	while (c != '\n') {
		//https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile?redirectedfrom=MSDN
		bool succ = ReadFile(fd, &c, 1, &byteRead, NULL);
		if (!succ) {
			return "";
		}
		out.push_back(c);
	}
	return out;
}