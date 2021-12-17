// ==========================================================================
// Test Code
// ==========================================================================

#include <iostream>
#include "smart_pointer_type_trait.hpp"

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