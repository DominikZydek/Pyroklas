#include <iostream>
#include "Game.h"

using namespace std;

int main() {
    Game game = Game();
    cout << endl;
    cout << "Witaj w grze Pyroklas" << endl;
    cout << "1. Nowa gra" << endl;
    cout << "2. Wczytaj gre" << endl;
    cout << "3. Wyjscie" << endl;
    cout << "Wybierz opcje: ";

    int option;
    string filename;
    cin >> option;

    switch (option) {
        case 1:
            cout << endl;
            game.setupGame();
            game.playGame();
            break;
        case 2:
            cout << endl;
            cout << "Podaj nazwe pliku: ";
            cin >> filename;
            game.loadGame(filename);
            game.worldMap.printMap();
            game.playGame();
            break;
        case 3:
            cout << "Do zobaczenia!" << endl;
            break;
        default:
            cout << "Niepoprawna opcja" << endl;
            break;
    }
    return 0;
}
