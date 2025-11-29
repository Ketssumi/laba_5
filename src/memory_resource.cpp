#include "my_memory_resource.hpp"
#include <stdexcept>
#include <algorithm>

MyMemoryResource::MyMemoryResource(size_t size) 
    : total_size(size), used_size(0) {
    if (size == 0) {
        throw std::invalid_argument("Memory size must be greater than zero");
    }
    fixed_memory = new unsigned char[size];
}

MyMemoryResource::~MyMemoryResource() noexcept {
    delete[] fixed_memory;
}

void* MyMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    if (bytes == 0) {
        throw std::bad_alloc();
    }
    
    // Try to reuse free block
    for (auto it = free_blocks.begin(); it != free_blocks.end(); ++it) {
        if (it->size >= bytes) {
            void* ptr = it->ptr;
            allocated_blocks.push_back({ptr, bytes});
            free_blocks.erase(it);
            return ptr;
        }
    }
    
    // Allocate from fixed memory
    if (used_size + bytes > total_size) {
        throw std::bad_alloc();
    }
    
    void* ptr = fixed_memory + used_size;
    allocated_blocks.push_back({ptr, bytes});
    used_size += bytes;
    return ptr;
}

void MyMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    if (p == nullptr) return;
    
    auto it = std::find_if(allocated_blocks.begin(), allocated_blocks.end(),
                          [p](const Block& b) { return b.ptr == p; });
    
    if (it != allocated_blocks.end()) {
        free_blocks.push_back({it->ptr, it->size});
        allocated_blocks.erase(it);
    }
}

bool MyMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}
