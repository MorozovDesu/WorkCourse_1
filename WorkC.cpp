#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <locale>
#include <regex>

using namespace std;

struct Player {
    string fullName;
    string dateOfBirth;
    int gamesPlayed;
    int goalsScored;
    int yellowCards;
    int redCards;
};

void saveDataToFile(const vector<Player>& players, const string& filename) {
    ::ofstream file(filename);
    if (file.is_open()) {
        for (const Player& player : players) {
            file << player.fullName << "," << player.dateOfBirth << "," << player.gamesPlayed
                << "," << player.goalsScored << "," << player.yellowCards << "," << player.redCards << "\n";
        }
        file.close();
        if (file.good()) {
            cout << "Данные успешно сохранены в файле: " << filename << endl;
        }
        else {
            cout << "Ошибка при сохранении данных в файл." << endl;
        }
    }
    else {
        cout << "Ошибка при открытии файла для записи." << endl;
    }
}

void split(const string& line, vector<string>& tokens, char delimiter) {
	istringstream iss(line);
    string token;
    while (getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
}

void loadDataFromFile(vector<Player>& players, const string& filename) {
    players.clear();
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Ошибка при открытии файла для чтения.");
    }

    string line;
    while (getline(file, line)) {
        Player player;
        vector<string> tokens;
        split(line, tokens, ',');

        if (tokens.size() >= 2) {
            player.fullName = tokens[0];
            transform(player.fullName.begin(), player.fullName.end(), player.fullName.begin(), ::tolower);
            player.dateOfBirth = tokens[1];
        }

        if (tokens.size() >= 6) {
            player.gamesPlayed = stoi(tokens[2]);
            player.goalsScored = stoi(tokens[3]);
            player.yellowCards = stoi(tokens[4]);
            player.redCards = stoi(tokens[5]);
        }
        else {
            player.gamesPlayed = 0;
            player.goalsScored = 0;
            player.yellowCards = 0;
            player.redCards = 0;
        }

        players.push_back(player);
    }

    file.close();

    if (!file.good()) {
        throw runtime_error("Ошибка при чтении данных из файла.");
    }

    cout << "Данные успешно загружены из файла: " << filename << endl;
}



void addPlayer(vector<Player>& players) {
    Player player;
    cin.ignore();
    cout << "Введите данные для нового игрока:" << endl;
    cout << "Ф.И.О. игрока: ";
    getline(cin, player.fullName);
    cout << "Дата рождения: ";
    getline(cin, player.dateOfBirth);
    cout << "Количество игр: ";
    cin >> player.gamesPlayed;
    cout << "Количество забитых мячей: ";
    cin >> player.goalsScored;
    cout << "Количество желтых карточек: ";
    cin >> player.yellowCards;
    cout << "Количество красных карточек: ";
    cin >> player.redCards;

    players.push_back(player);
    cout << "Игрок добавлен." << endl;
}

void editPlayer(vector<Player>& players) {
    int index;
    cout << "Введите индекс игрока, которого вы хотите отредактировать: ";
    cin >> index;

    if (index >= 0 && index < players.size()) {
        Player& player = players[index];
        cin.ignore();
        cout << "Введите новые данные для игрока #" << index << ":" << endl;
        cout << "Ф.И.О. игрока: ";
        getline(cin, player.fullName);
        cout << "Дата рождения: ";
        getline(cin, player.dateOfBirth);
        cout << "Количество игр: ";
        cin >> player.gamesPlayed;
        cout << "Количество забитых мячей: ";
        cin >> player.goalsScored;
        cout << "Количество желтых карточек: ";
        cin >> player.yellowCards;
        cout << "Количество красных карточек: ";
        cin >> player.redCards;
        cout << "Игрок отредактирован." << endl;
    }
    else {
        cout << "Индекс игрока недопустим." << endl;
    }
}

void deletePlayer(vector<Player>& players) {
    int index;
    cout << "Введите индекс игрока, которого вы хотите удалить: ";
    cin >> index;

    if (index >= 0 && index < players.size()) {
        players.erase(players.begin() + index);
        cout << "Игрок удален." << endl;
    }
    else {
        cout << "Индекс игрока недопустим." << endl;
    }
}

void printPlayers(const vector<Player>& players) {
    cout << "Список игроков:" << endl;
    for (int i = 0; i < players.size(); ++i) {
        const Player& player = players[i];
        int yellowCards = player.yellowCards < 0 ? 0 : player.yellowCards;
        int redCards = player.redCards < 0 ? 0 : player.redCards;
        cout << "Игрок #" << i << ": " << player.fullName << " (Дата рождения: "
            << player.dateOfBirth << ", Забитых мячей: " << player.goalsScored
            << ", Желтых карточек: " << yellowCards << ", Красных карточек: " << redCards << ")" << endl;
    }
}



void sortPlayers(vector<Player>& players, int field) {
    switch (field) {
    case 1:
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.dateOfBirth < b.dateOfBirth;
            });
        printPlayers(players);
        break;
    case 2:
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.gamesPlayed > b.gamesPlayed;
            });
        printPlayers(players);
        break;
    case 3:
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.goalsScored > b.goalsScored;
            });
        printPlayers(players);
        break;
    case 4:
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return (a.yellowCards + a.redCards) > (b.yellowCards + b.redCards);
            });
        printPlayers(players);
        break;
    default:
        cout << "Недопустимое поле сортировки." << endl;
        break;
    }
}

void searchPlayers(const vector<Player>& players, const string& searchTerm) {
    cout << "Результаты поиска:" << endl;
    for (int i = 0; i < players.size(); ++i) {
        const Player& player = players[i];
        if (player.fullName.find(searchTerm) != string::npos) {
            cout << "Игрок #" << i << ": " << player.fullName << " (Дата рождения: "
                << player.dateOfBirth << ", Забитых мячей: " << player.goalsScored << ")" << endl;
        }
    }
}

void displayAbout() {
    cout << "О программе:" << endl;
    cout << "Версия программы: 1.0" << endl;
    cout << "Дата последних изменений: 2023-10-30" << endl;
    cout << "Автор: Ваше имя и контактная информация" << endl;
}

void displayUserManual() {
    ifstream userManual("user_manual.txt");
    if (userManual.is_open()) {
        string line;
        while (getline(userManual, line)) {
            cout << line << endl;
        }
        userManual.close();
    }
    else {
        cout << "Ошибка при загрузке руководства пользователя." << endl;
    }
}

void displayTask() {
    cout << "Задание: ..." << endl;
}

void displayFootballMenu(vector<Player>& players, const string& filename) {
    string searchTerm;
    while (true) {
        cout << "\nМеню \"Работа с футболистами\":" << endl;
        cout << "1. Добавить игрока" << endl;
        cout << "2. Редактировать игрока" << endl;
        cout << "3. Удалить игрока" << endl;
        cout << "4. Вывести список игроков" << endl;
        cout << "5. Сортировать игроков" << endl;
        cout << "6. Поиск игроков" << endl;
        cout << "7. Сохранить данные" << endl;
        cout << "8. Вернуться в главное меню" << endl;

        int choice;
        cin >> choice;
        if (!(choice)) {
            cin.clear();  // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
        else {
            switch (choice) {
            case 1:
                addPlayer(players);
                break;
            case 2:
                editPlayer(players);
                break;
            case 3:
                deletePlayer(players);
                break;
            case 4:
                printPlayers(players);
                break;
            case 5:
                cout << "Выберите поле для сортировки (1-4):" << endl;
                cout << "1. Дата рождения" << endl;
                cout << "2. Количество игр" << endl;
                cout << "3. Количество забитых мячей" << endl;
                cout << "4. Количество карточек" << endl;
                int field;
                cin >> field;
                sortPlayers(players, field);
                break;
            case 6:
                cin.ignore();
                cout << "Введите строку для поиска: ";
                getline(cin, searchTerm);
                searchPlayers(players, searchTerm);
                break;
            case 7:
                saveDataToFile(players, filename);
                break;
            case 8:
                saveDataToFile(players, filename);
                cout << "Данные сохранены. Возвращение в главное меню." << endl;
                return;
            default:
                cout << "Недопустимое действие." << endl;
                break;
            }
        }
    }
}

int main() {
    locale::global(locale(""));
    vector<Player> players;
    const string filename = "players.csv";
    try {
        loadDataFromFile(players, filename);
    }
    catch (const exception& e) {
        cerr << "Ошибка тут: " << e.what() << endl;
    }

    while (true) {
        cout << "\nГлавное меню:" << endl;
        cout << "1. Работа с футболистами" << endl;
        cout << "2. О программе" << endl;
        cout << "3. Руководство пользователя" << endl;
        cout << "4. Задание" << endl;
        cout << "5. Выйти" << endl;

        int choice;
        cin >> choice;
        if (!(choice)) {
            cin.clear();  // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
        else {
            switch (choice) {
            case 1:
                displayFootballMenu(players, filename);
                break;
            case 2:
                displayAbout();
                break;
            case 3:
                displayUserManual();
                break;
            case 4:
                displayTask();
                break;
            case 5:
                cout << "Программа завершена." << endl;
                return 0;
            default:
                cout << "Недопустимое действие." << endl;
                break;
            }
        }
    }

    return 0;
}
