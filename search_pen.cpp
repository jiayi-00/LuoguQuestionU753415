#include <bits/stdc++.h>
using namespace std; using ll=long long; const int INF=1e9;
ll ceildiv(ll a,ll b){return (a+b-1)/b;}
ll unb(ll D,ll t,ll k){D=llabs(D);if(!D)return 0;if(k==0)return D%t?INF:D/t;set<ll> cand={0,D/t,D/t+1,ceildiv(D,t+k)-1,ceildiv(D,t+k),ceildiv(D,t+k)+1};ll ans=INF;for(ll m:cand){if(m<0)continue;ll q=max({0LL,ceildiv(m*t-D,k),ceildiv(D-m*(t+k),k)});ans=min(ans,m+2*q);}return ans;}
int main(){mt19937 rng(123);int mx=0;tuple<int,int,int,int,int,int,int> arg;int trials=0;
 for(int it=0;it<20000;it++){
  int t=rng()%150+2,k=rng()%150+2;
  int L=2*t+1 + rng()%(k-1<1?1:k-1);
  if(!(2*t<L && L<2*t+k))continue;
  int a=rng()%L+1,b=rng()%L+1;trials++;
  vector<int> d(L+1,INF);d[a]=0;queue<int>q;q.push(a);int U=min(t+k,L-1);
  while(!q.empty()){int x=q.front();q.pop();for(int dd=t;dd<=U;dd++)for(int y:{x-dd,x+dd})if(y>=1&&y<=L&&d[y]==INF){d[y]=d[x]+1;q.push(y);}}
  int tr=d[b];if(tr>=INF)continue;int ub=unb(b-a,t,k);int pen=tr-ub;
  if(pen>mx){mx=pen;arg={L,t,k,a,b,tr,ub};cout<<"new max "<<mx<<" L="<<L<<" t="<<t<<" k="<<k<<" a="<<a<<" b="<<b<<" tr="<<tr<<" ub="<<ub<<"\n";}
 }
 cerr<<"trials "<<trials<<" max "<<mx<<"\n";
}
