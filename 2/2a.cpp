#include <iostream>
#include <queue>
using namespace std;

typedef long double ld;
typedef long long ll;
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
    
    return 0;
}