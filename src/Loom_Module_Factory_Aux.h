#pragma once

#undef min
#undef max
#include <array>
#include <tuple>


namespace factory {


///////////////////////////////////////////////////////////////////////////////


	namespace ersatz {

	template<std::size_t I, typename T> using tuple_element_t = typename std::tuple_element<I, T>::type;
	template<typename T> using remove_reference_t = typename std::remove_reference<T>::type;
	template<typename... T> using common_type_t = typename std::common_type<T...>::type;

	template<typename Int, Int... Seq> struct integer_sequence {
		using value_type = Int;
		static constexpr std::size_t size() noexcept { return sizeof...(Seq); }
	};

	template<std::size_t... Seq> using index_sequence = integer_sequence<std::size_t, Seq...>;

	template<typename Int, std::size_t Acc, Int... Seq> struct build_sequence { using type = typename build_sequence<Int, Acc - 1, Acc - 1, Seq...>::type; };
	template<typename Int, Int... Seq> struct build_sequence<Int, 0, Seq...> { using type = integer_sequence<Int, Seq...>; };

	template<typename Int, Int N> using make_integer_sequence = typename build_sequence<Int, N>::type;
	template<std::size_t N> using make_index_sequence = make_integer_sequence<std::size_t, N>;
		
	} // ersatz

	template<
		typename Tuple,
		typename VTuple = ersatz::remove_reference_t<Tuple>,
		std::size_t... Indices
	>
	constexpr std::array<
		ersatz::common_type_t<ersatz::tuple_element_t<Indices, VTuple>...>,
		sizeof...(Indices)
	>
	to_array(Tuple&& tuple, ersatz::index_sequence<Indices...>) 
	{
		return { std::get<Indices>(std::forward<Tuple>(tuple))... };
	}

	template<typename Tuple, typename VTuple = ersatz::remove_reference_t<Tuple>>
	constexpr auto to_array(Tuple&& tuple) 
	{
		return to_array(
			std::forward<Tuple>(tuple),
			ersatz::make_index_sequence<std::tuple_size<VTuple>::value> {} );
	}



	///////////////////////////////////////////////////////////////////////////////



	using namespace std;

	template<size_t index, typename T, typename... Ts>
	inline constexpr typename std::enable_if<index==0, T>::type
	select(T&& t, Ts&&... ts) {
		return t;
	}

	template<size_t index, typename T, typename... Ts>
	inline constexpr typename std::enable_if<(index > 0) && index <= sizeof...(Ts),
			  typename tuple_element<index, tuple<T, Ts...>>::type>::type
	select(T&& t, Ts&&... ts) {
		return select<index-1>(std::forward<Ts>(ts)...);
	}

	// below is optional - just for getting a more readable compilation error
	// in case calling get with a bad index

	// inline template<long long index, typename... Ts>
	template<long long index, typename... Ts>
	constexpr bool index_ok() {
		return index >= 0 && index < sizeof...(Ts);
	}

	template<long long index, typename T, typename... Ts>
	inline constexpr
	typename std::enable_if<!index_ok<index, T, Ts...>(), T>::type
	select(T&& t, Ts&&... ts) {
		static_assert(index_ok<index, T, Ts...>(),
			"bad index in call to get, smaller than zero or above pack size");
		return t;
	}




} // end factory


