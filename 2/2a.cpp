#include <iostream>
#include <queue>
using namespace std;

typedef long double ld;
typedef long long ll;

struct Rectangle{
    ll leftlowerx;
    ll leftlowery;
    ll weight;
    ll hight;
};
ll startprogram(){
    cout<<"Select a number from the list for further action";
    cout<<"1:delete element ";
    cout<<"2:add element ";
    cout<<"3:display queue ";
    ll n;
    cin>>n;
    return n;
}

void print(queue<ll>& a){
    while(!a.empty()){
        cout<<a.back()<<" ";
    }
    cout<<"\n";
}

int main(){
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