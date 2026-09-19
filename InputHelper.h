#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <string>

// Centralizes console input validation so menu code receives clean values.
class InputHelper {
public:
    // Re-prompts until the user enters a whole number inside the given range.
    static int readIntInRange(const std::string& prompt, int minimum, int maximum);

    // Re-prompts until the user enters a finite amount greater than zero.
    static double readPositiveAmount(const std::string& prompt);

    // Reads non-empty text that is safe for the pipe-delimited save format.
    static std::string readRequiredText(const std::string& prompt);

    // Reads a real calendar date in YYYY-MM-DD format.
    static std::string readDate(const std::string& prompt);

    // Shared utilities are public because file loading also validates stored data.
    static std::string trim(const std::string& text);
    static bool isValidDate(const std::string& date);

private:
    static bool isLeapYear(int year);
    static bool containsDelimiter(const std::string& text);
};

#endif
