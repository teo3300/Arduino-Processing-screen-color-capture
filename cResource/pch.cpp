#include "pch.h"

SerialPort::SerialPort(const char *portName) {
	this->connected = false;

	this->handler = CreateFileA(static_cast<LPCSTR>(portName),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (!(this->handler == INVALID_HANDLE_VALUE)) {/*
	    if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			printf("ERROR: Handle was not attached. Reason: %s not available\n", portName);
		}
		else {
			printf("ERROR!!!");
		}
	}
	else {*/
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(this->handler, &dcbSerialParameters)) {
			printf("failed to get current serial parameters");
		}
		else {
			dcbSerialParameters.BaudRate = CBR_9600;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(handler, &dcbSerialParameters)) {
				printf("ALERT: could not set Serial port parameters\n");
			}
			else {
				this->connected = true;
				PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
}
SerialPort::~SerialPort() {
	if (this->connected) {
		this->connected = false;
		CloseHandle(this->handler);
	}
}
int SerialPort::readSerialPort(char *buffer, unsigned int buf_size) {
	DWORD bytesRead;
	unsigned int toRead = 0;

	ClearCommError(this->handler, &this->errors, &this->status);

	if (this->status.cbInQue > 0) {
		if (this->status.cbInQue > buf_size) {
			toRead = buf_size;
		}
		else toRead = this->status.cbInQue;
	}

	memset(buffer, 0, buf_size);

	if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

	return 0;
}
bool SerialPort::writeSerialPort(char *buffer, unsigned int buf_size) {
	DWORD bytesSend;

	if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, 0)) {
		ClearCommError(this->handler, &this->errors, &this->status);
		return false;
	}
	else return true;
}
bool SerialPort::isConnected() {
	if (!ClearCommError(this->handler, &this->errors, &this->status))
		this->connected = false;

	return this->connected;
}
/* See: Manash's blog https://blog.manash.me/serial-communication-with-an-arduino-using-c-on-windows-d08710186498*/

Scap::Scap() {
	a.x = 0;
	a.y = 0;
	b.x = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	b.y = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	w = b.x;
	h = b.y;
	hScreen = GetDC(NULL);
	hDC = CreateCompatibleDC(hScreen);
	/*info su schermo e area di acquisizione*/
	/*screen parameters*/

	hBitmap = CreateCompatibleBitmap(hScreen, b.x, b.y);
	/*crea bitmap*/

	bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);	/*dimensione header della bitmap*/
	bmi.biPlanes = 1;						/*layer della bitmap			*/
	bmi.biBitCount = 24;					/*profondità bit				*/
	bmi.biWidth = b.x;						/*larghezza bitmap				*/
	bmi.biHeight = -b.y;					/*altezza bitmap				*/
	bmi.biCompression = BI_RGB;				/*tipo do compressione			*/
	/*specifiche bitmap*/

	SelectObject(hDC, hBitmap);
	/*collega DC e bitmap*/

	res = w * h;
	ScreenData = new BYTE[res * 3];
	/*array per la bitmap*/

	printf("Risoluzione: %i x %i\n%i pixel\n\n", w, h, res);

	/*Create Windows bitmap to store data in*/
}
Scap::~Scap() {
	DeleteObject(hBitmap);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	delete[] ScreenData;
	/*altre magie eseguite a fine programma*/
}
void Scap::GetScreenshot() {
	BitBlt(hDC, 0, 0, b.x, b.y, hScreen, 0, 0, SRCCOPY);
	/*lo... dice il nome...*/
	/*Get a shot of the screen and store i into the bitmap*/
}
void Scap::ScreenshotToArray() {
	GetDIBits(hDC, hBitmap, 0, b.y, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
	/*anche questa...*/
	/*Store the bitmap into an array*/
}
/*Code is almost windows API's*/

void Scap::Mix(float&r, float&g, float&b) {
	r = 0;
	g = 0;
	b = 0;
	for (UINT32 i = 0; i < res; i++) {
		r += ScreenData[i + i + i + 2];
		g += ScreenData[i + i + i + 1];
		b += ScreenData[i + i + i];
	}
	r /= res;
	g /= res;
	b /= res;
	/*media dei valori RGB = (C1+C2+...+Cn)/n, questa è matematica*/
	/*calculate the average of all pixels' color*/
}
void Scap::ColorFill(float&r, float&g, float&b, float fillMax, float fillMed, float fillMin) {
	if (r > g) {
		if (r > b) {
			if (g > b) {
				r *= fillMax;
				g *= fillMed;
				b *= fillMin;
			}
			else {
				r *= fillMax;
				g *= fillMin;
				b *= fillMed;
			}
		}
		else {
			r *= fillMed;
			g *= fillMin;
			b *= fillMax;
		}
	}
	else {
		if (g > b) {
			if (r > b) {
				r *= fillMed;
				g *= fillMax;
				b *= fillMin;
			}
			else {
				r *= fillMin;
				g *= fillMax;
				b *= fillMed;
			}
		}
		else {
			r *= fillMin;
			g *= fillMed;
			b *= fillMax;
		}
	}
	/*condizionale: aumenta la componente dominante*/
	/*increase the dominant color's value by a specified proportional value (to avoid an almost gray color at every sampling)*/

	if (r > 255)
		r = 255;
	if (g > 255)
		g = 255;
	if (b > 255)
		b = 255;
	/*e la forza nei valori massimi e minimi (0 per divisione stringhe)*/
	/*every value that overcome the maximum will be reduced */
}
