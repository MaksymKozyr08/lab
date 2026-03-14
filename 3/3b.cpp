#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

typedef long long ll;
typedef long double ld;

struct test{
    ll id;
    ll a;
    ll hp;
};

istream& operator>>(istream& is, test& number) {
    is >> number.id >> number.a >> number.hp; 
    return is;
}

ostream& operator<<(ostream& os, const test& number) {
    os << "(" << number.id << " " << number.a << " " << number.hp << ")";
    return os;
}

void c_sort(vector<test>& arr){
    if(arr.empty()) return;
    ll max_v=arr[0].a;
    for(size_t i=0;i<arr.size();++i){
        if(arr[i].a>max_v) max_v=arr[i].a;
    }
    vector<ll> count(max_v+1,0);
    for(size_t i=0;i<arr.size();++i){
        count[arr[i].a]++;
    }
    for(size_t i=1;i<=max_v;++i){
        count[i]+=count[i-1];
    }
    vector<test> out(arr.size());
    for(ll i=arr.size()-1;i>=0;--i){
        out[count[arr[i].a]-1]=arr[i];
        count[arr[i].a]--;
    }
    arr=out;
}

void r_sort_h(vector<test>& arr, ll exp){
    vector<test> out(arr.size());
    ll count[10]={0};
    for(size_t i=0;i<arr.size();++i){
        count[(arr[i].hp/exp)%10]++;
    }
    for(int i=1;i<10;++i){
        count[i]+=count[i-1];
    }
    for(ll i=arr.size()-1;i>=0;--i){
        out[count[(arr[i].hp/exp)%10]-1]=arr[i];
        count[(arr[i].hp/exp)%10]--;
    }
    arr=out;
}

void r_sort(vector<test>& arr){
    if(arr.empty()) return;
    ll max_h=arr[0].hp;
    for(size_t i=0;i<arr.size();++i){
        if(arr[i].hp>max_h) max_h=arr[i].hp;
    }
    for(ll exp=1;max_h/exp>0;exp*=10){
        r_sort_h(arr,exp);
    }
}

bool cmp1(const test& x, const test& y){
    return x.hp<y.hp;
}

bool cmp2(const test& x, const test& y){
    return x.a<y.a;
}

bool cmp3(const test& x, const test& y){
    if(x.a!=y.a) return x.a<y.a;
    return x.hp<y.hp;
}

int main(){
    ll n=2000000;
    vector<test> arr(n);
    for(ll i=0;i<n;++i){
        arr[i].id=i;
        arr[i].a=rand()%21;
        arr[i].hp=rand()%1000000;
    }
    
    vector<test> t1=arr;
    auto start=high_resolution_clock::now();
    r_sort(t1);
    auto stop=high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Time radix hp: " << duration.count() << " mcs" << endl;

    vector<test> t2=arr;
    start=high_resolution_clock::now();
    stable_sort(t2.begin(),t2.end(),cmp1);
    stop=high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "Time std hp: " << duration.count() << " mcs" << endl;

    vector<test> t3=arr;
    start=high_resolution_clock::now();
    c_sort(t3);
    stop=high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "Time count attack: " << duration.count() << " mcs" << endl;

    vector<test> t4=arr;
    start=high_resolution_clock::now();
    stable_sort(t4.begin(),t4.end(),cmp2);
    stop=high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "Time std attack: " << duration.count() << " mcs" << endl;

    vector<test> t5=arr;
    start=high_resolution_clock::now();
    r_sort(t5);
    c_sort(t5);
    stop=high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "Time comb my: " << duration.count() << " mcs" << endl;

    vector<test> t6=arr;
    start=high_resolution_clock::now();
    stable_sort(t6.begin(),t6.end(),cmp3);
    stop=high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);
    cout << "Time comb std: " << duration.count() << " mcs" << endl;

    return 0;
}