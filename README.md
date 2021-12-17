# SmartPointerTypeTrait
C++ Type Traits for Smart Pointer


## is_a_pointer
* is_smart_pointer<T>
```c++
template < typename T > struct is_smart_ptr : is_smart_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};
```
```is_smart_ptr<T>::value```/```is_smart_ptr_v<T>``` is ```true``` if ```T``` is ```std::shared_ptr``` or ```std::unique_ptr```.

Evaluation occurs after removing const, volatile, reference.
* is_shared_ptr<T>
```c++
template < typename T > struct is_shared_ptr : is_shared_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};
```
```is_shared_ptr<T>::value```/```is_shared_ptr_v<T>``` is ```true``` if ```T``` is ```std::shared_ptr```.

Evaluation occurs after removing const, volatile, reference.
* is_unique_ptr<T>
```c++
template < typename T > struct is_unique_ptr : is_unique_ptr_impl< std::remove_cv_t< std::remove_reference_t< T > > > {};
```
```is_unique_ptr<T>::value```/```is_unique_ptr_v<T>``` is ```true``` if ```T``` is ```std::unique_ptr```.

Evaluation occurs after removing const, volatile, reference.

## inherit_from_a_pointer
* inherit_from_smart_pointer<T>
```c++
template < typename T >
using inherit_from_smart_ptr = decltype( inherit_from_smart_ptr_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
```
```inherit_from_smart_ptr<T>::value```/```inherit_from_smart_ptr_v<T>``` is ```true``` if ```T``` is based on ```std::shared_ptr``` or ```std::unique_ptr```.

Evaluation occurs after removing const, volatile, reference.

* inherit_from_shared_pointer<T>
```c++
template < typename T >
using inherit_from_shared_ptr = decltype( inherit_from_shared_ptr_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
```
```inherit_from_shared_ptr<T>::value```/```inherit_from_shared_ptr_v<T>``` is ```true``` if ```T``` is based on ```std::shared_ptr```.

Evaluation occurs after removing const, volatile, reference.

* inherit_from_unique_pointer<T>
```c++
template < typename T >
using inherit_from_unique_ptr = decltype( inherit_from_unique_ptr_impl( std::declval< std::remove_cv_t< std::remove_reference_t< T > >* >() ) );
```
```inherit_from_unique_ptr<T>::value```/```inherit_from_unique_ptr_v<T>``` is ```true``` if ```T``` is based on ```std::unique_ptr```.

Evaluation occurs after removing const, volatile, reference.

# example
![Smart Pointer Type Trait](https://user-images.githubusercontent.com/73771162/146510184-589f642d-3451-4a41-976b-d09cb144b2c9.PNG)
  
## reference

[Concept for Smart Pointers](https://stackoverflow.com/questions/65752626/concept-for-smart-pointers)

[씹어먹는 C++ - <16 - 3. 타입을 알려주는 키워드 decltype 와 친구 std::declval>](https://modoocode.com/294)

[std::conditional](https://en.cppreference.com/w/cpp/types/conditional)
