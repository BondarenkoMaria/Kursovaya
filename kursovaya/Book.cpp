#include "Book.h"
#include <iostream>

Book::Book() : nazvanie(""), author(""), status(""), rating(0), link("") {}

void Book::setData(const std::string& n, const std::string& a,
                   const std::string& s, int r, const std::string& l) {
    nazvanie = n;
    author = a;
    status = s;
    rating = r;
    link = l;
}

void Book::setStatus(const std::string& newStatus) { status = newStatus; }
void Book::setRating(int newRating) { rating = newRating; }

std::string Book::getNazvanie() const { return nazvanie; }
std::string Book::getAuthor() const { return author; }
std::string Book::getStatus() const { return status; }
int Book::getRating() const { return rating; }
std::string Book::getLink() const { return link; }

void Book::printInfo() const {
    using std::cout;
    cout << "Название: " << nazvanie << "\n";
    cout << "Автор: " << author << "\n";
    cout << "Статус: " << status << "\n";

    cout << "Оценка: ";
    if (rating >= 1 && rating <= 5) {
        for (int i = 0; i < rating; ++i) cout << "*";
        cout << " (" << rating << " из 5)\n";
    } else {
        cout << "Нет оценки\n";
    }

    if (!link.empty()) cout << "Ссылка: " << link << "\n";
}