#include <iostream>
#include <Windows.h>

using namespace std;

bool printFileTxt(LPWSTR FileName) {
    HANDLE hf = CreateFile(FileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hf == INVALID_HANDLE_VALUE)
        return false;

    DWORD n = NULL;
    char buf[2048];

    ZeroMemory(buf, sizeof(buf));
    bool readFile = ReadFile(hf, &buf, sizeof(buf), &n, NULL);

    std::cout << "File Info:" << endl << buf << std::endl;
    CloseHandle(hf);
}

BOOL delRowFileTxt(LPWSTR FileName, DWORD row)
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
        std::cout << "-- Create file or open succesfull\n";
        DWORD n = NULL;
        char rbuf[1024];

        ZeroMemory(rbuf, sizeof(rbuf));
        BOOL b = ReadFile(
            hf,
            &rbuf,
            sizeof(rbuf),
            &n,
            NULL);
        char wbuf[1024];
        ZeroMemory(wbuf, sizeof(wbuf));
        if (!b) throw "read file failed";
        int delStart = 0;
        int rbufpos = 0;
        int wbytes = 0;

        int suchRow = 1;
        bool isFinish = false;
        bool isWrite = false;
        if (row == 1) {
            isWrite = true;
            rbufpos++;
            while (rbuf[rbufpos] != '\n' || rbuf[rbufpos] == '\0') {
                rbufpos++;
            }
        }
        while (rbuf[rbufpos] != '\0') {
            if (rbuf[rbufpos] == '\n') {
                suchRow++;
                if (suchRow == row) {
                    isWrite = true;
                    rbufpos++;
                    delStart = rbufpos;
                    while (rbuf[rbufpos] != '\n') {
                        if (rbuf[rbufpos] == '\0') {
                            isFinish = true;
                            break;
                        }
                        rbufpos++;
                    }
                    if (isFinish) {
                        break;
                    }
                }
            }
            rbufpos++;
            if (isWrite && rbuf[rbufpos] != '\0') {
                wbuf[wbytes] = rbuf[rbufpos];
                wbytes++;
            }
        }


        LARGE_INTEGER newPointer;

        newPointer.QuadPart = delStart;

        if (!SetFilePointerEx(hf, newPointer, NULL, FILE_BEGIN))
            throw std::exception("SetFilePointerEx False");

        b = WriteFile(
            hf,
            wbuf,
            wbytes,
            &n,
            NULL
        );
        if (wbytes == 0) {
            return false;
        }
        if (isFinish) {
            newPointer.QuadPart = newPointer.QuadPart - 2;
            if (!SetFilePointerEx(hf, newPointer, NULL, FILE_BEGIN))
                throw std::exception("SetFilePointerEx False");
        }

        if (row <= suchRow && row > 0) {
            SetEndOfFile(hf);
        }

        CloseHandle(hf);
        return true;
    }
    catch (const char* em) {
        std::cout << "-- Error: " << em << " \n";
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    LPCWSTR path = L"D:\\Danil\\university\\OS\\2s\\Labs\\Lab1\\OS09_01.txt";
    printFileTxt(LPWSTR(path));

    delRowFileTxt(LPWSTR(path), 1);
    delRowFileTxt(LPWSTR(path), 3);
    delRowFileTxt(LPWSTR(path), 8);
    delRowFileTxt(LPWSTR(path), 10);

    printFileTxt(LPWSTR(path));
}