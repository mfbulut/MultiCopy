#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <array>
#include <string>
#include <iostream>

std::string GetClipboardText()
{
    OpenClipboard(nullptr);

    HANDLE hData = GetClipboardData(CF_TEXT);
    std::string text(static_cast<char*>(GlobalLock(hData)));

    GlobalUnlock(hData);
    CloseClipboard();

    return text;
}

void SetClipboardText(std::string data)
{
    const char* output = data.c_str();
    const size_t len = strlen(output) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

std::array<std::string, 10> copyPastes;

int main()
{
    do
    {
        if ((GetKeyState(VK_MENU) & 0x8000))
        {
            for (int i = 0; i < 10; i++)
            {
                if ((GetKeyState(0x30 + i) & 0x8000) || (GetKeyState(0x60 + i) & 0x8000))
                {
                    copyPastes[i] = GetClipboardText();
                    Beep(750, 100);
                }
            }
        }

        if ((GetKeyState(VK_CONTROL) & 0x8000))
        {
            for (int i = 0; i < 10; i++)
            {
                if ((GetKeyState(0x30 + i) & 0x8000) || (GetKeyState(0x60 + i) & 0x8000))
                {
                    SetClipboardText(copyPastes[i]);
                    Beep(500, 100);
                }
            }
        }

        if ((GetKeyState(VK_RCONTROL) & 0x8000))
        {
            for (int i = 0; i < 10; i++)
            {
                std::cout << i << ": " << copyPastes[i] << std::endl;
            }
            std::cout << std::endl;
        }

        Sleep(100);
    } while (!GetAsyncKeyState(VK_ESCAPE));
}

