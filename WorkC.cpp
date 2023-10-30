#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <locale>
#include <regex>

struct Player {
    std::string fullName;
    std::string dateOfBirth;
    int gamesPlayed;
    int goalsScored;
    int yellowCards;
    int redCards;
};

void saveDataToFile(const std::vector<Player>& players, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const Player& player : players) {
            file << player.fullName << "," << player.dateOfBirth << "," << player.gamesPlayed
                << "," << player.goalsScored << "," << player.yellowCards << "," << player.redCards << "\n";
        }
        file.close();
        if (file.good()) {
            std::cout << "Данные успешно сохранены в файле: " << filename << std::endl;
        }
        else {
            std::cout << "Ошибка при сохранении данных в файл." << std::endl;
        }
    }
    else {
        std::cout << "Ошибка при открытии файла для записи." << std::endl;
    }
}

void loadDataFromFile(std::vector<Player>& players, const std::string& filename) {
    std::ifstream file(filename);
    players.clear();

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            Player player;
            std::istringstream iss(line);
            std::string fullName, dateOfBirth;
            if (std::getline(iss, fullName, ',')) {
                // Преобразуем имя игрока к нижнему регистру
                std::transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
                player.fullName = fullName;
            }
            if (std::getline(iss, dateOfBirth, ',')) {
                player.dateOfBirth = dateOfBirth;
            }
            if (iss >> player.gamesPlayed >> player.goalsScored >> player.yellowCards >> player.redCards) {
                players.push_back(player);
            }
            else {
                // Если считывание не удалось, устанавливаем значения в 0
                player.gamesPlayed = 0;
                player.goalsScored = 0;
                player.yellowCards = 0;
                player.redCards = 0;
                players.push_back(player);
            }
        }
        file.close();
        if (file.good()) {
            std::cout << "Данные успешно загружены из файла: " << filename << std::endl;
        }
        else {
            std::cout << "Ошибка при чтении данных из файла." << std::endl;
        }
    }
    else {
        std::cout << "Ошибка при открытии файла для чтения." << std::endl;
    }
}



void addPlayer(std::vector<Player>& players) {
    Player player;
    std::cin.ignore();
    std::cout << "Введите данные для нового игрока:" << std::endl;
    std::cout << "Ф.И.О. игрока: ";
    std::getline(std::cin, player.fullName);
    std::cout << "Дата рождения: ";
    std::getline(std::cin, player.dateOfBirth);
    std::cout << "Количество игр: ";
    std::cin >> player.gamesPlayed;
    std::cout << "Количество забитых мячей: ";
    std::cin >> player.goalsScored;
    std::cout << "Количество желтых карточек: ";
    std::cin >> player.yellowCards;
    std::cout << "Количество красных карточек: ";
    std::cin >> player.redCards;

    players.push_back(player);
    std::cout << "Игрок добавлен." << std::endl;
}

void editPlayer(std::vector<Player>& players) {
    int index;
    std::cout << "Введите индекс игрока, которого вы хотите отредактировать: ";
    std::cin >> index;

    if (index >= 0 && index < players.size()) {
        Player& player = players[index];
        std::cin.ignore();
        std::cout << "Введите новые данные для игрока #" << index << ":" << std::endl;
        std::cout << "Ф.И.О. игрока: ";
        std::getline(std::cin, player.fullName);
        std::cout << "Дата рождения: ";
        std::getline(std::cin, player.dateOfBirth);
        std::cout << "Количество игр: ";
        std::cin >> player.gamesPlayed;
        std::cout << "Количество забитых мячей: ";
        std::cin >> player.goalsScored;
        std::cout << "Количество желтых карточек: ";
        std::cin >> player.yellowCards;
        std::cout << "Количество красных карточек: ";
        std::cin >> player.redCards;
        std::cout << "Игрок отредактирован." << std::endl;
    }
    else {
        std::cout << "Индекс игрока недопустим." << std::endl;
    }
}

void deletePlayer(std::vector<Player>& players) {
    int index;
    std::cout << "Введите индекс игрока, которого вы хотите удалить: ";
    std::cin >> index;

    if (index >= 0 && index < players.size()) {
        players.erase(players.begin() + index);
        std::cout << "Игрок удален." << std::endl;
    }
    else {
        std::cout << "Индекс игрока недопустим." << std::endl;
    }
}

void printPlayers(const std::vector<Player>& players) {
    std::cout << "Список игроков:" << std::endl;
    for (int i = 0; i < players.size(); ++i) {
        const Player& player = players[i];
        int yellowCards = player.yellowCards < 0 ? 0 : player.yellowCards;
        int redCards = player.redCards < 0 ? 0 : player.redCards;
        std::cout << "Игрок #" << i << ": " << player.fullName << " (Дата рождения: "
            << player.dateOfBirth << ", Забитых мячей: " << player.goalsScored
            << ", Желтых карточек: " << yellowCards << ", Красных карточек: " << redCards << ")" << std::endl;
    }
}



void sortPlayers(std::vector<Player>& players, int field) {
    switch (field) {
    case 1:
        std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.dateOfBirth < b.dateOfBirth;
            });
        printPlayers(players);
        break;
    case 2:
        std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.gamesPlayed > b.gamesPlayed;
            });
        printPlayers(players);
        break;
    case 3:
        std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.goalsScored > b.goalsScored;
            });
        printPlayers(players);
        break;
    case 4:
        std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return (a.yellowCards + a.redCards) > (b.yellowCards + b.redCards);
            });
        printPlayers(players);
        break;
    default:
        std::cout << "Недопустимое поле сортировки." << std::endl;
        break;
    }
}

void searchPlayers(const std::vector<Player>& players, const std::string& searchTerm) {
    std::cout << "Результаты поиска:" << std::endl;
    for (int i = 0; i < players.size(); ++i) {
        const Player& player = players[i];
        if (player.fullName.find(searchTerm) != std::string::npos) {
            std::cout << "Игрок #" << i << ": " << player.fullName << " (Дата рождения: "
                << player.dateOfBirth << ", Забитых мячей: " << player.goalsScored << ")" << std::endl;
        }
    }
}

void displayAbout() {
    std::cout << "О программе:" << std::endl;
    std::cout << "Версия программы: 1.0" << std::endl;
    std::cout << "Дата последних изменений: 2023-10-30" << std::endl;
    std::cout << "Автор: Ваше имя и контактная информация" << std::endl;
}

void displayUserManual() {
    std::ifstream userManual("user_manual.txt");
    if (userManual.is_open()) {
        std::string line;
        while (std::getline(userManual, line)) {
            std::cout << line << std::endl;
        }
        userManual.close();
    }
    else {
        std::cout << "Ошибка при загрузке руководства пользователя." << std::endl;
    }
}

void displayTask() {
    std::cout << "Задание: ..." << std::endl;
}

void displayFootballMenu(std::vector<Player>& players, const std::string& filename) {
    std::string searchTerm;
    while (true) {
        std::cout << "\nМеню \"Работа с футболистами\":" << std::endl;
        std::cout << "1. Добавить игрока" << std::endl;
        std::cout << "2. Редактировать игрока" << std::endl;
        std::cout << "3. Удалить игрока" << std::endl;
        std::cout << "4. Вывести список игроков" << std::endl;
        std::cout << "5. Сортировать игроков" << std::endl;
        std::cout << "6. Поиск игроков" << std::endl;
        std::cout << "7. Сохранить данные" << std::endl;
        std::cout << "8. Вернуться в главное меню" << std::endl;

        int choice;
        std::cin >> choice;
        if (!(choice)) {
            std::cin.clear();  // Сбрасываем флаг ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Очищаем буфер ввода
            std::cout << "Ошибка ввода. Введите цифру." << std::endl;
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
                std::cout << "Выберите поле для сортировки (1-4):" << std::endl;
                std::cout << "1. Дата рождения" << std::endl;
                std::cout << "2. Количество игр" << std::endl;
                std::cout << "3. Количество забитых мячей" << std::endl;
                std::cout << "4. Количество карточек" << std::endl;
                int field;
                std::cin >> field;
                sortPlayers(players, field);
                break;
            case 6:
                std::cin.ignore();
                std::cout << "Введите строку для поиска: ";
                std::getline(std::cin, searchTerm);
                searchPlayers(players, searchTerm);
                break;
            case 7:
                saveDataToFile(players, filename);
                break;
            case 8:
                saveDataToFile(players, filename);
                std::cout << "Данные сохранены. Возвращение в главное меню." << std::endl;
                return;
            default:
                std::cout << "Недопустимое действие." << std::endl;
                break;
            }
        }
    }
}

int main() {
    std::locale::global(std::locale(""));
    std::vector<Player> players;
    std::string filename = "football_players.txt";

    loadDataFromFile(players, filename);

    while (true) {
        std::cout << "\nГлавное меню:" << std::endl;
        std::cout << "1. Работа с футболистами" << std::endl;
        std::cout << "2. О программе" << std::endl;
        std::cout << "3. Руководство пользователя" << std::endl;
        std::cout << "4. Задание" << std::endl;
        std::cout << "5. Выйти" << std::endl;

        int choice;
        std::cin >> choice;
        if (!(choice)) {
            std::cin.clear();  // Сбрасываем флаг ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Очищаем буфер ввода
            std::cout << "Ошибка ввода. Введите цифру." << std::endl;
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
                std::cout << "Программа завершена." << std::endl;
                return 0;
            default:
                std::cout << "Недопустимое действие." << std::endl;
                break;
            }
        }
    }

    return 0;
}
