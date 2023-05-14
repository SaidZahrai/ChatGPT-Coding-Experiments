#include <vector>
#include <mutex>
#include <memory>

class ThreadSafeMemoryPool {
public:
    ThreadSafeMemoryPool(size_t numSegments, const std::vector<size_t>& segmentSizes, const std::vector<size_t>& segmentCounts)
        : m_numSegments(numSegments), m_segmentSizes(segmentSizes) {
        // Initialize the free lists
        for (size_t i = 0; i < numSegments; ++i) {
            m_freeLists.emplace_back(std::make_unique<std::vector<void*>>());
            m_locks.emplace_back(std::make_unique<std::mutex>());

            // Allocate and initialize memory for each segment
            size_t segmentMemorySize = segmentSizes[i] * segmentCounts[i];
            char* segmentMemory = new char[segmentMemorySize];
            m_memorySegments.push_back(segmentMemory);

            for (size_t j = 0; j < segmentCounts[i]; ++j) {
                m_freeLists[i]->push_back(segmentMemory);
                segmentMemory += segmentSizes[i];
            }
        }
    }

    ~ThreadSafeMemoryPool() {
        for (auto& segmentMemory : m_memorySegments) {
            delete[] segmentMemory;
        }
    }

    // ThreadSafeMemoryPool(size_t numSegments, const std::vector<size_t>& segmentSizes)
    //     : m_numSegments(numSegments), m_segmentSizes(segmentSizes) {
    //     // Initialize the free lists
    //     for (size_t i = 0; i < numSegments; ++i) {
    //         m_freeLists.emplace_back(std::make_unique<std::vector<void*>>());
    //         m_locks.emplace_back(std::make_unique<std::mutex>());
    //     }
    // }

    void* allocate(size_t size) {
        size_t segmentIndex = findSegmentIndex(size);
        if (segmentIndex == SIZE_MAX) {
            // Requested size is not supported by the memory pool
            return nullptr;
        }

        std::lock_guard<std::mutex> lock(*m_locks[segmentIndex]);

        if (m_freeLists[segmentIndex]->empty()) {
            // No free memory blocks available, request more memory or handle this case as needed
            return nullptr;
        }

        void* ptr = m_freeLists[segmentIndex]->back();
        m_freeLists[segmentIndex]->pop_back();
        return ptr;
    }

    void deallocate(void* ptr, size_t size) {
        size_t segmentIndex = findSegmentIndex(size);
        if (segmentIndex == SIZE_MAX) {
            // The given size is not supported by the memory pool, handle this case as needed
            return;
        }

        std::lock_guard<std::mutex> lock(*m_locks[segmentIndex]);
        m_freeLists[segmentIndex]->push_back(ptr);
    }

private:
    size_t findSegmentIndex(size_t size) {
        for (size_t i = 0; i < m_numSegments; ++i) {
            if (size <= m_segmentSizes[i]) {
                return i;
            }
        }
        return SIZE_MAX;
    }

    size_t m_numSegments;
    std::vector<size_t> m_segmentSizes;
    std::vector<char*> m_memorySegments;
    std::vector<std::unique_ptr<std::vector<void*>>> m_freeLists;
    std::vector<std::unique_ptr<std::mutex>> m_locks;
};

#include <iostream>
#include <thread>

// Include the ThreadSafeMemoryPool class definition here

void allocateAndDeallocate(ThreadSafeMemoryPool& memoryPool, size_t size, size_t count) {
    std::vector<void*> allocatedPointers;

    for (size_t i = 0; i < count; ++i) {
        void* ptr = memoryPool.allocate(size);
        if (ptr != nullptr) {
            allocatedPointers.push_back(ptr);
            std::cerr << "Allocated memory of size " << size << std::endl;
        } else {
            std::cerr << "Failed to allocate memory of size " << size << std::endl;
        }
    }

    for (void* ptr : allocatedPointers) {
        memoryPool.deallocate(ptr, size);
    }
}

// int main() {
//     const size_t numSegments = 3;
//     std::vector<size_t> segmentSizes = {32, 64, 128};

//     // Create a thread-safe memory pool with the specified segment sizes
//     ThreadSafeMemoryPool memoryPool(numSegments, segmentSizes);

//     // Launch multiple threads to allocate and deallocate memory
//     std::vector<std::thread> threads;
//     for (size_t i = 0; i < 10; ++i) {
//         size_t size = segmentSizes[i % numSegments];
//         threads.emplace_back(allocateAndDeallocate, std::ref(memoryPool), size, 1000);
//     }

//     // Wait for all threads to complete their work
//     for (std::thread& t : threads) {
//         t.join();
//     }

//     std::cout << "Finished processing all threads." << std::endl;
//     return 0;
// }

int main() {
    const size_t numSegments = 3;
    std::vector<size_t> segmentSizes = {32, 64, 128};
    std::vector<size_t> segmentCounts = {100, 50, 25};

    // Create a thread-safe memory pool with the specified segment sizes and counts
    ThreadSafeMemoryPool memoryPool(numSegments, segmentSizes, segmentCounts);

    // Launch multiple threads to allocate and deallocate memory
    std::vector<std::thread> threads;
    for (size_t i = 0; i < 10; ++i) {
        size_t size = segmentSizes[i % numSegments];
        threads.emplace_back(allocateAndDeallocate, std::ref(memoryPool), size, 1000);
    }

    // Wait for all threads to complete their work
    for (std::thread& t : threads) {
        t.join();
    }

    std::cout << "Finished processing all threads." << std::endl;
    return 0;
}