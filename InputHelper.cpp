#include "InputHelper.h"

#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

int InputHelper::readIntInRange(const std::string& prompt, int minimum, int maximum) {
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            throw std::runtime_error("Input stream closed.");
        }

        std::istringstream input(trim(line));
        int value = 0;
        char extra = '\0';

        // Reading an extra character rejects partial values such as "3abc".
        if ((input >> value) && !(input >> extra) && value >= minimum && value <= maximum) {
            return value;
        }

        std::cout << "Invalid choice. Enter a number from "
                  << minimum << " to " << maximum << ".\n";
    }
}

double InputHelper::readPositiveAmount(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            throw std::runtime_error("Input stream closed.");
        }

        std::istringstream input(trim(line));
        double amount = 0.0;
        char extra = '\0';

        // isfinite rejects special floating-point values such as infinity and NaN.
        if ((input >> amount) && !(input >> extra) && std::isfinite(amount) && amount > 0.0) {
            return amount;
        }

        std::cout << "Invalid amount. Enter a number greater than zero.\n";
    }
}

std::string InputHelper::readRequiredText(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string value;
        if (!std::getline(std::cin, value)) {
            throw std::runtime_error("Input stream closed.");
        }

        value = trim(value);
        // Pipes are reserved as separators in the transaction data file.
        if (!value.empty() && !containsDelimiter(value)) {
            return value;
        }

        if (value.empty()) {
            std::cout << "This field cannot be empty.\n";
        } else {
            std::cout << "The | character is not allowed.\n";
        }
    }
}

std::string InputHelper::readDate(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string date;
        if (!std::getline(std::cin, date)) {
            throw std::runtime_error("Input stream closed.");
        }

        date = trim(date);
        if (isValidDate(date)) {
            return date;
        }

        std::cout << "Invalid date. Use YYYY-MM-DD (for example, 2026-09-19).\n";
    }
}

std::string InputHelper::trim(const std::string& text) {
    std::size_t first = 0;
    while (first < text.size()
           && std::isspace(static_cast<unsigned char>(text[first]))) {
        ++first;
    }

    std::size_t last = text.size();
    while (last > first
           && std::isspace(static_cast<unsigned char>(text[last - 1]))) {
        --last;
    }

    return text.substr(first, last - first);
}

bool InputHelper::isValidDate(const std::string& date) {
    // Check the shape before using substrings and numeric conversion.
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }

    for (std::size_t index = 0; index < date.size(); ++index) {
        if (index != 4 && index != 7
            && !std::isdigit(static_cast<unsigned char>(date[index]))) {
            return false;
        }
    }

    const int year = std::stoi(date.substr(0, 4));
    const int month = std::stoi(date.substr(5, 2));
    const int day = std::stoi(date.substr(8, 2));

    if (year < 1 || month < 1 || month > 12) {
        return false;
    }

    const int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };
    int maximumDay = daysInMonth[month - 1];
    // February gains one day under the Gregorian leap-year rules.
    if (month == 2 && isLeapYear(year)) {
        maximumDay = 29;
    }

    return day >= 1 && day <= maximumDay;
}

bool InputHelper::isLeapYear(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

bool InputHelper::containsDelimiter(const std::string& text) {
    return text.find('|') != std::string::npos;
}
