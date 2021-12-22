// ==========================================================================
// Test Code
// ==========================================================================

#include <iostream>
#include "smart_pointer_type_trait.hpp"

template < typename Ty >
struct Sptr : std::shared_ptr< Ty >
{
	Sptr( const Ty& val ) : std::shared_ptr< Ty >( new Ty{ val } ) {}
};

template < typename Ty >
struct Uptr : std::unique_ptr< Ty >
{
	Uptr( const Ty& val ) : std::unique_ptr< Ty >( new Ty{ val } ) {}
};

int main()
{
	auto check = []( auto&& target )
	{
		std::cout << typeid( target ).name() << '\n';
		std::cout << "is_shared_ptr: " << woon2::is_shared_ptr_v< decltype( target ) > << '\n';
		std::cout << "is_unique_ptr: " << woon2::is_unique_ptr_v< decltype( target ) > << '\n';
		std::cout << "is_smart_ptr: " << woon2::is_smart_ptr_v< decltype( target ) > << '\n';
		std::cout << "is_shared_ptr_soft: " << woon2::is_shared_ptr_soft_v< decltype( target ) > << '\n';
		std::cout << "is_unique_ptr_soft: " << woon2::is_unique_ptr_soft_v< decltype( target ) > << '\n';
		std::cout << "is_smart_ptr_soft: " << woon2::is_smart_ptr_soft_v< decltype( target ) > << '\n';
		std::cout << "is_pointable: " << woon2::is_pointable_v< decltype( target ) > << '\n';
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

	int* f = &e;
	check( f );
}