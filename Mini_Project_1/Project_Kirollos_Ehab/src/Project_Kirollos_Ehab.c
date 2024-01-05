/*
 ============================================================================
 Name        : Project_Kirollos_Ehab.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#define WITH_ENGINE_TEMP_CONTROLLER 1

typedef enum{
	False =0,
	True=1
}bool;

struct Vehicle {
	bool engineState;
	bool acState;
	int vehicleSpeed;
	int roomTemperature;
	bool engineTempControllerState;
	int engineTemperature;
};

char mainMenu(){
	char c;
	printf("a. Turn on the vehicle engine\n");
	printf("b. Turn off the vehicle engine\n");
	printf("c. Quit the system\n");
	printf("Enter your choice: ");
	scanf(" %c", &c);
	return c;
}

char sensorMenu(){
	char c;
	printf("\nSensors Menu:\n");
	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color\n");
	printf("c. Set the room temperature\n");
	#if WITH_ENGINE_TEMP_CONTROLLER
	printf("d. Set the engine temperature\n");
	#endif
	printf("Enter your choice: ");
	scanf(" %c", &c);
	return c;
}

void simulateVehicle(struct Vehicle *vehicle) {
	char trafficLight;
	int roomTemp, engineTemp;

	while (1) {
		char choice = sensorMenu();

		switch (choice) {
		case 'a':
			vehicle->engineState = False;
			return;
		case 'b':
			printf("Enter traffic light color (G/O/R): ");
			scanf(" %c", &trafficLight);
			if (trafficLight == 'G') {
				vehicle->vehicleSpeed = 100;
			} else if (trafficLight == 'O') {
				vehicle->vehicleSpeed = 30;
				if (!vehicle->acState) {
					vehicle->acState = True;
					vehicle->roomTemperature = (vehicle->roomTemperature * 5 / 4) + 1;
				}
				#if WITH_ENGINE_TEMP_CONTROLLER
				if (!vehicle->engineTempControllerState) {
					vehicle->engineTempControllerState = True;
					vehicle->engineTemperature = (vehicle->engineTemperature * 5 / 4) + 1;
				}
				#endif
			} else if (trafficLight == 'R') {
				vehicle->vehicleSpeed = 0;
			}
			break;
		case 'c':
			printf("Enter room temperature: ");
			scanf("%d", &roomTemp);
			if (roomTemp < 10 || roomTemp > 30) {
				vehicle->acState = True;
				vehicle->roomTemperature = 20;
			} else {
				vehicle->acState = False;
			}
			break;
		#if WITH_ENGINE_TEMP_CONTROLLER
		case 'd':
			printf("Enter engine temperature: ");
			scanf("%d", &engineTemp);
			if (engineTemp < 100 || engineTemp > 150) {
				vehicle->engineTempControllerState = True;
				vehicle->engineTemperature = 125;
			} else {
				vehicle->engineTempControllerState = False;
			}
			break;
		#endif
		default:
			printf("Invalid choice!\n");
			break;
		}

		printf("\nCurrent Vehicle State:\n");
		printf("Engine state: %s\n", vehicle->engineState ? "ON" : "OFF");
		printf("AC: %s\n", vehicle->acState ? "ON" : "OFF");
		printf("Vehicle Speed: %d km/hr\n", vehicle->vehicleSpeed);
		printf("Room Temperature: %d\n", vehicle->roomTemperature);
		#if WITH_ENGINE_TEMP_CONTROLLER
		printf("Engine Temperature Controller State: %s\n", vehicle->engineTempControllerState ? "ON" : "OFF");
		printf("Engine Temperature: %d\n\n", vehicle->engineTemperature);
		#endif

	}
}

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	struct Vehicle vehicle = {False, False, 0, 25, False, 50};
	while (1) {
		char c = mainMenu();
		if (c == 'a') {
			vehicle.engineState = True;
			simulateVehicle(&vehicle);
		}
		else if (c == 'b') {
			puts("Engine Turned off");
		}
		else if (c == 'c') {
			puts("Quitting... ");
			break;
		}
		else {
			puts("Invalid input");
		}
	}
	return EXIT_SUCCESS;
}
