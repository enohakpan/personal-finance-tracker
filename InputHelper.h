#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <string>

class InputHelper {
public:
    static int readIntInRange(const std::string& prompt, int minimum, int maximum);
    static double readPositiveAmount(const std::string& prompt);
    static std::string readRequiredText(const std::string& prompt);
    static std::string readDate(const std::string& prompt);

    static std::string trim(const std::string& text);
    static bool isValidDate(const std::string& date);

private:
    static bool isLeapYear(int year);
    static bool containsDelimiter(const std::string& text);
};

#endif
