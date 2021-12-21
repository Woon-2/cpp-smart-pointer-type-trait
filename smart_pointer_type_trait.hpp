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

// =======================================================================================
// shared_ptr type trait
// =======================================================================================
template < typename T >
struct is_shared_ptr_impl : std::false_type {};

template < typename T >
struct is_shared_ptr_impl< std::shared_ptr< T > > : std::true_type {};

template < typename T >
struct is_shared_ptr : is_shared_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};

template < typename T >
constexpr bool is_shared_ptr_v = is_shared_ptr< T >::value;

template < typename T >
std::true_type is_shared_ptr_soft_impl( const std::shared_ptr< T >* );
std::false_type is_shared_ptr_soft_impl( ... );

template < typename T >
using is_shared_ptr_soft = decltype( is_shared_ptr_soft_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );

template < typename T >
constexpr bool is_shared_ptr_soft_v = is_shared_ptr_soft< T >::value;

// shared_ptr type trait end ==============================================================

// =======================================================================================
// unique_ptr type trait
// =======================================================================================
template < typename T >
struct is_unique_ptr_impl : std::false_type {};

template < typename T, typename Dx >
struct is_unique_ptr_impl< std::unique_ptr< T, Dx > > : std::true_type {};

template < typename T >
struct is_unique_ptr : is_unique_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};

template < typename T > constexpr bool is_unique_ptr_v = is_unique_ptr< T >::value;

template < typename T, typename Dx >
std::true_type is_unique_ptr_soft_impl( std::unique_ptr< T, Dx >* );
std::false_type is_unique_ptr_soft_impl( ... );

template < typename T >
using is_unique_ptr_soft = decltype( is_unique_ptr_soft_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );

template < typename T >
constexpr bool is_unique_ptr_soft_v = is_unique_ptr_soft< T >::value;

// unique_ptr type trait end ==============================================================

// =======================================================================================
// united type trait ( smart pointer, pointer( smart pointers + raw_pointer ) )
// =======================================================================================
template < typename T >
struct is_smart_ptr : std::conditional_t< is_shared_ptr_v< T >, std::true_type,
	std::conditional_t< is_unique_ptr_v< T >, std::true_type, std::false_type > > {};

template < typename T > constexpr bool is_smart_ptr_v = is_smart_ptr< T >::value;

template < typename T, typename Dx >
std::true_type is_smart_ptr_soft_impl( std::unique_ptr< T, Dx >* );
template < typename T >
std::true_type is_smart_ptr_soft_impl( std::shared_ptr< T >* );
std::false_type is_smart_ptr_soft_impl( ... );

template < typename T >
using is_smart_ptr_soft = decltype( is_smart_ptr_soft_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );

template < typename T >
constexpr bool is_smart_ptr_soft_v = is_smart_ptr_soft< T >::value;


template < typename T >
struct is_ptr_impl : std::conditional_t< std::is_pointer_v< T >, std::true_type,
	std::conditional_t< is_smart_ptr_v< T >, std::true_type, std::false_type > > {};

template < typename T >
struct is_ptr : is_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};

template < typename T >
constexpr bool is_ptr_v = is_ptr< T >::value;

template < typename T, typename Dx >
std::true_type is_ptr_soft_impl( std::unique_ptr< T, Dx >* );
template < typename T >
std::true_type is_ptr_soft_impl( std::shared_ptr< T >* );
template < typename T >
std::true_type is_ptr_soft_impl( T** );
std::false_type is_ptr_soft_impl( ... );

template < typename T >
using is_ptr_soft = decltype( is_ptr_soft_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );

template < typename T >
constexpr bool is_ptr_soft_v = is_ptr_soft< T >::value;

// united type trait end ==============================================================

#endif