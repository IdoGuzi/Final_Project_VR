#include <MpuReader.h>
#include <string>
#include <array>
#include <algorithm>
#include <iostream>
#include <Windows.h>


MpuReader::MpuReader(std::string port) {
	this->zero = { 0,0,0,0 };
	this->quaternion = { 1,0,0,0 };
	this->acceleration = { 1,0,0 };
	//LPCSTR p = (std::string("\\\\.\\") + port).c_str();
	this->discriptor = CreateFileA((std::string("\\\\.\\") + port).c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (this->discriptor == INVALID_HANDLE_VALUE) {
		throw std::exception("can't create discriptor");
	}
	//rate speed of communication
	DCB state;
	bool succ = GetCommState(this->discriptor, &state);
	if (!succ) {
		throw std::exception("can't get communication state");
	}
	state.BaudRate = CBR_115200;

	succ = SetCommState(this->discriptor, &state);
	if (!succ) {
		throw std::exception("can't set communication state");
	}
	//initializtion line
	readline();
}


void MpuReader::setZero() {
	const char* reset = "reset"; // size 6 = 5 + 1, 5 letters + '\0' string end symbol
	bool ok = WriteFile(this->discriptor, &reset, 6, NULL, NULL);
}

void MpuReader::setQuaternion(std::string str) {
	int i = 0;
	int index = str.find(',');
	while (index >= 0) {
		std::string temp = str.substr(0, index);
		this->quaternion.at(i++) = std::stod(temp);
		str = str.substr(index + int(1));
		index = str.find(',');
	}
	this->quaternion.at(i) = std::stod(str);
}

void MpuReader::setAcceleration(std::string str) {
	int i = 0;
	int index = str.find(',');
	while (index >= 0) {
		std::string temp = str.substr(0, index);
		this->acceleration.at(i++) = std::stod(temp);
		str = str.substr(index + 1);
		index = str.find(',');
	}
	this->acceleration.at(i) = std::stod(str);
}





void MpuReader::read() {
	FlushFileBuffers(this->discriptor);
	std::string str = readline();
	bool quat = false, acc = false;
	while (!quat || !acc) {
		if (std::count(str.begin(), str.end(), ',') == 3) {
			quat = true;
			setQuaternion(str);
		}
		else if (std::count(str.begin(), str.end(), ',') == 2) {
			acc = true;
			setAcceleration(str);
		}
		else {
			str = readline();
			continue;
		}
		str = readline();
	}
}






std::string MpuReader::readline() {
	std::string out = "";
	char c = NULL;
	DWORD byteRead = 0;
	while (c != '\n') {
		//https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile?redirectedfrom=MSDN
		bool succ = ReadFile(this->discriptor, &c, 1, &byteRead, NULL);
		if (!succ) {
			return "";
		}
		out.push_back(c);
	}
	return out;
}