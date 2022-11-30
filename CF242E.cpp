//This is a solution for Codeforces Contest 242 Problem E: Xor on Segment.
/*
具体思路：
对数组每个数的每一位进行拆位维护。对于更新的数，
假如说它在二进制表达下的第ii位(从右往左)位1，
那么就要对第ii位的区间进行01倒置，否则不用处理。
最后查询的时候将二进制下第ii位1的数目乘以该位置的权值2^i 
再相加即可。(区间反转操作由线段树维护).
*/
#include <bits/stdc++.h>
#define mid ((l + r) >> 1)
#define lson rt << 1, l, mid
#define rson rt << 1 | 1, mid + 1, r
#define ll long long
using namespace std;

const int maxn = 1e5 + 5;
int n, m;
int a[maxn];
int aprime[21][maxn];
ll ones[21][maxn * 4];
bool times[21][maxn * 4];

void build(int rt, int l, int r, int index) {
    if (l == r) {
        ones[index][rt] = aprime[index][l];
        return;
    }
    build(lson, index); build(rson, index);
    ones[index][rt] = ones[index][rt << 1] + ones[index][rt << 1 | 1];
    return;
}

void put_tag(int rt, int l, int r, int index) {
    ones[index][rt] = (r - l + 1) - ones[index][rt];
    times[index][rt] ^= 1;
}

void push_down(int rt, int l, int r, int index) {
    if (times[index][rt] == false) return;
    put_tag(lson, index);
    put_tag(rson, index);
    times[index][rt] ^= 1;
}

void update(int rt, int l, int r, int L, int R, int index) {
    if (L <= l && r <= R) {
        put_tag(rt, l, r, index);
        return;
    }
    push_down(rt, l, r, index);
    if (L <= mid) update(lson, L, R, index);
    if (R > mid) update(rson, L, R, index);
    ones[index][rt] = ones[index][rt << 1] + ones[index][rt << 1 | 1];
}

ll qry(int rt, int l, int r, int L, int R) {
    if (L <= l && r <= R) {
        ll ans = 0;
        for (int i = 0; i <= 20; i++) {
            ll tmp = (1 << i);
            tmp *= ones[i][rt];
            ans += tmp;
        }
        return ans;
    }
    ll ans = 0;
    for (int i = 0; i <= 20; i++) push_down(rt, l, r, i);
    if (L <= mid) ans += qry(lson, L, R);
    if (R > mid) ans += qry(rson, L, R);
    return ans;
}

int main()
{
    ios::sync_with_stdio(false);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        for (int j = 0; j <= 20; j++) {
            bool x = (a[i]) & (1 << j);
            aprime[j][i] = x;
        }
    }
    for (int i = 0; i <= 20; i++) {
        build(1, 1, n, i);
    }
    cin >> m;
    while (m--) {
        int ins; cin >> ins;
        if (ins == 1) {
            int l, r; cin >> l >> r;
            cout << qry(1, 1, n, l, r) << '\n';
        }else {
            int l, r, x;
            cin >> l >> r >> x;
            for (int i = 0; i <= 20; i++) {
                if (x & (1 << i)) update(1, 1, n, l, r, i);
            }
        }
    }
    return 0;
}