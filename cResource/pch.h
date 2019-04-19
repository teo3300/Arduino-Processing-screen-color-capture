#ifndef PCH_H
#define PCH_H

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#define RED     0
#define GREEN   1
#define BLUE    2
#define ENDCHAR 3


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "wtypes.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <thread>
#include <conio.h>
#include <time.h>
#include <chrono>

class SerialPort {
private:
	HANDLE handler;
	bool connected;
	COMSTAT status;
	DWORD errors;
public:
	SerialPort(const char *portName);
	~SerialPort();

	int readSerialPort(char *buffer, unsigned int buf_size);
	bool writeSerialPort(char *buffer, unsigned int buf_size);
	bool isConnected();
};



#define INLINE static inline

class Scap {
	int h, w;
	POINT a, b;
	HDC hDC;
	HDC hScreen;
	HBITMAP hBitmap;
	BYTE* ScreenData;
	BITMAPINFOHEADER bmi;
	UINT32 res;
public:
	Scap();
	~Scap();

	void GetScreenshot();
	void ScreenshotToArray();
	void Mix(float & r, float & g, float & b);
	void ColorFill(float & r, float & g, float & b, float fillMax, float fillMed, float fillMin);
};




#endif //PCH_H
