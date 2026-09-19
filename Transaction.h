#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

// Identifies whether a transaction adds to or subtracts from the balance.
enum class TransactionType {
    Income,
    Expense
};

// Stores the data for one financial transaction.
class Transaction {
public:
    // Creates an empty transaction or a fully initialized transaction.
    Transaction();
    Transaction(int id,
                const std::string& date,
                const std::string& description,
                const std::string& category,
                double amount,
                TransactionType type);

    // Accessors expose transaction data without allowing direct modification.
    int getId() const;
    const std::string& getDate() const;
    const std::string& getDescription() const;
    const std::string& getCategory() const;
    double getAmount() const;
    TransactionType getType() const;

    // Mutators allow editable fields to be changed after construction.
    void setDate(const std::string& date);
    void setDescription(const std::string& description);
    void setCategory(const std::string& category);
    void setAmount(double amount);
    void setType(TransactionType type);

    // Returns the user-facing name of the transaction type.
    std::string getTypeName() const;

    // Serializes the transaction as one pipe-delimited file record.
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
