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

bool isValidDate(const string& date) {
    // Проверка даты с использованием регулярного выражения
    regex datePattern(R"(\d{2}\.\d{2}\.\d{4})");
    if (!regex_match(date, datePattern)) {
        return false;
    }

    // Разбиваем дату на день, месяц и год
    istringstream dateStream(date);
    string dayStr, monthStr, yearStr;
    getline(dateStream, dayStr, '.');
    getline(dateStream, monthStr, '.');
    getline(dateStream, yearStr, '.');

    int day = stoi(dayStr);
    int month = stoi(monthStr);
    int year = stoi(yearStr);

    // Проверяем диапазоны для дня, месяца и года
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2023) {
        return false;
    }
    if (month == 2) {
        // Проверяем високосный год
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        // Дополнительная проверка для февраля в зависимости от високосности
        if ((isLeapYear && (day < 1 || day > 29)) || (!isLeapYear && (day < 1 || day > 28))) {
            return false;
        }
    }
    return true;
}

bool isLatinCharacters(const string& text) {
    // Проверка, что текст состоит только из латинских букв
    regex latinPattern(R"([a-zA-Z\s]+)");
    return regex_match(text, latinPattern);
}

void addPlayer(vector<Player>& players) {
    Player player;
    cin.ignore();
    cout << "Введите данные для нового игрока:" << endl;
    // Ввод Ф.И.О. игрока
    string fullName;
    do {
        cout << "Ф.И.О. игрока (латинскими буквами): ";
        getline(cin, fullName);
    } while (!isLatinCharacters(fullName));
    player.fullName = fullName;
    // Ввод даты рождения
    string dateOfBirth;
    do {
        cout << "Дата рождения (в формате дд.мм.гггг): ";
        getline(cin, dateOfBirth);
    } while (!isValidDate(dateOfBirth));
    player.dateOfBirth = dateOfBirth;
    // Ввод количества игр (только цифры)
    do {
        cout << "Количество игр: ";
        cin >> player.gamesPlayed;
        if (cin.fail()) {
            cin.clear();  // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
    } while (player.gamesPlayed < 0 || player.gamesPlayed > 10000);
    // Ввод количества забитых мячей (только цифры)
    do {
        cout << "Количество забитых мячей: ";
        cin >> player.goalsScored;
        if (cin.fail()) {
            cin.clear();  // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
    } while (player.goalsScored < 0 || player.goalsScored > 10000);
    // Ввод количества желтых карточек (только цифры)
    do {
        cout << "Количество желтых карточек: ";
        cin >> player.yellowCards;
        if (cin.fail()) {
            cin.clear();  // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
    } while (player.yellowCards < 0 || player.yellowCards > 20000);
    // Ввод количества красных карточек (только цифры)
    do {
        cout << "Количество красных карточек (не больше количества игр " << player.gamesPlayed << "): ";
        cin >> player.redCards;
        if (cin.fail()) {
            cin.clear();  // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
    } while (player.redCards < 0 || player.redCards > player.gamesPlayed);

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

void splitDateOfBirth(const string& dateOfBirth, int& day, int& month, int& year) {
    stringstream ss(dateOfBirth);
    string token;
    getline(ss, token, '.');
    day = stoi(token);
    getline(ss, token, '.');
    month = stoi(token);
    getline(ss, token, '.');
    year = stoi(token);
}

bool compareDateOfBirth(const Player& a, const Player& b) {
    int day1, month1, year1;
    int day2, month2, year2;
    splitDateOfBirth(a.dateOfBirth, day1, month1, year1);
    splitDateOfBirth(b.dateOfBirth, day2, month2, year2);

    if (year1 != year2) {
        return year1 < year2;
    }
    else if (month1 != month2) {
        return month1 < month2;
    }
    else {
        return day1 < day2;
    }
}


void sortPlayers(vector<Player>& players, int field) {
    switch (field) {
    case 1:
        sort(players.begin(), players.end(), compareDateOfBirth);
        printPlayers(players);
        break;
    case 2:
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.goalsScored > b.goalsScored;
            });
        printPlayers(players);
        break;
    case 3:
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
    cout << "Версия программы: 3.3" << endl;
    cout << "Дата последних изменений: 2023-11-12" << endl;
    cout << "Автор: Морозов Дмитрий Игоревич,https://github.com/MorozovDesu " << endl;
}

void displayUserManual() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
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
    locale::global(locale(""));
}

void displayTask() {
    cout << "Составить программу, обрабатывающую сведения об игроках фут-больной команды." << endl;
    cout << "Структура исходных данных: Ф.И.О. игрока, дата рожде-ния, количество игр, количество забитых мячей, количество карточек (жел-тых и красных)." << endl;
    cout << "Вывести список лучших бомбардиров (первая пятерка)."<< endl;
    cout << "Найти всех игроков, младше 20 лет, забивших более 2 голов." << endl;
    cout << "Определить самого недисциплинированного игрока команды." << endl;
}

bool isYoungScorer(const Player& player) {
    // Проверка на возраст младше 20 и забитые мячи более 2
    int currentYear = 2023; // Замените на текущий год, если необходимо
    int birthYear = stoi(player.dateOfBirth.substr(6));
    int age = currentYear - birthYear;

    return (age < 20 && player.goalsScored > 2);
}

bool endsWithCsv(const string& filename) {
    // Проверка, что имя файла оканчивается на ".csv"
    size_t pos = filename.find_last_of('.');
    if (pos != string::npos) {
        string extension = filename.substr(pos + 1);
        transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        return extension == "csv";
    }
    return false;
}

void displayTopScorers(const vector<Player>& players) {
    vector<Player> topScorers = players;
    sort(topScorers.begin(), topScorers.end(), [](const Player& a, const Player& b) {
        return a.goalsScored > b.goalsScored;
        });

    cout << "Топ-5 бомбардиров:" << endl;
    for (int i = 0; i < min(5, static_cast<int>(topScorers.size())); ++i) {
        const Player& player = topScorers[i];
        cout << "Игрок #" << i + 1 << ": " << player.fullName << " (Забитых мячей: " << player.goalsScored << ")" << endl;
    }
}

void displayMostUndisciplinedPlayer(const vector<Player>& players) {
    int maxCards = -1;
    const Player* mostUndisciplinedPlayer = nullptr;

    for (const Player& player : players) {
        int totalCards = player.yellowCards + (2 * player.redCards); // Modify the calculation here
        if (totalCards > maxCards) {
            maxCards = totalCards;
            mostUndisciplinedPlayer = &player;
        }
    }

    if (mostUndisciplinedPlayer) {
        cout << "Самый недисциплинированный игрок:" << endl;
        cout << "Игрок: " << mostUndisciplinedPlayer->fullName << " (Желтых карточек: " << mostUndisciplinedPlayer->yellowCards
            << ", Красных карточек: " << mostUndisciplinedPlayer->redCards << ")" << endl;
    }
    else {
        cout << "Нет информации о недисциплинированных игроках." << endl;
    }
}

void findYoungHighScorers(const vector<Player>& players) {
    cout << "Игроки младше 20 лет, забившие более 2 голов:" << endl;
    int currentYear = 2023;

    for (const Player& player : players) {
        int birthYear = stoi(player.dateOfBirth.substr(6));
        int age = currentYear - birthYear;

        if (age < 20 && player.goalsScored > 2) {
            cout << "Игрок: " << player.fullName << " (Возраст: " << age << " лет, Забитых мячей: " << player.goalsScored << ")" << endl;
        }
    }
}

void displayFootballMenu(vector<Player>& players, const string& filename) {
    string searchTerm;
    string newFilename;
    while (true) {
        cout << "\nМеню \"Работа с футболистами\":" << endl;
        cout << "1. Добавить игрока" << endl;
        cout << "2. Редактировать игрока" << endl;
        cout << "3. Удалить игрока" << endl;
        cout << "4. Вывести список игроков" << endl;
        cout << "5. Сортировать игроков" << endl;
        cout << "6. Поиск игроков" << endl;
        cout << "7. Вывести топ-5 бомбардиров" << endl;
        cout << "8. Вывести самого недисциплинированного игрока" << endl;
        cout << "9. Найти всех игроков, младше 20 лет, забивших более 2 голов" << endl;
        cout << "10. Сохранить данные в текущем файле (CSV формат)" << endl;
        cout << "11. Сохранить данные в другом файле (CSV формат)" << endl;
        cout << "12. Вернуться в главное меню" << endl;

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
                cout << "2. Количество забитых голов" << endl;
                cout << "3. Количество карточек" << endl;
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
                displayTopScorers(players);
                break;
            case 8:
                displayMostUndisciplinedPlayer(players);
                break;
            case 9:
                findYoungHighScorers(players);
                break;
            case 10:
                if (endsWithCsv(filename)) {
                    saveDataToFile(players, filename);
                    cout << "Данные успешно сохранены в текущем файле: " << filename << endl;
                }
                else {
                    cout << "Ошибка: Файл должен иметь расширение .csv для сохранения." << endl;
                }
                break;
            case 11:
                cin.ignore();
                cout << "Введите имя нового файла для сохранения данных: ";
                getline(cin, newFilename);
                if (endsWithCsv(newFilename)) {
                    saveDataToFile(players, newFilename);
                    cout << "Данные успешно сохранены в новом файле: " << newFilename << endl;
                }
                else {
                    cout << "Ошибка: Файл должен иметь расширение .csv для сохранения." << endl;
                }
                break;
            case 12:
                cout << "Возвращение в главное меню." << endl;
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
    bool dataSaved = false;
    try {
        loadDataFromFile(players, filename);
    }
    catch (const exception& e) {
        //cerr << "Ошибка тут: " << e.what() << endl;
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
                if (!dataSaved) {
                    char saveChoice;
                    cout << "Данные не были сохранены. Сохранить перед выходом? (y/n): ";
                    cin >> saveChoice;
                    if (saveChoice == 'y' || saveChoice == 'Y') {
                        saveDataToFile(players, filename);
                        cout << "Данные успешно сохранены перед выходом." << endl;
                    }
                }
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
