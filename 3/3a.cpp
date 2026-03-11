#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <iomanip>
using namespace std;
using namespace std::chrono;

typedef long long ll;
typedef long double ld;

struct Point3D{
    ld x,y,z;
    ld d() const{return x*x+y*y+z*z;}
    bool operator<(const Point3D& o) const{return d()<o.d();}
    bool operator>(const Point3D& o) const{return d()>o.d();}
    bool operator<=(const Point3D& o) const{return d()<=o.d();}
};

ostream& operator<<(ostream& os, const Point3D& p){
    os<<"("<<fixed<<setprecision(1)<<p.x<<","<<p.y<<","<<p.z<<")";
    return os;
}

void print(vector<Point3D>& a){
    for(Point3D i:a)cout<<i<<" ";
    cout<<"\n";
}

void insertionsort(vector<Point3D>& a,ll left,ll right,bool demo=false){
    if(right-left<1)return;
    for(ll i=left+1;i<=right;++i){
        Point3D key=a[i];
        ll j=i;
        while(j>left && a[j-1]>key){
            a[j]=a[j-1];
            --j;
        }
        a[j]=key;
        if(demo){
            cout<<"IS: ";
            print(a);
        }
    }
}

ll median(vector<Point3D>& a,ll left,ll right){
    ll mid=left+(right-left)/2;
    if(a[mid]<a[left])swap(a[left],a[mid]);
    if(a[right]<a[left])swap(a[left],a[right]);
    if(a[right]<a[mid])swap(a[mid],a[right]);
    return mid;
}

ll partition(vector<Point3D>& a,ll left,ll right){
    ll p=median(a,left,right);
    Point3D pv=a[p];
    ll i=left-1;
    ll j=right+1;
    while(true){
        do{i++;}while(a[i]<pv);
        do{j--;}while(a[j]>pv);
        if(i>=j)return j;
        swap(a[i],a[j]);
    }
}

void quicksort(vector<Point3D>& a,ll left,ll right,bool demo=false){
    if(left<right){
        ll p=partition(a,left,right);
        if(demo){
            cout<<"QS: ";
            print(a);
        }
        quicksort(a,left,p,demo);
        quicksort(a,p+1,right,demo);
    }
}

void merge(vector<Point3D>& a,vector<Point3D>& t,ll left,ll mid,ll right,bool demo){
    ll i=left,j=mid+1,k=left;
    while(i<=mid && j<=right){
        if(a[i]<=a[j])t[k++]=a[i++];
        else t[k++]=a[j++];
    }
    while(i<=mid)t[k++]=a[i++];
    while(j<=right)t[k++]=a[j++];
    for(i=left;i<=right;++i)a[i]=t[i];
    if(demo){
        cout<<"MS: ";
        print(a);
    }
}

void mergesort(vector<Point3D>& a,vector<Point3D>& t,ll left,ll right,bool demo){
    if(left<right){
        ll mid=left+(right-left)/2;
        mergesort(a,t,left,mid,demo);
        mergesort(a,t,mid+1,right,demo);
        merge(a,t,left,mid,right,demo);
    }
}

void hybrid(vector<Point3D>& a,ll left,ll right,ll th){
    if(right-left+1<=th){
        insertionsort(a,left,right);
        return;
    }
    if(left<right){
        ll p=partition(a,left,right);
        hybrid(a,left,p,th);
        hybrid(a,p+1,right,th);
    }
}

vector<Point3D> gen(ll size,ll type){
    vector<Point3D> a(size);
    for(ll i=0;i<size;++i){
        a[i]={(ld)(rand()%200-100),(ld)(rand()%200-100),(ld)(rand()%200-100)};
    }
    if(type==1 || type==2){
        sort(a.begin(),a.end());
        if(type==2)reverse(a.begin(),a.end());
        ll sw=size*0.05;
        for(ll i=0;i<sw;++i)swap(a[rand()%size],a[rand()%size]);
    }
    return a;
}

void demo(){
    ll n=10;
    vector<Point3D> o=gen(n,0);
    print(o);
    vector<Point3D> c=o;
    insertionsort(c,0,n-1,true);
    c=o;
    quicksort(c,0,n-1,true);
    c=o;
    vector<Point3D> t(n);
    mergesort(c,t,0,n-1,true);
}

void bench(){
    ll sizes[]={10000,50000};
    for(ll s:sizes){
        for(ll t=0;t<3;++t){
            vector<Point3D> o=gen(s,t);
            vector<Point3D> c=o;
            cout<<s<<" t:"<<t<<"\n";
            if(s<=10000){
                auto st=high_resolution_clock::now();
                insertionsort(c,0,s-1);
                cout<<"IS: "<<duration_cast<milliseconds>(high_resolution_clock::now()-st).count()<<"\n";
            }
            c=o;
            auto stq=high_resolution_clock::now();
            quicksort(c,0,s-1);
            cout<<"QS: "<<duration_cast<milliseconds>(high_resolution_clock::now()-stq).count()<<"\n";
            
            c=o;
            vector<Point3D> tm(s);
            auto stm=high_resolution_clock::now();
            mergesort(c,tm,0,s-1,false);
            cout<<"MS: "<<duration_cast<milliseconds>(high_resolution_clock::now()-stm).count()<<"\n";
            
            c=o;
            auto sts=high_resolution_clock::now();
            sort(c.begin(),c.end());
            cout<<"STD: "<<duration_cast<milliseconds>(high_resolution_clock::now()-sts).count()<<"\n";
        }
    }
    ll ths[]={5,10,15,20,30,50};
    vector<Point3D> ho=gen(100000,0);
    cout<<"hybrid th:\n";
    for(ll th:ths){
        vector<Point3D> c=ho;
        auto st=high_resolution_clock::now();
        hybrid(c,0,99999,th);
        cout<<th<<": "<<duration_cast<milliseconds>(high_resolution_clock::now()-st).count()<<"\n";
    }
}

int main(){
    srand(time(0));
    ll ch;
    cin>>ch;
    if(ch==1)demo();
    else if(ch==2)bench();
    return 0;
}