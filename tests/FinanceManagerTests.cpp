#include "../FinanceManager.h"
#include "../InputHelper.h"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>

namespace {
bool nearlyEqual(double left, double right) {
    return std::abs(left - right) < 0.001;
}
} // namespace

int main() {
    assert(InputHelper::isValidDate("2024-02-29"));
    assert(!InputHelper::isValidDate("2023-02-29"));
    assert(!InputHelper::isValidDate("09/19/2026"));

    FinanceManager manager;
    assert(manager.isEmpty());

    manager.addTransaction(
        "2026-09-19", "Monthly Salary", "Work", 2500.0,
        TransactionType::Income);
    manager.addTransaction(
        "2026-09-20", "Groceries", "Food", 150.0,
        TransactionType::Expense);
    manager.addTransaction(
        "2026-09-21", "Bus Pass", "Transport", 75.0,
        TransactionType::Expense);

    assert(manager.getTransactionCount() == 3);
    assert(nearlyEqual(manager.calculateIncome(), 2500.0));
    assert(nearlyEqual(manager.calculateExpenses(), 225.0));
    assert(nearlyEqual(manager.calculateBalance(), 2275.0));

    assert(manager.searchByDescription("salary").size() == 1);
    assert(manager.searchByDescription("BUS").size() == 1);
    assert(manager.searchByDescription("missing").empty());
    assert(manager.filterByType(TransactionType::Expense).size() == 2);
    assert(manager.filterByCategory("food").size() == 1);

    manager.sortByAmount(true);
    manager.sortByDate(false);
    manager.sortByCategory(true);

    const char* testFile = "data/test_transactions.txt";
    assert(manager.saveToFile(testFile));

    FinanceManager restored;
    const LoadResult result = restored.loadFromFile(testFile);
    assert(result.fileFound);
    assert(result.loaded == 3);
    assert(result.skipped == 0);
    assert(nearlyEqual(restored.calculateBalance(), 2275.0));

    std::remove(testFile);
    std::cout << "All FinanceManager tests passed.\n";
    return 0;
}
