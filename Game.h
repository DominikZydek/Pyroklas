//
// Created by domin on 29.05.2024.
//

#ifndef ZALICZENIE2_GAME_H
#define ZALICZENIE2_GAME_H

#include <iostream>
#include <unistd.h>
#include <windows.h>
#include <fstream>
#include <limits>
#include "WorldMap.h"
#include "GameUtils.h"

using namespace std;

class Game {
public:
    WorldMap worldMap = WorldMap();

    void setupGame() {
        cout << endl;
        GameUtils::printWithDelay("Witaj.");
        GameUtils::printWithDelay("Miasto Pyroklas od wiekow zmaga sie z atakami smokow.");
        GameUtils::printWithDelay("Ostatnio jednak ataki staly sie coraz czestsze i grozniejsze");
        GameUtils::printWithDelay("Mieszkancy twierdza, ze przebudzila sie tajemnicza sila, znana jako 'Wladca Zaru'.");
        GameUtils::printWithDelay("Jako mlody i obiecujacy rekrut Brygady Skrzydla Ognia, zostales wybrany do odnalezienia i pokonania Wladcy Zaru.");
        cout << endl;
        GameUtils::printWithDelay("Ponizej znajdziesz mape miasta Pyroklas.");
        GameUtils::printWithDelay("Punkt oznaczony jako 'P' to Twoje polozenie.");
        GameUtils::printWithDelay("Punkty oznaczone jako 'X' to miejsca, w ktore musisz sie udac, aby rozwiklac zagadke Wladcy Zaru.");
        GameUtils::printWithDelay("Punkty oznaczone jako 'S' to grozne smoki, ktorym musisz stawic czola.");
        GameUtils::printWithDelay("Finalowa lokacja to pole 2x2 oznaczone znakami zapytania.");
        cout << endl;
        GameUtils::printWithDelay("Poruszanie: ");
        GameUtils::printWithDelay("W - gora");
        GameUtils::printWithDelay("S - dol");
        GameUtils::printWithDelay("A - lewo");
        GameUtils::printWithDelay("D - prawo");
        cout << endl;
        GameUtils::printWithDelay("Aby zakonczyc gre, wcisnij 'Q'.");
        cout << endl;
        GameUtils::printWithDelay("Aby zobaczyc ekwipunek, wcisnij 'E'.");
        cout << endl;
        GameUtils::printWithDelay("Aby sprawdzic swoje zdrowie, wcisnij 'H'.");
        cout << endl;
        GameUtils::printWithDelay("Powodzenia, rekrucie.");
        GameUtils::waitForPlayerEnter();

        worldMap.generateMap();
        worldMap.printMap();

    }

    void playGame() {
        while (true) {
            if (GameUtils::wasKeyPressed('W')) {
                worldMap.movePlayer(-1, 0);
                worldMap.printMap();
            }
            if (GameUtils::wasKeyPressed('S')) {
                worldMap.movePlayer(1, 0);
                worldMap.printMap();
            }
            if (GameUtils::wasKeyPressed('A')) {
                worldMap.movePlayer(0, -1);
                worldMap.printMap();
            }
            if (GameUtils::wasKeyPressed('D')) {
                worldMap.movePlayer(0, 1);
                worldMap.printMap();
            }
            if (GameUtils::wasKeyPressed('Q')) {
                cout << endl;
                GameUtils::waitForPlayerEnter();
                cout << "Czy na pewno chcesz zakonczyc gre? (T/N)" << endl;
                char answer;
                cin >> answer;
                if (answer == 'T' || answer == 't') {
                    cout << endl;
                    cout << "Czy chcesz zapisac gre? (T/N)" << endl;
                    cin >> answer;
                    if (answer == 'T' || answer == 't') {
                        cout << endl;
                        cout << "Podaj nazwe pliku: ";
                        string filename;
                        cin >> filename;
                        saveGame(filename);
                    }
                    cout << endl;
                    cout << "Dziekujemy za gre." << endl;
                    exit(0);
                } else {
                    GameUtils::waitForPlayerEnter();
                }
            }
            if (GameUtils::wasKeyPressed('E')) {
                worldMap.printEquipment();
            }
            if (GameUtils::wasKeyPressed('H')) {
                worldMap.printHealth();
            }
        }
    }

    void saveGame(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            // save map state
            for (int i = 0; i < 20; ++i) {
                for (int j = 0; j < 20; ++j) {
                    file << worldMap.map[i][j];
                }
                file << endl;
            }

            // save other information
            file << worldMap.playerX << ' ' << worldMap.playerY << ' ' << worldMap.visitedFields << endl;
            file << worldMap.health << ' ' << worldMap.armor << ' ' << worldMap.attack << endl;
            file << worldMap.movesTotal << endl;

            // save equipment
            file << worldMap.equipment.size() << endl;
            for (const auto& item : worldMap.equipment) {
                file << item << endl;
            }

            file.close();
        } else {
            cout << "Nie udalo sie otworzyc pliku do zapisu." << endl;
        }
    }

    void loadGame(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            // load map state
            for (int i = 0; i < 20; ++i) {
                for (int j = 0; j < 20; ++j) {
                    file >> worldMap.map[i][j];
                }
            }

            // load other information
            file >> worldMap.playerX >> worldMap.playerY >> worldMap.visitedFields;
            file >> worldMap.health >> worldMap.armor >> worldMap.attack;
            file >> worldMap.movesTotal;

            // load equipment
            size_t equipmentSize;
            file >> equipmentSize;
            worldMap.equipment.clear();
            for (size_t i = 0; i < equipmentSize; ++i) {
                string item;
                file >> item;
                worldMap.equipment.push_back(item);
            }

            file.close();
        } else {
            cout << "Nie udalo sie otworzyc pliku do wczytania." << endl;
            exit(0);
        }
    }

};


#endif //ZALICZENIE2_GAME_H
