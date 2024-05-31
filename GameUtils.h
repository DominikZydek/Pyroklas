
#ifndef ZALICZENIE2_GAMEUTILS_H
#define ZALICZENIE2_GAMEUTILS_H

#include <iostream>
#include <limits>

using namespace std;

class GameUtils {
public:
    static void printWithDelay(const char *text) {
        for (int i = 0; text[i] != '\0'; i++) {
            cout << text[i];
            usleep(10000);
        }
        cout << endl;
    }

    static bool wasKeyPressed(int key) {
        static bool keyPressed[256] = {false};
        bool isKeyPressed = (GetAsyncKeyState(key) & 0x8000) != 0; // 0x8000 - key is pressed

        if (isKeyPressed && !keyPressed[key]) {
            keyPressed[key] = true;
            return true;
        } else if (!isKeyPressed && keyPressed[key]) {
            keyPressed[key] = false;
        }

        return false;
    }

    static void clearConsole() {
        for (int i = 0; i < 50; ++i) {
            cout << endl;
        }
    }

    static void startSpecialColor() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 14);
    }

    static void endSpecialColor() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 7);
    }

    static void clearBufferAndWaitForEnter() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    static void waitForPlayerEnter() {
        cout << endl;
        startSpecialColor();
        cout << "Aby kontynuowac, wcisnij Enter..." << endl;
        endSpecialColor();
        clearBufferAndWaitForEnter();
        cout << endl;
    }
};


#endif //ZALICZENIE2_GAMEUTILS_H
