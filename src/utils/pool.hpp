#ifndef POOL_HPP
#define POOL_HPP

#include <stdexcept>
#include <vector>

class IPool {
 public:
  virtual ~IPool() = default;
};

inline std::vector<IPool*> pools;

template <typename TComponent>
class Pool : public IPool {
 private:
  std::vector<TComponent> data;

 public:
  Pool(size_t size = 1000) { data.resize(size); }

  virtual ~Pool() = default;

  bool isEmpty() const { return data.empty(); }

  int getSize() const { return data.size(); }

  void resize(int newSize) { data.resize(newSize); }

  void clear() { data.clear(); }

  void add(const TComponent& object) { data.push_back(object); }

  void set(unsigned int index, const TComponent& object) {
    if (index < data.size()) {
      data[index] = object;
    }
  }

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