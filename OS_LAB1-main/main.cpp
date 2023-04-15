#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "employee.h"
#include <conio.h>

void executeCommand(char* command) {
	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(processInfo));

	if (!CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &processInfo))
	{
		std::cout << "Failed to execute command.";
		system("pause");
	}

	WaitForSingleObject(processInfo.hProcess, INFINITE);
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);
}

int main()
{
	char binaryFileName[256];
	std::cout << "Enter binary file name: ";
	std::cin.getline(binaryFileName, 256);

	int numRecords = 0;
	std::cout << "Enter number of records: ";
	std::cin >> numRecords;
	std::cin.ignore();

	char command[1000];
	sprintf_s(command, "%s %s %d", "creator.exe", binaryFileName, numRecords);
	executeCommand(command);

	std::cout << "Information in binary file:" << std::endl;
	std::ifstream inputFileStream(binaryFileName, std::ios::binary);
	inputFileStream.read((char *)&numRecords, sizeof(int));
	employee *employeeRecords = new employee[numRecords];

	for (int i = 0; i < numRecords; i++)
	{
		inputFileStream.read((char *)&employeeRecords[i], sizeof(employee));
	}

	std::cout << employeeRecords[0].num << " " << employeeRecords[0].name << " " << employeeRecords[0].hours;
	for (int i = 1; i < numRecords; i++)
	{
		std::cout << std::endl << employeeRecords[i].num << " " << employeeRecords[i].name << " " << employeeRecords[i].hours;
	}

	delete[] employeeRecords;
	inputFileStream.close();

	char outputFileName[256];
	std::cout << "\nEnter output file name: ";
	std::cin.getline(outputFileName, 256);

	double salaryPerHour = 0.0;
	std::cout << "Enter salary value per hour: ";
	std::cin >> salaryPerHour;

	command[0] = '\0';
	sprintf_s(command, "%s %s %s %lf", "reporter.exe", binaryFileName, outputFileName, salaryPerHour);
	executeCommand(command);
	
	std::ifstream outputFileStream(outputFileName);
	std::cout << std::endl << "Information in output file:" << std::endl;
	char str[1000];
	for (int i = 0; i < numRecords * 4; i++)
	{
		outputFileStream.getline(str, 1000);
		std::cout << str << std::endl;
	}
	outputFileStream.close();
	
	return 0;
}
