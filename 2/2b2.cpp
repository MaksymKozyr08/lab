#include <iostream>
using namespace std;

typedef long long ll;

struct job{
    ll id;
    ll burst;
};

struct Node{
    job date;
    Node* next;
};
struct Linkedlist{
    Node* head;
    Node* tail;
};
void init_linked_list(Linkedlist& a){
    a.head=nullptr;
    a.tail=nullptr;
}
void add_element2(Linkedlist& a, job val){
    Node* temp=new Node;
    temp->date=val;
    temp->next=nullptr;
    if(a.head==nullptr){
        a.head=temp;
        a.tail=temp;
    }else{
        a.tail->next=temp;
        a.tail=temp;
    }
}

void dequeue_linked_list(Linkedlist& a){
    if(a.head==nullptr)return;
    Node* temp=a.head;
    a.head=a.head->next;
    if(a.head==nullptr){
        a.tail=nullptr;
    }
    delete temp;
}

void round_robin(Linkedlist& a, ll q){
    ll t=0;
    while(a.head!=nullptr){
        job cur=a.head->date;
        dequeue_linked_list(a);
        if(cur.burst>q){
            t+=q;
            cur.burst-=q;
            add_element2(a, cur);
            cout<<t<<" "<<cur.id<<" "<<cur.burst<<"\n";
        }else{
            t+=cur.burst;
            cout<<t<<" "<<cur.id<<" 0\n";
        }
    }
}

int main(){
    Linkedlist a;
    init_linked_list(a);
    add_element2(a, {1, 10});
    add_element2(a, {2, 5});
    add_element2(a, {3, 8});
    round_robin(a, 3);
    return 0;
}