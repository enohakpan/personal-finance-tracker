#ifndef FINANCE_MANAGER_H
#define FINANCE_MANAGER_H

#include "Transaction.h"

#include <cstddef>
#include <string>
#include <vector>

// Reports what happened during a file load without printing from the manager.
struct LoadResult {
    bool fileFound;
    std::size_t loaded;
    std::size_t skipped;
};

// Owns the transaction collection and provides all finance operations.
class FinanceManager {
public:
    // Adds a transaction and assigns the next available ID.
    void addTransaction(const std::string& date,
                        const std::string& description,
                        const std::string& category,
                        double amount,
                        TransactionType type);

    // Displays either the full collection or a supplied result set.
    void displayTransactions() const;
    void displayTransactions(const std::vector<Transaction>& transactions,
                             const std::string& title) const;
    void displaySummary() const;

    // Calculates totals from the current transaction collection.
    double calculateIncome() const;
    double calculateExpenses() const;
    double calculateBalance() const;

    // Returns matching copies without changing the stored collection.
    std::vector<Transaction> searchByDescription(const std::string& term) const;
    std::vector<Transaction> filterByType(TransactionType type) const;
    std::vector<Transaction> filterByCategory(const std::string& category) const;

    // Reorders the stored collection using the selected field and direction.
    void sortByAmount(bool ascending);
    void sortByDate(bool ascending);
    void sortByCategory(bool ascending);

    // Persists all transactions or replaces them with records from a file.
    bool saveToFile(const std::string& filename) const;
    LoadResult loadFromFile(const std::string& filename);

    // Provides collection information without exposing the vector itself.
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
