#pragma once

#include <memory_resource>
#include <memory>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cassert>

template <typename T>
class pmr_vector {
public:
    using value_type = T;
    using allocator_type = std::pmr::polymorphic_allocator<T>;
    using size_type = std::size_t;

    // forward iterator (с минимальным набором для forward iterator)
    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;

        iterator() noexcept : ptr_(nullptr) {}
        explicit iterator(T* p) noexcept : ptr_(p) {}

        reference operator*() const noexcept { return *ptr_; }
        pointer operator->() const noexcept { return ptr_; }

        iterator& operator++() noexcept { ++ptr_; return *this; }
        iterator operator++(int) noexcept { iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator==(const iterator& a, const iterator& b) noexcept { return a.ptr_ == b.ptr_; }
        friend bool operator!=(const iterator& a, const iterator& b) noexcept { return !(a == b); }

    private:
        T* ptr_;
    };

    using const_iterator = iterator; // for simplicity: const safety not enforced (we can refine later)

    // ctor with pmr resource pointer (default -> default resource)
    explicit pmr_vector(std::pmr::memory_resource* mr = std::pmr::get_default_resource(), size_type reserve_cap = 0)
        : alloc_(mr), data_(nullptr), size_(0), capacity_(0)
    {
        if (reserve_cap > 0) reserve(reserve_cap);
    }

    ~pmr_vector() {
        clear();
        if (data_) {
            alloc_.deallocate(data_, capacity_);
            data_ = nullptr;
        }
    }

    // non-copyable (simpler), movable
    pmr_vector(const pmr_vector&) = delete;
    pmr_vector& operator=(const pmr_vector&) = delete;

    pmr_vector(pmr_vector&& other) noexcept
        : alloc_(other.alloc_), data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    pmr_vector& operator=(pmr_vector&& other) noexcept {
        if (this != &other) {
            clear();
            if (data_) alloc_.deallocate(data_, capacity_);
            alloc_ = other.alloc_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    allocator_type get_allocator() const noexcept { return alloc_; }

    // capacity / size
    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    // element access
    T& operator[](size_type i) noexcept { assert(i < size_); return data_[i]; }
    const T& operator[](size_type i) const noexcept { assert(i < size_); return data_[i]; }

    // iterators
    iterator begin() noexcept { return iterator(data_); }
    iterator end() noexcept { return iterator(data_ + size_); }
    const_iterator begin() const noexcept { return const_iterator(const_cast<T*>(data_)); }
    const_iterator end() const noexcept { return const_iterator(const_cast<T*>(data_ + size_)); }

    // reserve: allocate new storage and move elements
    void reserve(size_type new_cap) {
        if (new_cap <= capacity_) return;
        T* new_data = alloc_.allocate(new_cap);
        // move-construct existing elements into new_data
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, std::addressof(new_data[i]),
                                                             std::move_if_noexcept(data_[i]));
            std::allocator_traits<allocator_type>::destroy(alloc_, std::addressof(data_[i]));
        }
        if (data_) alloc_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_cap;
    }

    // grow policy
    size_type grow_capacity() const noexcept {
        return capacity_ == 0 ? 1 : capacity_ * 2;
    }

    // emplace_back / push_back
    template <typename... Args>
    T& emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            reserve(grow_capacity());
        }
        std::allocator_traits<allocator_type>::construct(alloc_, std::addressof(data_[size_]), std::forward<Args>(args)...);
        ++size_;
        return data_[size_-1];
    }

    void push_back(const T& v) { emplace_back(v); }
    void push_back(T&& v) { emplace_back(std::move(v)); }

    void pop_back() {
        assert(size_ > 0);
        --size_;
        std::allocator_traits<allocator_type>::destroy(alloc_, std::addressof(data_[size_]));
    }

    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<allocator_type>::destroy(alloc_, std::addressof(data_[i]));
        }
        size_ = 0;
    }

private:
    allocator_type alloc_;
    T* data_;
    size_type size_;
    size_type capacity_;
};
