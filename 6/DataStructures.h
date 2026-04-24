#pragma once
#include "IOrderedList.h"
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>

// 1. ArrayList (через вектор)
template <typename T>
class ArrayList : public IOrderedList<T> {
private:
    std::vector<T> data;
public:
    void insert(const T& val) override {
        // Звичайний пошук позиції і вставка.
        auto it = data.begin();
        while (it != data.end() && *it < val) {
            ++it;
        }
        data.insert(it, val);
    }

    void clear() override {
        data.clear();
    }

    std::string toJson() const override {
        std::stringstream ss;
        ss << R"({"type": "array", "nodes": [)";
        for (size_t i = 0; i < data.size(); ++i) {
            ss << "\"" << data[i] << "\"";
            if (i < data.size() - 1) ss << ", ";
        }
        ss << "]}";
        return ss.str();
    }
};

// 2. LinkedList
template <typename T>
class LinkedList : public IOrderedList<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };
    Node* head;
public:
    LinkedList() : head(nullptr) {}
    ~LinkedList() { clear(); }

    void insert(const T& val) override {
        Node* newNode = new Node(val);
        // Якщо список пустий або новий елемент менший за перший
        if (!head || val < head->data) {
            newNode->next = head;
            head = newNode;
            return;
        }
        Node* curr = head;
        while (curr->next && curr->next->data < val) {
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
    }

    void clear() override {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    std::string toJson() const override {
        std::stringstream ss;
        ss << R"({"type": "array", "nodes": [)";
        Node* curr = head;
        while (curr) {
            ss << "\"" << curr->data << "\"";
            if (curr->next) ss << ", ";
            curr = curr->next;
        }
        ss << "]}";
        return ss.str();
    }
};

// 3. Звичайне ДСП (без балансування)
template <typename T>
class BST : public IOrderedList<T> {
private:
    struct Node {
        T data;
        int id; // айдішник для візуалізатора
        Node* left;
        Node* right;
        Node(const T& d, int _id) : data(d), id(_id), left(nullptr), right(nullptr) {}
    };
    Node* root;
    int next_id;

    Node* insertRec(Node* node, const T& val) {
        if (!node) return new Node(val, next_id++);
        if (val < node->data) {
            node->left = insertRec(node->left, val);
        } else if (node->data < val) {
            node->right = insertRec(node->right, val);
        }
        return node;
    }

    void clearRec(Node* node) {
        if (!node) return;
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }

    void toJsonRec(Node* node, std::stringstream& ss, bool& first) const {
        if (!node) return;
        if (!first) ss << ", ";
        first = false;
        
        ss << "{\"id\": " << node->id << ", \"val\": \"" << node->data << "\"";
        ss << ", \"left\": " << (node->left ? std::to_string(node->left->id) : "null");
        ss << ", \"right\": " << (node->right ? std::to_string(node->right->id) : "null");
        ss << "}";

        toJsonRec(node->left, ss, first);
        toJsonRec(node->right, ss, first);
    }

public:
    BST() : root(nullptr), next_id(0) {}
    ~BST() { clear(); }

    void insert(const T& val) override {
        root = insertRec(root, val);
    }

    void clear() override {
        clearRec(root);
        root = nullptr;
        next_id = 0;
    }

    std::string toJson() const override {
        std::stringstream ss;
        ss << R"({"type": "tree", "nodes": [)";
        bool first = true;
        toJsonRec(root, ss, first);
        ss << "]}";
        return ss.str();
    }
};

// 4. AVLTree (з балансуванням)
template <typename T>
class AVLTree : public IOrderedList<T> {
private:
    struct Node {
        T data;
        int id;
        Node* left;
        Node* right;
        int height;
        Node(const T& d, int _id) : data(d), id(_id), left(nullptr), right(nullptr), height(1) {}
    };
    Node* root;
    int next_id;

    int getHeight(Node* N) {
        return N ? N->height : 0;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
        return y;
    }

    int getBalance(Node* N) {
        return N ? getHeight(N->left) - getHeight(N->right) : 0;
    }

    Node* insertRec(Node* node, const T& val) {
        if (!node) return new Node(val, next_id++);

        if (val < node->data) node->left = insertRec(node->left, val);
        else if (node->data < val) node->right = insertRec(node->right, val);
        else return node; // дублікати ігноруємо

        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        // Магія поворотів для AVL
        if (balance > 1 && val < node->left->data) return rightRotate(node);
        if (balance < -1 && node->right->data < val) return leftRotate(node);
        if (balance > 1 && node->left->data < val) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && val < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void clearRec(Node* node) {
        if (!node) return;
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }

    void toJsonRec(Node* node, std::stringstream& ss, bool& first) const {
        if (!node) return;
        if (!first) ss << ", ";
        first = false;
        
        ss << "{\"id\": " << node->id << ", \"val\": \"" << node->data << "\"";
        ss << ", \"left\": " << (node->left ? std::to_string(node->left->id) : "null");
        ss << ", \"right\": " << (node->right ? std::to_string(node->right->id) : "null");
        ss << "}";

        toJsonRec(node->left, ss, first);
        toJsonRec(node->right, ss, first);
    }

public:
    AVLTree() : root(nullptr), next_id(0) {}
    ~AVLTree() { clear(); }

    void insert(const T& val) override {
        root = insertRec(root, val);
    }

    void clear() override {
        clearRec(root);
        root = nullptr;
        next_id = 0;
    }

    std::string toJson() const override {
        std::stringstream ss;
        ss << R"({"type": "tree", "nodes": [)";
        bool first = true;
        toJsonRec(root, ss, first);
        ss << "]}";
        return ss.str();
    }
};

// 5. StdSetWrapper
// Використовую std::set, оскільки під капотом це Red-Black Tree. 
// Так закриваємо умову про бібліотечні структури для доп. балів.
template <typename T>
class StdSetWrapper : public IOrderedList<T> {
private:
    std::set<T> data;
public:
    void insert(const T& val) override {
        data.insert(val);
    }

    void clear() override {
        data.clear();
    }

    std::string toJson() const override {
        std::stringstream ss;
        ss << R"({"type": "array", "nodes": [)";
        auto it = data.begin();
        while (it != data.end()) {
            ss << "\"" << *it << "\"";
            auto next_it = std::next(it);
            if (next_it != data.end()) ss << ", ";
            it = next_it;
        }
        ss << "]}";
        return ss.str();
    }
};
