#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <utility>

class AnyPoolObject {
public:
    virtual ~AnyPoolObject() = default;
};

template <typename T>
class PoolObject : public AnyPoolObject {
public:
    T value;
};

class ThreadSafeMemoryPool {
public:
    template <typename T>
    std::shared_ptr<T> allocate() {
        static_assert(std::is_base_of<AnyPoolObject, PoolObject<T>>::value,
                      "T must be derived from PoolObject<T>");

        std::unique_lock<std::mutex> lock(mutex_);
        std::type_index typeIndex(typeid(PoolObject<T>));

        auto it = free_indices_.find(typeIndex);
        if (it != free_indices_.end() && !it->second.empty()) {
            size_t index = it->second.top();
            it->second.pop();

            auto& poolObjectPtr = pool_[index];
            return std::shared_ptr<T>(&static_cast<PoolObject<T>*>(poolObjectPtr.get())->value,
                                      [this, index](T* ptr) {
                                          std::unique_lock<std::mutex> lock(mutex_);
                                          free_indices_[typeIndex].push(index);
                                      });
        }

        if (pool_.size() >= max_capacity_) {
            throw std::runtime_error("Memory pool exhausted");
        }

        size_t index = pool_.size();
        pool_.emplace_back(std::make_unique<PoolObject<T>>());

        auto& poolObjectPtr = pool_[index];
        return std::shared_ptr<T>(&static_cast<PoolObject<T>*>(poolObjectPtr.get())->value,
                                  [this, index](T* ptr) {
                                      std::unique_lock<std::mutex> lock(mutex_);
                                      free_indices_[typeIndex].push(index);
                                  });
    }

private:
    static constexpr size_t max_capacity_ = 100;
    std::vector<std::unique_ptr<AnyPoolObject>> pool_;
    std::unordered_map<std::type_index, std::stack<size_t>> free_indices_;
    std::mutex mutex_;
};

int main() {
    ThreadSafeMemoryPool memoryPool;

    auto value1 = memoryPool.allocate<int>();
    auto value2 = memoryPool.allocate<double>();

    *value1 = 42;
    *value2 = 3.14;

    std::cout << "Value 1: " << *value1 << std::endl;
    std::cout << "Value 2: " << *value2 << std::endl;

    return 0;
}
