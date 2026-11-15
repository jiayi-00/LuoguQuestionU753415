#include <bits/stdc++.h>
using namespace std;
using ll=long long; const ll INF=(ll)4e18;
ll ceilDiv(ll a,ll b){if(a>=0)return (a+b-1)/b;return -((-a)/b);}
ll solveUnbounded(ll D,ll t,ll k){D=llabs(D);if(D==0)return 0;if(k==0)return D%t==0?D/t:INF;vector<ll> cand{0};ll a=D/t,b=D/(t+k);for(ll base:{a,b})for(ll m=base-2;m<=base+2;m++)if(m>=0)cand.push_back(m);ll ans=INF;for(ll m:cand){ll q=0;q=max(q,ceilDiv(m*t-D,k));q=max(q,ceilDiv(D-m*(t+k),k));q=max(q,0LL);ans=min(ans,m+2*q);}return ans;}
int main(){
 for(int t=1;t<=20;t++)for(int k=0;k<=10;k++){
  int start=2*t+k;
  for(int L=start;L<=min(80,start+15);L++){
   vector<vector<int>> D(L+1,vector<int>(L+1,1e9));
   int U=min(t+k,L-1);
   for(int a=1;a<=L;a++){auto &d=D[a];d[a]=0;queue<int>q;q.push(a);while(!q.empty()){int x=q.front();q.pop();for(int dd=t;dd<=U;dd++)for(int y:{x-dd,x+dd})if(y>=1&&y<=L&&d[y]==1e9){d[y]=d[x]+1;q.push(y);}}}
   for(int a=1;a<=L;a++)for(int b=1;b<=L;b++){
    ll tr=D[a][b]; if(tr>=1e9)tr=INF; ll ub=solveUnbounded(b-a,t,k);
    if(tr!=ub){cout<<"BAD L="<<L<<" t="<<t<<" k="<<k<<" a="<<a<<" b="<<b<<" true="<<tr<<" ub="<<ub<<"\n";return 0;}
   }
  }
 }
 cout<<"OK\n";
}
