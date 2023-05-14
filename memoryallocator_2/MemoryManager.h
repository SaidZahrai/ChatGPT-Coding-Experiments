#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>

struct SharedMemory {
    std::vector<char> buffer;  // Buffer to hold the shared memory
    size_t buffer_size;        // Total size of the buffer
    size_t buffer_used;        // Amount of used memory in the buffer

    SharedMemory(size_t size) : buffer(size), buffer_size(size), buffer_used(0) {}
};

// struct VariableMetadata {
//     std::string name;
//     std::type_index type;
//     size_t size;
//     size_t offset;
//     std::shared_timed_mutex access_mutex;
// };

struct VariableMetadata {
    std::string name;
    std::type_index type;
    size_t size;
    size_t offset;
    std::shared_timed_mutex access_mutex;

    // Copy constructor
    VariableMetadata(const VariableMetadata& other)
        : name(other.name),
          type(other.type),
          size(other.size),
          offset(other.offset),
          access_mutex() {} // Note: access_mutex is not copied

    // Regular constructor
    VariableMetadata(std::string name, std::type_index type, size_t size, size_t offset)
        : name(std::move(name)), type(type), size(size), offset(offset) {}
};


class MemoryManager {
private:
    std::shared_ptr<SharedMemory> shared_memory;
//    std::unordered_map<std::string, VariableMetadata> metadata_map;
    std::unordered_map<std::string, std::shared_ptr<VariableMetadata>> metadata_map;
    std::mutex allocation_mutex;
    std::mutex deallocation_mutex;

public:
    MemoryManager(size_t buffer_size) : shared_memory(std::make_shared<SharedMemory>(buffer_size)) {}

    template <typename T>
    std::tuple<void*, std::string, std::type_index, int> allocate(const std::string& name);
    //  {
    //     // ...
    // }

    template <typename T>
    bool deallocate(const std::string& name);
    //  {
    //     // ...
    // }

    template <typename T>
    std::shared_ptr<T> access(const std::string& name, std::shared_lock<std::shared_timed_mutex>& lock);
    //  {
    //     // ...
    // }
};

// template <typename T>
// std::tuple<void*, std::string, std::type_index, int> MemoryManager::allocate(const std::string& name) {
//     std::unique_lock<std::mutex> alloc_lock(allocation_mutex);

//     std::type_index type = std::type_index(typeid(T));
//     size_t size = sizeof(T);

//     if (metadata_map.find(name) != metadata_map.end()) {
//         VariableMetadata& metadata = metadata_map[name];
//         if (metadata.type == type) {
//             return {&shared_memory->buffer[metadata.offset], metadata.name, metadata.type, 1};
//         }
//     }

//     if (shared_memory->buffer_used + size > shared_memory->buffer_size) {
//         return {nullptr, "", std::type_index(typeid(void)), -1};
//     }

//     VariableMetadata metadata{name, type, size, shared_memory->buffer_used};
//     shared_memory->buffer_used += size;
//     metadata_map[name] = metadata;

//     return {&shared_memory->buffer[metadata.offset], metadata.name, metadata.type, 0};
// }

// template <typename T>
// std::tuple<void*, std::string, std::type_index, int> MemoryManager::allocate(const std::string& name) {
//     std::unique_lock<std::mutex> alloc_lock(allocation_mutex);

//     std::type_index type = std::type_index(typeid(T));
//     size_t size = sizeof(T);

//     if (metadata_map.find(name) != metadata_map.end()) {
//         VariableMetadata& metadata = metadata_map[name];
//         if (metadata.type == type) {
//             return {&shared_memory->buffer[metadata.offset], metadata.name, metadata.type, 1};
//         }
//     }

//     if (shared_memory->buffer_used + size > shared_memory->buffer_size) {
//         return {nullptr, "", std::type_index(typeid(void)), -1};
//     }

//     size_t offset = shared_memory->buffer_used;
//     shared_memory->buffer_used += size;
//     metadata_map.emplace(name, VariableMetadata{name, type, size, offset});

//     return {&shared_memory->buffer[offset], name, type, 0};
// }

// template <typename T>
// bool MemoryManager::deallocate(const std::string& name) {
//     std::unique_lock<std::mutex> dealloc_lock(deallocation_mutex);

//     auto iter = metadata_map.find(name);
//     if (iter == metadata_map.end()) {
//         return false;
//     }

//     VariableMetadata& metadata = iter->second;
//     if (metadata.type != std::type_index(typeid(T))) {
//         return false;
//     }

//     size_t size = metadata.size;
//     size_t offset = metadata.offset;

//     // Shift the memory after the deallocated variable
//     std::copy(shared_memory->buffer.begin() + offset + size,
//               shared_memory->buffer.begin() + shared_memory->buffer_used,
//               shared_memory->buffer.begin() + offset);

//     // Update metadata
//     shared_memory->buffer_used -= size;
//     for (auto& pair : metadata_map) {
//         VariableMetadata& current_metadata = pair.second;
//         if (current_metadata.offset > offset) {
//             current_metadata.offset -= size;
//         }
//     }

//     // Remove the deallocated variable from the metadata map
//     metadata_map.erase(iter);

//     return true;
// }

// template <typename T>
// std::shared_ptr<T> MemoryManager::access(const std::string& name, std::shared_lock<std::shared_timed_mutex>& lock) {
//     auto iter = metadata_map.find(name);
//     if (iter == metadata_map.end()) {
//         throw std::runtime_error("Variable not found: " + name);
//     }

//     VariableMetadata& metadata = iter->second;

//     if (metadata.type != std::type_index(typeid(T))) {
//         throw std::runtime_error("Variable type mismatch for: " + name);
//     }

//     lock = std::shared_lock<std::shared_timed_mutex>(metadata.access_mutex);
//     return std::shared_ptr<T>(reinterpret_cast<T*>(&shared_memory->buffer[metadata.offset]),
//                               [&, lock = std::move(lock)](T*) mutable { lock.unlock(); });
// }


template <typename T>
std::tuple<void*, std::string, std::type_index, int> MemoryManager::allocate(const std::string& name) {
    std::unique_lock<std::mutex> alloc_lock(allocation_mutex);

    std::type_index type = std::type_index(typeid(T));
    size_t size = sizeof(T);

    if (metadata_map.find(name) != metadata_map.end()) {
        std::shared_ptr<VariableMetadata> metadata = metadata_map[name];
        if (metadata->type == type) {
            return {&shared_memory->buffer[metadata->offset], metadata->name, metadata->type, 1};
        }
    }

    if (shared_memory->buffer_used + size > shared_memory->buffer_size) {
        return {nullptr, "", std::type_index(typeid(void)), -1};
    }

    size_t offset = shared_memory->buffer_used;
    shared_memory->buffer_used += size;
    std::shared_ptr<VariableMetadata> metadata = std::make_shared<VariableMetadata>(name, type, size, offset);
    metadata_map[name] = metadata;

    return {&shared_memory->buffer[offset], name, type, 0};
}

template <typename T>
bool MemoryManager::deallocate(const std::string& name) {
    std::unique_lock<std::mutex> dealloc_lock(deallocation_mutex);

    auto iter = metadata_map.find(name);
    if (iter == metadata_map.end()) {
        return false;
    }

    std::shared_ptr<VariableMetadata> metadata = iter->second;
    if (metadata->type != std::type_index(typeid(T))) {
        return false;
    }

    size_t size = metadata->size;
    size_t offset = metadata->offset;

    // Shift the memory after the deallocated variable
    std::copy(shared_memory->buffer.begin() + offset + size,
              shared_memory->buffer.begin() + shared_memory->buffer_used,
              shared_memory->buffer.begin() + offset);

    // Update metadata
    shared_memory->buffer_used -= size;
    for (auto& pair : metadata_map) {
        std::shared_ptr<VariableMetadata> current_metadata = pair.second;
        if (current_metadata->offset > offset) {
            current_metadata->offset -= size;
        }
    }

    // Remove the deallocated variable from the metadata map
    metadata_map.erase(iter);

    return true;
}

template <typename T>
std::shared_ptr<T> MemoryManager::access(const std::string& name, std::shared_lock<std::shared_timed_mutex>& lock) {
    auto iter = metadata_map.find(name);
    if (iter == metadata_map.end()) {
        throw std::runtime_error("Variable not found: " + name);
    }

    std::shared_ptr<VariableMetadata> metadata = iter->second;

    if (metadata->type != std::type_index(typeid(T))) {
        throw std::runtime_error("Variable type mismatch for: " + name);
    }

    lock = std::shared_lock<std::shared_timed_mutex>(metadata->access_mutex);
    return std::shared_ptr<T>(reinterpret_cast<T*>(&shared_memory->buffer[metadata->offset]),
                              [&, lock = std::move(lock)](T*) mutable { lock.unlock(); });
}
