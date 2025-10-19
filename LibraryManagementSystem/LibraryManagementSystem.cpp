#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>

class Book {
private:
    std::string title;
    std::string author;
    std::string ISBN;
    bool isAvailable;

public:
    // Constructor
    Book(const std::string& t, const std::string& a, const std::string& isbn)
        : title(t), author(a), ISBN(isbn), isAvailable(true) {
    }

    // Getters
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getISBN() const { return ISBN; }
    bool getAvailability() const { return isAvailable; }

    // Setters
    void setTitle(const std::string& t) { title = t; }
    void setAuthor(const std::string& a) { author = a; }
    void setISBN(const std::string& isbn) { ISBN = isbn; }

    // Member functions
    void displayInfo() const {
        std::cout << "Title: " << title << std::endl;
        std::cout << "Author: " << author << std::endl;
        std::cout << "ISBN: " << ISBN << std::endl;
        std::cout << "Status: " << (isAvailable ? "Available" : "Borrowed") << std::endl;
        std::cout << "------------------------" << std::endl;
    }

    void borrowBook() {
        if (isAvailable) {
            isAvailable = false;
            std::cout << "Book '" << title << "' has been borrowed." << std::endl;
        }
        else {
            std::cout << "Book '" << title << "' is already borrowed." << std::endl;
        }
    }

    void returnBook() {
        if (!isAvailable) {
            isAvailable = true;
            std::cout << "Book '" << title << "' has been returned." << std::endl;
        }
        else {
            std::cout << "Book '" << title << "' was not borrowed." << std::endl;
        }
    }

    // For file storage
    std::string toFileString() const {
        return title + "|" + author + "|" + ISBN + "|" + (isAvailable ? "1" : "0");
    }

    // Load from string
    static Book fromString(const std::string& data) {
        size_t pos1 = data.find('|');
        size_t pos2 = data.find('|', pos1 + 1);
        size_t pos3 = data.find('|', pos2 + 1);

        std::string title = data.substr(0, pos1);
        std::string author = data.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string ISBN = data.substr(pos2 + 1, pos3 - pos2 - 1);
        bool available = (data.substr(pos3 + 1) == "1");

        Book book(title, author, ISBN);
        if (!available) book.borrowBook();
        return book;
    }
};

class Library {
private:
    std::vector<Book> books;
    const std::string filename = "library_data.txt";

public:
    // Add a new book to the library
    void addBook() {
        std::string title, author, ISBN;

        std::cout << "\n=== Add New Book ===" << std::endl;

        std::cin.ignore(); // Clear input buffer
        std::cout << "Enter book title: ";
        std::getline(std::cin, title);

        std::cout << "Enter author: ";
        std::getline(std::cin, author);

        std::cout << "Enter ISBN: ";
        std::getline(std::cin, ISBN);

        // Check if ISBN already exists
        for (const auto& book : books) {
            if (book.getISBN() == ISBN) {
                std::cout << "Error: A book with this ISBN already exists!" << std::endl;
                return;
            }
        }

        Book newBook(title, author, ISBN);
        books.push_back(newBook);
        std::cout << "Book added successfully!" << std::endl;
    }

    // Search books by various criteria
    void searchBooks() {
        if (books.empty()) {
            std::cout << "No books in the library." << std::endl;
            return;
        }

        int choice;
        std::cout << "\n=== Search Books ===" << std::endl;
        std::cout << "1. Search by Title" << std::endl;
        std::cout << "2. Search by Author" << std::endl;
        std::cout << "3. Search by ISBN" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        std::cin.ignore(); // Clear input buffer
        std::string searchTerm;
        bool found = false;

        switch (choice) {
        case 1:
            std::cout << "Enter title to search: ";
            std::getline(std::cin, searchTerm);
            std::cout << "\nSearch Results:" << std::endl;
            for (const auto& book : books) {
                if (book.getTitle().find(searchTerm) != std::string::npos) {
                    book.displayInfo();
                    found = true;
                }
            }
            break;

        case 2:
            std::cout << "Enter author to search: ";
            std::getline(std::cin, searchTerm);
            std::cout << "\nSearch Results:" << std::endl;
            for (const auto& book : books) {
                if (book.getAuthor().find(searchTerm) != std::string::npos) {
                    book.displayInfo();
                    found = true;
                }
            }
            break;

        case 3:
            std::cout << "Enter ISBN to search: ";
            std::getline(std::cin, searchTerm);
            std::cout << "\nSearch Results:" << std::endl;
            for (const auto& book : books) {
                if (book.getISBN() == searchTerm) {
                    book.displayInfo();
                    found = true;
                    break; // ISBN should be unique
                }
            }
            break;

        default:
            std::cout << "Invalid choice!" << std::endl;
            return;
        }

        if (!found) {
            std::cout << "No books found matching your search criteria." << std::endl;
        }
    }

    // Borrow a book
    void borrowBook() {
        if (books.empty()) {
            std::cout << "No books in the library." << std::endl;
            return;
        }

        std::string ISBN;
        std::cout << "\n=== Borrow Book ===" << std::endl;
        std::cout << "Enter ISBN of the book to borrow: ";
        std::cin >> ISBN;

        for (auto& book : books) {
            if (book.getISBN() == ISBN) {
                book.borrowBook();
                return;
            }
        }

        std::cout << "Book with ISBN '" << ISBN << "' not found." << std::endl;
    }

    // Return a book
    void returnBook() {
        if (books.empty()) {
            std::cout << "No books in the library." << std::endl;
            return;
        }

        std::string ISBN;
        std::cout << "\n=== Return Book ===" << std::endl;
        std::cout << "Enter ISBN of the book to return: ";
        std::cin >> ISBN;

        for (auto& book : books) {
            if (book.getISBN() == ISBN) {
                book.returnBook();
                return;
            }
        }

        std::cout << "Book with ISBN '" << ISBN << "' not found." << std::endl;
    }

    // Display all books
    void displayAllBooks() {
        if (books.empty()) {
            std::cout << "No books in the library." << std::endl;
            return;
        }

        std::cout << "\n=== All Books in Library ===" << std::endl;
        for (const auto& book : books) {
            book.displayInfo();
        }
        std::cout << "Total books: " << books.size() << std::endl;
    }

    // Save library data to file
    void saveToFile() {
        std::ofstream file(filename);
        if (!file) {
            std::cout << "Error: Could not save library data!" << std::endl;
            return;
        }

        for (const auto& book : books) {
            file << book.toFileString() << std::endl;
        }

        file.close();
        std::cout << "Library data saved successfully!" << std::endl;
    }

    // Load library data from file
    void loadFromFile() {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "No existing library data found. Starting with empty library." << std::endl;
            return;
        }

        books.clear(); // Clear existing books
        std::string line;

        while (std::getline(file, line)) {
            if (!line.empty()) {
                books.push_back(Book::fromString(line));
            }
        }

        file.close();
        std::cout << "Library data loaded successfully! Loaded " << books.size() << " books." << std::endl;
    }

    // Display statistics
    void showStatistics() {
        if (books.empty()) {
            std::cout << "No books in the library." << std::endl;
            return;
        }

        int availableCount = 0;
        int borrowedCount = 0;

        for (const auto& book : books) {
            if (book.getAvailability()) {
                availableCount++;
            }
            else {
                borrowedCount++;
            }
        }

        std::cout << "\n=== Library Statistics ===" << std::endl;
        std::cout << "Total books: " << books.size() << std::endl;
        std::cout << "Available books: " << availableCount << std::endl;
        std::cout << "Borrowed books: " << borrowedCount << std::endl;
    }
};

// Utility function to clear screen (platform-specific)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Utility function to pause execution
void pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

int main() {
    Library library;
    int choice;

    // Load existing data
    library.loadFromFile();

    std::cout << "=== Welcome to Library Management System ===" << std::endl;

    do {
        std::cout << "\n=== Main Menu ===" << std::endl;
        std::cout << "1. Add Book" << std::endl;
        std::cout << "2. Search Books" << std::endl;
        std::cout << "3. Borrow Book" << std::endl;
        std::cout << "4. Return Book" << std::endl;
        std::cout << "5. Display All Books" << std::endl;
        std::cout << "6. Library Statistics" << std::endl;
        std::cout << "7. Save Data" << std::endl;
        std::cout << "8. Exit" << std::endl;
        std::cout << "Enter your choice (1-8): ";

        // Input validation
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number between 1-8." << std::endl;
            continue;
        }

        clearScreen();

        switch (choice) {
        case 1:
            library.addBook();
            break;
        case 2:
            library.searchBooks();
            break;
        case 3:
            library.borrowBook();
            break;
        case 4:
            library.returnBook();
            break;
        case 5:
            library.displayAllBooks();
            break;
        case 6:
            library.showStatistics();
            break;
        case 7:
            library.saveToFile();
            break;
        case 8:
            library.saveToFile(); // Auto-save on exit
            std::cout << "Thank you for using Library Management System!" << std::endl;
            break;
        default:
            std::cout << "Invalid choice! Please enter a number between 1-8." << std::endl;
        }

        if (choice != 8) {
            pause();
            clearScreen();
        }

    } while (choice != 8);

    return 0;
}