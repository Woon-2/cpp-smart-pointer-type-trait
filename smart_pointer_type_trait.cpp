#include <iostream>
#include <memory>

// ==========================================================================
// type traits for smart pointers.
// supporting is_unique_ptr< T >, is_shared_ptr< T >, is_smart_pointer< T >.
// usage is like std::is_pointer< T >.
// ==========================================================================

template < typename T > struct is_shared_ptr_impl : std::false_type {};
template < typename T > struct is_shared_ptr_impl< std::shared_ptr< T > > : std::true_type {};
template < typename T > struct is_shared_ptr : is_shared_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};
template < typename T > constexpr bool is_shared_ptr_v = is_shared_ptr< T >::value;

template < typename T > struct is_unique_ptr_impl : std::false_type {};
template < typename T, typename Dx > struct is_unique_ptr_impl< std::unique_ptr< T, Dx > > : std::true_type {};
template < typename T > struct is_unique_ptr : is_unique_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};
template < typename T > constexpr bool is_unique_ptr_v = is_unique_ptr< T >::value;

template < typename T >
std::true_type inherit_from_shared_ptr_impl( const std::shared_ptr< T >* );
std::false_type inherit_from_shared_ptr_impl( ... );
template < typename T >
using inherit_from_shared_ptr = decltype( inherit_from_shared_ptr_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
template < typename T > constexpr bool inherit_from_shared_ptr_v = inherit_from_shared_ptr< T >::value;

template < typename T, typename Dx >
std::true_type inherit_from_unique_ptr_impl( const std::unique_ptr< T, Dx >* );
std::false_type inherit_from_unique_ptr_impl( ... );
template < typename T >
using inherit_from_unique_ptr = decltype( inherit_from_unique_ptr_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
template < typename T > constexpr bool inherit_from_unique_ptr_v = inherit_from_unique_ptr< T >::value;

template < typename T > struct is_smart_ptr : std::conditional_t< is_shared_ptr_v< T >, std::true_type,
	std::conditional_t< is_unique_ptr_v< T >, std::true_type, std::false_type > > {};
template < typename T > constexpr bool is_smart_ptr_v = is_smart_ptr< T >::value;

template < typename T, typename Dx >
std::true_type inherit_from_smart_ptr_impl( const std::unique_ptr< T, Dx >* );
template < typename T >
std::true_type inherit_from_smart_ptr_impl( const std::shared_ptr< T >* );
std::false_type inherit_from_smart_ptr_impl( ... );
template < typename T >
using inherit_from_smart_ptr = decltype( inherit_from_smart_ptr_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
template < typename T > constexpr bool inherit_from_smart_ptr_v = inherit_from_smart_ptr< T >::value;



// ==========================================================================
// Test Code
// ==========================================================================

template < typename Ty >
struct Sptr : std::shared_ptr< Ty >
{
	void func()
	{
		std::cout << "¿¹\n";
	}
	Sptr( const Ty& val ) : std::shared_ptr< Ty >( new Ty{ val } ) {}
};
template < typename Ty >
struct Uptr : std::unique_ptr< Ty >
{
	void func()
	{
		std::cout << "¿¹\n";
	}
	Uptr( const Ty& val ) : std::unique_ptr< Ty >( new Ty{ val } ) {}
};
int main()
{
	auto check = []( auto&& target )
	{
		std::cout << typeid( target ).name() << '\n';
		std::cout << "is_shared_ptr: " << is_shared_ptr_v< decltype( target ) > << '\n';
		std::cout << "is_unique_ptr: " << is_unique_ptr_v< decltype( target ) > << '\n';
		std::cout << "is_smart_ptr: " << is_smart_ptr_v< decltype( target ) > << '\n';
		std::cout << "inherit_from_shared_ptr: " << inherit_from_shared_ptr_v< decltype( target ) > << '\n';
		std::cout << "inherit_from_unique_ptr: " << inherit_from_unique_ptr_v< decltype( target ) > << '\n';
		std::cout << "inherit_from_smart_ptr: " << inherit_from_smart_ptr_v< decltype( target ) > << '\n';
		std::cout << "\n\n\n";
	};
	const auto a = std::make_unique< int >( 3 );
	check( a );
	Uptr< int > b{ 2 };
	check( b );
	auto c = std::make_shared< int >( 4 );
	check( c );
	Sptr< int > d{ 5 };
	check( d );
	int e = 8;
	check( e );
}