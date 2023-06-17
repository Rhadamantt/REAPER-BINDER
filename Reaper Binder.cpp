#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <shellapi.h>

const std::string FILE_PATH = "Saved.REAPER";
const std::string WEBSITE_URL = "https://discord.gg/wfw59te7z";

struct KeyBinding {
    int key;
    std::string text;
};

void SendText(const std::string& text) {
    std::vector<INPUT> inputs;

    for (char c : text) {
        INPUT inputDown;
        inputDown.type = INPUT_KEYBOARD;
        inputDown.ki.wVk = 0;
        inputDown.ki.wScan = c;
        inputDown.ki.dwFlags = KEYEVENTF_UNICODE;
        inputDown.ki.time = 0;
        inputDown.ki.dwExtraInfo = GetMessageExtraInfo();

        INPUT inputUp;
        inputUp.type = INPUT_KEYBOARD;
        inputUp.ki.wVk = 0;
        inputUp.ki.wScan = c;
        inputUp.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        inputUp.ki.time = 0;
        inputUp.ki.dwExtraInfo = GetMessageExtraInfo();

        inputs.push_back(inputDown);
        inputs.push_back(inputUp);
    }

    SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

bool ReadSavedData(std::string& login, std::string& password, std::string& gtaName, std::string& gtaSurname, std::string& gtaAge, std::vector<KeyBinding>& bindings) {
    std::ifstream file(FILE_PATH);
    if (!file) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("LOGIN: ") != std::string::npos) {
            login = line.substr(line.find("LOGIN: ") + 7);
        }
        else if (line.find("PASSWORD: ") != std::string::npos) {
            password = line.substr(line.find("PASSWORD: ") + 10);
        }
        else if (line.find("Name: ") != std::string::npos) {
            gtaName = line.substr(line.find("Name: ") + 6);
        }
        else if (line.find("Surname: ") != std::string::npos) {
            gtaSurname = line.substr(line.find("Surname: ") + 9);
        }
        else if (line.find("Age: ") != std::string::npos) {
            gtaAge = line.substr(line.find("Age: ") + 5);
        }
        else if (line.find("Binded: NUMPAD") != std::string::npos) {
            int key;
            std::string text;
            if (sscanf_s(line.c_str(), "Binded: NUMPAD%d %s", &key, text.data()) == 2) {
                bindings.push_back({ key, text });
            }
        }
    }

    return true;
}

void SaveData(const std::string& login, const std::string& password, const std::string& gtaName, const std::string& gtaSurname, const std::string& gtaAge, const std::vector<KeyBinding>& bindings) {
    std::ofstream file(FILE_PATH);
    if (!file) {
        std::cerr << "Не удалось открыть файл (возможно используется другим процессом)." << std::endl;
        return;
    }

    file << "LOGIN: " << login << '\n';
    file << "PASSWORD: " << password << '\n';
    file << "Name: " << gtaName << '\n';
    file << "Surname: " << gtaSurname << '\n';
    file << "Age: " << gtaAge << '\n';

    for (const KeyBinding& binding : bindings) {
        file << "Binded: NUMPAD" << binding.key << ' ' << binding.text << '\n';
    }

    file.close();
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT* keyboardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        switch (keyboardStruct->vkCode) {
        case VK_NUMPAD1: {
            std::string login, password, gtaName, gtaSurname, gtaAge;
            std::vector<KeyBinding> bindings;
            if (ReadSavedData(login, password, gtaName, gtaSurname, gtaAge, bindings)) {
                SendText(login);
            }
            return 1; 
        }
        case VK_NUMPAD2: {
            std::string login, password, gtaName, gtaSurname, gtaAge;
            std::vector<KeyBinding> bindings;
            if (ReadSavedData(login, password, gtaName, gtaSurname, gtaAge, bindings)) {
                SendText(password);
            }
            return 1;
        }
        case VK_NUMPAD4: {
            std::string login, password, gtaName, gtaSurname, gtaAge;
            std::vector<KeyBinding> bindings;
            if (ReadSavedData(login, password, gtaName, gtaSurname, gtaAge, bindings)) {
                SendText(gtaName);
            }
            return 1;
        }
        case VK_NUMPAD5: {
            std::string login, password, gtaName, gtaSurname, gtaAge;
            std::vector<KeyBinding> bindings;
            if (ReadSavedData(login, password, gtaName, gtaSurname, gtaAge, bindings)) {
                SendText(gtaSurname);
            }
            return 1;
        }
        case VK_NUMPAD6: {
            std::string login, password, gtaName, gtaSurname, gtaAge;
            std::vector<KeyBinding> bindings;
            if (ReadSavedData(login, password, gtaName, gtaSurname, gtaAge, bindings)) {
                SendText(gtaAge);
            }
            return 1;
        }
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    std::ifstream file(FILE_PATH);
    if (!file) {
        std::wstring wideUrl(WEBSITE_URL.begin(), WEBSITE_URL.end());
        ShellExecute(NULL, L"open", wideUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }

    std::string login, password, gtaName, gtaSurname, gtaAge;
    std::vector<KeyBinding> bindings;

    if (!ReadSavedData(login, password, gtaName, gtaSurname, gtaAge, bindings)) {
        std::cout << "Your login: ";
        std::getline(std::cin, login);

        std::cout << "Your password: ";
        std::getline(std::cin, password);

        std::cout << "Your GTA name: ";
        std::getline(std::cin, gtaName);

        std::cout << "Your GTA surname: ";
        std::getline(std::cin, gtaSurname);

        std::cout << "Your GTA age: ";
        std::getline(std::cin, gtaAge);

        SaveData(login, password, gtaName, gtaSurname, gtaAge, bindings);
    }

    system("cls");

    std::cout << "Created by REAPER Family / DISCORD: https://discord.gg/wfw59te7z\n" << std::endl;

    std::cout << "Login:    " << login << "         Binded:    NUMPAD1" << std::endl;
    std::cout << "Password: " << password << "      Binded:    NUMPAD2" << std::endl;
    std::cout << "Name:     " << gtaName << "       Binded:    NUMPAD4" << std::endl;
    std::cout << "Surname:  " << gtaSurname << "    Binded:    NUMPAD5" << std::endl;
    std::cout << "Age:      " << gtaAge << "        Binded:    NUMPAD6" << std::endl;

    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (hook == NULL) {
        std::cerr << "Не удалось назначить клавиши." << std::endl;
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);

    return 0;
}
