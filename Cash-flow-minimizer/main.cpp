#include "debt_manager.h"

int main() {
    DebtGraph graph;
    int numParticipants;

    std::cout << "Enter number of participants: ";
    std::cin >> numParticipants;
    std::cin.ignore();

    for (int i = 0; i < numParticipants; i++) {
        char name[100];
        std::cout << "Enter name of participant " << i + 1 << ": ";
        std::cin.getline(name, 100);
        String participantName(name);
        graph.addParticipant(participantName);
    }

    graph.displayParticipants();

    int numDebts;
    std::cout << "Enter number of debt records (should be <= number of participants): ";
    std::cin >> numDebts;

    if (numDebts > numParticipants) {
        std::cout << "Error: The number of debts cannot exceed the number of participants.\n";
        return 1;
    }

    std::cin.ignore();

    for (int i = 0; i < numDebts; i++) {
        char from[100], to[100];
        int amount;
        std::cout << "Enter debt " << i + 1 << " (format: Person A Person B Amount): ";
        std::cin.getline(from, 100);
        std::cin.getline(to, 100);
        std::cin >> amount;
        std::cin.ignore();

        String fromName(from);
        String toName(to);
        bool debtRecorded = graph.addDebt(fromName, toName, amount);

        if (!debtRecorded) {
            i--;
        }
    }

    graph.minimizeTransactions();
    return 0;
}
