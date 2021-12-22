// ==========================================================================
// Implementation: added new pointer class
// this is an example of smart_pointer_type_trait.hpp's additional type trait guide.
// ==========================================================================

#include <iostream>
#include "smart_pointer_type_trait.hpp"

template< typename Ty >
class value_pointer
{
public:
	// methods
	void reset( const Ty& val = Ty{} )
	{
		if ( !raw_ptr ) raw_ptr = new Ty{ val };
		else *raw_ptr = val;
	}

	void reset( Ty&& val )
	{
		if ( !raw_ptr ) raw_ptr = new Ty{ std::move( val ) };
		else *raw_ptr = std::move_if_noexcept( val );
	}

	void release()
	{
		delete raw_ptr;
		raw_ptr = nullptr;
	}

	void swap( value_pointer& right )
	{
		if ( !raw_ptr ) throw std::exception{ "value_pointer< Ty >::swap(): raw_ptr was null pointer." };
		if ( !right.raw_ptr ) throw std::exception{ "value_pointer< Ty >::swap(): invalid argument( right was null pointer. )" };

		auto temp = std::move_if_noexcept( *right.raw_ptr );
		*right.raw_ptr = std::move_if_noexcept( *raw_ptr );
		*raw_ptr = std::move_if_noexcept( temp );
	}

	// operators
	Ty& operator*() { return *raw_ptr; }
	const Ty& operator*() const { return *raw_ptr; }
	Ty* operator->() { return raw_ptr; }
	const Ty* operator->() const { return raw_ptr; }

	// special member functions
	value_pointer( const Ty& val ) : raw_ptr{ new Ty{ val } } {}
	value_pointer( Ty&& val ) : raw_ptr{ new Ty{ std::move( val ) } } {}
	value_pointer( const value_pointer& other ) : raw_ptr{ new Ty{ *other } } {}
	value_pointer& operator=( const value_pointer& other )
	{
		if ( this != &other ) reset( *other );
		return *this;
	}
	value_pointer( value_pointer&& other ) noexcept : raw_ptr{ new Ty{ std::move( *other ) } } {}
	value_pointer& operator=( value_pointer&& other ) noexcept
	{
		if ( this != &other ) reset( std::move( *other ) );
		return *this;
	}
	~value_pointer() { if ( raw_ptr ) delete raw_ptr; }

private:
	Ty* raw_ptr;
};

template < typename Ty >
struct Vptr : value_pointer< Ty > {};

// =====================================================================
// *********************************************************************
// new type trait ( value_ptr )
// just retyped the pointer's name.
// *********************************************************************
// =====================================================================

namespace woon2
{
	namespace detail
	{
		template < typename T >
		struct is_value_pointer_impl : std::false_type {};

		template < typename T >
		struct is_value_pointer_impl< value_pointer< T > > : std::true_type {};

		template < typename T >
		std::true_type is_value_pointer_soft_impl( value_pointer< T >* );
		std::false_type is_value_pointer_soft_impl( ... );
	}

	template < typename T >
	using is_value_pointer = detail::is_value_pointer_impl< detail::remove_cvr_t< T > >;

	template < typename T >
	constexpr bool is_value_pointer_v = is_value_pointer< T >::value;

	template < typename T >
	using is_value_pointer_soft = decltype( detail::is_value_pointer_soft_impl( std::declval< detail::remove_cvr_t< T >* >() ) );

	template < typename T >
	constexpr bool is_value_pointer_soft_v = is_value_pointer_soft< T >::value;
}

// =====================================================================

struct INT
{
	INT( int impl = 0 ) : impl{ impl } { std::cout << "INT constructor called\n"; }
	~INT() { std::cout << "INT destructor called\n"; }
	INT( const INT& other ) : impl{ other.impl } { std::cout << "INT copy constructor called\n"; }
	INT& operator=( const INT& other )
	{
		if ( this != &other )
		{
			std::cout << "INT copy allocator called\n";
			impl = other.impl;
		}

		return *this;
	}
	INT( INT&& other ) noexcept : impl{ other.impl } { std::cout << "INT move constructor called\n"; }
	INT& operator=( INT&& other ) noexcept
	{
		if ( this != &other )
		{
			std::cout << "INT move allocator called\n";
			impl = other.impl;
		}

		return *this;
	}
	operator int() { return impl; }

	int impl;
};

int main()
{
	// test type traits ==============================================
	static_assert( woon2::is_pointable_v< value_pointer< int > >, "is_pointable trait doesn't work." );
	static_assert( woon2::is_value_pointer_v< value_pointer< int > >, "is_value_pointer trait doesn't work." );
	static_assert( woon2::is_value_pointer_soft_v< Vptr< int > >, "is_value_pointer_soft trait doesn't work." );
	static_assert( !woon2::has_get_deleter_v< value_pointer< int > >, "has_get_deleter trait doesn't work." );
	static_assert( woon2::has_release_v< value_pointer< int > >, "has_release trait doesn't work." );
	static_assert( woon2::has_swap_v< value_pointer< int > >, "has_swap trait doesn't work." );
	static_assert( woon2::has_reset_v< value_pointer< int > >, "has_reset trait doesn't work." );
	// ===============================================================

	std::cout << "====================================================\n";
	std::cout << "value_pointer test\n";
	std::cout << "====================================================\n\n\n";

	try {
		std::cout << "construct a ========================================\n";
		value_pointer< INT > a{ 3 };
		std::cout << "====================================================\n\n\n";

		std::cout << "construct b ========================================\n";
		value_pointer< INT > b{ 4 };
		std::cout << "====================================================\n\n\n";

		std::cout << "print values =======================================\n";
		std::cout << "a: " << *a << ", b: " << *b << '\n';
		std::cout << "====================================================\n\n\n";

		std::cout << "swap a, b ==========================================\n";
		a.swap( b );
		std::cout << "====================================================\n\n\n";

		std::cout << "print values =======================================\n";
		std::cout << "a: " << *a << ", b: " << *b << '\n';
		std::cout << "====================================================\n\n\n";

		std::cout << "release a ==========================================\n";
		a.release();
		std::cout << "====================================================\n\n\n";

		std::cout << "reset a with b =====================================\n";
		a.reset( *b );
		std::cout << "====================================================\n\n\n";

		std::cout << "print values =======================================\n";
		std::cout << "a: " << *a << ", b: " << *b << '\n';
		std::cout << "====================================================\n\n\n";

		std::cout << "must call destructors ==============================\n";
	}
	catch ( const std::exception& e )
	{
		std::cout << e.what();
		exit( -1 );
	}
}