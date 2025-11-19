#pragma once

#include <cstddef>
#include <cstdint>
#include <list>
#include <memory_resource>

class memory_pool_resourse: public std::pmr::memory_resource {
public:
    explicit memory_pool_resourse(std::size_t block_size = 64 * 1024);
    ~memory_pool_resourse() override;

protected:
    void *do_allocate(std::size_t count_bytes, std::size_t align) override;
    void do_deallocate(void *p, std::size_t count_bytes, std::size_t align) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

public:
    struct Block {
        char *begin;
        char *end;
        char *cur;
    };

    std::size_t block_size;
    std::list<Block> blocks;
    void allocate_block(std::size_t count_bytes);
    static std::uintptr_t align_up(std::uintptr_t adr, std::size_t align) noexcept;
};