#include <iostream>
#include <vector>
using namespace std;

typedef long long ll;

struct job{
    ll id;
    ll arrival;
    ll duration;
};

struct Dynamiccmasive{
    vector<job> date;
};

void createDynamicmasive(Dynamiccmasive& a){
    a.date.clear();
}

void add_element1(Dynamiccmasive& a, job val){
    a.date.push_back(val);
}

void delete_element_by_index(Dynamiccmasive& a, ll index){
    if(a.date.empty() || index >= a.date.size()) return;
    a.date.erase(a.date.begin() + index);
}

void sort_by_arrival(Dynamiccmasive& a){
    for(ll i=0; i<a.date.size(); i++){
        for(ll j=i+1; j<a.date.size(); j++){
            if(a.date[i].arrival > a.date[j].arrival){
                job temp = a.date[i];
                a.date[i] = a.date[j];
                a.date[j] = temp;
            }
        }
    }
}

void FCFS(Dynamiccmasive a){
    if(a.date.empty()){
        cout<<"Empty\n";
        return;
    }
    sort_by_arrival(a);
    ll current_time = 0;
    
    for(ll i=0; i<a.date.size(); i++){
        if(current_time < a.date[i].arrival){
            current_time = a.date[i].arrival;
        }
        cout<<"[FCFS] Job "<<a.date[i].id<<" | Start: "<<current_time;
        current_time += a.date[i].duration;
        cout<<" | End: "<<current_time<<"\n";
    }
}

void SJF(Dynamiccmasive a){
    if(a.date.empty()){
        cout<<"Empty\n";
        return;
    }
    ll current_time = 0;
    ll n = a.date.size();
    ll completed = 0;
    
    while(completed < n){
        ll shortest_idx = -1;
        ll min_duration = 1e18;
        
        for(ll i=0; i<a.date.size(); i++){
            if(a.date[i].arrival <= current_time && a.date[i].duration < min_duration){
                min_duration = a.date[i].duration;
                shortest_idx = i;
            }
        }
        
        if(shortest_idx == -1){
            ll next_arrival = 1e18;
            for(ll i=0; i<a.date.size(); i++){
                if(a.date[i].arrival < next_arrival){
                    next_arrival = a.date[i].arrival;
                }
            }
            current_time = next_arrival;
        } else {
            cout<<"[SJF] Job "<<a.date[shortest_idx].id<<" | Start: "<<current_time;
            current_time += a.date[shortest_idx].duration;
            cout<<" | End: "<<current_time<<"\n";
            
            delete_element_by_index(a, shortest_idx);
            completed++;
        }
    }
}

int main(){
    Dynamiccmasive q; 
    createDynamicmasive(q);
    ll id_counter = 1;
    while(true){
        cout<<"Menu: 1.Add Job  2.Run FCFS  3.Run SJF  0.Exit: ";
        ll op; 
        cin>>op;
        if(op==0) break;
        if(op==1){
            ll arr, dur;
            cout<<"Arrival Time: "; 
            cin>>arr;
            cout<<"Duration (Burst Time): "; 
            cin>>dur;
            add_element1(q, {id_counter, arr, dur});
            id_counter++;
            cout<<"Job Added!\n";
        }
        if(op==2){
            FCFS(q);
        }
        if(op==3){
            SJF(q);
        }
    }
    return 0;
}