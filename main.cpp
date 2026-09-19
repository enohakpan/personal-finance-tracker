#include "FinanceManager.h"
#include "InputHelper.h"

#include <exception>
#include <iostream>
#include <string>

namespace {
const std::string DATA_FILE = "data/transactions.txt";

// Prints the choices available in the main application loop.
void displayMenu() {
    std::cout << "\n========================================\n"
              << "       PERSONAL FINANCE TRACKER\n"
              << "========================================\n"
              << "1. Add Income\n"
              << "2. Add Expense\n"
              << "3. View Transactions\n"
              << "4. Financial Summary\n"
              << "5. Search Transactions\n"
              << "6. Filter Transactions\n"
              << "7. Sort Transactions\n"
              << "8. Save Data\n"
              << "9. Load Data\n"
              << "0. Exit\n";
}

// Collects validated fields and delegates storage to FinanceManager.
void addTransaction(FinanceManager& manager, TransactionType type) {
    std::cout << "\n========== ADD "
              << (type == TransactionType::Income ? "INCOME" : "EXPENSE")
              << " ==========\n";

    const std::string date =
        InputHelper::readDate("Enter date (YYYY-MM-DD): ");
    const std::string description =
        InputHelper::readRequiredText("Enter description: ");
    const std::string category =
        InputHelper::readRequiredText("Enter category: ");
    const double amount =
        InputHelper::readPositiveAmount("Enter amount: ");

    manager.addTransaction(date, description, category, amount, type);
    std::cout << (type == TransactionType::Income ? "Income" : "Expense")
              << " added successfully.\n";
}

// Displays transactions whose descriptions contain the user's search term.
void searchTransactions(const FinanceManager& manager) {
    const std::string term =
        InputHelper::readRequiredText("Enter description search term: ");
    manager.displayTransactions(
        manager.searchByDescription(term), "SEARCH RESULTS");
}

// Handles the filter submenu without modifying the original collection.
void filterTransactions(const FinanceManager& manager) {
    std::cout << "\n========== FILTER TRANSACTIONS ==========\n"
              << "1. Show all\n"
              << "2. Show income\n"
              << "3. Show expenses\n"
              << "4. Filter by category\n"
              << "0. Return to main menu\n";

    const int choice =
        InputHelper::readIntInRange("Choose an option: ", 0, 4);

    switch (choice) {
        case 1:
            manager.displayTransactions();
            break;
        case 2:
            manager.displayTransactions(
                manager.filterByType(TransactionType::Income),
                "INCOME TRANSACTIONS");
            break;
        case 3:
            manager.displayTransactions(
                manager.filterByType(TransactionType::Expense),
                "EXPENSE TRANSACTIONS");
            break;
        case 4: {
            const std::string category =
                InputHelper::readRequiredText("Enter category: ");
            manager.displayTransactions(
                manager.filterByCategory(category),
                "CATEGORY: " + category);
            break;
        }
        default:
            break;
    }
}

// Selects a sort field and direction, then displays the reordered collection.
void sortTransactions(FinanceManager& manager) {
    std::cout << "\n========== SORT TRANSACTIONS ==========\n"
              << "1. Sort by amount\n"
              << "2. Sort by date\n"
              << "3. Sort by category\n"
              << "0. Return to main menu\n";

    const int field =
        InputHelper::readIntInRange("Choose an option: ", 0, 3);
    if (field == 0) {
        return;
    }

    std::cout << "\n1. Ascending\n"
              << "2. Descending\n";
    const bool ascending =
        InputHelper::readIntInRange("Choose an order: ", 1, 2) == 1;

    if (field == 1) {
        manager.sortByAmount(ascending);
    } else if (field == 2) {
        manager.sortByDate(ascending);
    } else {
        manager.sortByCategory(ascending);
    }

    std::cout << "Transactions sorted successfully.\n";
    manager.displayTransactions();
}

// Converts the manager's save result into a user-facing message.
void saveData(const FinanceManager& manager) {
    if (manager.saveToFile(DATA_FILE)) {
        std::cout << "Transactions saved successfully.\n";
    } else {
        std::cout << "Unable to save transactions. Check the data folder permissions.\n";
    }
}

// Loads saved data and reports missing or malformed records safely.
void loadData(FinanceManager& manager, bool startup) {
    const LoadResult result = manager.loadFromFile(DATA_FILE);
    if (!result.fileFound) {
        std::cout << "No saved transaction file found.";
        if (startup) {
            std::cout << " Starting with an empty transaction list.";
        }
        std::cout << '\n';
        return;
    }

    std::cout << "Transactions loaded successfully. "
              << result.loaded << " record(s) loaded";
    if (result.skipped > 0) {
        std::cout << "; " << result.skipped << " invalid record(s) skipped";
    }
    std::cout << ".\n";
}
} // namespace

int main() {
    FinanceManager manager;

    std::cout << "Welcome to the Personal Finance Tracker.\n";
    // Restore the previous session when a save file is available.
    loadData(manager, true);

    try {
        bool running = true;
        // Continue dispatching menu choices until the user selects Exit.
        while (running) {
            displayMenu();
            const int choice =
                InputHelper::readIntInRange("Choose an option: ", 0, 9);

            switch (choice) {
                case 1:
                    addTransaction(manager, TransactionType::Income);
                    break;
                case 2:
                    addTransaction(manager, TransactionType::Expense);
                    break;
                case 3:
                    manager.displayTransactions();
                    break;
                case 4:
                    manager.displaySummary();
                    break;
                case 5:
                    searchTransactions(manager);
                    break;
                case 6:
                    filterTransactions(manager);
                    break;
                case 7:
                    sortTransactions(manager);
                    break;
                case 8:
                    saveData(manager);
                    break;
                case 9:
                    loadData(manager, false);
                    break;
                case 0: {
                    const int saveChoice = InputHelper::readIntInRange(
                        "Save before exiting? (1 = Yes, 2 = No): ", 1, 2);
                    if (saveChoice == 1) {
                        saveData(manager);
                    }
                    running = false;
                    break;
                }
                default:
                    break;
            }
        }
    } catch (const std::exception& error) {
        std::cout << "\n" << error.what() << " Exiting safely.\n";
    }

    std::cout << "Thank you for using the Personal Finance Tracker.\n";
    return 0;
}
