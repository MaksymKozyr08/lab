#include <iostream>
#include <queue>
#include <vector>
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
    rectangle date[100];
    ll head;
    ll tail;
    ll count;
};
//--------------------------------------------------------------

//--------------------------------------------------------------
// create
void createstatilmasive(staticmasive& a){
    a.head=0;
    a.tail=0;
    a.count=0;
    for(ll i=0;i<100;++i){
        a.date[i].leftlowerx=0;
        a.date[i].leftlowery=0;
        a.date[i].weight=0;
        a.date[i].hight=0;
    }
}
//--------------------------------------------------------------


//--------------------------------------------------------------
//add
void add_element0(staticmasive& a, rectangle val){
    if(a.count==100){
        cout<<"Error"<<"\n";
        return;
    }
    a.date[a.tail]=val;
    a.tail=(a.tail+1)%100;;
    a.count++;
}
//--------------------------------------------------------------



//--------------------------------------------------------------
//delete
void delete_element0(staticmasive& a){
    if(a.count==0){
        cout<<"Error";
        return;
    }
    a.head=(a.head+1)%100;
    a.count--;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
//start program
ll startprogram(){
    cout<<"Select a number from the list for further action";
    cout<<"1:delete element ";
    cout<<"2:add element ";
    cout<<"3:display queue ";
    ll n;
    cin>>n;
    return n;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
//print
void print1(vector<ll>& a){
    for(ll i=0;i<a.size();++i)cout<<a[i]<<" ";
    cout<<"\n";
}

void print(queue<ll> a){
    while(!a.empty()){
        cout<<a.front()<<" ";
        a.pop();
    }
    cout<<"\n";
}
//--------------------------------------------------------------

int main(){
    staticmasive myStaticQ;
    createstatilmasive(myStaticQ);

    ll n;
    cin>>n;
    queue<ll> a;
    for(ll i=0;i<n;++i){
        a.push(rand()%10);
    }
    print(a);
    if(a.empty())cout<<true;
    else{
        cout<<false;
    }
    vector<ll> a1(n);
    return 0;
}