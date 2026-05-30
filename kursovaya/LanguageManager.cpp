#include "LibraryManager.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

std::vector<Book> library;

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

void loadLibrary(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Файл не найден. Если добавите новую книгу, файл создастся автоматически.\n";
        return;
    }

    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;

        std::vector<std::string> tokens;
        size_t start = 0, end = 0;
        while ((end = line.find(';', start)) != std::string::npos) {
            tokens.push_back(trim(line.substr(start, end - start)));
            start = end + 1;
        }
        tokens.push_back(trim(line.substr(start)));

        if (tokens.size() >= 2) {
            std::string nazvanie = tokens[0];
            std::string author = tokens[1];
            std::string status = (tokens.size() > 2) ? tokens[2] : "";
            int rating = 0;
            std::string link = "";

            if (tokens.size() == 4) {
                if (!tokens[3].empty() && isdigit(tokens[3][0]) && tokens[3].length() == 1) {
                    rating = std::stoi(tokens[3]);
                }
                else {
                    link = tokens[3];
                }
            }
            else if (tokens.size() >= 5) {
                if (!tokens[3].empty()) {
                    try { rating = std::stoi(tokens[3]); }
                    catch (...) { rating = 0; }
                }
                link = tokens[4];
            }

            Book tempBook;
            tempBook.setData(nazvanie, author, status, rating, link);
            library.push_back(tempBook);
        }
        else {
            std::cout << "Строка " << lineNum << " пропущена, т.к. не хватает данных.\n";
        }
    }

    file.close();
    std::cout << "Загружено книг: " << library.size() << "\n";
}

void saveLibrary(const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл!\n";
        return;
    }
    for (size_t i = 0; i < library.size(); i++) {
        file << library[i].getNazvanie() << ";"
            << library[i].getAuthor() << ";"
            << library[i].getStatus() << ";"
            << library[i].getRating() << ";"
            << library[i].getLink() << "\n";
    }
    file.close();
    std::cout << "Сохранено книг: " << library.size() << "\n";
}

void addOneBook(const std::string& filename, const Book& book) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Книга добавлена в память, но не сохранена в файл.\n";
        return;
    }
    file << book.getNazvanie() << ";"
        << book.getAuthor() << ";"
        << book.getStatus() << ";"
        << book.getRating() << ";"
        << book.getLink() << "\n";
    file.close();
}

void searchByNazvanie() {
    std::cout << "\n    ПОИСК ПО НАЗВАНИЮ\n";
    std::cout << "Введите часть названия: ";
    std::string query;
    std::getline(std::cin, query);

    std::vector<int> foundIndex;
    for (size_t i = 0; i < library.size(); i++) {
        if (library[i].getNazvanie().find(query) != std::string::npos) {
            foundIndex.push_back(i);
        }
    }
    if (foundIndex.empty()) {
        std::cout << "Книги не найдены.\n";
        return;
    }
    else {
        std::cout << "Найдено: " << foundIndex.size() << "\n";
        for (size_t i = 0; i < foundIndex.size(); i++) {
            std::cout << i + 1 << ". " << library[foundIndex[i]].getNazvanie() << "\n";
        }
        std::cout << "\nВведите номер книги для вывода информации (или 0 для отмены): ";
        std::string choiceStr;
        std::getline(std::cin, choiceStr);

        try {
            int choice = std::stoi(choiceStr);
            if (choice > 0 && choice <= static_cast<int>(foundIndex.size())) {
                std::cout << "\nИнформация о книге: \n";
                library[foundIndex[choice - 1]].printInfo();
            }
            else if (choice != 0) {
                std::cout << "Неверный номер. Действие отменено.\n";
            }
        }
        catch (...) {
            std::cout << "Неверный ввод. Действие отменено.\n";
        }
    }
}

void editStatus(const std::string& filename) {
    std::cout << "\n    ИЗМЕНЕНИЕ СТАТУСА\n";
    std::cout << "Введите точное название книги: ";
    std::string nazvanie;
    std::getline(std::cin, nazvanie);
    for (size_t i = 0; i < library.size(); i++) {
        if (library[i].getNazvanie() == nazvanie) {
            std::cout << "\nТекущая информация:\n";
            library[i].printInfo();

            std::cout << "Новый статус (прочитано / читаю / в планах): ";
            std::string newStatus;
            std::getline(std::cin, newStatus);
            library[i].setStatus(newStatus);
            saveLibrary(filename);
            std::cout << "Статус обновлен.\n";
            return;
        }
    }
    std::cout << "Книга \"" << nazvanie << "\" не найдена.\n";
}

void editRating(const std::string& filename) {
    std::cout << "\n    ИЗМЕНЕНИЕ ОЦЕНКИ\n";
    std::cout << "Введите точное название книги: ";
    std::string nazvanie;
    std::getline(std::cin, nazvanie);
    for (size_t i = 0; i < library.size(); i++) {
        if (library[i].getNazvanie() == nazvanie) {
            std::cout << "\nТекущая информация:\n";
            library[i].printInfo();

            std::cout << "Введите новую оценку (от 1 до 5): ";
            std::string ratingStr;
            std::getline(std::cin, ratingStr);
            try {
                int newRating = std::stoi(ratingStr);
                if (newRating >= 1 && newRating <= 5) {
                    library[i].setRating(newRating);
                    saveLibrary(filename);
                    std::cout << "Оценка успешно обновлена.\n";
                }
                else {
                    std::cout << "Ошибка: оценка должна быть в диапазоне от 1 до 5.\n";
                }
            }
            catch (...) {
                std::cout << "Неверный ввод. Действие отменено.\n";
            }
            return;
        }
    }
    std::cout << "Книга \"" << nazvanie << "\" не найдена.\n";
}

void addBook(const std::string& filename) {
    std::cout << "\n    ДОБАВЛЕНИЕ КНИГИ\n";

    std::string nazvanie, author, status, ratingStr, link;

    std::cout << "Название: ";
    std::getline(std::cin, nazvanie);
    std::cout << "Автор: ";
    std::getline(std::cin, author);
    std::cout << "Статус (прочитано / читаю / в планах): ";
    std::getline(std::cin, status);

    std::cout << "Оценка (число от 1 до 5, или Enter чтобы пропустить): ";
    std::getline(std::cin, ratingStr);
    int rating = 0;
    if (!ratingStr.empty()) {
        try {
            rating = std::stoi(ratingStr);
            if (rating < 1 || rating > 5) rating = 0;
        }
        catch (...) {
            rating = 0;
        }
    }

    std::cout << "Ссылка (Enter — пропустить): ";
    std::getline(std::cin, link);

    Book tempBook;
    tempBook.setData(nazvanie, author, status, rating, link);
    library.push_back(tempBook);

    addOneBook(filename, tempBook);

    std::cout << "Книга \"" << nazvanie << "\" добавлена.\n";
}

void showAllBooks() {
    std::cout << "\n      ВСЕ КНИГИ (" << library.size() << ")      \n";
    if (library.empty()) {
        std::cout << "Библиотека пуста.\n";
        return;
    }
    for (size_t i = 0; i < library.size(); i++) {
        std::cout << "Книга No." << (i + 1) << ":\n";
        library[i].printInfo();
        std::cout << "--------------------------------------\n";
    }
}

void showBooksByStatus() {
    std::cout << "\n    ВЫВОД КНИГ ПО СТАТУСУ\n";
    std::cout << "Выберите интересующий статус:\n";
    std::cout << "1. Прочитано\n";
    std::cout << "2. Читаю\n";
    std::cout << "3. В планах\n";
    std::cout << "Ваш выбор (1-3): ";

    std::string choiceStr;
    std::getline(std::cin, choiceStr);
    std::string targetStatus = "";

    if (choiceStr == "1") targetStatus = "прочитано";
    else if (choiceStr == "2") targetStatus = "читаю";
    else if (choiceStr == "3") targetStatus = "в планах";
    else {
        std::cout << "Неверный выбор. Возврат в меню.\n";
        return;
    }

    std::cout << "\n--- Книги со статусом \"" << targetStatus << "\" ---\n";
    int count = 0;
    for (size_t i = 0; i < library.size(); i++) {
        if (library[i].getStatus() == targetStatus) {
            count++;
            std::cout << "Книга No." << count << ":\n";
            library[i].printInfo();
            std::cout << "--------------------------------------\n";
        }
    }
    if (count == 0) {
        std::cout << "Книг с таким статусом не найдено.\n";
    }
    else {
        std::cout << "Всего найдено: " << count << "\n";
    }
}

void searchByAuthor() {
    std::cout << "\n    ПОИСК ПО АВТОРУ\n";
    std::cout << "Введите часть имени автора: ";
    std::string query;
    std::getline(std::cin, query);

    std::vector<int> foundIndex;
    for (size_t i = 0; i < library.size(); i++) {
        if (library[i].getAuthor().find(query) != std::string::npos) {
            foundIndex.push_back(i);
        }
    }

    if (foundIndex.empty())
        std::cout << "Книги не найдены.\n";
    else {
        std::cout << "Найдено: " << foundIndex.size() << "\n";
        for (size_t i = 0; i < foundIndex.size(); i++) {
            std::cout << i + 1 << ". " << library[foundIndex[i]].getNazvanie() << " (" << library[foundIndex[i]].getAuthor() << ")\n";
        }

        std::cout << "\nВведите номер книги для вывода информации (или 0 для отмены): ";
        std::string choiceStr;
        std::getline(std::cin, choiceStr);

        try {
            int choice = std::stoi(choiceStr);
            if (choice > 0 && choice <= static_cast<int>(foundIndex.size())) {
                std::cout << "\nИнформация о книге: \n";
                library[foundIndex[choice - 1]].printInfo();
            }
            else if (choice != 0) {
                std::cout << "Неверный номер. Действие отменено.\n";
            }
        }
        catch (...) {
            std::cout << "Неверный ввод. Действие отменено.\n";
        }
    }
}

void showStatistics() {
    std::cout << "\n     СТАТИСТИКА\n";
    if (library.empty()) {
        std::cout << "Библиотека пуста.\n";
        return;
    }

    int countRead = 0;
    int countReading = 0;
    int countPlanned = 0;
    int countOther = 0;

    for (size_t i = 0; i < library.size(); i++) {
        std::string s = library[i].getStatus();
        if (s == "прочитано") countRead++;
        else if (s == "читаю") countReading++;
        else if (s == "в планах") countPlanned++;
        else countOther++;
    }
    int pctRead = countRead * 100 / static_cast<int>(library.size());
    int pctReading = countReading * 100 / static_cast<int>(library.size());
    int pctPlanned = countPlanned * 100 / static_cast<int>(library.size());

    std::cout << "Всего книг: " << library.size() << "\n";
    std::cout << "Прочитано: " << countRead << " (" << pctRead << "%)\n";
    std::cout << "Читаю сейчас: " << countReading << " (" << pctReading << "%)\n";
    std::cout << "В планах: " << countPlanned << " (" << pctPlanned << "%)\n";
    if (countOther > 0)
        std::cout << "Другие: " << countOther << "\n";
}

void showMenu() {
    std::cout << "\n--------------------------------------\n";
    std::cout << "          ДОМАШНЯЯ БИБЛИОТЕКА\n";
    std::cout << "--------------------------------------\n";
    std::cout << "1. Найти книгу по названию\n";
    std::cout << "2. Найти книги по автору\n";
    std::cout << "3. Изменить статус книги\n";
    std::cout << "4. Поставить/изменить оценку книги\n";
    std::cout << "5. Добавить новую книгу\n";
    std::cout << "6. Показать все книги\n";
    std::cout << "7. Показать книги по статусу\n";
    std::cout << "8. Показать статистику\n";
    std::cout << "9. Сохранить изменения\n";
    std::cout << "10. Сохранить и выйти\n";
    std::cout << "--------------------------------------\n";
    std::cout << "Выберите действие (1-10): ";
}

int readChoice() {
    std::string input;
    std::getline(std::cin, input);
    try {
        return std::stoi(input);
    }
    catch (...) {
        return 0;
    }
}