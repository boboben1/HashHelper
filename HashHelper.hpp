/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
*/

#ifndef _HASHHELPER_H
#define _HASHHELPER_H


#include <functional>
#include <utility>
#include <cstddef>
#include <tuple>
#include <cstdint>

//BEGIN swansontec/map-macro
//https://github.com/swansontec/map-macro/blob/master/map.h
#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL4(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL(...)  EVAL4(EVAL4(EVAL4(__VA_ARGS__)))

#define MAP_END(...)
#define MAP_OUT
#define MAP_COMMA ,

#define MAP_GET_END2() 0, MAP_END
#define MAP_GET_END1(...) MAP_GET_END2
#define MAP_GET_END(...) MAP_GET_END1
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(test, next) MAP_NEXT0(test, next, 0)
#define MAP_NEXT(test, next)  MAP_NEXT1(MAP_GET_END test, next)

#define MAP0(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP1)(f, peek, __VA_ARGS__)
#define MAP1(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP0)(f, peek, __VA_ARGS__)

#define MAP_LIST_NEXT1(test, next) MAP_NEXT0(test, MAP_COMMA next, 0)
#define MAP_LIST_NEXT(test, next)  MAP_LIST_NEXT1(MAP_GET_END test, next)

#define MAP_LIST0(f, x, peek, ...) f(x) MAP_LIST_NEXT(peek, MAP_LIST1)(f, peek, __VA_ARGS__)
#define MAP_LIST1(f, x, peek, ...) f(x) MAP_LIST_NEXT(peek, MAP_LIST0)(f, peek, __VA_ARGS__)

/**
 * Applies the function macro `f` to each of the remaining parameters.
 */
#define MAP(f, ...) EVAL(MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters and
 * inserts commas between the results.
 */
#define MAP_LIST(f, ...) EVAL(MAP_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))
//END swansontec/map-macro


template<typename T>
std::size_t __hash_value(const T& value)
{
	static std::hash<T> hasher;
	return hasher(value);
}


inline void __hash_combine(std::size_t& seed) { }

template <typename T>
inline void __hash_combine(std::size_t& seed, const T& v) {
	seed ^= __hash_value(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

#ifndef BOOST_PREPROCESSOR_HPP
#define hash_combine __hash_combine
#define hash_value __hash_value
#endif

#define _DOT(t, val) t.val
#define _HASH_COMBINE(val) __hash_combine(ret, _DOT(t,val));

#define MAKE_HASHABLE(type, ...) \
	namespace std \
	{\
		template<> struct hash< type > {\
			std::size_t operator()(const type & t) const {\
				std::size_t ret = 0; \
				MAP(_HASH_COMBINE, __VA_ARGS__) \
				return ret;\
			}\
		};\
	}

#define MAKE_TEMPLATE(type, ...) \
	type<__VA_ARGS__>

#define _HASH_TUPLE_COMBINE(val) __hash_combine(ret, ##val);

#define MAKE_TUPLE_HASHABLE_(type, ...) \
	namespace std \
	{\
		template<> struct hash< type > {\
			std::size_t operator()(const type & t) {\
				std::size_t ret = 0; \
				__hash_combine(ret,std::get<0>(t));\
				__hash_combine(ret,std::get<1>(t));\
				__hash_combine(ret,std::get<2>(t));\
				return ret;\
			}\
		};\
	}

#define MAKE_TUPLE_HASHABLE(t1, t2, t3) \
    MAKE_TUPLE_HASHABLE_(MAKE_TEMPLATE(std::tuple, t1,t2,t3))
#define MAKE_PAIR_HASHABLE(t1, t2) \
    MAKE_HASHABLE(MAKE_TEMPLATE(std::pair, t1, t2), first, second)

#endif // !_HASHHEPER_H
