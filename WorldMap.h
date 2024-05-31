#ifndef ZALICZENIE2_WORLDMAP_H
#define ZALICZENIE2_WORLDMAP_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Game.h"
#include "GameUtils.h"
#include "Dragon.h"

using namespace std;

class WorldMap {
public:
    char map[20][20] = {};
    int playerX = 10;
    int playerY = 10;
    int visitedFields = 0;
    vector<string> equipment;
    int health = 100;
    int armor = 0;
    int attack = 25;
    int movesTotal = 0;

    bool escapedDragon = false;

    void printMap() {
        // 'clear' console before printing the map
        GameUtils::clearConsole();
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                cout << map[i][j] << "  ";
            }
            cout << endl;
        }
    };

    void printEquipment() {
        cout << endl;
        cout << "Ekwipunek: ";
        for (int i = 0; i < equipment.size(); i++) {
            cout << equipment[i] << ", ";
        }
        cout << endl;
    }

    void printHealth() {
        cout << endl;
        cout << "Zdrowie: " << health << endl;
        cout << "Pancerz: " << armor << endl;
        cout << "Atak: " << attack << endl;
        cout << endl;
    }

    void generateMap() {
        // initialize the map with '.'
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                map[i][j] = '.';
            }
        }

        // place the player on the map at [10][10]
        map[playerX][playerY] = 'P';

        // randomly place 5 special fields marked as 'X'
        srand(time(0)); // Inicjalizacja generatora liczb losowych
        for (int i = 0; i < 5; i++) {
            int x, y;
            do {
                x = rand() % 20;
                y = rand() % 20;
            } while (map[x][y] != '.'); // make sure the field is empty
            map[x][y] = 'X';
        }

        // randomly place final field marked as '????'
        int x, y;
        bool placed = false;
        while (!placed) {
            x = rand() % 19; // 0-18 range to fit 2x2 area
            y = rand() % 19; // 0-18 range to fit 2x2 area

            // make sure the fields are empty
            if (map[x][y] == '.' && map[x+1][y] == '.' && map[x][y+1] == '.' && map[x+1][y+1] == '.') {
                map[x][y] = '?';
                map[x+1][y] = '?';
                map[x][y+1] = '?';
                map[x+1][y+1] = '?';
                placed = true;
            }
        }

        // randomly generate 3 dragons marked as 'S'
        for (int i = 0; i < 3; i++) {
            do {
                x = rand() % 20;
                y = rand() % 20;
            } while (map[x][y] != '.'); // make sure the field is empty
            map[x][y] = 'S';
        }

    };

    void movePlayer(int dx, int dy) {
        int newX = playerX + dx;
        int newY = playerY + dy;

        // check if the new position is within the map
        if (newX >= 0 && newX < 20 && newY >= 0 && newY < 20) {
            // check if the new position is not a special field or final field
            if (map[newX][newY] != 'X' && map[newX][newY] != '?') {
                int oldPlayerX = playerX;
                int oldPlayerY = playerY;
                map[playerX][playerY] = '.'; // remove player from the old position
                playerX = newX;
                playerY = newY;
                // check if the new position is a dragon
                if (map[playerX][playerY] == 'S') {
                    fightDragon();
                }
                if (!escapedDragon) {
                    map[playerX][playerY] = 'P'; // place player on the new position
                } else {
                    escapedDragon = false;
                    map[playerX][playerY] = 'S'; // place the dragon back on the map
                    playerX = oldPlayerX;
                    playerY = oldPlayerY;
                    map[playerX][playerY] = 'P'; // place player back on the map
                }
            } else if (map[newX][newY] == 'X') {
                enteredSpecialField();
                map[playerX][playerY] = '.'; // remove player from the old position
                playerX = newX;
                playerY = newY;
                map[playerX][playerY] = 'P'; // place player on the new position
            } else if (map[newX][newY] == '?') {
                enteredFinalField();
            }
            movesTotal++;
        }

        if (movesTotal % 5 == 0) {
            moveDragon();
        }
    }



    void moveDragon() {
        // find all fields with dragons
        vector<pair<int, int>> dragons;
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                if (map[i][j] == 'S') {
                    dragons.push_back({i, j});
                }
            }
        }

        // for each dragon, move it to a random empty field
        for (auto& dragon : dragons) {
            int x = dragon.first;
            int y = dragon.second;
            int newX, newY;
            do {
                int dx = rand() % 3 - 1; // randomize between -1, 0, 1
                int dy = rand() % 3 - 1; // randomize between -1, 0, 1
                newX = x + dx;
                newY = y + dy;
            } while (newX < 0 || newX >= 20 || newY < 0 || newY >= 20 || map[newX][newY] != '.');

            // move dragon to the new position
            map[x][y] = '.';
            map[newX][newY] = 'S';

            // check if the dragon is on the player's position
            if (newX == playerX && newY == playerY) {
                fightDragon();
            }
        }
    }

    void fightDragon() {
        Dragon dragon(0, 0);
        if (visitedFields < 3) {
            // initialize a dragon with lower health and attack
            dragon = Dragon(60, 15);
        } else {
            // initialize a dragon with higher health and attack
            dragon = Dragon(80, 30);
        }

        cout << endl;
        GameUtils::printWithDelay("Spotkales smoka!");
        GameUtils::waitForPlayerEnter();
        GameUtils::waitForPlayerEnter();

        while (dragon.health > 0 && health > 0) {
            cout << "Twoje zdrowie: " << health << endl;
            cout << "Zdrowie smoka: " << dragon.health << endl;
            cout << "1. Atakuj" << endl;
            cout << "2. Uciekaj" << endl;
            cout << "3. Uzyj przedmiotu" << endl;
            int choice;
            cin >> choice;
            if (choice == 1) {
                dragon.health -= attack;
                cout << "Zadano smokowi " << attack << " obrazen." << endl;
                int damage = 0;
                if (armor > dragon.attack) {
                    damage = 0;
                    health -= damage;
                } else {
                    damage = dragon.attack - armor;
                    health -= damage;
                }
                cout << "Smok zadal Tobie " << damage << " obrazen." << endl;
            } else if (choice == 2) {
                cout << "Uciekasz z pola walki." << endl;
                escapedDragon = true;
                GameUtils::waitForPlayerEnter();
                return;
            } else if (choice == 3) {
                if (equipment.size() > 0) {
                    cout << "Wybierz przedmiot do uzycia: " << endl;
                    for (int i = 0; i < equipment.size(); i++) {
                        cout << i + 1 << ". " << equipment[i] << endl;
                    }
                    int itemChoice;
                    cin >> itemChoice;
                    if (itemChoice > 0 && itemChoice <= equipment.size()) {
                        if (equipment[itemChoice - 1] == "Lek") {
                            health += 80;
                            cout << "Uzyles leku. Twoje zdrowie zostalo zwiekszone o 80." << endl;
                            equipment.erase(equipment.begin() + itemChoice - 1);
                        } else if (equipment[itemChoice - 1] == "Extinctio") {
                            dragon.health = 0;
                            cout << "Uzyles zaklecia 'Extinctio'. Smok zostal pokonany." << endl;
                            equipment.erase(equipment.begin() + itemChoice - 1);
                        }
                    }
                } else {
                    cout << "Nie masz zadnych przedmiotow." << endl;
                }
            }
        }
        if (health <= 0) { // player lost
            cout << "Zostales pokonany przez smoka." << endl;
            GameUtils::waitForPlayerEnter();
            exit(0);
        } else { // player won
            cout << "Pokonales smoka." << endl;
            map[playerX][playerY] = '.'; // Usuń smoka z mapy
            GameUtils::waitForPlayerEnter();
        }
    }


    void fightFinalBoss() {
        // initialize powerful dragon
        Dragon dragon(200, 75);

        cout << endl;
        GameUtils::printWithDelay("Wladca Zaru: Yfpn ojxyjx tibfesd?? Ernjwe xnj ej rsf! (Taki jestes odwazny?? Zmierz sie ze mna!)");
        GameUtils::waitForPlayerEnter();
        GameUtils::waitForPlayerEnter();

        while (dragon.health > 0 && health > 0) {
            cout << "Twoje zdrowie: " << health << endl;
            cout << "Zdrowie Wladcy Zaru: " << dragon.health << endl;
            cout << "1. Atakuj" << endl;
            cout << "2. Uciekaj" << endl;
            cout << "3. Uzyj przedmiotu" << endl;
            int choice;
            cin >> choice;
            if (choice == 1) {
                dragon.health -= attack;
                cout << "Zadano Wladcy Zaru " << attack << " obrazen." << endl;
                int damage = 0;
                if (armor > dragon.attack) {
                    damage = 0;
                    health -= damage;
                } else {
                    damage = dragon.attack - armor;
                    health -= damage;
                }
                cout << "Wladca Zaru zadal Tobie " << damage << " obrazen." << endl;
            } else if (choice == 2) {
                cout << "Uciekasz z pola walki." << endl;
                return;
            } else if (choice == 3) {
                if (equipment.size() > 0) {
                    cout << "Wybierz przedmiot do uzycia: " << endl;
                    for (int i = 0; i < equipment.size(); i++) {
                        cout << i + 1 << ". " << equipment[i] << endl;
                    }
                    int itemChoice;
                    cin >> itemChoice;
                    if (itemChoice > 0 && itemChoice <= equipment.size()) {
                        if (equipment[itemChoice - 1] == "Lek") {
                            health += 80;
                            cout << "Uzyles leku. Twoje zdrowie zostalo zwiekszone o 80." << endl;
                            equipment.erase(equipment.begin() + itemChoice - 1);
                        } else if (equipment[itemChoice - 1] == "Extinctio") {
                            GameUtils::printWithDelay("Wladca Zaru: Erfwstbfqjx efpqjhnj, lqzuhej! Ojxyjr sf snj egdy utyjesd! (Zmarnowales zaklecie, glupcze! Jestem na nie zbyt potezny!)");
                            equipment.erase(equipment.begin() + itemChoice - 1);
                        } else if (equipment[itemChoice - 1] == "Tajemnicze zaklecie") {
                            GameUtils::printWithDelay("Ty: Fuag! Lumes! Deg!");
                            equipment.erase(equipment.begin() + itemChoice - 1);
                            GameUtils::printWithDelay("Wladca Zaru: Ffff!!! Xpfi esfxe yj xqtbf? (Aaaa!!! Skad znasz te slowa?)");
                            dragon.health -= 100;
                        }
                    } else {
                        cout << "Nie masz zadnych przedmiotow." << endl;
                    }
                }
            }
            if (dragon.health <= 0) { // player won
                cout << "Pokonales Wladce Zaru." << endl;
                GameUtils::waitForPlayerEnter();
            } else if (health <= 0) { // player lost
                cout << "Zostales pokonany przez Wladce Zaru." << endl;
                GameUtils::waitForPlayerEnter();
                exit(0);
            }
        }
    }

    void enteredSpecialField() {
        // get a random number between 1 and 100 for case 2
        int bookNumber = rand() % 100 + 1;
        int guess = 0;


        cout << endl;
        switch(visitedFields) {
            case 0:
                cout << endl;
                GameUtils::printWithDelay("Spotkales Aptekarza.");
                cout << endl;
                GameUtils::printWithDelay("Aptekarz: Czesc, potrzebujesz czegos?");
                GameUtils::printWithDelay("Ty: Jestem strazakiem Brygady Skrzydla Ognia. Szukam Wladcy Zaru.");
                GameUtils::printWithDelay("Aptekarz: Wladca Zaru? Podobno ostatnio sie przebudzil. To okropne.");
                GameUtils::printWithDelay("Ty: Czy wiesz, gdzie go moge znalezc?");
                GameUtils::printWithDelay("Aptekarz: Nie, ale jest blizej niz nam sie wydaje.");
                GameUtils::printWithDelay("Ty: Hmm...");
                GameUtils::printWithDelay("Aptekarz: Skoro juz tu jestes, moze moge jakos Ci pomoc?");
                GameUtils::printWithDelay("Aptekarz: Mam dla ciebie lek, ktory przyda Ci sie w walce.");
                cout << endl;
                GameUtils::startSpecialColor();
                cout << "[Otrzymano przedmiot: 'Lek']" << endl;
                GameUtils::endSpecialColor();
                equipment.push_back("Lek");
                cout << endl;
                GameUtils::printWithDelay("Ty: Dziekuje, na pewno sie przyda.");
                GameUtils::printWithDelay("Aptekarz: Powodzenia, strazaku.");
                GameUtils::waitForPlayerEnter();
                break;
            case 1:
                cout << endl;
                GameUtils::printWithDelay("Spotkales Proroka.");
                cout << endl;
                GameUtils::printWithDelay("Prorok: Wiem kim jestes.");
                GameUtils::printWithDelay("Prorok: Wladca Zaru nie jest zwyklym bytem. To potwor, ktory zniszczy caly swiat.");
                GameUtils::printWithDelay("Prorok: Tylko Ty mozesz go powstrzymac.");
                GameUtils::printWithDelay("Ty: Proroku, jak moge go pokonac?");
                GameUtils::printWithDelay("Prorok: Na Twojej drodze napotkasz wiele groznych przeciwnikow.");
                GameUtils::printWithDelay("Prorok: Musisz uzyc sprytu i odwagi, aby ich pokonac.");
                GameUtils::printWithDelay("Prorok: Mam dla ciebie zaklecie, ktorym zgladzisz smoka ognia. Mozesz uzyc go tylko raz.");
                GameUtils::printWithDelay("Ty: Jak ono brzmi?");
                GameUtils::printWithDelay("Prorok: Nie moge wymowic jego slow, ale mam dla ciebie wskazowke.");
                GameUtils::printWithDelay("Prorok: Udaj sie w kolejne miejsce, znajdziesz tam biblioteke.");
                GameUtils::printWithDelay("Prorok: Strona 42, wiersz 3, slowo 5. Zapamietales?");
                GameUtils::waitForPlayerEnter();
                break;
            case 2:
                cout << endl;
                GameUtils::printWithDelay("Dotarles do biblioteki.");
                cout << endl;
                GameUtils::printWithDelay("Bibliotekarz: Witaj, czym moge sluzyc?");
                GameUtils::printWithDelay("Ty: Szukam ksiegi zaklec, o ktorej wspomnial Prorok.");
                GameUtils::printWithDelay("Bibliotekarz: Och, to pewnie chodzi o 'Zakazana Ksiege Magii'.");
                GameUtils::printWithDelay("Ty: Tak, to pewnie ona. Czy moge ja zobaczyc?");
                GameUtils::printWithDelay("Bibliotekarz: Niestety, jest schowana w piwnicy. Musisz podejsc do szefowej.");
                GameUtils::printWithDelay("Ty: Dziekuje, zrobie to.");
                cout << endl;
                GameUtils::printWithDelay("Szefowa: Kim jestes? Jestem zajeta.");
                GameUtils::printWithDelay("Ty: Jestem strazakiem Brygady Skrzydla Ognia. Szukam 'Zakazanej Ksiegi Magii'.");
                GameUtils::printWithDelay("Szefowa: To najcenniejszy skarb w naszej bibliotece. Dlaczego powinnam dac ja Tobie?");
                GameUtils::printWithDelay("Ty: Prorok powiedzial, ze musze ja miec, aby pokonac Wladce Zaru.");
                GameUtils::printWithDelay("Szefowa: Prorok? Hmm... Dobrze. Ale pamietaj, to nie jest zabawka.");
                GameUtils::printWithDelay("Szefowa: W piwnicy jest 100 ksiazek. Musisz sam ja znalezc.");
                GameUtils::printWithDelay("Szefowa: Tutaj masz klucz. Powodzenia.");
                cout << endl;
                GameUtils::printWithDelay("W piwnicy...");
                cout << endl;
                GameUtils::printWithDelay("Ty: Ale obrzydliwe miejsce. O moze to ona? Nie...");
                GameUtils::waitForPlayerEnter();

                while (guess != bookNumber) {
                    GameUtils::startSpecialColor();
                    cout << "Podaj liczbe od 1 do 100: ";
                    GameUtils::endSpecialColor();
                    cin >> guess;
                    if (guess < bookNumber) {
                        cout << "Za malo." << endl;
                    } else if (guess > bookNumber) {
                        cout << "Za duzo." << endl;
                    }
                }

                GameUtils::printWithDelay("Ty: To ona! 'Zakazana Ksiega Magii'.");
                GameUtils::printWithDelay("Ty: Tylko gdzie jest to zaklecie...");
                cout << endl;

                guess = 0;

                while (guess != 42) {
                    GameUtils::startSpecialColor();
                    cout << "Podaj numer strony: ";
                    GameUtils::endSpecialColor();
                    cin >> guess;
                    if (guess != 42) {
                        cout << "Zla strona." << endl;
                    }
                }

                guess = 0;

                while (guess != 3) {
                    GameUtils::startSpecialColor();
                    cout << "Podaj numer wiersza: ";
                    GameUtils::endSpecialColor();
                    cin >> guess;
                    if (guess != 3) {
                        cout << "Zly wiersz." << endl;
                    }
                }

                guess = 0;

                while (guess != 5) {
                    GameUtils::startSpecialColor();
                    cout << "Podaj numer slowa: ";
                    GameUtils::endSpecialColor();
                    cin >> guess;
                    if (guess != 5) {
                        cout << "Zle slowo." << endl;
                    }
                }

                cout << endl;
                GameUtils::printWithDelay("Ty: Jest! Znalazlem!");
                cout << endl;

                GameUtils::startSpecialColor();
                cout << "[Otrzymano zaklecie: 'Extinctio']" << endl;
                GameUtils::endSpecialColor();
                equipment.push_back("Extinctio");
                GameUtils::waitForPlayerEnter();
                break;
            case 3:
                cout << endl;
                GameUtils::printWithDelay("Dotarles do remizy strazackiej.");
                cout << endl;
                GameUtils::printWithDelay("Strazak: Witaj, kolego. Co cie do nas sprowadza?");
                GameUtils::printWithDelay("Ty: Jestem strazakiem Brygady Skrzydla Ognia. Szukam Wladcy Zaru.");
                GameUtils::printWithDelay("Strazak: Wladca Zaru? Ostatnio widzialem go w poblizu lasu.");
                GameUtils::printWithDelay("Strazak: Zreszta niewazne. Mam dla ciebie cos, co moze ci sie przydac.");
                GameUtils::printWithDelay("Strazak: Oto nasz specjalny pyroklaski stroj strazacki, ktory zabezpieczy cie przed obrazeniami.");
                cout << endl;
                GameUtils::startSpecialColor();
                cout << "[Zmiana statystyk: Pancerz +20]" << endl;
                GameUtils::endSpecialColor();
                armor += 20;
                cout << endl;
                GameUtils::printWithDelay("Ty: Dziekuje, na pewno sie przyda.");
                GameUtils::printWithDelay("Strazak: Jeszcze jedno, pamietaj, zeby nie ufac nikomu, zwlaszcza lesniczemu.");
                GameUtils::printWithDelay("Strazak: Krazy plotka, ze jest zwiazany z Wladca Zaru.");
                GameUtils::printWithDelay("Ty: Dziekuje za ostrzezenie.");
                GameUtils::printWithDelay("Strazak: Powodzenia, kolego.");
                GameUtils::waitForPlayerEnter();
                break;
            case 4:
                cout << endl;
                GameUtils::printWithDelay("Dotarles do lasu.");
                cout << endl;
                GameUtils::printWithDelay("Lesniczy: Witaj, podrozniku. Co cie tu sprowadza?");
                GameUtils::printWithDelay("Ty: Jestem strazakiem Brygady Skrzydla Ognia. Szukam Wladcy Zaru.");
                GameUtils::printWithDelay("Lesniczy: Wladca Zaru? Nie pozwole Ci go pokonac.");
                GameUtils::printWithDelay("Ty: Dlaczego?");
                GameUtils::printWithDelay("Lesniczy: Nie wszystko, czego ludzie sie boja, jest zle.");
                GameUtils::printWithDelay("Lesniczy: Jesli chcesz go pokonac, musisz pokonac mnie.");
                cout << endl;
                GameUtils::startSpecialColor();
                cout << "[Wydarzenie: Lesniczy zamienia sie w smoka lasu!]" << endl;
                GameUtils::endSpecialColor();
                cout << endl;
                GameUtils::printWithDelay("Lesniczy: Przygotuj sie na smierc, strazaku.");
                fightDragon();
                cout << endl;
                GameUtils::printWithDelay("Smok lasu: Nieee...");
                GameUtils::waitForPlayerEnter();
                break;
        }
        visitedFields++;
    }

    void enteredFinalField() {
        if (visitedFields == 5) {
            cout << endl;
            GameUtils::printWithDelay("Dotarles do kryjowki Wladcy Zaru.");
            cout << endl;
            GameUtils::printWithDelay("Wladca Zaru: Pnr ojxyjx n ofp xrnjxe sfwzxefh rto xutpto??");
            GameUtils::printWithDelay("Ty: Co? Nie rozumiem.");
            GameUtils::printWithDelay("Wladca Zaru: Utiifo xnj sjiesd xrnjwyjqsnpz!");
            GameUtils::waitForPlayerEnter();
            cout << endl;
            cout << "1. Uciekaj" << endl;
            cout << "2. Zajrzyj do 'Zakazanej Ksiegi Magii'" << endl;
            int choice;
            cin >> choice;
            if (choice == 1) {
                cout << endl;
                cout << "Uciekasz z kryjowki Wladcy Zaru." << endl;
                GameUtils::waitForPlayerEnter();
            } else if (choice == 2) {
                cout << endl;
                GameUtils::printWithDelay("Ty: Szybko, moze uda mi sie zrozumiec jego slowa.");
                usleep(2000000);
                GameUtils::printWithDelay("Ty: To moze byc to...");
                GameUtils::printWithDelay("Ty: Bqfiht Efwz, hed wtezrnjxe rtoj xqtbf? (Wladco Zaru, czy rozumiesz moje slowa?)");
                GameUtils::printWithDelay("Wladca Zaru: Ht? Xpfi esfxe rto uwfifbsd ojedp? (Co? Skad znasz moj pradawny jezyk?)");
                cout << endl;
                GameUtils::startSpecialColor();
                cout << "[Wydarzenie: Wladca Zaru uzywa niepooskromionej sily ognia i zamyka Cie w kregu plomieni]" << endl;
                GameUtils::endSpecialColor();
                cout << endl;
                GameUtils::printWithDelay("Ty: Nieee...");
                GameUtils::waitForPlayerEnter();
                cout << endl;
                cout << "1. Probuj gasic pozar" << endl;
                cout << "2. Szukaj odpowiedzi w 'Zakazanej Ksiedze Magii'" << endl;
                int choice;
                cin >> choice;
                if (choice == 1) {
                    cout << endl;
                    GameUtils::printWithDelay("Ty: Musze zgasic ten ogien.");
                    GameUtils::printWithDelay("Ty: Aaaaa...");
                    cout << endl;
                    health -= 50;
                    cout << "Zadano Ci 50 obrazen." << endl;
                    cout << "Twoje zdrowie: " << health << endl;
                    if (health <= 0) {
                        cout << "Zostales spalony przez plomienie Wladcy Zaru." << endl;
                        GameUtils::waitForPlayerEnter();
                        exit(0);
                    } else {
                        cout << "Udalo Ci sie zgasic ogien." << endl;
                        fightFinalBoss();
                    }
                } else if (choice == 2) {
                    cout << endl;
                    GameUtils::printWithDelay("Ty: Jest! Znalazlem odpowiedz!");
                    GameUtils::printWithDelay("Seari Cariso Non Continua Ex!");
                    cout << endl;
                    GameUtils::startSpecialColor();
                    cout << "[Wydarzenie: Ogien zostaje ugaszony zakleciem!]" << endl;
                    equipment.push_back("Tajemnicze zaklecie");
                    GameUtils::endSpecialColor();
                    fightFinalBoss();
                }
            }

        } else {
            cout << endl;
            GameUtils::printWithDelay("To nie jest jeszcze koniec Twojej podrozy.");
            GameUtils::waitForPlayerEnter();
        }
        return;
    }

};

#endif //ZALICZENIE2_WORLDMAP_H
