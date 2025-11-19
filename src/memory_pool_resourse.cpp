#include <algorithm>
#include <cstdlib>
#include <new>
#include "../includes/memory_pool_resourse.hpp"

memory_pool_resourse::memory_pool_resourse(std::size_t block_size): block_size{std::max<std::size_t>(block_size, 64)} {}

memory_pool_resourse::~memory_pool_resourse() {
    for (auto &i: blocks) {
        std::free(i.begin);
    }
}

void *memory_pool_resourse::do_allocate(std::size_t count_bytes, std::size_t align) {
    if (!count_bytes) {
        return nullptr;
    }

    if (!align) {
        align = alignof(std::max_align_t);
    }

    if (blocks.empty()) {
        allocate_block(count_bytes + align);
    }

    Block &b = blocks.back();
    auto cur_adr = reinterpret_cast<std::uintptr_t>(b.cur);
    auto align_adr = align_up(cur_adr, align);
    auto end_adr = reinterpret_cast<std::uintptr_t>(b.end);
    if (align_adr + count_bytes <= end_adr) {
        b.cur = reinterpret_cast<char*>(align_adr + count_bytes);
        return reinterpret_cast<void*>(align_adr);
    }

    allocate_block(std::max(block_size, count_bytes + align));
    Block &b1 = blocks.back();
    cur_adr = reinterpret_cast<std::uintptr_t>(b1.cur);
    align_adr = align_up(cur_adr, align);
    end_adr = reinterpret_cast<std::uintptr_t>(b1.end);

    if (align_adr + count_bytes <= end_adr) {
        b1.cur = reinterpret_cast<char*>(align_adr + count_bytes);
        return reinterpret_cast<void*>(align_adr);
    }

    throw std::bad_alloc();
}

void memory_pool_resourse::do_deallocate(void *, std::size_t, std::size_t) {
};

bool memory_pool_resourse::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

void memory_pool_resourse::allocate_block(std::size_t count_bytes) {
    auto ptr = static_cast<char *>(std::malloc(count_bytes));
    if (!ptr) {
        throw std::bad_alloc();
    }

    Block b = {ptr, ptr + static_cast<std::ptrdiff_t>(count_bytes), ptr};
    blocks.push_back(std::move(b));
}

std::uintptr_t memory_pool_resourse::align_up(std::uintptr_t adr, std::size_t align) noexcept {
    if (!align) {
        align = alignof(std::max_align_t);
    }

    std::uintptr_t mask = align - 1;
    return (adr + mask) & ~mask;
}
