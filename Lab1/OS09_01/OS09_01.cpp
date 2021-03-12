#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

BOOL printFileInfo(LPWSTR FileName) {
	HANDLE fileHandle = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE)
		return false;

	_BY_HANDLE_FILE_INFORMATION fileInformation;
	if (!GetFileInformationByHandle(fileHandle, &fileInformation)) {
		CloseHandle(fileHandle);
		return false;
	}

	wcout << "1.File path:" << FileName << endl;

	cout << "2.File type:";
	DWORD fileType = GetFileType(fileHandle);
	switch (fileType) {
	case FILE_TYPE_CHAR: cout << "FILE_TYPE_CHAR" << std::endl; break;
	case FILE_TYPE_DISK: cout << "FILE_TYPE_DISK" << std::endl; break;
	case FILE_TYPE_PIPE: cout << "FILE_TYPE_PIPE" << std::endl; break;
	case FILE_TYPE_REMOTE: cout << "FILE_TYPE_REMOTE" << std::endl; break;
	case FILE_TYPE_UNKNOWN: cout << "FILE_TYPE_UNKNOWN" << std::endl; break;
	default: std::cout << "--------->PRIORITY -->? " << std::endl << std::endl; break;
	}

	LARGE_INTEGER lFileSize;
	LONGLONG fileSize = 0;
	BOOL bGetSize = GetFileSizeEx(fileHandle, &lFileSize);
	if (!bGetSize)
		return false;
	fileSize += lFileSize.QuadPart;
	cout << "3.File size:" << fileSize << endl;

	FILETIME createFile = fileInformation.ftCreationTime;
	SYSTEMTIME createFileToSystemTime;
	FileTimeToSystemTime(&createFile, &createFileToSystemTime);
	cout << "4.Create date and time:"
		<< " " << createFileToSystemTime.wYear
		<< "." << createFileToSystemTime.wMonth
		<< "." << createFileToSystemTime.wDay
		<< " " << createFileToSystemTime.wHour + 3
		<< " " << createFileToSystemTime.wMinute
		<< endl;

	FILETIME lastUpdate = fileInformation.ftLastWriteTime;
	SYSTEMTIME lastUpdateToSysTime;
	FileTimeToSystemTime(&lastUpdate, &lastUpdateToSysTime);
	cout << "5.Last update date and time:"
		<< " " << lastUpdateToSysTime.wYear
		<< "." << lastUpdateToSysTime.wMonth
		<< "." << lastUpdateToSysTime.wDay
		<< " " << lastUpdateToSysTime.wHour
		<< " " << lastUpdateToSysTime.wMinute
		<< endl;
	long result = GetFileSize(fileHandle, NULL);
	cout << "6.Size file:" << result << endl;
	CloseHandle(fileHandle);

	return true;

}

BOOL printFileTxt(LPWSTR FileName) {
	try {
		HANDLE fileHandle = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fileHandle == INVALID_HANDLE_VALUE) {
			CloseHandle(fileHandle);
			return false;
		}

		DWORD n = NULL;
		char buf[1024];

		ZeroMemory(buf, sizeof(buf));
		LARGE_INTEGER lFileSize;
		LONGLONG fileSize = 0;
		BOOL bGetSize = GetFileSizeEx(fileHandle, &lFileSize);
		if (!bGetSize)
			return false;
		fileSize += lFileSize.QuadPart;
		bool readFile = ReadFile(fileHandle, &buf,fileSize, &n, NULL);

		cout << "Read file:" << endl << buf << endl;
		CloseHandle(fileHandle);


		return true;
	}
	catch (char* em) {
		cout << " Error: " << em << endl;
	}
	
	return false;
}


int main() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	LPCWSTR path = L"D:\\Danil\\university\\OS\\2s\\Labs\\Lab1\\OS09_01.txt";
	if (!printFileInfo(LPWSTR(path))) {
		cout << "Error"<<endl;
	}
	if (!printFileTxt(LPWSTR(path))) {
		cout << "Error" << endl;
	}
	system("pause");
}			