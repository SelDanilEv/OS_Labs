#include <iostream>
#include <Windows.h>
#include <string>
BOOL insRowFileTxt(LPCWSTR FileName, LPCWSTR str, int row);
bool printFileTxt(LPWSTR FileName);
using namespace std;
int main()
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    LPCWSTR path = L"D:\\Danil\\university\\OS\\2s\\Labs\\Lab1\\OS09_01.txt";
    printFileTxt((LPWSTR)path);
    insRowFileTxt((LPWSTR)path, L"In start", 0);
    insRowFileTxt((LPWSTR)path, L"In end", -1);
    insRowFileTxt((LPWSTR)path, L"In 5", 5);
    insRowFileTxt((LPWSTR)path, L"In 7", 7);

    printFileTxt((LPWSTR)path);
}

bool printFileTxt(LPWSTR FileName) {
    HANDLE fileHandle = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
        return false;

    DWORD n = NULL;
    char buf[1024];

    ZeroMemory(buf, sizeof(buf));
    bool readFile = ReadFile(fileHandle, &buf, 700, &n, NULL);

    std::cout << "Read file:"  << endl << buf << std::endl;
    CloseHandle(fileHandle);
}

BOOL insRowFileTxt(LPCWSTR FileName, LPCWSTR str, int row)
{
    try {
        HANDLE hf = CreateFile(
            FileName,
            GENERIC_WRITE | GENERIC_READ,
            NULL,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (hf == INVALID_HANDLE_VALUE) throw "create or open file failed";
        std::cout << "-- Create file ot open succesfull\n";
        DWORD insN = NULL;
        DWORD n = NULL;
        char rbuf[1024];

        ZeroMemory(rbuf, sizeof(rbuf));

        char wbuf[1024];
        ZeroMemory(wbuf, sizeof(wbuf));
        BOOL b = ReadFile(
            hf,
            &rbuf,
            sizeof(rbuf),
            &n,
            NULL);
        string insInf = string();
        int rbufpos = 0;
        int insStart = 0;
        wstring wStr = wstring(str);
        string insString = string(wStr.begin(), wStr.end());
        LARGE_INTEGER newPointer;
        int suchRow = 1;
        switch (row)
        {
        case 0:
        case 1:
            insString += "\r\n";
            newPointer.QuadPart = 0;
            if (!SetFilePointerEx(hf, newPointer, NULL, FILE_BEGIN))
                throw std::exception("SetFilePointerEx False");

            b = WriteFile(
                hf,
                insString.c_str(),
                insString.length(),
                &insN,
                NULL
            );
            newPointer.QuadPart = insString.length();
            if (!SetFilePointerEx(hf, newPointer, NULL, FILE_BEGIN))
                throw std::exception("SetFilePointerEx False");
            b = WriteFile(
                hf,
                rbuf,
                n,
                &n,
                NULL
            );
            break;
        case -1:

            newPointer.QuadPart = 0;
            if (!SetFilePointerEx(hf, newPointer, NULL, FILE_END))
                throw std::exception("SetFilePointerEx False");
            insString = "\r\n" + insString;
            b = WriteFile(
                hf,
                insString.c_str(),
                insString.length(),
                &insN,
                NULL
            );
            break;
        default:
            while (rbuf[rbufpos] != '\0') {
                if (rbuf[rbufpos] == '\n') {
                    suchRow++;
                    rbufpos++;
                    if (suchRow == row+1) {
                        insStart = rbufpos;
                        while (rbuf[rbufpos] != '\0') {
                            insInf += rbuf[rbufpos];
                            rbufpos++;
                        }
                        break;
                    }
                }
                rbufpos++;
            }

            insString += "\r\n";
            newPointer.QuadPart = insStart;
            if (!SetFilePointerEx(hf, newPointer, NULL, FILE_BEGIN))
                throw std::exception("SetFilePointerEx False");

            b = WriteFile(
                hf,
                insString.c_str(),
                insString.length(),
                &insN,
                NULL
            );
            newPointer.QuadPart = insStart + insString.length();
            if (!SetFilePointerEx(hf, newPointer, NULL, FILE_BEGIN))
                throw std::exception("SetFilePointerEx False");
            b = WriteFile(
                hf,
                insInf.c_str(),
                insInf.length(),
                &n,
                NULL
            );
            break;
        }
        CloseHandle(hf);
    }
    catch (const char* em) {
        std::cout << "-- Error: " << em << " \n";
        return FALSE;
    }
    return TRUE;
}