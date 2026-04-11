#include <stdlib.h>

#include <iostream>
#include <string>

#include <Windows.h>
#include <winsvc.h>


int main(void) {
	SC_HANDLE ActiveServicesHandle = OpenSCManagerA(
		NULL,
		NULL,
		GENERIC_READ
	);

	if (ActiveServicesHandle == NULL) {
		std::cerr << "Failed to get active services handle" << std::endl;
		return -1;
	}
	std::cout << "Got services handle" << std::endl;

	unsigned long pcbBytesNeeded = 0;
	unsigned long lpServicesReturned = 0;
	unsigned long lpResumeHandle = 0;
	void* lpServices = NULL;

	int enumServicesRet = EnumServicesStatusExA(
		ActiveServicesHandle,
		SC_ENUM_PROCESS_INFO,
		SERVICE_DRIVER | SERVICE_FILE_SYSTEM_DRIVER | SERVICE_KERNEL_DRIVER | SERVICE_WIN32 | SERVICE_WIN32_OWN_PROCESS | SERVICE_WIN32_SHARE_PROCESS,
		SERVICE_ACTIVE,
		NULL, // Needs to be pointer to buffer
		0, // Needs to be number
		&pcbBytesNeeded,
		&lpServicesReturned,
		&lpResumeHandle,
		NULL
	);

	if (enumServicesRet == 0) {
		std::cout << "Tried first call to services enum. Got needed buffer lengths" << std::endl;
		std::cout << "\tpcbBytesNeeded = " << pcbBytesNeeded << std::endl;
		std::cout << "\tlpServicesReturned = " << lpServicesReturned << std::endl;
		std::cout << "\tlpResumeHandle = " << lpResumeHandle << std::endl;

		lpServices = malloc(pcbBytesNeeded);
		if (!lpServices) {
			std::cerr << "Failed to allocate memory for service buffer" << std::endl;
			return -1;
		}

		enumServicesRet = EnumServicesStatusExA(
			ActiveServicesHandle,
			SC_ENUM_PROCESS_INFO,
			SERVICE_DRIVER | SERVICE_FILE_SYSTEM_DRIVER | SERVICE_KERNEL_DRIVER | SERVICE_WIN32 | SERVICE_WIN32_OWN_PROCESS | SERVICE_WIN32_SHARE_PROCESS,
			SERVICE_ACTIVE,
			(unsigned char*)lpServices,
			pcbBytesNeeded,
			&pcbBytesNeeded,
			&lpServicesReturned,
			&lpResumeHandle,
			NULL
		);

		if (enumServicesRet == 0) {
			std::cerr << "Failed second call to services enum" << std::endl;
			return -1;
		}
	}
	else {
		std::cerr << "Succeeded first services enum ..?" << std::endl;
		return -1;
	}
	std::cout << "Successfully enumerated services" << std::endl;
	std::cout << "\tpcbBytesNeeded = " << pcbBytesNeeded << std::endl;
	std::cout << "\tlpServicesReturned = " << lpServicesReturned << std::endl;
	std::cout << "\tlpResumeHandle = " << lpResumeHandle << std::endl;

	// Iterate through services
	int maxServiceDisplay = lpServicesReturned;
	//int maxServiceDisplay = 50;
	for (int i = 0; i < maxServiceDisplay; i++) {
		std::cout << "Service entry [" << i << "]:" << std::endl;
		std::cout << "\tService Name: " << ((ENUM_SERVICE_STATUS_PROCESSA*)lpServices)[i].lpServiceName << std::endl;
		std::cout << "\tDisplay Name: " << ((ENUM_SERVICE_STATUS_PROCESSA*)lpServices)[i].lpDisplayName << std::endl;

		DWORD serviceTypeId = ((ENUM_SERVICE_STATUS_PROCESSA*)lpServices)[i].ServiceStatusProcess.dwServiceType;
		std::string serviceTypeName = "";

		if (serviceTypeId & SERVICE_FILE_SYSTEM_DRIVER) {
			serviceTypeName += "SERVICE_FILE_SYSTEM_DRIVER, ";
		}
		if (serviceTypeId & SERVICE_KERNEL_DRIVER) {
			serviceTypeName += "SERVICE_KERNEL_DRIVER, ";
		}
		if (serviceTypeId & SERVICE_WIN32_OWN_PROCESS) {
			serviceTypeName += "SERVICE_WIN32_OWN_PROCESS, ";
		}
		if (serviceTypeId & SERVICE_WIN32_SHARE_PROCESS) {
			serviceTypeName += "SERVICE_WIN32_SHARE_PROCESS, ";
		}
		std::cout << "\tService Type: " << serviceTypeName << std::endl;
	}

	free(lpServices);

	return 0;
}