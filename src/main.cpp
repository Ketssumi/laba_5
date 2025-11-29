#include <iostream>
#include <memory_resource>
#include "my_memory_resource.hpp"
#include "queue.h"

struct Student {
    int id;
    char name[50];
    int labs_count;
};

int main() {
    std::cout << "=== Custom Memory Resource and Queue Demo ===\n\n";
    
    std::cout << "Example 1: Queue with int values\n";
    {
        MyMemoryResource resource(1024);
        Queue<int> q(&resource);
        
        q.push(10);
        q.push(20);
        q.push(30);
        
        std::cout << "Queue contents: ";
        for (int value : q)
            std::cout << value << " ";
        std::cout << "\n";
        
        std::cout << "Pop: " << q.pop() << "\n";
        std::cout << "Front: " << q.front() << "\n";
        std::cout << "Size: " << q.size() << "\n\n";
    }
    
    std::cout << "Example 2: Queue with Student struct\n";
    {
        MyMemoryResource resource(2048);
        Queue<Student> q(&resource);
        
        q.push({1, "Alice", 5});
        q.push({2, "Bob", 3});
        q.push({3, "Charlie", 7});
        
        std::cout << "Queue contents:\n";
        for (const auto &s : q)
            std::cout << "  Student #" << s.id << " " << s.name 
                      << " (labs: " << s.labs_count << ")\n";
        
        std::cout << "Front: " << q.front().name << "\n";
        std::cout << "Size: " << q.size() << "\n\n";
    }
    
    std::cout << "=== Demo Completed ===\n";
    return 0;
}
