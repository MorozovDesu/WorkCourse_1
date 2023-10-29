#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <locale>

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
        std::cout << "Данные сохранены в файле: " << filename << std::endl;
    }
    else {
        std::cout << "Ошибка при сохранении данных в файл." << std::endl;
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
            std::getline(iss, player.fullName, ',');
            std::getline(iss, player.dateOfBirth, ',');
            iss >> player.gamesPlayed >> player.goalsScored >> player.yellowCards >> player.redCards;
            players.push_back(player);
        }
        file.close();
        std::cout << "Данные загружены из файла: " << filename << std::endl;
    }
    else {
        std::cout << "Ошибка при загрузке данных из файла." << std::endl;
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
        std::cout << "Игрок #" << i << ": " << player.fullName << " (Дата рождения: "
            << player.dateOfBirth << ", Забитых мячей: " << player.goalsScored << ")" << std::endl;
    }
}

void sortPlayers(std::vector<Player>& players, int field) {
    switch (field) {
    case 1:
        std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.dateOfBirth < b.dateOfBirth;
            });
        break;
    case 2:
        std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.gamesPlayed > b.gamesPlayed;
            });
        break;
    case 3:
        std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.goalsScored > b.goalsScored;
            });
        break;
    case 4:
        std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return (a.yellowCards + a.redCards) > (b.yellowCards + b.redCards);
            });
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

int main() {
    std::locale::global(std::locale(""));
    std::vector<Player> players;
    std::string filename = "football_players.txt";



    loadDataFromFile(players, filename);
    std::string searchTerm;
    while (true) {
        std::cout << "\nВыберите действие:" << std::endl;
        std::cout << "1. Добавить игрока" << std::endl;
        std::cout << "2. Редактировать игрока" << std::endl;
        std::cout << "3. Удалить игрока" << std::endl;
        std::cout << "4. Вывести список игроков" << std::endl;
        std::cout << "5. Сортировать игроков" << std::endl;
        std::cout << "6. Поиск игроков" << std::endl;
        std::cout << "7. Сохранить данные" << std::endl;
        std::cout << "8. Выйти" << std::endl;

        int choice;
        std::cin >> choice;

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
            std::cout << "Данные сохранены. Программа завершена." << std::endl;
            return 0;
        default:
            std::cout << "Недопустимое действие." << std::endl;
            break;
        }
    }

    return 0;
}
