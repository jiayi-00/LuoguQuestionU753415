INF = 10**18

def ceildiv(a, b):
    return (a + b - 1) // b if a >= 0 else -((-a) // b)

def hi_fast(x, q, L, t, k):
    if q == 0 or k == 0:
        return x
    T = t + 1
    C = L - t
    if x >= T:
        return min(L, x + q * k)
    s = ceildiv(T - x, k)
    if q <= s:
        return min(C, x + q * k)
    y = min(C, x + s * k)
    return min(L, y + (q - s) * k) if y >= T else y

def lo_fast(x, q, L, t, k):
    return L + 1 - hi_fast(L + 1 - x, q, L, t, k)

def ap(seg, q, L, t, k):
    if not seg:
        return None
    l, h = seg
    return (lo_fast(l, q, L, t, k), hi_fast(h, q, L, t, k))

def ar(seg, L, t, U):
    if not seg:
        return None
    l, h = seg
    nl, nh = l + t, min(L, h + U)
    return (nl, nh) if nl <= L else None

def ev(a, q1, q2, q3, L, t, k):
    U = min(t + k, L - 1)
    s = (a, a)
    s = ap(s, q1, L, t, k)
    if q2 is not None:
        s = ar(s, L, t, U)
        s = ap(s, q2, L, t, k)
    if q3 is not None:
        s = ar(s, L, t, U)
        s = ap(s, q3, L, t, k)
    return s

def tmax(f, l, r):
    while r - l > 2:
        m1 = l + (r - l) // 3
        m2 = r - (r - l) // 3
        if f(m1) <= f(m2):
            l = m1
        else:
            r = m2
    return max(f(x) for x in range(l, r + 1))

def tmin(f, l, r):
    while r - l > 2:
        m1 = l + (r - l) // 3
        m2 = r - (r - l) // 3
        if f(m1) >= f(m2):
            l = m1
        else:
            r = m2
    return min(f(x) for x in range(l, r + 1))

def fixed_interval(a, q, m, L, t, k):
    U = min(t + k, L - 1)
    if m == 0:
        return ap((a, a), q, L, t, k)
    if m == 1:
        fh = lambda q1: (lambda s: s[1] if s else -10**15)(ev(a, q1, q - q1, None, L, t, k))
        fl = lambda q1: (lambda s: s[0] if s else 10**15)(ev(a, q1, q - q1, None, L, t, k))
        hi = tmax(fh, 0, q)
        lo = tmin(fl, 0, q)
        return None if hi < -10**14 else (lo, hi)
    if m == 2:
        def fh1(q1):
            f = lambda q2: (lambda s: s[1] if s else -10**15)(ev(a, q1, q2, q - q1 - q2, L, t, k))
            return tmax(f, 0, q - q1)
        def fl1(q1):
            f = lambda q2: (lambda s: s[0] if s else 10**15)(ev(a, q1, q2, q - q1 - q2, L, t, k))
            return tmin(f, 0, q - q1)
        hi = tmax(fh1, 0, q)
        lo = tmin(fl1, 0, q)
        return None if hi < -10**14 else (lo, hi)
    return None

def unb(D, t, k):
    D = abs(D)
    if D == 0:
        return 0
    if k == 0:
        return D // t if D % t == 0 else INF
    cands = {0, D // t, D // t + 1, ceildiv(D, t + k) - 1, ceildiv(D, t + k), ceildiv(D, t + k) + 1}
    best = INF
    for m in cands:
        if m < 0:
            continue
        q = max(0, ceildiv(m * t - D, k), ceildiv(D - m * (t + k), k))
        best = min(best, m + 2 * q)
    return best

def solve(L, a, b, t, k):
    if a == b:
        return 0
    if b < a:
        return solve(L, L + 1 - a, L + 1 - b, t, k)
    if L <= 2 * t:
        return None
    D = b - a
    U = unb(D, t, k)
    if U >= INF:
        return INF
    for r in range(U, U + 3):
        A = 2 * t + k
        lo = max(0, ceildiv(r * t + D, A))
        hi = min(r, (r * (t + k) + D) // A)
        for p in range(lo, hi + 1):
            q = r - p
            if q < 0:
                continue
            m = p - q
            if m < 0:
                # 反射：变成“多走 |m| 步向右”的情况
                aa = L + 1 - b
                bb = L + 1 - a
                mm = -m
                qq = min(p, q)
                if q == 0:
                    seg = None
                elif mm in (0, 1, 2):
                    seg = fixed_interval(aa, qq, mm, L, t, k)
                else:
                    return ('unsupported', L, a, b, t, k, r, p, q, m)
                if seg and seg[0] <= bb <= seg[1]:
                    return r
                continue
            if q == 0:
                seg = (a + m * t, min(L, a + m * min(t + k, L - 1)))
                if seg[0] > L:
                    seg = None
            elif m in (0, 1, 2):
                seg = fixed_interval(a, q, m, L, t, k)
            else:
                return ('unsupported', L, a, b, t, k, r, p, q, m)
            if seg and seg[0] <= b <= seg[1]:
                return r
    return INF
