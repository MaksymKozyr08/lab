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

void flash_sort(vector<test>& a){
    ll n = a.size();
    if(n <= 1) return;
    ll m = 0.45 * n;
    vector<ll> L(m, 0);
    ll min_val = a[0].hp;
    ll max_idx = 0;
    
    for(ll i = 1; i < n; ++i){
        if(a[i].hp < min_val) min_val = a[i].hp;
        if(a[i].hp > a[max_idx].hp) max_idx = i;
    }
    
    if(min_val == a[max_idx].hp) return;
    
    double c1 = (double)(m - 1) / (a[max_idx].hp - min_val);
    
    for(ll i = 0; i < n; ++i){
        ll k = c1 * (a[i].hp - min_val);
        L[k]++;
    }
    
    for(ll k = 1; k < m; ++k){
        L[k] += L[k - 1];
    }
    
    test hold = a[max_idx];
    a[max_idx] = a[0];
    a[0] = hold;
    
    ll nmove = 0;
    ll j = 0;
    ll k = m - 1;
    
    while(nmove < n - 1){
        while(j > L[k] - 1){
            j++;
            k = c1 * (a[j].hp - min_val);
        }
        test flash = a[j];
        while(j != L[k]){
            k = c1 * (flash.hp - min_val);
            ll hold_pos = --L[k];
            test temp = a[hold_pos];
            a[hold_pos] = flash;
            flash = temp;
            nmove++;
            if(is_demo) p_arr(a);
        }
    }
    
    for(ll i = 1; i < n; ++i){
        test temp = a[i];
        ll idx = i - 1;
        while(idx >= 0 && a[idx].hp > temp.hp){
            a[idx + 1] = a[idx];
            idx--;
        }
        a[idx + 1] = temp;
        if(is_demo) p_arr(a);
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
        flash_sort(t1);
    } else {
        vector<test> t1 = arr;
        auto start = high_resolution_clock::now();
        flash_sort(t1);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time flashsort: " << duration.count() << " mcs\n";

        vector<test> t2 = arr;
        start = high_resolution_clock::now();
        stable_sort(t2.begin(), t2.end(), cmp);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        cout << "Time std sort: " << duration.count() << " mcs\n";
    }

    return 0;
}