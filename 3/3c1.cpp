#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <chrono>
#include <future>
#include <random>

using namespace std;
using namespace std::chrono;

typedef long long ll;

struct test{
    ll id;
    ll a;
    ll hp;
};

bool cmp(const test& x, const test& y){
    return x.hp < y.hp;
}

void p_arr(const vector<test>& arr){
    for(const auto& x : arr) cout << "(" << x.id << " " << x.a << " " << x.hp << ") ";
    cout << "\n";
}

bool is_demo = false;

void merge_v(vector<test>& arr, ll l, ll m, ll r){
    ll n1 = m - l + 1;
    ll n2 = r - m;
    vector<test> L(n1), R(n2);
    for(ll i = 0; i < n1; ++i) L[i] = arr[l + i];
    for(ll i = 0; i < n2; ++i) R[i] = arr[m + 1 + i];
    ll i = 0, j = 0, k = l;
    while(i < n1 && j < n2){
        if(L[i].hp <= R[j].hp) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while(i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];
    
    if(is_demo) p_arr(arr);
}

void seq_m_sort(vector<test>& arr, ll l, ll r){
    if(l < r){
        ll m = l + (r - l) / 2;
        seq_m_sort(arr, l, m);
        seq_m_sort(arr, m + 1, r);
        merge_v(arr, l, m, r);
    }
}

void par_m_sort(vector<test>& arr, ll l, ll r, int depth = 0){
    if(l < r){
        ll m = l + (r - l) / 2;
        if(depth < 4){
            auto f = async(launch::async, par_m_sort, ref(arr), l, m, depth + 1);
            par_m_sort(arr, m + 1, r, depth + 1);
            f.get();
        } else {
            seq_m_sort(arr, l, m);
            seq_m_sort(arr, m + 1, r);
        }
        merge_v(arr, l, m, r);
    }
}

int main(){
    int mode;
    cin >> mode;
    
    ll n;
    if(mode == 1){
        cin >> n;
        is_demo = true;
    } else {
        n = 2000000;
        is_demo = false;
    }

    vector<test> arr(n);
    for(ll i = 0; i < n; ++i){
        arr[i].id = i;
        arr[i].a = rand() % 21;
        arr[i].hp = rand() % 1000;
    }

    if(is_demo){
        vector<test> t1 = arr;
        p_arr(t1);
        seq_m_sort(t1, 0, n - 1);
        
        vector<test> t2 = arr;
        p_arr(t2);
        par_m_sort(t2, 0, n - 1);
    } else {
        vector<test> t1 = arr;
        auto start = high_resolution_clock::now();
        seq_m_sort(t1, 0, n - 1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time seq merge: " << duration.count() << " mcs\n";

        vector<test> t2 = arr;
        start = high_resolution_clock::now();
        par_m_sort(t2, 0, n - 1);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time par merge: " << duration.count() << " mcs\n";

        vector<test> t3 = arr;
        start = high_resolution_clock::now();
        stable_sort(t3.begin(), t3.end(), cmp);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time std sort: " << duration.count() << " mcs\n";
    }

    return 0;
}