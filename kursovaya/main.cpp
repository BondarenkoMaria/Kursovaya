#include "LibraryManager.h"
#include <iostream>
#include <clocale>

int main() {
    system("chcp 1251 > nul");
    std::setlocale(LC_ALL, "Russian");

    std::string filename = "books.txt";
    std::cout << "Загрузка библиотеки из файла \"" << filename << "\"...\n";
    loadLibrary(filename);

    if (!library.empty()) showStatistics();

    bool running = true;
    while (running) {
        showMenu();
        int choice = readChoice();
        std::cout << "\n";
        switch (choice) {
            case 1: searchByNazvanie(); break;
            case 2: searchByAuthor();   break;
            case 3: editStatus(filename); break;
            case 4: editRating(filename); break;
            case 5: addBook(filename); break;
            case 6: showAllBooks(); break;
            case 7: showBooksByStatus(); break;
            case 8: showStatistics(); break;
            case 9: saveLibrary(filename); break;
            case 10:
                std::cout << "Сохраняем и выходим...\n";
                saveLibrary(filename);
                std::cout << "До свидания!\n";
                running = false;
                break;
            default:
                std::cout << "Введите число от 1 до 10.\n";
        }
    }
    return 0;
}