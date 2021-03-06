#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>
#include<queue>
using namespace std;
const int inf = 0x3fffffff, u = 40010, w = 300010;
int head[u], ver[w], edge[w], Next[w], d[u], e[w], c[u], sta[u], ins[u], dfn[u], low[u];
int n, m, p, s, t, i, j, tot, maxflow, ans, x, y, scc, st, num;
char str[10];
vector<int> a[u];
queue<int> q;

void add(int x, int y, int z) {
	ver[++tot] = y, edge[tot] = z, Next[tot] = head[x], head[x] = tot;
	ver[++tot] = x, edge[tot] = 0, Next[tot] = head[y], head[y] = tot;
}

bool bfs() {
	memset(d, 0, sizeof(d));
	while (q.size()) q.pop();
	q.push(s); d[s] = 1;
	while (q.size()) {
		int x = q.front(); q.pop();
		for (int i = head[x]; i; i = Next[i])
			if (edge[i] && !d[ver[i]]) {
				q.push(ver[i]);
				d[ver[i]] = d[x] + 1;
				if (ver[i] == t) return 1;
			}
	}
	return 0;
}

int dinic(int x, int flow) {
	if (x == t) return flow;
	int rest = flow, k;
	for (int i = head[x]; i && rest; i = Next[i])
		if (edge[i] && d[ver[i]] == d[x] + 1) {
			k = dinic(ver[i], min(rest, edge[i]));
			if (!k) d[ver[i]] = 0;
			edge[i] -= k;
			edge[i ^ 1] += k;
			rest -= k;
		}
	return flow - rest;
}

void add2(int x, int y)
{
	a[x].push_back(y);
}

void tarjan(int x)
{
	dfn[x] = ++num; low[x] = num;
	sta[++st] = x; ins[x] = 1;
	int y;
	for (int i = 0; i<a[x].size(); i++)
		if (!dfn[y = a[x][i]])
		{
			tarjan(y);
			low[x] = min(low[x], low[y]);
		}
		else if (ins[y]) low[x] = min(low[x], dfn[y]);
		if (dfn[x] == low[x])
		{
			scc++;
			do { y = sta[st]; st--; ins[y] = 0; c[y] = scc; } while (x != y);
		}
}

int main()
{
	while (cin >> n >> m >> p)
	{
		memset(head, 0, sizeof(head));
		s = 0, t = n + m + 1; tot = 1; maxflow = 0;
		for (i = 1; i <= n; i++) add(s, i, 1);  //源点s到左部点连有向边，容量为1，偶数边为s到i,奇数边为i到s
		for (i = 1; i <= m; i++) add(i + n, t, 1); //右部点到汇点t连有向边，容量为1,偶数边为i到t,奇数边为t到i
		for (i = 1; i <= p; i++) //左部点到右部点连边
		{
			scanf("%d%d", &x, &y);
			add(x, n + y, 1), e[i] = tot;//记录当前建的右部点到左部点这条边属于哪条边
		}

		while (bfs())//求最大流即最大匹配
			while (i = dinic(s, inf)) maxflow += i;

		for (i = s; i <= t; i++) a[i].clear();
		for (i = 1; i <= p; i++)//在残量网络上建图
			if (!edge[e[i]]) add2(ver[e[i]], ver[e[i] ^ 1]);//匹配边，建边的方向：从右到左
			else add2(ver[e[i] ^ 1], ver[e[i]]);//非匹配边，建边的方向：从左到右
	  for (i = 1; i <= n; i++)//源点与左部点间的有向边
				if (!edge[2 * i]) add2(i, s);
				else add2(s, i);
		for (i = 1; i <= m; i++)
				if (!edge[2 * (n + i)]) add2(t, n + i);//i到t剩余容量为0，则建反向边t到i
				else add2(n + i, t);
		memset(dfn, 0, sizeof(dfn));
		memset(ins, 0, sizeof(ins));
		memset(c, 0, sizeof(c));
		st = num = scc = ans = 0;
		for (i = s; i <= t; i++)//求强连通分量
				if (!dfn[i]) tarjan(i);
		for (i = 1; i <= p; i++)
				if (edge[e[i]] || c[ver[e[i]]] == c[ver[e[i] ^ 1]]) ans++;//可行边
		cout << (ans = p - ans) << endl;
		if (!ans) cout << endl;
		for (i = 1; i <= p; i++)
				if (!edge[e[i]] && c[ver[e[i]]] != c[ver[e[i] ^ 1]])//非可行边
					if (--ans) printf("%d ", i);
					else printf("%d\n", i);
	}
}
