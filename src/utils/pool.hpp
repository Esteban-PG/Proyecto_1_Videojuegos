#ifndef POOL_HPP
#define POOL_HPP

#include <stdexcept>
#include <vector>

/** @brief Type-erased base for Pool<T> so Registry can store heterogeneous pools. */
class IPool {
 public:
  virtual ~IPool() = default;
};

inline std::vector<IPool*> pools;

/**
 * @brief Contiguous array of components of type TComponent.
 *
 * Indexed by entity ID so component lookup is O(1).
 * Registry resizes the pool automatically when new entities are created.
 *
 * @tparam TComponent Component type stored in this pool.
 */
template <typename TComponent>
class Pool : public IPool {
 private:
  std::vector<TComponent> data;

 public:
  /** @brief Pre-allocate @p size slots. */
  Pool(size_t size = 1000) { data.resize(size); }

  virtual ~Pool() = default;

  /** @brief @return True if the pool contains no elements. */
  bool isEmpty() const { return data.empty(); }
  /** @brief @return Number of allocated slots. */
  int  getSize() const { return data.size(); }
  /** @brief Resize the pool to @p newSize slots. */
  void resize(int newSize) { data.resize(newSize); }
  /** @brief Remove all elements. */
  void clear() { data.clear(); }
  /** @brief Append @p object at the end. */
  void add(const TComponent& object) { data.push_back(object); }

  /**
   * @brief Write @p object at @p index (entity ID).
   * @param index Entity ID
   * @param object Component value to store
   */
  void set(unsigned int index, const TComponent& object) {
    if (index < data.size()) {
      data[index] = object;
    }
  }

  /**
   * @brief @return Reference to component at @p index.
   * @throws std::out_of_range if @p index >= size
   */
  TComponent& get(unsigned int index) {
    if (index < data.size()) {
      return static_cast<TComponent&>(data[index]);
    }
    throw std::out_of_range("Index out of range");
  }

  TComponent& operator[](unsigned int index) {
    return static_cast<TComponent&>(data[index]);
  }
};

#endif  // POOL_HPP