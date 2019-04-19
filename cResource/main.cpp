#include "pch.h"
#include <iostream>

/*Include le librerie maggiche per fare tutto*/

#define DEBUG TRUE
/*TRUE per scrivere su schermo i messaggi di debug*/

clock_t t;
/*utilizzata per statistiche di tempo*/



/**VARIABILI PER LA COMUNICAZIONE SERIALE*/

#define MAX_DATA_LENGTH 255
/*dimensione massima del messaggio da inviare o ricevere*/

#define SEND
/*comment to read*/

using namespace std;

char incomingData[MAX_DATA_LENGTH];
/*array contenente i dati da inviare*/


//Control signals for turning on and turning off the led
//Check arduino code
/*char ledON[] = "ON\n";*/
/*char ledOFF[] = "OFF\n";*/
/* Took from another code: see Manash's blog https://blog.manash.me/serial-communication-with-an-arduino-using-c-on-windows-d08710186498 */



/**VARIABILI PER I COLORI SU SCHERMO*/

float red,
	  green,
	  blue;

char dati[4];
/*array contenente i valori come caratteri*/
/*colors as characters*/

float domMax, domMed, domMin;
/*utilizzabili per la correzione del colore*/
/*used to modify colors: see Scap::ColorFill*/

SerialPort *arduino;
Scap *screen;
/*Oggetti della classe SerialPort e Scap*/

/************************DECLARATIONS*******************************************
********************************************************************************
********************************************************************************
********************************************************************************
*************************FUNCTIONS**********************************************/

bool serialConnect(const char* portName) {

	arduino = new SerialPort(portName);
	/*crea un primo oggetto*/

	/*riprova fino a quando non stabilisce una connessione*/
	if (!arduino->isConnected()) {
		arduino = new SerialPort(portName);
		return false;
	} else {
		printf("Connection established at port %s\n", portName);
		return true;
	}
}
/* See: Manash's blog https://blog.manash.me/serial-communication-with-an-arduino-using-c-on-windows-d08710186498*/

/************************FUNCTIONS**********************************************
********************************************************************************
********************************************************************************
********************************************************************************
*************************MAIN***************************************************/

int main(int argc, char* argv[]) {

	if (argc != 5) {
		printf("Arguments error");
		return 0;
	}
	dati[3] = 0x00;
	char* portName = argv[1];
	domMax = atof(argv[2]);
	domMed = atof(argv[3]);
	domMin = atof(argv[4]);

	/*assegna correzione RGB*/
	#ifdef DEBUG
		printf("Press\"Escape\" to exit\n");
		/*info tasti*/
	#endif // DEBUG

	screen = new Scap();

	bool prevConnect = false;
	do {
		prevConnect = serialConnect(portName);
		while (arduino->isConnected()) {

			/*SKIDADLE SKIDOODLE...*/
			/**/t = clock();
			screen->GetScreenshot();
			screen->ScreenshotToArray();
			screen->Mix(red, green, blue);
			screen->ColorFill(red, green, blue, domMax, domMed, domMin);
			dati[RED]  = ((int)red)    & 0xff;
			dati[GREEN] = ((int)green) & 0xff;
			dati[BLUE]  = ((int)blue)  & 0xff;
			
			for (int i = 0; i < ENDCHAR; i++) {
				if (dati[i] == 0x00)
					dati[i] = 1;
			}
			/*trasforma un eventuale 0 in 1 (0 è usato per fine stringa)*/
			/*0x00 is used as an end character*/

			/**/t = clock() - t;

			#ifdef DEBUG
				printf("\n\tClock: %i\tR: %5.2f\tG: %5.2f\tB: %5.2f\t%c , %c , %c", t, red, green, blue, dati[RED], dati[GREEN], dati[BLUE]);
			#endif // DEBUG

			arduino->writeSerialPort(dati, MAX_DATA_LENGTH);
			std::this_thread::sleep_for(std::chrono::milliseconds(4));
			/*...IL CODICE QUI È UN NOODLE*/
		}
		if (prevConnect)
			printf("Connection lost\n");
	} while (!GetAsyncKeyState(VK_ESCAPE));
	delete arduino;
	delete screen;
	/*rilascia DC ed esce*/
	printf("\nApplication closed\n");
	system("pause");
	return 0;
}
