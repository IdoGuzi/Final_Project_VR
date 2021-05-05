#pragma once

#include <Windows.h>
#include <vector>
#include <string>


/*
*https://www.youtube.com/watch?v=C7JQ7Rpwn2k&t=2778s
* a great talk to check about noise and drift.
* 
*/

class MpuReader {
  private:
	bool readThread;
	bool available;
	std::string port;
	std::vector<float> zero[4];
	std::vector<float> quaternion[4];
	std::vector<float> acceleration[3];
  public:
	  MpuReader(std::string port);
	  ~MpuReader() {}

	bool isAvailable();

	std::vector<float> getZero();
	void setZero();
	std::vector<float> getQuaternion();
	void setQuaternion(std::string data);
	std::vector<float> getAcceleration();
	void setAcceleration(std::string data);

	bool readFromMpu();

  private:
	void read();
	std::string readline(HANDLE fd);
};