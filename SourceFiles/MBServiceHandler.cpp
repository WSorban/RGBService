#include "MBServiceHandler.h"
#include <iostream>
#include <windows.h>
#include <winsvc.h>


void pause()
{
	std::cin.sync();
	std::cin.ignore();
}


void restartService()
{
	LPCWSTR Service = L"AsRogAuraService";

	SERVICE_STATUS Status;

	SC_HANDLE SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE SHandle = OpenService(SCManager, Service, SC_MANAGER_ALL_ACCESS);

	if (SHandle == NULL)
	{
		std::cout << "ERROR " << GetLastError() << std::endl;
	}

	if (!ControlService(SHandle, SERVICE_CONTROL_STOP, &Status))
	{
		std::cout << "FAILED TO SEND STOP SERVICE COMMAND: " << GetLastError();
	}
	else
	{
		std::cout << "Service Stop Command Sent\n";
	}

	do
	{
		QueryServiceStatus(SHandle, &Status);
		std::cout << "Checking Service Status...\n";
	} while (Status.dwCurrentState != SERVICE_STOPPED);


	if (!StartService(SHandle, 0, NULL))
	{
		std::cout << "Service Did Not Start Up Again: " << GetLastError() << std::endl;
	}
	else
	{
		std::cout << "Service Started\n";
	}

	pause();

	CloseServiceHandle(SCManager);
	CloseServiceHandle(SHandle);

	return;
}