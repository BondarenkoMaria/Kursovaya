#ifndef LIBRARY_MANAGER_H
#define LIBRARY_MANAGER_H

#include "Book.h"
#include <vector>
#include <string>

extern std::vector<Book> library;

std::string trim(const std::string& str);
void loadLibrary(const std::string& filename);
void saveLibrary(const std::string& filename);
void addOneBook(const std::string& filename, const Book& book);

void searchByNazvanie();
void searchByAuthor();
void editStatus(const std::string& filename);
void editRating(const std::string& filename);
void addBook(const std::string& filename);
void showAllBooks();
void showBooksByStatus();
void showStatistics();

// Меню и вспомогательные функции
void showMenu();
int readChoice();

#endif