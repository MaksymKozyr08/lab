#include <iostream>
#include <vector>
using namespace std;
typedef long double ld;
typedef long long ll;

class TreeNode{
public:
    ll date;
    vector<TreeNode*> children;
    TreeNode(ll value):date(value){}
};

class NaryTree{
private:
    TreeNode* root;
    TreeNode* get_node_by_path(TreeNode* curr,const vector<ll>& p,ll idx){
        if(!curr)return nullptr;
        if(idx==p.size())return curr;
        if(p[idx]<0||p[idx]>=curr->children.size())return nullptr;
        return get_node_by_path(curr->children[p[idx]],p,idx+1);
    }
    void print_recursive(TreeNode* node,ll level){
        if(!node)return;
        for(ll i=0;i<level;i++)cout<<"  ";
        cout<<"- "<<node->date<<"\n";
        for(ll i=0;i<node->children.size();i++){
            print_recursive(node->children[i],level+1);
        }
    }
public:
    NaryTree():root(nullptr){}
    void add_node(const vector<ll>& path,ll val){
        TreeNode* temp=new TreeNode(val);
        if(!root){
            root=temp;
            return;
        }
        TreeNode* parent=get_node_by_path(root,path,0);
        if(parent){
            parent->children.push_back(temp);
        }else{
            cout<<"Invalid path!\n";
            delete temp;
        }
    }
    TreeNode* delete_node(const vector<ll>& path){
        if(!root)return nullptr;
        if(path.empty()){
            TreeNode* temp=root;
            root=nullptr;
            return temp;
        }
        vector<ll> parent_path=path;
        ll child_idx=parent_path.back();
        parent_path.pop_back();
        TreeNode* parent=get_node_by_path(root,parent_path,0);
        if(parent&&child_idx>=0&&child_idx<parent->children.size()){
            TreeNode* target=parent->children[child_idx];
            parent->children.erase(parent->children.begin()+child_idx);
            return target;
        }
        cout<<"Invalid path for deletion!\n";
        return nullptr;
    }
    void print_tree(){
        if(!root){cout<<"Empty\n";return;}
        print_recursive(root,0);
    }
    void demo(){
        cout<<"--- DEMO MODE ---\n";
        add_node({},10);
        add_node({},20);
        add_node({},30);
        add_node({0},40);
        add_node({0},50);
        add_node({1},60);
        cout<<"Tree Structure:\n";
        print_tree();
        cout<<"\nDeleting subtree at path [0, 1] (Node 50)...\n";
        TreeNode* deleted=delete_node({0,1});
        if(deleted)cout<<"Deleted node val: "<<deleted->date<<"\n";
        cout<<"\nTree after deletion:\n";
        print_tree();
        cout<<"\n";
    }
    void print_task_tree(){
        cout<<"--- TASK TREE (BONUS) ---\n";
        cout<<"Root\n";
        cout<<"  |- Block 0: Core (Tree Structure)\n";
        cout<<"      |- Block 1: Provides PATH (Path handling)\n";
        cout<<"          |- Block 1: Core (Add element by PATH)\n";
        cout<<"              |- Block 2: Core (Print with indents)\n";
        cout<<"                  |- Block 3: Core (Delete subtree by PATH)\n\n";
    }
};

int main(){
    cout<<"Select Mode:\n1.Interactive\n2.Demo\n3.Task Tree (Bonus)\n";
    ll mode;
    cin>>mode;
    NaryTree t;
    if(mode==2){
        t.demo();
    }else if(mode==3){
        t.print_task_tree();
    }else if(mode==1){
        while(true){
            cout<<"Tree: 1.Add 2.Del 3.Print 0.Exit: ";
            ll op;
            cin>>op;
            if(op==0)break;
            if(op==1){
                ll val;
                cout<<"val: ";cin>>val;
                cout<<"path length (0 for root/parent): ";
                ll len;cin>>len;
                vector<ll> p;
                if(len>0){
                    cout<<"path indices separated by space: ";
                    for(ll i=0;i<len;i++){
                        ll x;cin>>x;p.push_back(x);
                    }
                }
                t.add_node(p,val);
            }
            if(op==2){
                cout<<"path length: ";
                ll len;cin>>len;
                vector<ll> p;
                if(len>0){
                    cout<<"path indices separated by space: ";
                    for(ll i=0;i<len;i++){
                        ll x;cin>>x;p.push_back(x);
                    }
                }
                TreeNode* del=t.delete_node(p);
                if(del)cout<<"Deleted subtree with root: "<<del->date<<"\n";
            }
            if(op==3){
                t.print_tree();
            }
        }
    }
    return 0;
}