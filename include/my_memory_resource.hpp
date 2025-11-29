#ifndef MEMORY_RESOURCE_H
#define MEMORY_RESOURCE_H

#include <memory_resource>
#include <vector>
#include <cstddef>

class MyMemoryResource : public std::pmr::memory_resource {
private:
    struct Block {
        void* ptr;
        size_t size;
    };
    
    unsigned char* fixed_memory;
    size_t total_size;
    size_t used_size;
    std::vector<Block> allocated_blocks;
    std::vector<Block> free_blocks;
    
protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
    
public:
    explicit MyMemoryResource(size_t size);
    ~MyMemoryResource() noexcept override;
    
    MyMemoryResource(const MyMemoryResource&) = delete;
    MyMemoryResource& operator=(const MyMemoryResource&) = delete;
};

#endif
