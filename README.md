# Smart Pointer Type Trait🎈  
<a href="https://isocpp.org/"><img src="https://img.shields.io/badge/Launguage-C%2B%2B-green"/></a>  <a href="https://en.wikipedia.org/wiki/C%2B%2B17"><img src="https://img.shields.io/badge/C%2B%2B-14%2C%2017-blue"/></a>  <a href="https://github.com/MyeongWoonJang/cpp-smart-pointer-type-trait/releases/tag/release"><img src="https://img.shields.io/badge/build-passing-brightgreen"/></a>

A simple, header-only cpp library implementing smart pointer type traits.  
You can easily compile your code diffrently depending on which pointer-type is used.  
You can detect inheritance too and you won't have trouble with const, volatile qualifiers.  

Type traits are generally used for static assertions and static polymorphism. (C++ 17 if constexpr)  
However, the standard library's type traits do not support smart pointers, and do not detect inheritance.  
This is why smart pointer type traits are made.

# Release Note
Version 2.0 released.
> https://github.com/MyeongWoonJang/cpp-smart-pointer-type-trait/releases/tag/v2.0


If you are looking for old versions, visit [here](https://github.com/MyeongWoonJang/cpp-smart-pointer-type-trait/releases).


# Installation
All you need to to is copy over the smart_pointer_type_trait.hpp header file.  
Include it in your solution, and you are good to go.


# Guide
All evaluations occurs after removing const, volatile, reference.  
Implementation is in namespace woon2.  
```c++
#include "smart_pointer_type_trait.hpp"		// include the header file.

// ...

	woon2::is_smart_ptr_v<T>;		// usage guide 1

	using namespace woon2;			// usage guide 2
	is_smart_ptr_v<T>;

// ...
```

## Hard Type Traits
* is_pointable<T>
```c++
template < typename T >
using is_pointable = detail::is_pointable_impl< detail::remove_cvr_t< T > >;
```
```is_ptr<T>::value```/```is_ptr_v<T>``` is ```true``` if ```T``` has ```operator*``` and ```operator->```.  
detects all pointable classes such as ```std::unique_ptr<T>```, ```std::shared_ptr<T>```, ```T*```.

* is_smart_ptr<T>
```c++
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
```
```is_smart_ptr<T>::value```/```is_smart_ptr_v<T>``` is ```true``` if ```T``` is ```std::shared_ptr``` or ```std::unique_ptr```.


* is_shared_ptr<T>
```c++
template < typename T >
using is_shared_ptr = detail::is_shared_ptr_impl< detail::remove_cvr_t< T > >;
```
```is_shared_ptr<T>::value```/```is_shared_ptr_v<T>``` is ```true``` if ```T``` is ```std::shared_ptr```.

* is_unique_ptr<T>
```c++
template < typename T >
using is_unique_ptr = detail::is_unique_ptr_impl< detail::remove_cvr_t< T > >;
```
```is_unique_ptr<T>::value```/```is_unique_ptr_v<T>``` is ```true``` if ```T``` is ```std::unique_ptr```.


## Soft Type Traits
* is_smart_ptr_soft<T>
```c++
template < typename T >
using is_smart_ptr_soft = decltype( detail::is_smart_ptr_soft_impl( std::declval< detail::remove_cvr_t< T >* >() ) );
```
```is_smart_ptr_soft<T>::value```/```is_smart_ptr_soft_v<T>``` is ```true``` if ```T``` is derived from ```std::shared_ptr``` or ```std::unique_ptr```.

* is_shared_ptr_soft<T>
```c++
template < typename T >
using is_shared_ptr_soft = decltype( detail::is_shared_ptr_soft_impl( std::declval< detail::remove_cvr_t< T >* >() ) );
```
```is_shared_ptr_soft<T>::value```/```is_shared_ptr_soft_v<T>``` is ```true``` if ```T``` is derived from ```std::shared_ptr```.

* is_unique_ptr_soft<T>
```c++
template < typename T >
using is_unique_ptr_soft = decltype( detail::is_unique_ptr_soft_impl( std::declval< detail::remove_cvr_t< T >* >() ) );
```
```is_unique_ptr_soft<T>::value```/```is_unique_ptr_soft_v<T>``` is ```true``` if ```T``` is derived from ```std::unique_ptr```.

# Example
![Smart Pointer Type Trait1](https://user-images.githubusercontent.com/73771162/147122310-58b2a730-2e5e-4f7f-a734-0f690dedfdea.PNG)
![Smart Pointer Type Trait2](https://user-images.githubusercontent.com/73771162/147122315-176ae870-1687-4681-840e-a6493a2bb584.PNG)
  
# Contributing
  we are welcoming your contributions!😊


# Credit
  ### **Woon2**
  
# Reference
[How to write a type trait is container or is vector](https://stackoverflow.com/questions/12042824/how-to-write-a-type-trait-is-container-or-is-vector) - stackoverflow
  
[Concept for Smart Pointers](https://stackoverflow.com/questions/65752626/concept-for-smart-pointers) - stackoverflow

[씹어먹는 C++ - <16 - 3. 타입을 알려주는 키워드 decltype 와 친구 std::declval>](https://modoocode.com/294) - blog

[std::conditional](https://en.cppreference.com/w/cpp/types/conditional) - cppreference
  
[SFINAE](https://en.cppreference.com/w/cpp/language/sfinae) - cppreference
