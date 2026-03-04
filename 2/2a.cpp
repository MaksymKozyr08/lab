#include <iostream>
#include <queue>
#include <vector>
#include <chrono> 
using namespace std;

typedef long double ld;
typedef long long ll;

//--------------------------------------------------------------
// struct
struct Rectangle{
    ll leftlowerx;
    ll leftlowery;
    ll width;
    ll height;
};

struct StaticQueue{
    Rectangle data[20000]; 
    ll head;
    ll tail;
    ll count;
};

struct Dynamiccmasive{
    vector<Rectangle> date;
};

struct Node{
    Rectangle date;
    Node* next;
};

struct Linkedlist{
    Node* head;
    Node* tail;
};
//--------------------------------------------------------------

//--------------------------------------------------------------
// create
void init_static_queue(StaticQueue& a){
    a.head=0;
    a.tail=0;
    a.count=0;
}

void init_dynamic_queue(Dynamiccmasive& a){
    a.date.clear();
}

void init_linked_list(Linkedlist& a){
    a.head=nullptr;
    a.tail=nullptr;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
//add
void enqueue_static(StaticQueue& a, Rectangle val){
    if(a.count==20000)return;
    a.data[a.tail]=val;
    a.tail=(a.tail+1)%20000;
    a.count++;
}

void enqueue_dynamic(Dynamiccmasive& a, Rectangle val){
    a.date.push_back(val);
}

void enqueue_linked_list(Linkedlist& a, Rectangle val){
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
//--------------------------------------------------------------

//--------------------------------------------------------------    
//delete
void dequeue_static(StaticQueue& a){
    if(a.count==0)return;
    a.head=(a.head+1)%20000;
    a.count--;
}
void dequeue_dynamic(Dynamiccmasive& a){
    if(a.date.empty())return;
    a.date.erase(a.date.begin());
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
//--------------------------------------------------------------

//--------------------------------------------------------------
//print
void print_static(StaticQueue& a){
    if(a.count==0){ cout<<"Empty\n"; return; }
    ll curr=a.head;
    for(ll i=0; i<a.count; i++){
        cout<<"["<<a.data[curr].leftlowerx<<","<<a.data[curr].leftlowery<<","<<a.data[curr].width<<","<<a.data[curr].height<<"] ";
        curr=(curr+1)%20000;
    }
    cout<<"\n";
}

void print_dynamic(Dynamiccmasive& a){
    if(a.date.empty()){ cout<<"Empty\n"; return; }
    for(ll i=0; i<a.date.size(); i++){
        cout<<"["<<a.date[i].leftlowerx<<","<<a.date[i].leftlowery<<","<<a.date[i].width<<","<<a.date[i].height<<"] ";
    }
    cout<<"\n";
}

void print_linked_list(Linkedlist& a){
    if(a.head==nullptr){ cout<<"Empty\n"; return; }
    Node* temp=a.head;
    while(temp!=nullptr){
        cout<<"["<<temp->date.leftlowerx<<","<<temp->date.leftlowery<<","<<temp->date.width<<","<<temp->date.height<<"] ";
        temp=temp->next;
    }
    cout<<"\n";
}
//--------------------------------------------------------------

// DEMO MODE
void demo(){
    cout<<"--- DEMO MODE ---\n";
    
    cout<<"1. Static Queue:\n";
    StaticQueue s; 
    init_static_queue(s);
    enqueue_static(s,{1,1,10,10});
    enqueue_static(s,{2,2,20,20});
    cout<<"Added 2 elements. Count: "<<s.count<<"\n";
    dequeue_static(s);
    cout<<"Deleted 1. Count: "<<s.count<<"\n\n";

    cout<<"2. Dynamic Queue (Vector):\n";
    Dynamiccmasive d; 
    init_dynamic_queue(d);
    enqueue_dynamic(d,{3,3,30,30});
    enqueue_dynamic(d,{4,4,40,40});
    cout<<"Added 2 elements. Size: "<<d.date.size()<<"\n";
    dequeue_dynamic(d);
    cout<<"Deleted 1. Size: "<<d.date.size()<<"\n\n";
    cout<<"3. Linked List:\n";
    Linkedlist l; 
    init_linked_list(l);
    enqueue_linked_list(l,{5,5,50,50});
    enqueue_linked_list(l,{6,6,60,60});
    cout<<"Added 2 elements.\n";
    dequeue_linked_list(l);
    cout<<"Deleted 1. List works correctly.\n\n";
}

// BENCHMARK MODE
void benchmark(){
    cout<<"--- BENCHMARK MODE ---\n";
    ll N=20000; 
    cout<<"Operations: "<<N<<"\n";
    Rectangle r={0,0,10,10};

    // 1. Static
    StaticQueue s; 
    init_static_queue(s);
    auto start=chrono::high_resolution_clock::now();
    for(ll i=0;i<N;++i)enqueue_static(s,r);
    for(ll i=0;i<N;++i)dequeue_static(s);
    auto end=chrono::high_resolution_clock::now();
    chrono::duration<double> diff=end-start;
    cout<<"Static Array: "<<diff.count()<<" sec\n";

    // 2. Dynamic (Vector)
    Dynamiccmasive d; 
    init_dynamic_queue(d);
    start=chrono::high_resolution_clock::now();
    for(ll i=0;i<N;++i)enqueue_dynamic(d,r);
    for(ll i=0;i<N;++i)dequeue_dynamic(d);
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Dynamic Vector: "<<diff.count()<<" sec\n";

    // 3. Linked List
    Linkedlist l; 
    init_linked_list(l);
    start=chrono::high_resolution_clock::now();
    for(ll i=0;i<N;++i)enqueue_linked_list(l,r);
    for(ll i=0;i<N;++i)dequeue_linked_list(l);
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Linked List: "<<diff.count()<<" sec\n";
}
//--------------------------------------------------------------

int main(){
    cout<<"Select Mode:\n1.Interactive\n2.Demo\n3.Benchmark\n";
    ll mode;
    cin>>mode;

    if(mode==2){
        demo();
    }
    else if(mode==3){
        benchmark();
    }
    else if(mode==1){
        cout<<"Select Queue Type:\n1.Static\n2.Dynamic\n3.LinkedList\n";
        ll type;
        cin>>type;
        
        if(type==1){
            StaticQueue q; 
            init_static_queue(q);
            while(true){
                cout<<"Static: 1.Add 2.Del 3.Print 0.Exit: ";
                ll op; cin>>op;
                if(op==0)break;
                if(op==1){
                    ll x,y,w,h;
                    cout<<"x y w h: "; cin>>x>>y>>w>>h;
                    enqueue_static(q,{x,y,w,h});
                }
                if(op==2){
                    dequeue_static(q);
                    cout<<"Deleted\n";
                }
                if(op==3){
                    print_static(q);
                }
            }
        }
        else if(type==2){
            Dynamiccmasive q; 
            init_dynamic_queue(q);
            while(true){
                cout<<"Dynamic: 1.Add 2.Del 3.Print 0.Exit: ";
                ll op; cin>>op;
                if(op==0)break;
                if(op==1){
                    ll x,y,w,h;
                    cout<<"x y w h: "; cin>>x>>y>>w>>h;
                    enqueue_dynamic(q,{x,y,w,h});
                }
                if(op==2){
                    dequeue_dynamic(q);
                    cout<<"Deleted\n";
                }
                if(op==3){
                    print_dynamic(q);
                }
            }
        }
        else if(type==3){
            Linkedlist q; 
            init_linked_list(q);
            while(true){
                cout<<"List: 1.Add 2.Del 3.Print 0.Exit: ";
                ll op; cin>>op;
                if(op==0)break;
                if(op==1){
                    ll x,y,w,h;
                    cout<<"x y w h: "; cin>>x>>y>>w>>h;
                    enqueue_linked_list(q, {x, y, w, h});
                }
                if(op==2){
                    dequeue_linked_list(q);
                    cout<<"Deleted\n";
                }
                if(op==3){
                    print_linked_list(q);
                }
            }
        }
    }
    return 0;
}