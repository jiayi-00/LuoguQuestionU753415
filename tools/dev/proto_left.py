from proto_middle import ap, ar, tmax, tmin

def al(seg, n, L, t, U):
    if not seg:
        return None
    l, h = seg
    nl, nh = max(1, l - n * U), h - n * t
    return (nl, nh) if nh >= 1 else None

def left_interval(a, p, q, L, t, k):
    U = min(t + k, L - 1)
    if q == 0:
        l = a + p * t
        h = min(L, a + p * U)
        return (l, h) if l <= L else None
    if q == 1:
        def fh(p1):
            s = (a, a)
            s = ar(s, L, t, U) if False else None
            # right block p1
            l, h = a, a
            nl, nh = l + p1 * t, min(L, h + p1 * U)
            s = (nl, nh) if nl <= L else None
            s = al(s, 1, L, t, U)
            if not s:
                return -10**15
            p2 = p - p1
            nl, nh = s[0] + p2 * t, min(L, s[1] + p2 * U)
            return nh if nl <= L else -10**15
        def fl(p1):
            l, h = a, a
            nl, nh = l + p1 * t, min(L, h + p1 * U)
            s = (nl, nh) if nl <= L else None
            s = al(s, 1, L, t, U)
            if not s:
                return 10**15
            p2 = p - p1
            nl, nh = s[0] + p2 * t, min(L, s[1] + p2 * U)
            return nl if nl <= L else 10**15
        hi = tmax(fh, 0, p)
        lo = tmin(fl, 0, p)
        return None if hi < -10**14 else (lo, hi)
    if q == 2:
        def eval_split(p1, p2):
            p3 = p - p1 - p2
            if p3 < 0:
                return None
            l, h = a, a
            nl, nh = l + p1 * t, min(L, h + p1 * U)
            s = (nl, nh) if nl <= L else None
            s = al(s, 1, L, t, U)
            nl, nh = (s[0] + p2 * t, min(L, s[1] + p2 * U)) if s else (1, 0)
            s = (nl, nh) if s and nl <= L else None
            s = al(s, 1, L, t, U)
            nl, nh = (s[0] + p3 * t, min(L, s[1] + p3 * U)) if s else (1, 0)
            return (nl, nh) if s and nl <= L else None
        def fh1(p1):
            f = lambda p2: (lambda s: s[1] if s else -10**15)(eval_split(p1, p2))
            return tmax(f, 0, p - p1)
        def fl1(p1):
            f = lambda p2: (lambda s: s[0] if s else 10**15)(eval_split(p1, p2))
            return tmin(f, 0, p - p1)
        hi = tmax(fh1, 0, p)
        lo = tmin(fl1, 0, p)
        return None if hi < -10**14 else (lo, hi)
    return None
