INF = 10**18

def cd(a, b):
    return (a + b - 1) // b if a >= 0 else -((-a) // b)

def hi_pair(x, n, L, t, k):
    if n == 0 or k == 0:
        return x
    T, C = t + 1, L - t
    if x >= T:
        return min(L, x + n * k)
    s = cd(T - x, k)
    if n <= s:
        return min(C, x + n * k)
    y = min(C, x + s * k)
    return min(L, y + (n - s) * k) if y >= T else y

def lo_pair(x, n, L, t, k):
    return L + 1 - hi_pair(L + 1 - x, n, L, t, k)

def pairs(seg, n, L, t, k):
    if not seg:
        return None
    return (lo_pair(seg[0], n, L, t, k), hi_pair(seg[1], n, L, t, k))

def rblock(seg, n, L, t, U):
    if not seg:
        return None
    l, h = seg
    nl, nh = l + n * t, min(L, h + n * U)
    return (nl, nh) if nl <= L else None

def lblock(seg, n, L, t, U):
    if not seg:
        return None
    l, h = seg
    nl, nh = max(1, l - n * U), h - n * t
    return (nl, nh) if nh >= 1 else None

def smax(f, l, r):
    if l > r:
        return -10**15
    best = max(f(l), f(r))
    while r - l > 2:
        m1 = l + (r - l) // 3
        m2 = r - (r - l) // 3
        v1, v2 = f(m1), f(m2)
        best = max(best, v1, v2)
        if v1 <= v2:
            l = m1
        else:
            r = m2
    for x in range(l, r + 1):
        best = max(best, f(x))
    return best

def smin(f, l, r):
    if l > r:
        return 10**15
    best = min(f(l), f(r))
    while r - l > 2:
        m1 = l + (r - l) // 3
        m2 = r - (r - l) // 3
        v1, v2 = f(m1), f(m2)
        best = min(best, v1, v2)
        if v1 >= v2:
            l = m1
        else:
            r = m2
    for x in range(l, r + 1):
        best = min(best, f(x))
    return best

def right_major(a, q, m, L, t, k):
    U = min(t + k, L - 1)
    if m == 0:
        return pairs((a, a), q, L, t, k)
    if m == 1:
        def ev(q1):
            s = pairs((a, a), q1, L, t, k)
            s = rblock(s, 1, L, t, U)
            return pairs(s, q - q1, L, t, k)
        hi = smax(lambda q1: ev(q1)[1] if ev(q1) else -10**15, 0, q)
        lo = smin(lambda q1: ev(q1)[0] if ev(q1) else 10**15, 0, q)
        return None if hi < -10**14 else (lo, hi)
    # m == 2
    def ev(q1, q2):
        s = pairs((a, a), q1, L, t, k)
        s = rblock(s, 1, L, t, U)
        s = pairs(s, q2, L, t, k)
        s = rblock(s, 1, L, t, U)
        return pairs(s, q - q1 - q2, L, t, k)
    def hi1(q1):
        return smax(lambda q2: ev(q1, q2)[1] if ev(q1, q2) else -10**15, 0, q - q1)
    def lo1(q1):
        return smin(lambda q2: ev(q1, q2)[0] if ev(q1, q2) else 10**15, 0, q - q1)
    hi = smax(hi1, 0, q)
    lo = smin(lo1, 0, q)
    return None if hi < -10**14 else (lo, hi)

def left_major(a, p, q, L, t, k):
    U = min(t + k, L - 1)
    if q == 1:
        def ev(p1):
            s = rblock((a, a), p1, L, t, U)
            s = lblock(s, 1, L, t, U)
            return rblock(s, p - p1, L, t, U)
        hi = smax(lambda p1: ev(p1)[1] if ev(p1) else -10**15, 0, p)
        lo = smin(lambda p1: ev(p1)[0] if ev(p1) else 10**15, 0, p)
        return None if hi < -10**14 else (lo, hi)
    # q == 2
    def ev(p1, p2):
        s = rblock((a, a), p1, L, t, U)
        s = lblock(s, 1, L, t, U)
        s = rblock(s, p2, L, t, U)
        s = lblock(s, 1, L, t, U)
        return rblock(s, p - p1 - p2, L, t, U)
    def hi1(p1):
        return smax(lambda p2: ev(p1, p2)[1] if ev(p1, p2) else -10**15, 0, p - p1)
    def lo1(p1):
        return smin(lambda p2: ev(p1, p2)[0] if ev(p1, p2) else 10**15, 0, p - p1)
    hi = smax(hi1, 0, p)
    lo = smin(lo1, 0, p)
    return None if hi < -10**14 else (lo, hi)

def unb(D, t, k):
    D = abs(D)
    if D == 0:
        return 0
    if k == 0:
        return D // t if D % t == 0 else INF
    c = {0, D // t, D // t + 1, cd(D, t + k) - 1, cd(D, t + k), cd(D, t + k) + 1}
    ans = INF
    for m in c:
        if m < 0:
            continue
        q = max(0, cd(m * t - D, k), cd(D - m * (t + k), k))
        ans = min(ans, m + 2 * q)
    return ans

def solve_mid(L, a, b, t, k):
    if a == b:
        return 0
    if b < a:
        return solve_mid(L, L + 1 - a, L + 1 - b, t, k)
    if L <= 2 * t:
        return None
    D = b - a
    U0 = unb(D, t, k)
    if U0 >= INF:
        return INF
    Ustep = min(t + k, L - 1)
    for r in range(U0, U0 + 3):
        A = 2 * t + k
        pl = max(0, cd(r * t + D, A))
        pr = min(r, (r * (t + k) + D) // A)
        for p in range(pl, pr + 1):
            q = r - p
            if q < 0:
                continue
            if p < q:
                aa, bb = L + 1 - b, L + 1 - a
                pp, qq = q, p
                m = pp - qq
                if qq == 0:
                    seg = None
                elif m <= 2:
                    seg = right_major(aa, qq, m, L, t, k)
                elif qq <= 2:
                    seg = left_major(aa, pp, qq, L, t, k)
                else:
                    return ('unsupported', L, a, b, t, k, r, p, q)
                if seg and seg[0] <= bb <= seg[1]:
                    return r
                continue
            m = p - q
            if q == 0:
                l, h = a + p * t, min(L, a + p * Ustep)
                seg = (l, h) if l <= L else None
            elif m <= 2:
                seg = right_major(a, q, m, L, t, k)
            elif q <= 2:
                seg = left_major(a, p, q, L, t, k)
            else:
                return ('unsupported', L, a, b, t, k, r, p, q)
            if seg and seg[0] <= b <= seg[1]:
                return r
    return INF
