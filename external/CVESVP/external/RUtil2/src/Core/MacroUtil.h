//Lists
#define _NumRev32 \
    31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, \
    13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
#define _Token32 \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, \
    _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31

//Arg Num
#define _ANum(...) __ANum(__VA_ARGS__, _NumRev32)
#define __ANum(...) __AMatch(__VA_ARGS__)
#define __AMatch(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, \
    _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, \
    _29, _30, _31, N, ...) N

//Expand
#define _E(...) __VA_ARGS__

//Delay
#define __none()
#define _D(...) __VA_ARGS__ __none()

//String
#define _S(a) __S(a)
#define __S(a) #a

//Concat
#define _C(...) _E(_C1(_C, _ANum(__VA_ARGS__)))(__VA_ARGS__)


#define _C0(a, b) a##b
#define _C1(a, b) _C0(a, b)
#define _C2(a, b, ...) _C1(a, _C1(b, __VA_ARGS__))
#define _C3(a, b, ...) _C1(a, _C2(b, __VA_ARGS__))
#define _C4(a, b, ...) _C1(a, _C3(b, __VA_ARGS__))
#define _C5(a, b, ...) _C1(a, _C4(b, __VA_ARGS__))
#define _C6(a, b, ...) _C1(a, _C5(b, __VA_ARGS__))
#define _C7(a, b, ...) _C1(a, _C6(b, __VA_ARGS__))
#define _C8(a, b, ...) _C1(a, _C7(b, __VA_ARGS__))
#define _C9(a, b, ...) _C1(a, _C8(b, __VA_ARGS__))
#define _C10(a, b, ...) _C1(a, _C9(b, __VA_ARGS__))
#define _C11(a, b, ...) _C1(a, _C10(b, __VA_ARGS__))
#define _C12(a, b, ...) _C1(a, _C11(b, __VA_ARGS__))
#define _C13(a, b, ...) _C1(a, _C12(b, __VA_ARGS__))
#define _C14(a, b, ...) _C1(a, _C13(b, __VA_ARGS__))
#define _C15(a, b, ...) _C1(a, _C14(b, __VA_ARGS__))

