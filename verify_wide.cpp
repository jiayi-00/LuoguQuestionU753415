#include <bits/stdc++.h>
using namespace std;
int main(){
    const int INF=1e9;
    for(int t=1;t<=15;t++)for(int k=0;k<=10;k++){
        for(int L=2*t+k; L<=2*t+k+20; L++){
            vector<vector<int>> D(L+1, vector<int>(L+1, INF));
            for(int a=1;a<=L;a++){
                auto &d=D[a]; d[a]=0; queue<int>q; q.push(a);
                int U=min(t+k,L-1);
                while(!q.empty()){int x=q.front();q.pop();for(int dd=t;dd<=U;dd++)for(int y:{x-dd,x+dd})if(y>=1&&y<=L&&d[y]==INF){d[y]=d[x]+1;q.push(y);}}
            }
            for(int a=1;a<=L;a++)for(int b=1;b<=L;b++){
                int Dd=abs(b-a), ub=INF;
                if(Dd==0)ub=0;
                else for(int r=1;r<=200&&ub==INF;r++)for(int p=0;p<=r;p++){
                    int q=r-p;
                    if(p*t-q*(t+k)<=Dd && Dd<=p*(t+k)-q*t){ub=r;break;}
                }
                int tr=D[a][b];
                if(tr!=ub){cout<<"BAD L="<<L<<" t="<<t<<" k="<<k<<" a="<<a<<" b="<<b<<" true="<<tr<<" ub="<<ub<<"\n";return 0;}
            }
        }
    }
    cout<<"OK\n";
}
