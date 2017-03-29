#ifndef _HASHHELPER_H
#define _HASHHELPER_H


#include <functional>
#include <utility>
#include <cstddef>
#include <tuple>
#include <cstdint>


#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0 (EVAL0 (EVAL0 (__VA_ARGS__)))
#define EVAL2(...) EVAL1 (EVAL1 (EVAL1 (__VA_ARGS__)))
#define EVAL3(...) EVAL2 (EVAL2 (EVAL2 (__VA_ARGS__)))
#define EVAL4(...) EVAL3 (EVAL3 (EVAL3 (__VA_ARGS__)))
#define EVAL(...) EVAL4 (EVAL4 (EVAL4 (__VA_ARGS__)))
#define MAP_END(...)
#define MAP_OUT
#define MAP_GET_END() 0, MAP_END
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(item, next) EVAL(MAP_NEXT0 (item, next, 0))
#define MAP_NEXT(test, next) MAP_NEXT1(MAP_GET_END test, next)
#define MAP0(f, x, peek, ...) f(x) MAP_NEXT (peek, MAP1) (f, peek, __VA_ARGS__)
#define MAP1(f, x, peek, ...) f(x) MAP_NEXT (peek, MAP0) (f, peek, __VA_ARGS__)
#define MAP(f, ...) EVAL (MAP1 (f, __VA_ARGS__, (), 0))



template<typename T>
std::size_t hash_value(const T& value)
{
	static std::hash<T> hasher;
	return hasher(value);
}


inline void hash_combine(std::size_t& seed) { }

template <typename T>
inline void hash_combine(std::size_t& seed, const T& v) {
	seed ^= hash_value(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}



#define _HASH_COMBINE(val) hash_combine(ret, t.##val);

#define MAKE_HASHABLE(type, ...) \
	namespace std \
	{\
		template<> struct hash< type > {\
			std::size_t operator()(const type & t) {\
				std::size_t ret = 0; \
				MAP(_HASH_COMBINE, ##__VA_ARGS__) \
				return ret;\
			}\
		};\
	}

#define MAKE_TEMPLATE(type, ...) \
	type ## < ## __VA_ARGS__ ## >

#define _HASH_TUPLE_COMBINE(val) hash_combine(ret, ##val);

#define MAKE_TUPLE_HASHABLE_(type, ...) \
	namespace std \
	{\
		template<> struct hash< type > {\
			std::size_t operator()(const type & t) {\
				std::size_t ret = 0; \
				hash_combine(ret,std::get<0>(t));\
				hash_combine(ret,std::get<1>(t));\
				hash_combine(ret,std::get<2>(t));\
				return ret;\
			}\
		};\
	}

#define MAKE_TUPLE_HASHABLE(t1, t2, t3) MAKE_TUPLE_HASHABLE_(MAKE_TEMPLATE(std::tuple, t1,t2,t3))


#endif // !_HASHHEPER_H
