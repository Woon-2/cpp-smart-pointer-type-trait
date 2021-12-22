// ==========================================================================
// Implementation: easily changable pointer for code decoupling
// Just retype the pointer type in line 93.
// ==========================================================================

#include <iostream>
#include "smart_pointer_type_trait.hpp"

template < typename Ptr >
decltype( auto ) get_raw_pointer( Ptr&& ptr )
{
	if constexpr ( std::is_pointer_v< std::decay_t< Ptr > > ) return std::forward< Ptr >( ptr );
	else return ptr.operator->();
}

// a wrapper class of a pointer, for safe compilation.
// if the wrapped class doesn't have required method, it does nothing.
// you can get the original pointer type by using unwrapped ( pointer_impl< Ty, Ptr >::unwrapped ).
template < typename Ty, typename Ptr >
class pointer_impl
{
public:
	using unwrapped = Ptr;

	void reset( Ty* target = nullptr )
	{
		static_assert( woon2::has_reset_v< Ptr > || std::is_pointer_v< Ptr >,
			"pointer_impl< Ty, Ptr >::reset(): Ptr doesn't have reset()." );

		if constexpr ( woon2::has_reset_v< Ptr > ) impl.reset( target );
		else
		{
			delete impl;
			impl = target;
		}
	}

	auto release()
	{
		static_assert( woon2::has_release_v< Ptr > || std::is_pointer_v< Ptr > || woon2::is_shared_ptr_soft_v< Ptr >,
			"pointer_impl< Ty, Ptr >::release(): Ptr doesn't have release()." );

		if constexpr ( woon2::has_release_v< Ptr > ) return impl.release();
		else
		{
			auto ret = impl;
			reset();
			return ret;
		}
	}

	decltype( auto ) get_deleter() noexcept
	{
		if constexpr ( woon2::has_get_deleter_v< Ptr > ) return impl.get_deleter();
		else return nullptr;
	}

	decltype( auto ) get_deleter() const noexcept
	{
		if constexpr ( woon2::has_get_deleter_v< Ptr > ) return impl.get_deleter();
		else return nullptr;
	}

	Ty* get() noexcept
	{
		return operator->();
	}

	const Ty* get() const noexcept
	{
		return operator->();
	}

	Ty& operator*() noexcept { return *impl; }
	const Ty& operator*() const noexcept { return *impl; }
	Ty* operator->() noexcept { return get_raw_pointer( impl ); }
	const Ty* operator->() const noexcept { return get_raw_pointer( impl ); }
	operator bool() const noexcept { return static_cast< bool >( get_raw_pointer( impl ) ); }

	// special member functions
	pointer_impl( Ty* impl = nullptr ) : impl{ impl } {}

private:
	Ptr impl;
};

// ======================================================
// ******************************************************
// Decoupling by using keyword.
// Memory allocation policy has the only dependancy on this code.
// ======================================================
template < typename Ty >
using pointer = pointer_impl< Ty, std::unique_ptr< Ty > >;		// can change to diffrent pointer
																// if you add more pointer class,
																// you can also change to the pointer class.
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
	std::cout << "must call constructor ==================\n";
	auto a = make< INT >( 4 );
	std::cout << "========================================\n\n\n";

	std::cout << "get_deleter() call =====================\n";
	std::cout << typeid( a.get_deleter() ).name() << '\n';
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
	if constexpr ( std::is_pointer_v< decltype( a )::unwrapped > )
	{
		std::cout << "cause unwrapped pointer type was a raw pointer, called release().\n";
		a.release();
	}
}