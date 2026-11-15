#include <bits/stdc++.h>
using namespace std;
using ll=long long;
const ll INF=(ll)4e18;

ll ceilDiv(ll a,ll b){ if(a>=0) return (a+b-1)/b; return -((-a)/b); }

ll solveUnbounded(ll D,ll t,ll k){
    D=llabs(D); if(D==0)return 0;
    if(k==0)return (D%t==0)?D/t:INF;
    vector<ll> cand{0};
    ll a=D/t,b=D/(t+k);
    for(ll base:{a,b}) for(ll m=base-2;m<=base+2;m++) if(m>=0)cand.push_back(m);
    ll ans=INF;
    for(ll m:cand){
        ll q=0;
        q=max(q,ceilDiv(m*t-D,k));
        q=max(q,ceilDiv(D-m*(t+k),k));
        q=max(q,0LL);
        ans=min(ans,m+2*q);
    }
    return ans;
}

pair<ll,ll> pRange(ll r,ll D,ll t,ll k){
    ll A=2*t+k;
    ll lo=max(0LL,ceilDiv(r*t+D,A));
    ll hi=min(r,(r*(t+k)+D)/A);
    return {lo,hi};
}

int main(int argc,char**argv){
    int trials=200000; unsigned seed=12345;
    if(argc>1) trials=atoi(argv[1]);
    if(argc>2) seed=strtoul(argv[2],nullptr,10);
    mt19937 rng(seed);

    int maxDiff=0; tuple<int,int,int,int,int,int,int> best;
    vector<string> diff1ex,diff2ex;
    int cntDiff[5]={0};
    int UinfeasU1=0,U1infeasU2=0;
    string exU1,exU2;
    int pWidth[5]={0}; // max widths at U+0..U+4
    string pWidthEx[5];

    auto now=chrono::steady_clock::now();
    for(int it=0; it<trials; ++it){
        int t=1+rng()%100;
        int k=2+rng()%99; // need k>=2
        int lo=2*t+1, hi=2*t+k-1;
        if(lo>hi){--it; continue;}
        hi=min(hi,500);
        if(lo>hi){--it; continue;}
        int L=lo+rng()%(hi-lo+1);
        int a=1+rng()%L;
        int Umax=min(t+k,L-1);
        vector<int> d(L+1,-1), par(L+1,-1), pd(L+1,0);
        set<int> unvis; for(int i=1;i<=L;i++) unvis.insert(i);
        queue<int> q; d[a]=0; unvis.erase(a); q.push(a);
        while(!q.empty()){
            int x=q.front(); q.pop();
            vector<pair<int,int>> segs={{x-Umax,x-t},{x+t,x+Umax}};
            for(auto [l,r]:segs){
                l=max(l,1); r=min(r,L); if(l>r)continue;
                auto it=unvis.lower_bound(l);
                while(it!=unvis.end() && *it<=r){
                    int y=*it;
                    d[y]=d[x]+1; par[y]=x; pd[y]=y-x;
                    it=unvis.erase(it); q.push(y);
                }
            }
        }
        for(int b=1;b<=L;b++){
            if(b==a)continue;
            ll D=abs(b-a);
            ll U=solveUnbounded(D,t,k);
            // verify U with p range
            ll ur=U;
            while(ur<=2000){auto [lo2,hi2]=pRange(ur,D,t,k); if(lo2<=hi2)break; ur++;}
            if(ur!=U){cerr<<"U mismatch\n";return 1;}
            int f=d[b];
            if(f<0) continue;
            int diff=f-(int)U;
            if(diff>=0 && diff<5)cntDiff[diff]++;
            if(diff>maxDiff){maxDiff=diff; best={L,a,b,t,k,f,(int)U};}
            if(diff>=3){
                cout<<"COUNTEREXAMPLE diff="<<diff<<" L="<<L<<" a="<<a<<" b="<<b<<" t="<<t<<" k="<<k<<" f="<<f<<" U="<<U<<"\n";
                vector<int> path; int x=b; while(x!=-1){path.push_back(x);x=par[x];} reverse(path.begin(),path.end());
                cout<<"path:"; for(int v:path)cout<<" "<<v; cout<<"\n";
                return 0;
            }
            if(diff==1 && diff1ex.size()<3){
                vector<int> path; int x=b; while(x!=-1){path.push_back(x);x=par[x];} reverse(path.begin(),path.end());
                string s="L="+to_string(L)+" a="+to_string(a)+" b="+to_string(b)+" t="+to_string(t)+" k="+to_string(k)+" f="+to_string(f)+" U="+to_string(U)+" path:";
                for(int v:path)s+=" "+to_string(v);
                diff1ex.push_back(s);
            }
            if(diff==2 && diff2ex.size()<3){
                vector<int> path; int x=b; while(x!=-1){path.push_back(x);x=par[x];} reverse(path.begin(),path.end());
                string s="L="+to_string(L)+" a="+to_string(a)+" b="+to_string(b)+" t="+to_string(t)+" k="+to_string(k)+" f="+to_string(f)+" U="+to_string(U)+" path:";
                for(int v:path)s+=" "+to_string(v);
                diff2ex.push_back(s);
            }
            // p widths
            for(int off=0;off<=2;off++){
                auto [lo2,hi2]=pRange(U+off,D,t,k);
                if(lo2<=hi2){
                    int w=hi2-lo2+1;
                    if(w>pWidth[off]){pWidth[off]=w;pWidthEx[off]="L="+to_string(L)+" a="+to_string(a)+" b="+to_string(b)+" t="+to_string(t)+" k="+to_string(k)+" r="+to_string(U+off)+" p=["+to_string(lo2)+","+to_string(hi2)+"]";}
                }
            }
            // U infeasible but U+1 feasible
            auto pr=pRange(U,D,t,k); bool uf=pr.first<=pr.second;
            auto pr1=pRange(U+1,D,t,k); bool u1f=pr1.first<=pr1.second;
            if(!uf && u1f && !UinfeasU1){UinfeasU1=1;exU1="L="+to_string(L)+" a="+to_string(a)+" b="+to_string(b)+" t="+to_string(t)+" k="+to_string(k)+" f="+to_string(f)+" U="+to_string(U);}
            if(!u1f && pRange(U+2,D,t,k).first<=pRange(U+2,D,t,k).second && !U1infeasU2){U1infeasU2=1;exU2="L="+to_string(L)+" a="+to_string(a)+" b="+to_string(b)+" t="+to_string(t)+" k="+to_string(k)+" f="+to_string(f)+" U="+to_string(U);}
        }
    }
    auto end=chrono::steady_clock::now();
    cerr<<"elapsed "<<chrono::duration<double>(end-now).count()<<"s\n";
    cout<<"trials="<<trials<<" seed="<<seed<<" maxDiff="<<maxDiff<<"\n";
    cout<<"best L,a,b,t,k,f,U = "<<get<0>(best)<<" "<<get<1>(best)<<" "<<get<2>(best)<<" "<<get<3>(best)<<" "<<get<4>(best)<<" "<<get<5>(best)<<" "<<get<6>(best)<<"\n";
    cout<<"cntDiff0..4:"; for(int i=0;i<5;i++)cout<<" "<<cntDiff[i]; cout<<"\n";
    for(auto&s:diff1ex)cout<<"DIFF1 "<<s<<"\n";
    for(auto&s:diff2ex)cout<<"DIFF2 "<<s<<"\n";
    cout<<"p widths at U+0..2:"; for(int i=0;i<3;i++)cout<<" "<<pWidth[i]; cout<<"\n";
    for(int i=0;i<3;i++)if(pWidth[i])cout<<"PWidth U+"<<i<<": "<<pWidthEx[i]<<"\n";
    cout<<"U infeasible but U+1 feasible: "<<exU1<<"\n";
    cout<<"U+1 infeasible but U+2 feasible: "<<exU2<<"\n";
    return 0;
}
