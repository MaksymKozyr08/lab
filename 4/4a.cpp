#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef long long ll;

class TreeNode {
public:
    ll data;
    vector<TreeNode*> children;
    TreeNode(ll val) : data(val) {}
};

class NaryTree {
private:
    TreeNode* root;

    TreeNode* get_node_by_path(TreeNode* curr, const vector<ll>& path, ll idx) {
        if (!curr) return nullptr;
        if (idx == path.size()) return curr;
        if (path[idx] < 0 || path[idx] >= curr->children.size()) return nullptr;
        return get_node_by_path(curr->children[path[idx]], path, idx + 1);
    }

    bool find_path_helper(TreeNode* node, ll val, vector<ll>& curPath, vector<ll>& resPath) {
        if (!node) return false;
        if (node->data == val) {
            resPath = curPath;
            return true;
        }
        for (size_t i = 0; i < node->children.size(); i++) {
            curPath.push_back(i);
            if (find_path_helper(node->children[i], val, curPath, resPath)) return true;
            curPath.pop_back();
        }
        return false;
    }

    void print_recursive(TreeNode* node, ll level) {
        if (!node) return;
        for (ll i = 0; i < level; i++) cout << "  ";
        cout << "- " << node->data << "\n";
        for (size_t i = 0; i < node->children.size(); i++) {
            print_recursive(node->children[i], level + 1);
        }
    }

    void add_prob_helper(TreeNode* curr, ll val, double p) {
        if (!curr) return;
        double r = (double)rand() / RAND_MAX;
        if (r <= p || curr->children.empty()) {
            curr->children.push_back(new TreeNode(val));
        } else {
            ll random_child = rand() % curr->children.size();
            add_prob_helper(curr->children[random_child], val, p);
        }
    }

public:
    NaryTree() { root = nullptr; }

    void add_node(const vector<ll>& path, ll val) {
        TreeNode* tmp = new TreeNode(val);
        if (!root) {
            root = tmp;
            return;
        }
        TreeNode* parent = get_node_by_path(root, path, 0);
        if (parent) {
            parent->children.push_back(tmp);
        } else {
            cout << "Path not found!\n";
            delete tmp;
        }
    }

    void add_with_probability(ll val, double p) {
        if (!root) {
            root = new TreeNode(val);
        } else {
            add_prob_helper(root, val, p);
        }
    }

    bool get_path_to_element(ll val, vector<ll>& outPath) {
        vector<ll> tempPath;
        return find_path_helper(root, val, tempPath, outPath);
    }

    TreeNode* delete_node(const vector<ll>& path) {
        if (!root) return nullptr;
        if (path.empty()) {
            TreeNode* tmp = root;
            root = nullptr;
            return tmp;
        }
        vector<ll> pPath = path;
        ll childIdx = pPath.back();
        pPath.pop_back();

        TreeNode* parent = get_node_by_path(root, pPath, 0);
        if (parent && childIdx >= 0 && childIdx < parent->children.size()) {
            TreeNode* target = parent->children[childIdx];
            parent->children.erase(parent->children.begin() + childIdx);
            return target;
        }
        return nullptr;
    }

    TreeNode* delete_by_value(ll val) {
        vector<ll> path;
        if (get_path_to_element(val, path)) {
            return delete_node(path);
        }
        return nullptr;
    }

    void clear_memory(TreeNode* node) {
        if (!node) return;
        for (auto child : node->children) {
            clear_memory(child);
        }
        delete node;
    }

    void print_tree() {
        if (!root) cout << "Tree is empty\n";
        else print_recursive(root, 0);
    }

    void print_bfs() {
        if (!root) return;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                TreeNode* curr = q.front(); q.pop();
                cout << curr->data << " ";
                for (auto c : curr->children) q.push(c);
            }
            cout << "\n";
        }
    }

    void demo() {
        cout << "--- N-ary Tree Demonstration ---\n";
        add_node({}, 10);
        add_node({}, 20);
        add_node({}, 30);
        add_node({0}, 40);
        add_node({1}, 50);
        
        cout << "Added nodes (print with indents):\n";
        print_tree();

        cout << "\nLevel-order print (BFS bonus):\n";
        print_bfs();

        vector<ll> p;
        if (get_path_to_element(50, p)) {
            cout << "\nFound path to 50: ";
            for (ll x : p) cout << x << " ";
            cout << "\n";
        }

        cout << "\nDeleting subtree with value 20...\n";
        TreeNode* del = delete_by_value(20);
        clear_memory(del);
        print_tree();
    }
};

class BSTNode {
public:
    ll data;
    BSTNode *left, *right;
    BSTNode(ll v) : data(v), left(nullptr), right(nullptr) {}
};

class BinaryTree {
    BSTNode* root;

    BSTNode* insert_rec(BSTNode* node, ll val) {
        if (!node) return new BSTNode(val);
        if (val < node->data) node->left = insert_rec(node->left, val);
        else node->right = insert_rec(node->right, val);
        return node;
    }

    void pre_order(BSTNode* node, vector<ll>& res) {
        if (!node) return;
        res.push_back(node->data);
        pre_order(node->left, res);
        pre_order(node->right, res);
    }

public:
    BinaryTree() { root = nullptr; }
    void add(ll val) { root = insert_rec(root, val); }
    
    void demo_block_5() {
        add(50); add(30); add(70); add(20); add(40);
        vector<ll> seq;
        pre_order(root, seq);
        cout << "Pre-order (sequential) representation of BST: ";
        for (ll x : seq) cout << x << " ";
        cout << "\n";
    }
};

class ExprNode {
public:
    string val;
    ExprNode *left, *right;
    ExprNode(string v) : val(v), left(nullptr), right(nullptr) {}
};

class ExprTree {
    double eval(ExprNode* node) {
        if (!node->left && !node->right) return stod(node->val);
        double l = eval(node->left);
        double r = eval(node->right);
        if (node->val == "+") return l + r;
        if (node->val == "*") return l * r;
        return 0;
    }
public:
    void demo() {
        ExprNode* root = new ExprNode("*");
        root->left = new ExprNode("+");
        root->right = new ExprNode("4");
        root->left->left = new ExprNode("2");
        root->left->right = new ExprNode("3");
        cout << "Expression: (2 + 3) * 4 = " << eval(root) << "\n";
    }
};

int main() {
    srand(time(0));
    NaryTree tree;
    BinaryTree bst;
    ExprTree expr;

    while (true) {
        cout << "\n1.Interactive 2.Demo(Block 0-3) 3.Binary(Block 4-5) 4.Expressions(Block 6) 0.Exit\nChoose: ";
        int mode; cin >> mode;
        if (mode == 0) break;

        if (mode == 2) {
            tree.demo();
        } else if (mode == 3) {
            bst.demo_block_5();
        } else if (mode == 4) {
            expr.demo();
        } else if (mode == 1) {
            while (true) {
                cout << "\n1.Add by path 2.Add with prob. 3.Print indents 4.Print levels 5.Find path 6.Delete by value 0.Back: ";
                int op; cin >> op;
                if (op == 0) break;

                if (op == 1) {
                    ll val, len;
                    cout << "Value: "; cin >> val;
                    cout << "Path length (0 for root): "; cin >> len;
                    vector<ll> path;
                    for (int i = 0; i < len; i++) {
                        ll x; cin >> x; path.push_back(x);
                    }
                    tree.add_node(path, val);
                }
                else if (op == 2) {
                    ll val; double p;
                    cout << "Value: "; cin >> val;
                    cout << "Probability (0.0 - 1.0): "; cin >> p;
                    tree.add_with_probability(val, p);
                }
                else if (op == 3) tree.print_tree();
                else if (op == 4) tree.print_bfs();
                else if (op == 5) {
                    ll val;
                    cout << "Search value: "; cin >> val;
                    vector<ll> path;
                    if (tree.get_path_to_element(val, path)) {
                        cout << "Path: ";
                        for (ll x : path) cout << x << " ";
                        cout << "\n";
                    } else cout << "Not found.\n";
                }
                else if (op == 6) {
                    ll val;
                    cout << "Delete value: "; cin >> val;
                    TreeNode* del = tree.delete_by_value(val);
                    if (del) {
                        cout << "Deleted!\n";
                        tree.clear_memory(del);
                    } else cout << "Element not found.\n";
                }
            }
        }
    }
    return 0;
}