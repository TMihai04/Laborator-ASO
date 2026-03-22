#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <winreg.h>
#include <winerror.h>

int main() {
	HKEY regHandle = NULL;
	//char regPath[]  = "HARDWARE\\ACPI\\SSDT\\VBOX__\\VBOXCPUT\\00000002";
	char regPath[] = "SOFTWARE";
	//char regPath[] = "SOFTWARE\\Mozilla\\Firefox\\TaskBarIDs";
	LSTATUS openStatus = RegOpenKeyExA(HKEY_LOCAL_MACHINE, (LPCSTR)regPath, 0, KEY_READ | KEY_WOW64_64KEY, &regHandle);

	if (openStatus != ERROR_SUCCESS) {
		std::cout << "Failed to open registry: " << openStatus << std::endl;
		return -1;
	}

	std::cout << "Opened " << regPath << ":" << std::endl;

	DWORD subkeyIdx = 0;
	char subkeyName[256] = { 0 };
	DWORD nameLen = sizeof(subkeyName);

	while (RegEnumKeyExA(regHandle, subkeyIdx, subkeyName, &nameLen, NULL, NULL, NULL, NULL) != ERROR_NO_MORE_ITEMS) {
		std::cout << subkeyIdx << ": " << subkeyName << std::endl;
		subkeyIdx++;
		nameLen = sizeof(subkeyName);
	}

	return 0;
}