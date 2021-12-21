#include <iostream>
#include "smart_pointer_type_trait.hpp"

template < typename Ptr >
auto get_raw_pointer( Ptr& ptr )
{
	if constexpr ( std::is_pointer_v< Ptr > ) return ptr;
	else return ptr.operator->();
}

template < typename Ty, typename Ptr >
class pointer_impl
{
public:
	void reset( Ty* target = nullptr )
	{
		if constexpr ( is_smart_ptr_v< Ptr > ) impl.reset( target );
		else
		{
			if ( impl ) delete impl;
			impl = target;
		}
	}

	void release()
	{
		if constexpr ( std::is_pointer_v< Ptr > ) delete impl;
		impl = nullptr;
	}

	Ty* get() noexcept { return operator->(); }
	const Ty* get() const noexcept { return operator->(); }

	decltype( auto ) get_deleter() noexcept
	{
		static_assert( is_unique_ptr_v< Ptr >, "pointer_impl< Ty, Ptr >::get_deleter(): Ptr was not a unique pointer." );
		return impl.get_deleter();
	}
	decltype( auto ) get_deleter() const noexcept
	{
		static_assert( is_unique_ptr_v< Ptr >, "pointer_impl< Ty, Ptr >::get_deleter(): Ptr was not a unique pointer." );
		return impl.get_deleter();
	}

	Ty& operator*() noexcept { return *impl; }
	const Ty& operator*() const noexcept { return *impl; }
	Ty* operator->() noexcept { return get_raw_pointerr( impl ); }
	const Ty* operator->() const noexcept { return get_raw_pointer( impl ); }
	operator bool() const noexcept { return static_cast< bool >( get_raw_pointer( impl ) ); }

	// special member functions
	pointer_impl( Ty* impl = nullptr ) : impl{ impl } {}
	~pointer_impl() { release(); }

private:
	Ptr impl;
};

// ======================================================
// ******************************************************
// Decoupling by using keyword.
// memory allocation policy has only-one dependancy, this code.
// ======================================================
template < typename Ty >
using pointer = pointer_impl< Ty, std::unique_ptr< Ty > >;		// can change to diffrent pointer
																// If you add more pointer object, ( and similar type trait )
																// can also change to the pointer object.
// ******************************************************
// ======================================================

template < typename Ty, typename ... Args >
auto make( Args&& ... args )
{
	return pointer< Ty >{ new Ty{ std::forward< Args >( args )... } };
}

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
	INT( INT&& other ) : impl{ other.impl } { std::cout << "INT move constructor called\n"; }
	INT& operator=( INT&& other )
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
	std::cout << "must call constructor ==================\n";
	auto a = make< INT >( 4 );
	std::cout << "========================================\n\n\n";

	std::cout << "get_deleter() call =====================\n";
	std::cout << typeid( a.get_deleter() ).name() << '\n';			// only can executed with unique pointer
	std::cout << "========================================\n\n\n";
	
	std::cout << "print value ============================\n";
	std::cout << "value: " << *a << '\n';
	std::cout << "========================================\n\n\n";
	
	std::cout << "must call constructor and destructor ===\n";
	a.reset( new INT{ 5 } );
	std::cout << "========================================\n\n\n";

	std::cout << "print value ============================\n";
	std::cout << "value: " << *a << '\n';
	std::cout << "========================================\n\n\n";

	std::cout << "must call destructor ===================\n";
}