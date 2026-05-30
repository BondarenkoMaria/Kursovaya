#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
private:
    std::string nazvanie;
    std::string author;
    std::string status;
    int rating;
    std::string link;

public:
    Book();
    void setData(const std::string& n, const std::string& a,
                 const std::string& s, int r, const std::string& l);
    void setStatus(const std::string& newStatus);
    void setRating(int newRating);

    std::string getNazvanie() const;
    std::string getAuthor() const;
    std::string getStatus() const;
    int getRating() const;
    std::string getLink() const;

    void printInfo() const;
};

#endif