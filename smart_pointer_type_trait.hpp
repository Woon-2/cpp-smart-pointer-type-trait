// ==========================================================================
// Type Traits for smart pointers
// Supporting is_unique_ptr< T >, is_shared_ptr< T >, is_smart_ptr< T >, is_ptr< T >
// Usage is like std::is_pointer< T >.
// Soft version type traits additionally evalute a derived class from a pointer as true.
// Just write "soft" after a type_trait to embrace inheritance.
// ==========================================================================

#ifndef _smart_pointer_type_trait
#define _smart_pointer_type_trait

#include <memory>

namespace woon2
{
	namespace detail
	{
		// for removing const, volatile, reference.
		// via this, type traits have no dependency on qualifiers.
		template < typename T >
		using remove_cvr_t = std::remove_cv_t< std::remove_reference_t< T > >;
	}

	// =======================================================================================
	// shared_ptr type trait
	// =======================================================================================
	namespace detail
	{
		template < typename T >
		struct is_shared_ptr_impl : std::false_type {};

		template < typename T >
		struct is_shared_ptr_impl< std::shared_ptr< T > > : std::true_type {};

		template < typename T >
		std::true_type is_shared_ptr_soft_impl( const std::shared_ptr< T >* );
		std::false_type is_shared_ptr_soft_impl( ... );
	}

	template < typename T >
	using is_shared_ptr = detail::is_shared_ptr_impl< detail::remove_cvr_t< T > >;

	template < typename T >
	constexpr bool is_shared_ptr_v = is_shared_ptr< T >::value;

	template < typename T >
	using is_shared_ptr_soft = decltype( detail::is_shared_ptr_soft_impl( std::declval< detail::remove_cvr_t< T >* >() ) );

	template < typename T >
	constexpr bool is_shared_ptr_soft_v = is_shared_ptr_soft< T >::value;

	// shared_ptr type trait end =============================================================

	// =======================================================================================
	// unique_ptr type trait
	// =======================================================================================
	namespace detail
	{
		template < typename T >
		struct is_unique_ptr_impl : std::false_type {};

		template < typename T, typename Dx >
		struct is_unique_ptr_impl< std::unique_ptr< T, Dx > > : std::true_type {};

		template < typename T, typename Dx >
		std::true_type is_unique_ptr_soft_impl( std::unique_ptr< T, Dx >* );
		std::false_type is_unique_ptr_soft_impl( ... );
	}

	template < typename T >
	using is_unique_ptr = detail::is_unique_ptr_impl< detail::remove_cvr_t< T > >;

	template < typename T >
	constexpr bool is_unique_ptr_v = is_unique_ptr< T >::value;

	template < typename T >
	using is_unique_ptr_soft = decltype( detail::is_unique_ptr_soft_impl( std::declval< detail::remove_cvr_t< T >* >() ) );

	template < typename T >
	constexpr bool is_unique_ptr_soft_v = is_unique_ptr_soft< T >::value;

	// unique_ptr type trait end =============================================================


	// =======================================================================================
	// united type trait ( smart pointers )
	// =======================================================================================
	template < typename T >
	using is_smart_ptr = std::conditional_t<
		is_shared_ptr_v< T >,
		std::true_type,
		std::conditional_t<
			is_unique_ptr_v< T >,
			std::true_type,
			std::false_type
			>
		>;

	template < typename T >
	constexpr bool is_smart_ptr_v = is_smart_ptr< T >::value;
	
	namespace detail
	{
		template < typename T, typename Dx >
		std::true_type is_smart_ptr_soft_impl( std::unique_ptr< T, Dx >* );
		template < typename T >
		std::true_type is_smart_ptr_soft_impl( std::shared_ptr< T >* );
		std::false_type is_smart_ptr_soft_impl( ... );
	}

	template < typename T >
	using is_smart_ptr_soft = decltype( detail::is_smart_ptr_soft_impl( std::declval< detail::remove_cvr_t< T >* >() ) );

	template < typename T >
	constexpr bool is_smart_ptr_soft_v = is_smart_ptr_soft< T >::value;

	// united type trait ( smart pointers ) end ==============================================

	// =======================================================================================
	// united type trait ( all pointable classes )
	// =======================================================================================
	namespace detail
	{
		template < typename T, typename _ = void >
		struct is_pointable_impl : std::false_type {};

		template < typename T >
		struct is_pointable_impl< T* > : std::true_type {};

		template < typename ... Ts >
		struct is_pointable_helper {};

		template < typename T >
		struct is_pointable_impl< T,
			std::conditional_t<
				false,
				is_pointable_helper<
					decltype( std::declval< T >().operator->() ),
					decltype( std::declval< T >().operator*() )
					>,
				void
				>
			> : std::true_type {};
	}

	template < typename T >
	using is_pointable = detail::is_pointable_impl< detail::remove_cvr_t< T > >;

	template < typename T >
	constexpr bool is_pointable_v = is_pointable< T >::value;

	// united type trait ( all pointable classes ) end =======================================

	// method trait ( trace if a pointer class has a specified method. ) =====================
	namespace detail
	{
		template < typename T, typename _ = void >
		struct has_reset_impl : std::false_type {};

		template < typename ... Ts >
		struct has_reset_helper {};

		template < typename T >
		struct has_reset_impl< T, std::conditional_t<
			false,
			has_reset_helper<
				decltype( std::declval< T >().reset() )
				>,
			void
			>
		> : std::true_type {};

		template < typename T, typename _ = void >
		struct has_release_impl : std::false_type {};

		template < typename ... Ts >
		struct has_release_helper {};

		template < typename T >
		struct has_release_impl< T, std::conditional_t<
			false,
			has_release_helper<
				decltype( std::declval< T >().release() )
				>,
			void
			>
		> : std::true_type {};

		template < typename T, typename _ = void >
		struct has_get_deleter_impl : std::false_type {};
		
		template < typename ... Ts >
		struct has_get_deleter_helper {};

		template < typename T >
		struct has_get_deleter_impl< T, std::conditional_t<
			false,
			has_get_deleter_helper<
				decltype( std::declval< T >().get_deleter() ),
				decltype( std::declval< T >().get_deleter() )
				>,
			void
			>
		> : std::true_type {};

		template < typename T, typename _ = void >
		struct has_swap_impl : std::false_type {};

		template < typename ... Ts >
		struct has_swap_helper {};

		template < typename T >
		struct has_swap_impl< T, std::conditional_t<
			false,
			has_swap_helper<
				decltype( std::declval< T >().swap( std::declval< std::add_lvalue_reference_t< T > >() ) )
				>,
			void
			>
		> : std::true_type {};
	}

	template < typename T >
	using has_reset = detail::has_reset_impl< detail::remove_cvr_t< T > >;

	template < typename T >
	constexpr bool has_reset_v = has_reset< T >::value;

	template < typename T >
	using has_release = detail::has_release_impl< detail::remove_cvr_t< T > >;

	template < typename T >
	constexpr bool has_release_v = has_release< T >::value;

	template < typename T >
	using has_get_deleter = detail::has_get_deleter_impl< detail::remove_cvr_t< T > >;

	template < typename T >
	constexpr bool has_get_deleter_v = has_get_deleter< T >::value;

	template < typename T >
	using has_swap = detail::has_swap_impl< detail::remove_cvr_t< T > >;

	template < typename T >
	constexpr bool has_swap_v = has_swap< T >::value;

	// method trait end ======================================================================

	// =======================================================================================
	// Additional pointer type trait
	// You can add your own pointer class's type trait.
	// This is the template for that.
	// Can be detected by is_pointable< T >
	// =======================================================================================

	// just for sample, it will be better for you to make your pointer class be in another header file.
	template < typename T >
	struct your_pointer {};
	// /sample
	// just alter all "your_pointer"s to your pointer class's name.

	namespace detail
	{
		template < typename T >
		struct is_your_pointer_impl : std::false_type {};

		template < typename T >
		struct is_your_pointer_impl< your_pointer< T > > : std::true_type {};

		template < typename T >
		std::true_type is_your_pointer_soft_impl( your_pointer< T >* );
		std::false_type is_your_pointer_soft_impl( ... );
	}

	template < typename T >
	using is_your_pointer = detail::is_your_pointer_impl< detail::remove_cvr_t< T > >;

	template < typename T >
	constexpr bool is_your_pointer_v = is_your_pointer< T >::value;

	template < typename T >
	using is_your_pointer_soft = decltype( detail::is_your_pointer_soft_impl( std::declval< detail::remove_cvr_t< T >* >() ) );

	template < typename T >
	constexpr bool is_your_pointer_soft_v = is_your_pointer_soft< T >::value;

	// additional pointer type trait end =============================================================
}

#endif // _smart_pointer_type_trait