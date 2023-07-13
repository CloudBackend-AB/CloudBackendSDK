#ifndef cbe__util__Optional_h__
#define cbe__util__Optional_h__

#include <new>          // placement new
#include <stdexcept>    // std::exception
#include <type_traits>  // std::aligned_storage
#include <utility>      // std::forward, std::move

namespace cbe {
  namespace util {

/**
 * @brief
 * Class template Optional manages an optional contained value - i.e., a
 * value that is either present or absent.
 *
 * @tparam  T Type of the contained value
 */
template <typename T>
class Optional {
  bool hasValue_ = false;
  // Byte array allocating the memory for an instance of T
  using RawMemory = typename std::aligned_storage<sizeof(T), alignof(T)>::type;
  RawMemory payLoad[1]; // Conceptually: unsigned char payLoad[sizeof(T)];
public:
  /**
   * @brief
   * Creates an empty Optional, i.e., no value present.
   */
  Optional() = default;

  /**
   * @brief
   * Copy value ctor.
   */
  Optional(const T& value) : hasValue_{true} {
    // Copy construct the instance of T in the raw memory of payLoad with help
    // placement new
    new (payLoad) T{value};
  }
  /**
   * @brief
   * Move value ctor.
   */
  Optional(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
      : hasValue_{true} {
    // Move construct ditto ...
    new (payLoad) T{std::move(value)};
  }

  /**
   * @brief
   * Copy ctor
   */
  Optional(const Optional& rh) : hasValue_{rh.hasValue_} {
    if (hasValue_) {
      new (payLoad) T{rh.value_cast()}; // Copy construction
    }
  }
  /**
   * @brief
   * Move ctor
   */
  Optional(Optional&& rh) noexcept(std::is_nothrow_move_constructible<T>::value)
      : hasValue_{rh.hasValue_} {
    if (hasValue_) {
      new (payLoad) T{std::move(rh.value_cast())}; // Move construction
    }
  }

  /**
   * @brief
   * Copy assignment operator.
   */
  Optional& operator=(const Optional& rh) {
    Optional tmp{rh}; // Deep copy of rh, implemented in terms of the copy ctor
    swap(tmp /* other */); // Primary purpose of swap: Transfer state from the
                           // deep copied rh into tmp, from tmp to left-hand-side
                           // object
    return *this;
  }
  /**
   * @brief
   * Move assignment operator.
   */
  Optional& operator=(Optional&& rh) noexcept {
    // The primary task is to move rh to this (i.e., left-hand-side).
    // If we swap, we move rh to this, but also move this to rh
    swap(rh /* other */);
    return *this;
  }

  ~Optional() {
    reset(); // Destruct possible present value object
  };

  /**
   * @brief
   * Thrown by value() method when accessing an object that does not contain a
   * value.
   */
  class BadAccess : public std::runtime_error {
    using std::runtime_error::runtime_error; // Inherit base class' ctors
  };

  /**
   * @brief
   * Value access non-const method.
   */
  T& value() {
    if(!hasValue_) {
      throw BadAccess{"Value access of an empty Optional"};
    }
    return value_cast();
  }
  T& operator*() {
    return value();
  }
  T* operator->() {
    return &value();
  }

  /**
   * @brief
   * Value access const method.
   */
  const T& value() const {
    // Implemented in terms of the non-const value() method above.
    // Cast way the const-ness, i.e., const Optional* -> Optional*
    return const_cast<Optional*>(this)->value();
  }
  const T& operator*() const {
    return value();
  }
  const T* operator->() const {
    return &value();
  }

  /**
   * @brief
   * Checks whether a value is present.
   */
  bool hasValue() const noexcept { return hasValue_; }
  /**
   * @brief
   * Checks whether a value is present.
   * @code
  cbe::util::Optional<int> inquireInt(const char* prompt) {
    while (true) {
      std::cout << prompt << ": ";
      std::string answer;
      std::cin >> answer;
      try {
        return std::stoi(answer); // I.e., return cbe::util::Optional<int>{std::stoi(answer)}
      }
      catch (...) {
        // Invalid input
        return {}; // I.e., return cbe::util::Optional<int>{};
      }
    }
  }
  ~~~
  auto n = inquireInt("Please, give an integer value");
  if (n) {
    std::cout << "Thanks for the number" << *n << "!\n";
  } else {
    std::cout << "Not a number!\n";
  }
  ~~~
   * @endcode
   */
  explicit operator bool() const noexcept { return hasValue(); }

  /**
   * @brief
   * Invalidates possible contained value. The contained value will be
   * destructed.
   */
  void reset() {
    if (hasValue_) {
      value_cast().~T(); // Apply the destructor on the value object
      hasValue_ = false;
    }
  }
  /**
   * @brief
   * Swaps the contents with those of other.
   */
  void swap(Optional& other) noexcept {
    if (this != &other) {
      if (!other.hasValue_) {
        if (hasValue_) {
          // Move construct lh into rh
          new (&other) Optional{std::move(value_cast())};
          // Reset this side since other side originally was reset
          reset();
        }
      } else {
        if (!hasValue_) {
          // Move construct rh into lh with hwlp from placement new
          new (this) Optional{std::move(other.value_cast())};
          // Reset other side since this side originally was reset
          other.reset();
        } else {
          std::swap(value_cast(), other.value_cast());
        }
      }
    }
  }

  /**
   * @brief
   * Constructs the contained value in-place.
   * If \c *this already contains a value before the call, the contained value is
   * destroyed by calling its destructor.
   */
  template <typename... ArgTs>
  void emplace(ArgTs&&... args) {
    reset();
    new (payLoad) T(std::forward<ArgTs>(args)...);
    hasValue_ = true;
  }
private:
  inline T& value_cast() noexcept {
    // Cast the memory buffer, RawBuffer*, to value's type, T*, and dereference
    return *reinterpret_cast<T*>(payLoad);
  }
  inline const T& value_cast() const noexcept {
    return const_cast<Optional*>(this)->value_cast();;
  }
}; // class Optional

  } // namespace util
} // namespace cbe


#endif // #ifndef cbe__util__Optional_h__
