#include "Transaction.h"

#include <iomanip>
#include <sstream>

// The default values provide a safe placeholder object when one is needed.
Transaction::Transaction()
    : id_(0), amount_(0.0), type_(TransactionType::Expense) {
}

Transaction::Transaction(int id,
                         const std::string& date,
                         const std::string& description,
                         const std::string& category,
                         double amount,
                         TransactionType type)
    : id_(id),
      date_(date),
      description_(description),
      category_(category),
      amount_(amount),
      type_(type) {
}

int Transaction::getId() const {
    return id_;
}

const std::string& Transaction::getDate() const {
    return date_;
}

const std::string& Transaction::getDescription() const {
    return description_;
}

const std::string& Transaction::getCategory() const {
    return category_;
}

double Transaction::getAmount() const {
    return amount_;
}

TransactionType Transaction::getType() const {
    return type_;
}

void Transaction::setDate(const std::string& date) {
    date_ = date;
}

void Transaction::setDescription(const std::string& description) {
    description_ = description;
}

void Transaction::setCategory(const std::string& category) {
    category_ = category;
}

void Transaction::setAmount(double amount) {
    amount_ = amount;
}

void Transaction::setType(TransactionType type) {
    type_ = type;
}

std::string Transaction::getTypeName() const {
    return type_ == TransactionType::Income ? "Income" : "Expense";
}

std::string Transaction::toFileString() const {
    std::ostringstream output;
    // Two decimal places keep saved monetary values consistent and readable.
    output << id_ << '|'
           << date_ << '|'
           << description_ << '|'
           << category_ << '|'
           << std::fixed << std::setprecision(2) << amount_ << '|'
           << getTypeName();
    return output.str();
}
