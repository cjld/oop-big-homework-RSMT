#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

namespace pre {

typedef pair<int,pair<int,int> > pair3;
#define mkPair3(x,y,z) make_pair(x,make_pair(y,z))
#define FOR(i,l,r) for (int i=(l);i<=(r);i++)
#define FORE(i,j,k) for (int k=d[i],j=p[k];k;k=next[k],j=p[k])
#define SZ(x) ((int)(x.size()))

pair<int,int> revPair(pair<int,int> a) {return make_pair(a.second,a.first);}

struct Statu {
  pair<int,int> a[4];

  Statu(int x1,int y1,int x2,int y2,int d,int t) {
    a[0]=make_pair(x1,y1);
    a[3]=make_pair(x2,y2);
    if (t)
      a[1]=make_pair(d,y1),a[2]=make_pair(d,y2);
    else
      a[1]=make_pair(x1,d),a[2]=make_pair(x2,d);
  }

  void print(vector<int> *lineX,vector<int> *lineY) {
    FOR(i,0,2) {
      //cout<<a[i].first<<' '<<a[i].second<<' '<<a[i+1].first<<' '<<a[i+1].second<<endl;
      lineX->push_back(a[i].first);
      lineX->push_back(a[i+1].first);
      lineY->push_back(a[i].second);
      lineY->push_back(a[i+1].second);
    }
  }

  void add(vector<pair<pair<int,int>,int> > &x,vector<pair<pair<int,int>,int> > &y) {
    FOR(i,0,2) {
      if (a[i].first==a[i+1].first)
        if (a[i].second<a[i+1].second)
          x.push_back(make_pair(a[i],a[i+1].second-a[i].second));
        else
          x.push_back(make_pair(a[i+1],a[i].second-a[i+1].second));
      else
        if (a[i].first<a[i+1].first)
          y.push_back(make_pair(revPair(a[i]),a[i+1].first-a[i].first));
        else
          y.push_back(make_pair(revPair(a[i+1]),a[i].first-a[i+1].first));
    }
  }

  int length() {return abs(a[0].first-a[3].first)+abs(a[0].second-a[3].second);}
};

struct FStatu {
  vector<Statu> s;
  vector<int> f;
};

struct Tree {
  int *d=0,*next=0,*p=0,*du=0,*x=0,*y=0;
  int t=0,n,root,ans;
  FStatu *f;
  vector<int> findSolve,solve,*lineX,*lineY;
  int lFlag;

  ~Tree() {
#define DELARR(x) if (x!=0) delete[] x;
    DELARR(d);
    DELARR(next);
    DELARR(p);
    DELARR(du);
    DELARR(f);
  }

  void link(int a,int b) {
    next[++t]=d[a],d[a]=t,p[t]=b;
    next[++t]=d[b],d[b]=t,p[t]=a;
    du[a]++,du[b]++;
  }

  void init(int nn,int *xx,int *yy) {
    n=nn;
    d=new int[n*3];
    FOR(i,1,n) d[i]=0;
    next=new int[n*3];
    p=new int[n*3];
    du=new int[n*3];
    FOR(i,1,n) du[i]=0;
    x=xx,y=yy;
    f=new FStatu[n+1];
  }

  bool isNb(int x, int y) {
      if (x==y) return 1;
      FORE(x,j,k) if (y==j) return 1;
      return 0;
  }

  void initStatu(FStatu &f,int x1,int y1,int x2,int y2,int ii,int jj) {
    int l=x1,r=x2;
    if (l>r) swap(l,r);
    set<int> sx,sy;
    FOR(i,1,n) if (x[i]>=l && x[i]<=r && (isNb(i,ii) || isNb(i,jj))) {
      if (lFlag && l!=x[i] && r!=x[i]) continue;
      if (sx.count(x[i])) continue;
      sx.insert(x[i]);
      f.s.push_back(Statu(x1,y1,x2,y2,x[i],1));
      f.f.push_back(1<<29);
    }
    l=y1,r=y2;
    if (l>r) swap(l,r);
    FOR(i,1,n) if (y[i]>l && y[i]<r && (isNb(i,ii) || isNb(i,jj))) {
      if (lFlag && l!=y[i] && r!=y[i]) continue;
      if (sy.count(y[i])) continue;
      sy.insert(y[i]);
      f.s.push_back(Statu(x1,y1,x2,y2,y[i],0));
      f.f.push_back(1<<29);
    }
  }

  int calRepeat(pair<pair<int,int>,int> a,vector<pair<pair<int,int>,int> > &b,int lim) {
    FOR(i,0,lim-1) if (a.first.first==b[i].first.first) {
      int &y1=a.first.second,&l1=a.second;
      int &y2=b[i].first.second,&l2=b[i].second;
      if (y2<=y1 && y2+l2>y1) {
        int d=y2+l2-y1;
        y1+=d,l1-=d;
      }
      if (y2+l2>=y1+l1 && y2<y1+l1) {
        int d=y1+l1-y2;
        l1-=d;
      }
      if (l1<0) l1=0;
    }
    return a.second;
  }

  int calStatu(vector<Statu> &st) {
    vector<pair<pair<int,int>,int> > lx,ly;
    FOR(i,0,SZ(st)-1)
      st[i].add(lx,ly);
    int s=0;
    FOR(i,0,SZ(lx)-1)
      s+=calRepeat(lx[i],lx,i);
    FOR(i,0,SZ(ly)-1)
      s+=calRepeat(ly[i],ly,i);
    return s;
  }

  void searchStatu(vector<int> &a,int i,int s,int fn) {
    static vector<Statu> st;
    FStatu &ff=f[a[i]];
    FOR(j,0,SZ(ff.s)-1) {
      st.push_back(ff.s[j]);
      findSolve.push_back(j);
      if (i==SZ(a)-1) {
        int w=calStatu(st);
        if (fn==-1)
          f[a[i]].f[j]=min(f[a[i]].f[j],s+w);
        else {
          if (j==fn && f[a[i]].f[j]==s+w)
            solve=findSolve;
        }
      } else
        searchStatu(a,i+1,s+ff.f[j]-ff.s[j].length(),fn);
      findSolve.pop_back();
      st.pop_back();
    }
  }

  void dfs(int i,int fa) {
    vector<int> sp;
    FORE(i,j,k) if (j!=fa) {
      dfs(j,i);
      sp.push_back(j);
    }
    sp.push_back(i);
    initStatu(f[i],x[i],y[i],x[fa],y[fa],i,fa);
    searchStatu(sp,0,0,-1);
  }

  void getSolve(int i,int fa,int fn) {
    vector<int> sp;
    f[i].s[fn].print(lineX,lineY);
    FORE(i,j,k) if (j!=fa)
      sp.push_back(j);
    sp.push_back(i);
    findSolve.clear();
    solve.clear();
    searchStatu(sp,0,0,fn);
    vector<int> mySolve=solve;
    int dd=0;
    FORE(i,j,k) if (j!=fa)
      getSolve(j,i,mySolve[dd++]);
  }

  void dp(vector<int> &lineX,vector<int> &lineY) {
    lineX.clear();
    lineY.clear();
    if (n<=1) return;
    this->lineX=&lineX;
    this->lineY=&lineY;
    FOR(i,1,n) if (du[i]<=1) {
      root=i;break;
    }
    dfs(p[d[root]],root);
    int fn=-1;
    ans=1<<29;
    FOR(j,0,SZ(f[p[d[root]]].f)-1)
      if (f[p[d[root]]].f[j]<ans)
        ans=f[p[d[root]]].f[j],fn=j;
    cerr<<ans<<endl;
    getSolve(p[d[root]],root,fn);
  }

  void getMst(vector<int> &lineX,vector<int> &lineY) {
      lineX.clear();lineY.clear();
      FOR(i,1,n) FORE(i,j,k) {
          lineX.push_back(x[i]);
          lineY.push_back(y[i]);
          lineX.push_back(x[j]);
          lineY.push_back(y[j]);
      }
  }
};


struct GetTree {
  pair<pair3,pair<int,int> > *e=0;
  int *fa=0,*x=0,*y=0;
  int n,t=0,w1,w2,w3;
  Tree tree;
  int lFlag=0;

  ~GetTree() {
    DELARR(e);
    DELARR(fa);
    DELARR(x);
    DELARR(y);
  }

  int find(int x) {return fa[x]==x?x:fa[x]=find(fa[x]);}

  void work(int nn,int *xx,int *yy) {
    n=nn;
    e=new pair<pair3,pair<int,int> >[(n+1)*(n+1)];
    fa=new int[n+1];
    x=new int[n+1];
    y=new int[n+1];
    FOR(i,1,n) {
      fa[i]=i;
      x[i]=xx[i],y[i]=yy[i];
      FOR(j,1,i-1)
        e[++t]=make_pair(mkPair3(abs(x[i]-x[j])+abs(y[i]-y[j]),-abs(y[i]-y[j]),-max(x[i],x[j])),make_pair(i,j));
    }
    tree.init(n,x,y);
    tree.lFlag=lFlag;
    sort(e+1,e+1+t);
    w1=0,w2=0,w3=0;
    FOR(i,1,t) {
      int a=e[i].second.first;
      int b=e[i].second.second;
      if (find(a)!=find(b)) {
        fa[find(a)]=find(b);
        tree.link(a,b);
        w1+=e[i].first.first;
        w2+=e[i].first.second.first;
        w3+=e[i].first.second.second;
      }
    }
    cerr<<"MST complete "<<w1<<' '<<w2<<' '<<w3<<endl;
  }

};

}
