#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>

using namespace std;

// Структура для опису задачі
struct Task {
    int id;         // Ідентифікатор задачі
    int arrivalTime; // Час надходження (AT)
    int burstTime;   // Час виконання (BT)
    
    // Поля для результатів обчислень
    int completionTime = 0; // Час завершення
    int waitingTime = 0;    // Час очікування
    int turnAroundTime = 0; // Час перебування в системі (Waiting + Burst)
    bool completed = false; // Прапорець для SJF
};

// Допоміжна функція для виведення таблиці результатів
void printSchedule(const vector<Task>& tasks, const string& algorithmName) {
    cout << "\n--- " << algorithmName << " Результати ---" << endl;
    cout << "ID\tAT\tBT\tCT\tTAT\tWT\n";
    cout << "--------------------------------------------\n";

    double totalWT = 0, totalTAT = 0;

    for (const auto& t : tasks) {
        cout << "P" << t.id << "\t" 
             << t.arrivalTime << "\t" 
             << t.burstTime << "\t" 
             << t.completionTime << "\t" 
             << t.turnAroundTime << "\t" 
             << t.waitingTime << endl;

        totalWT += t.waitingTime;
        totalTAT += t.turnAroundTime;
    }

    cout << "--------------------------------------------\n";
    cout << fixed << setprecision(2);
    cout << "Середній час очікування: " << (totalWT / tasks.size()) << endl;
    cout << "Середній час перебування: " << (totalTAT / tasks.size()) << endl;
}

// 1. Алгоритм FCFS (First Come First Served)
void runFCFS(vector<Task> tasks) { // Передаємо копію вектора, щоб не псувати оригінал
    // Сортуємо задачі за часом надходження
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;

    for (auto& t : tasks) {
        // Якщо процесор вільний, а задача ще не прийшла - чекаємо
        if (currentTime < t.arrivalTime) {
            currentTime = t.arrivalTime;
        }

        // Виконання задачі
        currentTime += t.burstTime;

        // Запис результатів
        t.completionTime = currentTime;
        t.turnAroundTime = t.completionTime - t.arrivalTime;
        t.waitingTime = t.turnAroundTime - t.burstTime;
    }

    printSchedule(tasks, "FCFS");
}

// 2. Алгоритм SJF (Shortest Job First) - Невитісняючий (Non-preemptive)
void runSJF(vector<Task> tasks) {
    int n = tasks.size();
    int completedCount = 0;
    int currentTime = 0;
    
    // Вектор для збереження порядку виконання (опціонально, тут ми просто оновлюємо tasks)
    // Важливо: для SJF ми не можемо просто відсортувати все на початку, 
    // бо ми повинні вибирати тільки з тих задач, які ВЖЕ надійшли.

    while (completedCount < n) {
        int idx = -1;
        int minBurst = INT_MAX;

        // Шукаємо задачу, яка:
        // 1. Вже надійшла (arrivalTime <= currentTime)
        // 2. Ще не виконана (!completed)
        // 3. Має найменший час виконання (burstTime)
        for (int i = 0; i < n; i++) {
            if (tasks[i].arrivalTime <= currentTime && !tasks[i].completed) {
                if (tasks[i].burstTime < minBurst) {
                    minBurst = tasks[i].burstTime;
                    idx = i;
                }
                // Якщо burstTime однакові, пріоритет у тієї, що прийшла раніше (FCFS для рівних)
                if (tasks[i].burstTime == minBurst) {
                    if (tasks[i].arrivalTime < tasks[idx].arrivalTime) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            // Знайшли задачу
            Task& t = tasks[idx];
            currentTime += t.burstTime;
            t.completionTime = currentTime;
            t.turnAroundTime = t.completionTime - t.arrivalTime;
            t.waitingTime = t.turnAroundTime - t.burstTime;
            t.completed = true;
            completedCount++;
        } else {
            // Якщо жодна задача ще не надійшла, рухаємо час вперед
            currentTime++;
        }
    }

    printSchedule(tasks, "SJF");
}

int main() {
    // Вхідні дані (можна змінити на введення з клавіатури)
    // Формат: {ID, ArrivalTime, BurstTime}
    vector<Task> taskList = {
        {1, 0, 8},
        {2, 1, 4},
        {3, 2, 9},
        {4, 3, 5}
    };

    cout << "Вхідні задачі:\nID\tAT\tBT\n";
    for(const auto& t : taskList) cout << t.id << "\t" << t.arrivalTime << "\t" << t.burstTime << endl;

    // Запуск FCFS
    runFCFS(taskList);

    // Запуск SJF
    runSJF(taskList);

    return 0;
}