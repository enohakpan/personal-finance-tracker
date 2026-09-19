#include "FinanceManager.h"

#include "InputHelper.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <utility>

namespace {
// These parsers require the entire field to be valid, not just its prefix.
bool parseInteger(const std::string& text, int& value) {
    std::istringstream input(text);
    char extra = '\0';
    return (input >> value) && !(input >> extra);
}

bool parseAmount(const std::string& text, double& value) {
    std::istringstream input(text);
    char extra = '\0';
    return (input >> value) && !(input >> extra)
           && std::isfinite(value) && value > 0.0;
}

// Splits one saved record while preserving an empty final field for validation.
std::vector<std::string> splitRecord(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream input(line);
    std::string field;

    while (std::getline(input, field, '|')) {
        fields.push_back(field);
    }

    if (!line.empty() && line.back() == '|') {
        fields.emplace_back();
    }
    return fields;
}
} // namespace

void FinanceManager::addTransaction(const std::string& date,
                                    const std::string& description,
                                    const std::string& category,
                                    double amount,
                                    TransactionType type) {
    transactions_.emplace_back(
        nextId_++, date, description, category, amount, type);
}

void FinanceManager::displayTransactions() const {
    displayTransactions(transactions_, "ALL TRANSACTIONS");
}

void FinanceManager::displayTransactions(
    const std::vector<Transaction>& transactions,
    const std::string& title) const {
    std::cout << "\n========== " << title << " ==========\n";
    if (transactions.empty()) {
        std::cout << "No transactions found.\n";
        return;
    }

    const std::string separator(95, '-');
    std::cout << separator << '\n'
              << std::left
              << std::setw(6) << "ID"
              << std::setw(12) << "DATE"
              << std::setw(10) << "TYPE"
              << std::setw(18) << "CATEGORY"
              << std::setw(32) << "DESCRIPTION"
              << std::right << std::setw(15) << "AMOUNT" << '\n'
              << separator << '\n';

    for (const Transaction& transaction : transactions) {
        std::cout << std::left
                  << std::setw(6) << transaction.getId()
                  << std::setw(12) << transaction.getDate()
                  << std::setw(10) << transaction.getTypeName()
                  << std::setw(18) << shorten(transaction.getCategory(), 17)
                  << std::setw(32) << shorten(transaction.getDescription(), 31)
                  << std::right << std::setw(15)
                  << formatMoney(transaction.getAmount()) << '\n';
    }
    std::cout << separator << '\n';
}

void FinanceManager::displaySummary() const {
    std::cout << "\n========== FINANCIAL SUMMARY ==========\n"
              << std::left << std::setw(20) << "Total Income:"
              << std::right << std::setw(15) << formatMoney(calculateIncome()) << '\n'
              << std::left << std::setw(20) << "Total Expenses:"
              << std::right << std::setw(15) << formatMoney(calculateExpenses()) << '\n'
              << std::left << std::setw(20) << "Current Balance:"
              << std::right << std::setw(15) << formatMoney(calculateBalance()) << '\n'
              << "Transaction count: " << transactions_.size() << '\n';
}

double FinanceManager::calculateIncome() const {
    double total = 0.0;
    for (const Transaction& transaction : transactions_) {
        if (transaction.getType() == TransactionType::Income) {
            total += transaction.getAmount();
        }
    }
    return total;
}

double FinanceManager::calculateExpenses() const {
    double total = 0.0;
    for (const Transaction& transaction : transactions_) {
        if (transaction.getType() == TransactionType::Expense) {
            total += transaction.getAmount();
        }
    }
    return total;
}

double FinanceManager::calculateBalance() const {
    return calculateIncome() - calculateExpenses();
}

std::vector<Transaction> FinanceManager::searchByDescription(
    const std::string& term) const {
    std::vector<Transaction> matches;
    // Normalize both strings so search behavior is case-insensitive.
    const std::string normalizedTerm = toLower(term);

    for (const Transaction& transaction : transactions_) {
        if (toLower(transaction.getDescription()).find(normalizedTerm)
            != std::string::npos) {
            matches.push_back(transaction);
        }
    }
    return matches;
}

std::vector<Transaction> FinanceManager::filterByType(TransactionType type) const {
    std::vector<Transaction> matches;
    for (const Transaction& transaction : transactions_) {
        if (transaction.getType() == type) {
            matches.push_back(transaction);
        }
    }
    return matches;
}

std::vector<Transaction> FinanceManager::filterByCategory(
    const std::string& category) const {
    std::vector<Transaction> matches;
    const std::string normalizedCategory = toLower(category);

    for (const Transaction& transaction : transactions_) {
        if (toLower(transaction.getCategory()) == normalizedCategory) {
            matches.push_back(transaction);
        }
    }
    return matches;
}

void FinanceManager::sortByAmount(bool ascending) {
    // stable_sort preserves the existing order when two amounts are equal.
    std::stable_sort(
        transactions_.begin(),
        transactions_.end(),
        [ascending](const Transaction& left, const Transaction& right) {
            return ascending
                ? left.getAmount() < right.getAmount()
                : left.getAmount() > right.getAmount();
        });
}

void FinanceManager::sortByDate(bool ascending) {
    std::stable_sort(
        transactions_.begin(),
        transactions_.end(),
        [ascending](const Transaction& left, const Transaction& right) {
            return ascending
                ? left.getDate() < right.getDate()
                : left.getDate() > right.getDate();
        });
}

void FinanceManager::sortByCategory(bool ascending) {
    std::stable_sort(
        transactions_.begin(),
        transactions_.end(),
        [ascending](const Transaction& left, const Transaction& right) {
            const std::string leftCategory = toLower(left.getCategory());
            const std::string rightCategory = toLower(right.getCategory());
            return ascending
                ? leftCategory < rightCategory
                : leftCategory > rightCategory;
        });
}

bool FinanceManager::saveToFile(const std::string& filename) const {
    const std::filesystem::path path(filename);
    // Create the data directory automatically on a first run.
    if (path.has_parent_path()) {
        std::error_code error;
        std::filesystem::create_directories(path.parent_path(), error);
        if (error) {
            return false;
        }
    }

    std::ofstream output(filename);
    if (!output) {
        return false;
    }

    for (const Transaction& transaction : transactions_) {
        output << transaction.toFileString() << '\n';
    }
    return static_cast<bool>(output);
}

LoadResult FinanceManager::loadFromFile(const std::string& filename) {
    std::ifstream input(filename);
    if (!input) {
        return {false, 0, 0};
    }

    std::vector<Transaction> loadedTransactions;
    std::unordered_set<int> usedIds;
    std::size_t skipped = 0;
    int highestId = 0;
    std::string line;

    // Invalid records are counted and skipped so one bad line cannot crash loading.
    while (std::getline(input, line)) {
        if (InputHelper::trim(line).empty()) {
            continue;
        }

        const std::vector<std::string> fields = splitRecord(line);
        int id = 0;
        double amount = 0.0;

        if (fields.size() != 6
            || !parseInteger(fields[0], id)
            || id <= 0
            || usedIds.count(id) != 0
            || !InputHelper::isValidDate(fields[1])
            || InputHelper::trim(fields[2]).empty()
            || InputHelper::trim(fields[3]).empty()
            || !parseAmount(fields[4], amount)
            || (fields[5] != "Income" && fields[5] != "Expense")) {
            ++skipped;
            continue;
        }

        const TransactionType type =
            fields[5] == "Income" ? TransactionType::Income : TransactionType::Expense;
        loadedTransactions.emplace_back(
            id,
            fields[1],
            InputHelper::trim(fields[2]),
            InputHelper::trim(fields[3]),
            amount,
            type);
        usedIds.insert(id);
        highestId = std::max(highestId, id);
    }

    // Replace existing data only after the complete file has been processed.
    transactions_ = std::move(loadedTransactions);
    // Continue IDs above the largest loaded ID to avoid future duplicates.
    nextId_ = highestId + 1;
    return {true, transactions_.size(), skipped};
}

bool FinanceManager::isEmpty() const {
    return transactions_.empty();
}

std::size_t FinanceManager::getTransactionCount() const {
    return transactions_.size();
}

std::string FinanceManager::toLower(const std::string& text) {
    std::string result = text;
    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::tolower(character));
        });
    return result;
}

std::string FinanceManager::shorten(const std::string& text, std::size_t width) {
    if (text.size() <= width) {
        return text;
    }
    if (width <= 3) {
        return text.substr(0, width);
    }
    return text.substr(0, width - 3) + "...";
}

std::string FinanceManager::formatMoney(double amount) {
    std::ostringstream output;
    if (amount < 0.0) {
        output << "-$" << std::fixed << std::setprecision(2) << std::abs(amount);
    } else {
        output << '$' << std::fixed << std::setprecision(2) << amount;
    }
    return output.str();
}
