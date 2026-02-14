#include <iostream>
#include <queue>
#include <vector>
#include <chrono> 
using namespace std;

typedef long double ld;
typedef long long ll;

//--------------------------------------------------------------
// struct
struct rectangle{
    ll leftlowerx;
    ll leftlowery;
    ll weight;
    ll hight;
};

struct staticmasive{
    rectangle date[100000]; 
    ll head;
    ll tail;
    ll count;
};

struct Dynamiccmasive{
    vector<rectangle> date;
};

struct Node{
    rectangle date;
    Node* next;
};

struct Linkedlist{
    Node* head;
    Node* tail;
};
//--------------------------------------------------------------

//--------------------------------------------------------------
// create
void createstatilmasive(staticmasive& a){
    a.head=0;
    a.tail=0;
    a.count=0;
}

void createDynamicmasive(Dynamiccmasive& a){
    a.date.clear();
}

void createlinkedlist(Linkedlist& a){
    a.head=nullptr;
    a.tail=nullptr;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
//add
void add_element0(staticmasive& a, rectangle val){
    if(a.count==100000)return;
    a.date[a.tail]=val;
    a.tail=(a.tail+1)%100000;
    a.count++;
}

void add_element1(Dynamiccmasive& a, rectangle val){
    a.date.push_back(val);
}

void add_element2(Linkedlist& a, rectangle val){
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
void delete_element0(staticmasive& a){
    if(a.count==0)return;
    a.head=(a.head+1)%100000;
    a.count--;
}
void delete_element1(Dynamiccmasive& a){
    if(a.date.empty())return;
    a.date.erase(a.date.begin());
}

void delete_element2(Linkedlist& a){
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
// DEMO MODE
void demo(){
    cout<<"--- DEMO MODE ---\n";
    
    cout<<"1. Static Queue:\n";
    staticmasive s; 
    createstatilmasive(s);
    add_element0(s,{1,1,10,10});
    add_element0(s,{2,2,20,20});
    cout<<"Added 2 elements. Count: "<<s.count<<"\n";
    delete_element0(s);
    cout<<"Deleted 1. Count: "<<s.count<<"\n\n";

    cout<<"2. Dynamic Queue (Vector):\n";
    Dynamiccmasive d; 
    createDynamicmasive(d);
    add_element1(d,{3,3,30,30});
    add_element1(d,{4,4,40,40});
    cout<<"Added 2 elements. Size: "<<d.date.size()<<"\n";
    delete_element1(d);
    cout<<"Deleted 1. Size: "<<d.date.size()<<"\n\n";

    cout<<"3. Linked List:\n";
    Linkedlist l; 
    createlinkedlist(l);
    add_element2(l,{5,5,50,50});
    add_element2(l,{6,6,60,60});
    cout<<"Added 2 elements.\n";
    delete_element2(l);
    cout<<"Deleted 1. List works correctly.\n\n";
}

// BENCHMARK MODE
void benchmark(){
    cout<<"--- BENCHMARK MODE ---\n";
    ll N=20000; 
    cout<<"Operations: "<<N<<"\n";
    rectangle r={0,0,10,10};

    // 1. Static
    staticmasive s; 
    createstatilmasive(s);
    auto start=chrono::high_resolution_clock::now();
    for(ll i=0;i<N;++i)add_element0(s,r);
    for(ll i=0;i<N;++i)delete_element0(s);
    auto end=chrono::high_resolution_clock::now();
    chrono::duration<double> diff=end-start;
    cout<<"Static Array: "<<diff.count()<<" sec\n";

    // 2. Dynamic (Vector)
    Dynamiccmasive d; 
    createDynamicmasive(d);
    start=chrono::high_resolution_clock::now();
    for(ll i=0;i<N;++i)add_element1(d,r);
    for(ll i=0;i<N;++i)delete_element1(d);
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Dynamic Vector: "<<diff.count()<<" sec\n";

    // 3. Linked List
    Linkedlist l; 
    createlinkedlist(l);
    start=chrono::high_resolution_clock::now();
    for(ll i=0;i<N;++i)add_element2(l,r);
    for(ll i=0;i<N;++i)delete_element2(l);
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
            staticmasive q; 
            createstatilmasive(q);
            while(true){
                cout<<"Static: 1.Add 2.Del 0.Exit: ";
                ll op; cin>>op;
                if(op==0)break;
                if(op==1){
                    ll x,y,w,h;
                    cout<<"x y w h: "; cin>>x>>y>>w>>h;
                    add_element0(q,{x,y,w,h});
                }
                if(op==2){
                    delete_element0(q);
                    cout<<"Deleted\n";
                }
            }
        }
        else if(type==2){
            Dynamiccmasive q; 
            createDynamicmasive(q);
            while(true){
                cout<<"Dynamic: 1.Add 2.Del 0.Exit: ";
                ll op; cin>>op;
                if(op==0)break;
                if(op==1){
                    ll x,y,w,h;
                    cout<<"x y w h: "; cin>>x>>y>>w>>h;
                    add_element1(q,{x,y,w,h});
                }
                if(op==2){
                    delete_element1(q);
                    cout<<"Deleted\n";
                }
            }
        }
        else if(type==3){
            Linkedlist q; 
            createlinkedlist(q);
            while(true){
                cout<<"List: 1.Add 2.Del 0.Exit: ";
                ll op; cin>>op;
                if(op==0)break;
                if(op==1){
                    ll x,y,w,h;
                    cout<<"x y w h: "; cin>>x>>y>>w>>h;
                    add_element2(q,{x,y,w,h});
                }
                if(op==2){
                    delete_element2(q);
                    cout<<"Deleted\n";
                }
            }
        }
    }
    return 0;
}