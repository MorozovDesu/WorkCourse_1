#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <locale>
#include <regex>

using namespace std;

// Определение структуры Player для хранения данных об игроке
struct Player {
    string fullName;      // Полное имя игрока
    string dateOfBirth;   // Дата рождения игрока
    int gamesPlayed;      // Количество сыгранных игр
    int goalsScored;      // Количество забитых мячей
    int yellowCards;      // Количество желтых карточек
    int redCards;         // Количество красных карточек
};

// Функция для сохранения данных в файл
void saveDataToFile(const vector<Player>& players, const string& filename) {
    // Открытие файла для записи
    ::ofstream file(filename);
    if (file.is_open()) {
        // Запись данных каждого игрока в файл
        for (const Player& player : players) {
            file << player.fullName << "," << player.dateOfBirth << "," << player.gamesPlayed
                << "," << player.goalsScored << "," << player.yellowCards << "," << player.redCards << "\n";
        }
        // Закрытие файла после записи
        file.close();
        // Проверка успешности записи в файл
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

// Функция для разделения строки на подстроки с использованием разделителя
void split(const string& line, vector<string>& tokens, char delimiter) {
    istringstream iss(line);
    string token;
    // Извлечение подстрок из строки с использованием заданного разделителя
    while (getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
}

// Функция для загрузки данных из файла
void loadDataFromFile(vector<Player>& players, const string& filename) {
    // Очистка вектора перед загрузкой новых данных
    players.clear();
    // Открытие файла для чтения
    ifstream file(filename);

    // Проверка успешности открытия файла
    if (!file.is_open()) {
        throw runtime_error("Ошибка при открытии файла для чтения.");
    }

    string line;
    // Чтение данных из файла построчно
    while (getline(file, line)) {
        Player player;
        vector<string> tokens;
        // Разделение строки на подстроки с использованием запятой в качестве разделителя
        split(line, tokens, ',');

        // Заполнение структуры Player данными из подстрок
        if (tokens.size() >= 2) {
            player.fullName = tokens[0];
            // Преобразование имени в нижний регистр
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
            // Установка значений по умолчанию, если данные не полные
            player.gamesPlayed = 0;
            player.goalsScored = 0;
            player.yellowCards = 0;
            player.redCards = 0;
        }

        // Добавление игрока в вектор
        players.push_back(player);
    }

    // Закрытие файла после чтения
    file.close();

    // Проверка успешности чтения из файла
    if (!file.good()) {
        throw runtime_error("Ошибка при чтении данных из файла.");
    }

    cout << "Данные успешно загружены из файла: " << filename << endl;
}

// Функция для проверки корректности даты
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

// Функция для проверки, что текст состоит только из латинских букв
bool isLatinCharacters(const string& text) {
    regex latinPattern(R"([a-zA-Z\s]+)");
    return regex_match(text, latinPattern);
}

// Функция для добавления нового игрока
void addPlayer(vector<Player>& players) {
    Player player;
    // Очистка буфера ввода
    cin.ignore();
    cout << "Введите данные для нового игрока:" << endl;
    // Ввод Ф.И.О. игрока (латинскими буквами)
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
            cin.clear();  // Сброс флага ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очистка буфера ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
    } while (player.gamesPlayed < 0 || player.gamesPlayed > 10000);
    // Ввод количества забитых мячей (только цифры)
    do {
        cout << "Количество забитых мячей: ";
        cin >> player.goalsScored;
        if (cin.fail()) {
            cin.clear();  // Сброс флага ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очистка буфера ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
    } while (player.goalsScored < 0 || player.goalsScored > 10000);
    // Ввод количества желтых карточек (только цифры)
    do {
        cout << "Количество желтых карточек: ";
        cin >> player.yellowCards;
        if (cin.fail()) {
            cin.clear();  // Сброс флага ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очистка буфера ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
    } while (player.yellowCards < 0 || player.yellowCards > 20000);
    // Ввод количества красных карточек (только цифры)
    do {
        cout << "Количество красных карточек (не больше количества игр " << player.gamesPlayed << "): ";
        cin >> player.redCards;
        if (cin.fail()) {
            cin.clear();  // Сброс флага ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очистка буфера ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
    } while (player.redCards < 0 || player.redCards > player.gamesPlayed);

    // Добавление игрока в вектор
    players.push_back(player);
    cout << "Игрок добавлен." << endl;
}

// Функция для редактирования данных об игроке
void editPlayer(vector<Player>& players) {
    int index;
    cout << "Введите индекс игрока, которого вы хотите отредактировать: ";
    cin >> index;

    // Проверка валидности индекса
    if (index >= 0 && index < players.size()) {
        Player& player = players[index];
        // Очистка буфера ввода
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

// Функция для удаления игрока из вектора
void deletePlayer(vector<Player>& players) {
    int index;
    cout << "Введите индекс игрока, которого вы хотите удалить: ";
    cin >> index;

    // Проверка валидности индекса
    if (index >= 0 && index < players.size()) {
        // Удаление игрока из вектора
        players.erase(players.begin() + index);
        cout << "Игрок удален." << endl;
    }
    else {
        cout << "Индекс игрока недопустим." << endl;
    }
}

// Функция вывода информации о игроках в консоль
void printPlayers(const vector<Player>& players) {
    // Итерируем по вектору игроков и выводим информацию о каждом
    cout << "Список игроков:" << endl;
    for (int i = 0; i < players.size(); ++i) {
        const Player& player = players[i];
        // Проверяем, чтобы количество желтых и красных карточек не было отрицательным числом
        int yellowCards = player.yellowCards < 0 ? 0 : player.yellowCards;
        int redCards = player.redCards < 0 ? 0 : player.redCards;
        // Выводим информацию о каждом игроке
        cout << "Игрок #" << i << ": " << player.fullName << " (Дата рождения: "
            << player.dateOfBirth << ", Забитых мячей: " << player.goalsScored
            << ", Желтых карточек: " << yellowCards << ", Красных карточек: " << redCards << ")" << endl;
    }
}

// Функция разбивки строки даты рождения на день, месяц и год
void splitDateOfBirth(const string& dateOfBirth, int& day, int& month, int& year) {
    // Используем stringstream для извлечения компонентов даты
    stringstream ss(dateOfBirth);
    string token;
    // Извлекаем день, месяц и год
    getline(ss, token, '.');
    day = stoi(token);
    getline(ss, token, '.');
    month = stoi(token);
    getline(ss, token, '.');
    year = stoi(token);
}

// Функция сравнения дат рождения двух игроков для сортировки
bool compareDateOfBirth(const Player& a, const Player& b) {
    // Извлекаем день, месяц и год даты рождения для обоих игроков
    int day1, month1, year1;
    int day2, month2, year2;
    splitDateOfBirth(a.dateOfBirth, day1, month1, year1);
    splitDateOfBirth(b.dateOfBirth, day2, month2, year2);

    // Сравниваем года, месяцы и дни для определения порядка сортировки
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

// Функция сортировки игроков в зависимости от выбранного поля
void sortPlayers(vector<Player>& players, int field) {
    // Используем switch для выбора поля сортировки
    switch (field) {
    case 1:
        // Сортируем игроков по дате рождения и выводим результат
        sort(players.begin(), players.end(), compareDateOfBirth);
        printPlayers(players);
        break;
    case 2:
        // Сортируем игроков по количеству забитых мячей и выводим результат
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return a.goalsScored > b.goalsScored;
            });
        printPlayers(players);
        break;
    case 3:
        // Сортируем игроков по общему количеству карточек и выводим результат
        sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
            return (a.yellowCards + a.redCards) > (b.yellowCards + b.redCards);
            });
        printPlayers(players);
        break;
    default:
        // Выводим сообщение об ошибке, если указано недопустимое поле сортировки
        cout << "Недопустимое поле сортировки." << endl;
        break;
    }
}

// Функция поиска игроков по строке с именем
void searchPlayers(const vector<Player>& players, const string& searchTerm) {
    // Итерируем по вектору игроков и выводим информацию о тех, чьи имена содержат строку поиска
    cout << "Результаты поиска:" << endl;
    for (int i = 0; i < players.size(); ++i) {
        const Player& player = players[i];
        if (player.fullName.find(searchTerm) != string::npos) {
            cout << "Игрок #" << i << ": " << player.fullName << " (Дата рождения: "
                << player.dateOfBirth << ", Забитых мячей: " << player.goalsScored << ")" << endl;
        }
    }
}

// Функция вывода информации о программе
void displayAbout() {
    // Выводим информацию о версии, дате изменений и авторе программы
    cout << "О программе:" << endl;
    cout << "Версия программы: 3.5" << endl;
    cout << "Дата последних изменений: 15-11-2023" << endl;
    cout << "Автор: Морозов Дмитрий Игоревич, https://github.com/MorozovDesu" << endl;
}

// Функция вывода руководства пользователя из файла
void displayUserManual() {
    // Устанавливаем локаль для корректного отображения русского текста
    setlocale(LC_ALL, "ru_RU.UTF-8");
    // Открываем файл с руководством пользователя и выводим его содержимое
    ifstream userManual("user_manual.txt");
    if (userManual.is_open()) {
        string line;
        while (getline(userManual, line)) {
            cout << line << endl;
        }
        userManual.close();
    }
    else {
        // Выводим сообщение об ошибке, если файл не удалось открыть
        cout << "Ошибка при загрузке руководства пользователя." << endl;
    }
    // Восстанавливаем глобальную локаль
    locale::global(locale(""));
}

// Функция вывода задания программы
void displayTask() {
    // Выводим текст задания программы
    cout << "Составить программу, обрабатывающую сведения об игроках фут-больной команды." << endl;
    cout << "Структура исходных данных: Ф.И.О. игрока, дата рожде-ния, количество игр, количество забитых мячей, количество карточек (жел-тых и красных)." << endl;
    cout << "Вывести список лучших бомбардиров (первая пятерка)." << endl;
    cout << "Найти всех игроков, младше 20 лет, забивших более 2 голов." << endl;
    cout << "Определить самого недисциплинированного игрока команды." << endl;
}

// Функция проверки, является ли игрок младше 20 лет и забил более 2 голов
bool isYoungScorer(const Player& player) {
    // Проверка на возраст младше 20 и забитые мячи более 2
    int currentYear = 2023;
    int birthYear = stoi(player.dateOfBirth.substr(6));
    int age = currentYear - birthYear;

    return (age < 20 && player.goalsScored > 2);
}

// Функция проверки, оканчивается ли имя файла на ".csv"
bool endsWithCsv(const string& filename) {
    // Проверка, что имя файла оканчивается на ".csv"
    size_t pos = filename.find_last_of('.');
    if (pos != string::npos) {
        string extension = filename.substr(pos + 1);
        // Преобразуем расширение в нижний регистр для надежности
        transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        return extension == "csv";
    }
    return false;
}

// Функция вывода топ-5 бомбардиров
void displayTopScorers(const vector<Player>& players) {
    // Создаем копию вектора игроков и сортируем по убыванию забитых мячей
    vector<Player> topScorers = players;
    sort(topScorers.begin(), topScorers.end(), [](const Player& a, const Player& b) {
        return a.goalsScored > b.goalsScored;
        });

    // Выводим информацию о топ-5 бомбардирах
    cout << "Топ-5 бомбардиров:" << endl;
    for (int i = 0; i < min(5, static_cast<int>(topScorers.size())); ++i) {
        const Player& player = topScorers[i];
        cout << "Игрок #" << i + 1 << ": " << player.fullName << " (Забитых мячей: " << player.goalsScored << ")" << endl;
    }
}
// Функция для вывода информации о самом недисциплинированном игроке
void displayMostUndisciplinedPlayer(const vector<Player>& players) {
    // Инициализация переменных для отслеживания максимального количества карточек и указателя на игрока
    int maxCards = -1;
    const Player* mostUndisciplinedPlayer = nullptr;

    // Проход по всем игрокам в векторе
    for (const Player& player : players) {
        // Изменение расчета общего количества карточек
        int totalCards = player.yellowCards + (2 * player.redCards);

        // Проверка, является ли текущий игрок самым недисциплинированным
        if (totalCards > maxCards) {
            maxCards = totalCards;
            mostUndisciplinedPlayer = &player;
        }
    }

    // Вывод информации о самом недисциплинированном игроке
    if (mostUndisciplinedPlayer) {
        cout << "Самый недисциплинированный игрок:" << endl;
        cout << "Игрок: " << mostUndisciplinedPlayer->fullName << " (Желтых карточек: " << mostUndisciplinedPlayer->yellowCards
            << ", Красных карточек: " << mostUndisciplinedPlayer->redCards << ")" << endl;
    }
    else {
        cout << "Нет информации о недисциплинированных игроках." << endl;
    }
}

// Функция для поиска и вывода информации о молодых игроках, забивших более 2 голов
void findYoungHighScorers(const vector<Player>& players) {
    cout << "Игроки младше 20 лет, забившие более 2 голов:" << endl;
    int currentYear = 2023;

    // Проход по всем игрокам в векторе
    for (const Player& player : players) {
        // Расчет возраста игрока
        int birthYear = stoi(player.dateOfBirth.substr(6));
        int age = currentYear - birthYear;

        // Проверка, соответствует ли игрок условиям
        if (age < 20 && player.goalsScored > 2) {
            cout << "Игрок: " << player.fullName << " (Возраст: " << age << " лет, Забитых мячей: " << player.goalsScored << ")" << endl;
        }
    }
}

// Функция для отображения меню управления футболистами
// Функция для отображения меню управления данными о футболистах
void displayFootballMenu(vector<Player>& players, const string& filename) {
    // Переменные для хранения строковых данных
    string searchTerm;
    string newFilename;

    // Бесконечный цикл для отображения меню
    while (true) {
        // Вывод пунктов меню
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

        // Ввод выбора пользователя
        int choice;
        cin >> choice;

        // Обработка ошибочного ввода
        if (!(choice)) {
            cin.clear();  // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
        else {
            // Обработка выбора пользователя
            switch (choice) {
            case 1:
                // Вызов функции для добавления нового игрока
                addPlayer(players);
                break;
            case 2:
                // Вызов функции для редактирования данных игрока
                editPlayer(players);
                break;
            case 3:
                // Вызов функции для удаления игрока
                deletePlayer(players);
                break;
            case 4:
                // Вызов функции для вывода списка игроков
                printPlayers(players);
                break;
            case 5:
                // Выбор поля для сортировки и вызов функции сортировки
                cout << "Выберите поле для сортировки (1-4):" << endl;
                cout << "1. Дата рождения" << endl;
                cout << "2. Количество забитых голов" << endl;
                cout << "3. Количество карточек" << endl;
                int field;
                cin >> field;
                sortPlayers(players, field);
                break;
            case 6:
                // Ввод строки для поиска и вызов функции поиска
                cin.ignore();
                cout << "Введите строку Ф.И.О. для поиска: ";
                getline(cin, searchTerm);
                searchPlayers(players, searchTerm);
                break;
            case 7:
                // Вызов функции для вывода топ-5 бомбардиров
                displayTopScorers(players);
                break;
            case 8:
                // Вызов функции для вывода самого недисциплинированного игрока
                displayMostUndisciplinedPlayer(players);
                break;
            case 9:
                // Вызов функции для поиска и вывода молодых игроков, забивших более 2 голов
                findYoungHighScorers(players);
                break;
            case 10:
                // Сохранение данных в текущем файле (CSV формат) или вывод ошибки
                if (endsWithCsv(filename)) {
                    saveDataToFile(players, filename);
                    cout << "Данные успешно сохранены в текущем файле: " << filename << endl;
                }
                else {
                    cout << "Ошибка: Файл должен иметь расширение .csv для сохранения." << endl;
                }
                break;
            case 11:
                // Ввод нового имени файла и сохранение данных или вывод ошибки
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
                // Возвращение в главное меню
                cout << "Возвращение в главное меню." << endl;
                return;
            default:
                // Вывод сообщения об ошибке при недопустимом действии
                cout << "Недопустимое действие." << endl;
                break;
            }
        }
    }
}


// Главная функция программы
int main() {
    // Устанавливаем глобальную локаль для корректного отображения символов
    locale::global(locale(""));

    // Вектор для хранения данных о футболистах
    vector<Player> players;

    // Имя файла для загрузки и сохранения данных
    const string filename = "players.csv";

    // Флаг для отслеживания сохранения данных перед выходом
    bool dataSaved = false;

    // Попытка загрузить данные из файла
    try {
        loadDataFromFile(players, filename);
    }
    // Обработка исключений при возникновении ошибок
    catch (const exception& e) {
        // В случае ошибки загрузки, сообщение об ошибке может быть выведено в консоль с использованием cerr
        //cerr << "Ошибка тут: " << e.what() << endl;
    }

    // Бесконечный цикл для отображения главного меню
    while (true) {
        // Вывод основных пунктов главного меню
        cout << "\nГлавное меню:" << endl;
        cout << "1. Работа с футболистами" << endl;
        cout << "2. О программе" << endl;
        cout << "3. Руководство пользователя" << endl;
        cout << "4. Задание" << endl;
        cout << "5. Выйти" << endl;

        // Ввод выбора пользователя
        int choice;
        cin >> choice;

        // Обработка ошибочного ввода
        if (!(choice)) {
            cin.clear();  // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Очищаем буфер ввода
            cout << "Ошибка ввода. Введите цифру." << endl;
        }
        else {
            // Обработка выбора пользователя
            switch (choice) {
            case 1:
                // Вызываем функцию для отображения меню управления футболистами
                displayFootballMenu(players, filename);
                break;
            case 2:
                // Вызываем функцию для отображения информации о программе
                displayAbout();
                break;
            case 3:
                // Вызываем функцию для отображения руководства
            }
        }
    }
}