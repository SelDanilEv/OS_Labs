#include <iostream>
#include "Windows.h"
#include <string>
BOOL printWathRowFileTxt(LPCWSTR FileName, DWORD msec);

int main()
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    LPCWSTR path = L"D:\\Danil\\university\\OS\\2s\\Labs\\Lab1\\OS09_01.txt";
    printWathRowFileTxt(LPWSTR(path),  30000);
}

int getStrCount(LPCWSTR FileName);

void startListen(LPCWSTR FileName) {

    std::wstring wFilePath = std::wstring(FileName);
    std::string file = std::string(wFilePath.begin(), wFilePath.end());

    std::string dir = std::string(wFilePath.begin(), wFilePath.end());

    for (int i = dir.length() - 1; i >= 0; i--) {
        if (dir[i] != '\\')
            dir.pop_back();
        else {
            break;
        }
    }
    HANDLE hNotification = FindFirstChangeNotificationA(dir.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);

    if (hNotification == INVALID_HANDLE_VALUE) 
    {
        MessageBox(NULL, L"Check input path", L"Error", MB_OK);
    }

    int fileStrsCount = getStrCount(FileName) + 1;

    while (1) 
    {
        FindNextChangeNotification(hNotification);
        DWORD dwResult = WaitForSingleObject(hNotification, INFINITE);
        int newfileStrsCount = getStrCount(FileName) + 1;
        if (fileStrsCount != newfileStrsCount) {
            std::cout << "Strings:  " << newfileStrsCount << std::endl;
            fileStrsCount = newfileStrsCount;
        }
    }
    FindCloseChangeNotification(hNotification);
}

int getStrCount(LPCWSTR FileName) {
    char rbuf[10240];
    Sleep(1000);
    HANDLE hf = CreateFile(
        FileName,
        GENERIC_WRITE | GENERIC_READ,
        NULL,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    DWORD n = NULL;
    ZeroMemory(rbuf, sizeof(rbuf));
    BOOL b = ReadFile(
        hf,
        &rbuf,
        sizeof(rbuf),
        &n,
        NULL);
    int i = 0;
    int strCount = 0;
    while (rbuf[i] != '\0') {
        if (rbuf[i] == '\n')
            strCount++;
        i++;
    }
    CloseHandle(hf);
    return strCount;
}

BOOL printWathRowFileTxt(LPCWSTR FileName, DWORD msec) {
        
    HANDLE thtimer = CreateWaitableTimer(NULL, FALSE, NULL);
    long long sec = msec * 10000;
    sec = -sec;
    HANDLE thread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)startListen, (LPVOID)FileName, NULL, FALSE);
    SetWaitableTimer(thtimer, (LARGE_INTEGER*)&sec, 0, NULL, NULL, FALSE);
    WaitForSingleObject(thtimer, INFINITE);
    TerminateThread(thread2, -1);
    return TRUE;
}