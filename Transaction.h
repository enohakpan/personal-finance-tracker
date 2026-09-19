#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

enum class TransactionType {
    Income,
    Expense
};

class Transaction {
public:
    Transaction();
    Transaction(int id,
                const std::string& date,
                const std::string& description,
                const std::string& category,
                double amount,
                TransactionType type);

    int getId() const;
    const std::string& getDate() const;
    const std::string& getDescription() const;
    const std::string& getCategory() const;
    double getAmount() const;
    TransactionType getType() const;

    void setDate(const std::string& date);
    void setDescription(const std::string& description);
    void setCategory(const std::string& category);
    void setAmount(double amount);
    void setType(TransactionType type);

    std::string getTypeName() const;
    std::string toFileString() const;

private:
    int id_;
    std::string date_;
    std::string description_;
    std::string category_;
    double amount_;
    TransactionType type_;
};

#endif
