# Smart Pointer Type Trait🎈  
A simple, header-only cpp library implementing smart pointer type traits.  
You can easily compile your code diffrently depending on which pointer-type is used.  
You can find inheritance and you won't have trouble with const, volatile keywords.  

Type traits are generally used for static assertions and static polymorphism. (C++ 17 if constexpr)  
However, the standard library's type traits do not support smart pointers, and do not support inheritance.  
This is why smart pointer type traits made.

# Release Note
version 1.0 released.
> https://github.com/MyeongWoonJang/cpp-smart-pointer-type-trait/releases/tag/release


# Installation
All you need to to is copy over the smart_pointer_type_trait.hpp header file.  
include it in your solution, and you are good to go.


# Guide
All evaluations occurs after removing const, volatile, reference.

## Hard Type Traits
* is_ptr<T>
```c++
template < typename T >
struct is_ptr : is_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};
```
```is_ptr<T>::value```/```is_ptr_v<T>``` is ```true``` if ```T``` is ```std::shared_ptr``` or ```std::unique_ptr``` or a raw pointer.

* is_smart_ptr<T>
```c++
template < typename T >
struct is_smart_ptr : is_smart_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};
```
```is_smart_ptr<T>::value```/```is_smart_ptr_v<T>``` is ```true``` if ```T``` is ```std::shared_ptr``` or ```std::unique_ptr```.


* is_shared_ptr<T>
```c++
template < typename T >
struct is_shared_ptr : is_shared_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};
```
```is_shared_ptr<T>::value```/```is_shared_ptr_v<T>``` is ```true``` if ```T``` is ```std::shared_ptr```.

* is_unique_ptr<T>
```c++
template < typename T >
struct is_unique_ptr : is_unique_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};
```
```is_unique_ptr<T>::value```/```is_unique_ptr_v<T>``` is ```true``` if ```T``` is ```std::unique_ptr```.


## Soft Type Traits
* is_ptr_soft<T>
```c++
template < typename T >
using is_ptr_soft = decltype( is_ptr_soft_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
```
```is_ptr_soft<T>::value```/```is_ptr_soft_v<T>``` is ```true``` if ```T``` is derived from ```std::shared_ptr``` or ```std::unique_ptr``` or is a raw pointer.
  
* is_smart_ptr_soft<T>
```c++
template < typename T >
using is_smart_ptr_soft = decltype( is_smart_ptr_soft_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
```
```is_smart_ptr_soft<T>::value```/```is_smart_ptr_soft_v<T>``` is ```true``` if ```T``` is derived from ```std::shared_ptr``` or ```std::unique_ptr```.

* is_shared_ptr_soft<T>
```c++
template < typename T >
using is_shared_ptr_soft = decltype( is_shared_ptr_soft_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
```
```is_shared_ptr_soft<T>::value```/```is_shared_ptr_soft_v<T>``` is ```true``` if ```T``` is derived from ```std::shared_ptr```.

* is_unique_ptr_soft<T>
```c++
template < typename T >
using is_unique_ptr_soft = decltype( is_unique_ptr_soft_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
```
```is_unique_ptr_soft<T>::value```/```is_unique_ptr_soft_v<T>``` is ```true``` if ```T``` is derived from ```std::unique_ptr```.

# Example
![Smart Pointer Type Trait1](https://user-images.githubusercontent.com/73771162/146930931-ff47a89d-86ab-402c-a558-fccd9670edc3.PNG)
![Smart Pointer Type Trait2](https://user-images.githubusercontent.com/73771162/146931121-43c0be51-8a52-4f49-a87f-b00562bacd1f.PNG)

  
# Reference

[Concept for Smart Pointers](https://stackoverflow.com/questions/65752626/concept-for-smart-pointers)

[씹어먹는 C++ - <16 - 3. 타입을 알려주는 키워드 decltype 와 친구 std::declval>](https://modoocode.com/294)

[std::conditional](https://en.cppreference.com/w/cpp/types/conditional)
