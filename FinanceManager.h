#ifndef FINANCE_MANAGER_H
#define FINANCE_MANAGER_H

#include "Transaction.h"

#include <cstddef>
#include <string>
#include <vector>

struct LoadResult {
    bool fileFound;
    std::size_t loaded;
    std::size_t skipped;
};

class FinanceManager {
public:
    void addTransaction(const std::string& date,
                        const std::string& description,
                        const std::string& category,
                        double amount,
                        TransactionType type);

    void displayTransactions() const;
    void displayTransactions(const std::vector<Transaction>& transactions,
                             const std::string& title) const;
    void displaySummary() const;

    double calculateIncome() const;
    double calculateExpenses() const;
    double calculateBalance() const;

    std::vector<Transaction> searchByDescription(const std::string& term) const;
    std::vector<Transaction> filterByType(TransactionType type) const;
    std::vector<Transaction> filterByCategory(const std::string& category) const;

    void sortByAmount(bool ascending);
    void sortByDate(bool ascending);
    void sortByCategory(bool ascending);

    bool saveToFile(const std::string& filename) const;
    LoadResult loadFromFile(const std::string& filename);

    bool isEmpty() const;
    std::size_t getTransactionCount() const;

private:
    std::vector<Transaction> transactions_;
    int nextId_ = 1;

    static std::string toLower(const std::string& text);
    static std::string shorten(const std::string& text, std::size_t width);
    static std::string formatMoney(double amount);
};

#endif
