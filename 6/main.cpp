#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <cstdlib>
#include <string>

#include "Point3D.h"
#include "IOrderedList.h"
#include "DataStructures.h"

using namespace std;

// Функція для логування кроку у JSON
void logStep(ofstream& out, int step_idx, const Point3D& pt, 
             IOrderedList<Point3D>& arr, IOrderedList<Point3D>& ll, 
             IOrderedList<Point3D>& bst, IOrderedList<Point3D>& avl, 
             IOrderedList<Point3D>& setWrapper, bool is_first) {
             
    if (!is_first) out << ",\n";
    out << "    {\n";
    out << "      \"step_idx\": " << step_idx << ",\n";
    out << "      \"inserted_point\": \"Point" << pt << "\",\n";
    out << "      \"structures\": {\n";
    out << "        \"ArrayList\": " << arr.toJson() << ",\n";
    out << "        \"LinkedList\": " << ll.toJson() << ",\n";
    out << "        \"BST\": " << bst.toJson() << ",\n";
    out << "        \"AVLTree\": " << avl.toJson() << ",\n";
    out << "        \"StdSetWrapper\": " << setWrapper.toJson() << "\n";
    out << "      }\n";
    out << "    }";
}

void demoMode(bool launch_python = true) {
    ArrayList<Point3D> arr;
    LinkedList<Point3D> ll;
    BST<Point3D> bst;
    AVLTree<Point3D> avl;
    StdSetWrapper<Point3D> setWrapper;

    ofstream out("steps.json");
    out << "{\n  \"steps\": [\n";

    random_device rd;
    mt19937 gen(42); // фіксований сід для передбачуваності на демо
    uniform_int_distribution<> dis(-10, 10);

    if (launch_python) cout << "\n[!] Запуск Демо-режиму...\n[*] Генеруємо 10 точок.\n";

    for (int i = 0; i < 10; ++i) {
        Point3D pt(dis(gen), dis(gen), dis(gen));
        arr.insert(pt);
        ll.insert(pt);
        bst.insert(pt);
        avl.insert(pt);
        setWrapper.insert(pt);

        logStep(out, i, pt, arr, ll, bst, avl, setWrapper, (i == 0));
    }

    out << "\n  ]\n}\n";
    out.close();

    if (launch_python) {
        cout << "[*] Файл steps.json успішно згенеровано!\n";
        cout << "[!] Запускаю Python візуалізатор...\n";
        system("py visualizer.py");
    }
}

void interactiveFileMode(const string& filename) {
    ArrayList<Point3D> arr;
    LinkedList<Point3D> ll;
    BST<Point3D> bst;
    AVLTree<Point3D> avl;
    StdSetWrapper<Point3D> setWrapper;

    ofstream out("steps.json");
    out << "{\n  \"steps\": [\n";

    ifstream in(filename);
    double x, y, z;
    int step = 0;
    while (in >> x >> y >> z) {
        Point3D pt(x, y, z);
        arr.insert(pt);
        ll.insert(pt);
        bst.insert(pt);
        avl.insert(pt);
        setWrapper.insert(pt);

        logStep(out, step, pt, arr, ll, bst, avl, setWrapper, (step == 0));
        step++;
    }

    out << "\n  ]\n}\n";
    out.close();
}

void interactiveMode() {
    ArrayList<Point3D> arr;
    LinkedList<Point3D> ll;
    BST<Point3D> bst;
    AVLTree<Point3D> avl;
    StdSetWrapper<Point3D> setWrapper;

    ofstream out("steps.json");
    out << "{\n  \"steps\": [\n";

    cout << "\n[!] Інтерактивний режим.\n";
    cout << "Введіть координати X Y Z (або будь-яку літеру для завершення та показу):\n";
    
    double x, y, z;
    int step = 0;
    while (true) {
        cout << "Точка " << step + 1 << ": ";
        if (!(cin >> x >> y >> z)) {
            break;
        }
        Point3D pt(x, y, z);
        arr.insert(pt);
        ll.insert(pt);
        bst.insert(pt);
        avl.insert(pt);
        setWrapper.insert(pt);

        logStep(out, step, pt, arr, ll, bst, avl, setWrapper, (step == 0));
        step++;
    }
    
    cin.clear();
    cin.ignore(10000, '\n');

    out << "\n  ]\n}\n";
    out.close();

    if (step > 0) {
        cout << "[!] Запускаю Python візуалізатор...\n";
        system("py visualizer.py");
    } else {
        cout << "[-] Нічого не введено.\n";
    }
}

void benchmarkMode() {
    cout << "\n[!] Бенчмарк-режим\n";
    const int N = 10000;
    cout << "[*] Генеруємо " << N << " точок...\n";

    vector<Point3D> points;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-100.0, 100.0);
    
    for (int i = 0; i < N; ++i) {
        points.emplace_back(dis(gen), dis(gen), dis(gen));
    }

    auto measure = [&](IOrderedList<Point3D>& list, const string& name) {
        auto start = chrono::high_resolution_clock::now();
        for (const auto& pt : points) {
            list.insert(pt);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = end - start;
        cout << name << " -> " << ms.count() << " ms\n";
    };

    ArrayList<Point3D> arr;
    LinkedList<Point3D> ll;
    BST<Point3D> bst;
    AVLTree<Point3D> avl;
    StdSetWrapper<Point3D> setWrapper;

    cout << "[*] Вставка елементів:\n";
    measure(arr, "ArrayList");
    measure(ll, "LinkedList");
    measure(bst, "BST");
    measure(avl, "AVLTree");
    measure(setWrapper, "StdSetWrapper");
}

int main(int argc, char* argv[]) {
    // фікс кирилиці консолі (типовий студентський милиць)
    system("chcp 65001 > nul"); 

    // Обробка аргументів командного рядка (для викликів з Python GUI)
    if (argc > 1) {
        string mode = argv[1];
        if (mode == "--demo") {
            demoMode(false); // генеруємо json, але не запускаємо python знову
            return 0;
        } else if (mode == "--benchmark") {
            benchmarkMode();
            return 0;
        } else if (mode == "--file" && argc > 2) {
            interactiveFileMode(argv[2]);
            return 0;
        }
    }

    // Звичайне консольне меню
    while (true) {
        cout << "\n===== ЛАБОРАТОРНА РОБОТА 6 =====\n";
        cout << "1. Демо режим (10 точок + Візуалізатор)\n";
        cout << "2. Інтерактив (Ручний ввід + Візуалізатор)\n";
        cout << "3. Бенчмарк (Тест на швидкість)\n";
        cout << "0. Вихід\n";
        cout << "Оберіть опцію: ";
        
        int choice;
        if (!(cin >> choice)) {
            break;
        }

        switch (choice) {
            case 1: demoMode(true); break;
            case 2: interactiveMode(); break;
            case 3: benchmarkMode(); break;
            case 0: cout << "Вихід...\n"; return 0;
            default: cout << "Некоректний вибір!\n"; break;
        }
    }
    return 0;
}
