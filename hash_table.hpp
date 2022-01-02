// Copyright 2021 GNDavydov

#ifndef INCLUDE_HASH_TABLE_HPP_
#define INCLUDE_HASH_TABLE_HPP_

#include <vector>
#include <iostream>

enum States {
    Deleted = -1,
    Free = 0,
    Used = 1
};

template<typename Key>
class HashFunc {
public:
    std::size_t operator()(const Key &key) const {
        return std::hash<Key>{}(key);
    }
};


template<typename Key, typename Value, typename Hash = HashFunc<Key>,
        class = typename std::enable_if<std::is_default_constructible<Key>::value>::type,
        class = typename std::enable_if<std::is_default_constructible<Value>::value>::type>
class HashTable {
private:
    static constexpr double coefficient_resize_ = 0.75;
    static constexpr double coefficient_rehash_ = 0.2;
    static constexpr std::size_t default_capacity_ = 500;
    static constexpr std::size_t step_ = 23;

    struct Node {
        int state_;
        Key key_;
        Value value_;

        Node() : state_(Free), key_(), value_() {}

        explicit Node(const Key &key, const Value &value) : state_(Used), key_(key), value_(value) {}
    };

    std::size_t capacity_;
    std::size_t size_;
    std::size_t deleted_elements_;
    std::vector<Node> elements_;
    Hash hash_;

public:
    explicit HashTable() : capacity_(default_capacity_), size_(0),
                           deleted_elements_(0),
                           elements_(capacity_), hash_() {}

    ~HashTable() noexcept = default;

    void insert(const Key &key, const Value &value) {
        std::size_t index = hash_(key) % capacity_;
        while (elements_[index].state_ == Used) {
            index = (index + step_) % capacity_;
        }

        if (elements_[index].state_ == Deleted) --deleted_elements_;
        else ++size_;

        elements_[index] = Node(key, value);

        if (size_ > coefficient_resize_ * capacity_) resize();
    }

    void remove(const Key &key) {
        std::size_t index = hash_(key) % capacity_;
        while (elements_[index].state_ != Free && elements_[index].key_ != key) {
            index = (index + step_) % capacity_;
        }

        if (elements_[index].state_ == Free) return;

        elements_[index] = Node();
        elements_[index].state_ = Deleted;

        ++deleted_elements_;
        if (deleted_elements_ > coefficient_rehash_ * capacity_) rehash();
    }

    bool find(const Key &key) const {
        std::size_t index = hash_(key) % capacity_;
        while (elements_[index].state_ != Free && elements_[index].key_ != key) {
            index = (index + step_) % capacity_;
        }

        if (elements_[index].state_ == Free) return false;
        return true;
    }

    Value get(const Key &key) const {
        std::size_t index = hash_(key) % capacity_;
        while (elements_[index].state_ != Free && elements_[index].key_ != key) {
            index = (index + step_) % capacity_;
        }

        if (elements_[index].state_ == Free) throw std::runtime_error("Element does not exist");
        return elements_[index].value_;
    }

    [[nodiscard]] inline std::size_t size() const { return size_; }

    [[nodiscard]] inline std::size_t capacity() const { return capacity_; }

    [[nodiscard]] inline std::size_t used_elements() const { return size_ - deleted_elements_; }

private:
    void rehash() {
        std::vector<Node> tmp_elements(elements_);
        elements_.clear();
        elements_.resize(capacity_);

        for (std::size_t i = 0; i < capacity_; ++i) {
            if (tmp_elements[i].state_ == Used) insert(tmp_elements[i].key_, tmp_elements[i].value_);
        }

        size_ -= deleted_elements_;
        deleted_elements_ = 0;
    }

    void resize() {
        std::vector<Node> tmp_elements(elements_);
        capacity_ *= 2;
        elements_.clear();
        elements_.resize(capacity_);

        for (std::size_t i = 0; i < tmp_elements.size(); ++i) {
            if (tmp_elements[i].state_ == Used) insert(tmp_elements[i].key_, tmp_elements[i].value_);
        }

        size_ -= deleted_elements_;
        deleted_elements_ = 0;
    }
};

#endif //INCLUDE_HASH_TABLE_HPP_
