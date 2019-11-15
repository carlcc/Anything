#pragma once
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <queue>

template <class T>
class SimpleThreadSafeQueue {
 public:
  SimpleThreadSafeQueue(size_t capacity = size_t(-1))
      : mutex_(), not_empty_cv_(), not_full_cv_(), queue_(), size_(0), capacity_(capacity) {}
  ~SimpleThreadSafeQueue() {}

  void put(const T &t) {
    std::unique_lock<std::mutex> lck(mutex_);
    if (is_full()) {
      not_full_cv_.wait(lck, [this]() { return !is_full(); });
    }

    queue_.push(t);
    not_empty_cv_.notify_one();
    ++size_;
  }

  void put(T &&t) {
    std::unique_lock<std::mutex> lck(mutex_);
    if (is_full()) {
      not_full_cv_.wait(lck, [this]() { return !is_full(); });
    }

    queue_.push(std::forward<T &&>(t));
    not_empty_cv_.notify_one();
    ++size_;
  }

  bool put(int64_t timeout_millis, const T &t) {
    if (timeout_millis < 0) {
      put(t);
      return true;
    }

    std::unique_lock<std::mutex> lck(mutex_);
    if (timeout_millis == 0) {
      if (is_full()) {
        return false;
      }

      queue_.push(t);
      not_empty_cv_.notify_one();
      ++size_;
      return true;
    }

    // else block for timeout_millis at most
    if (is_full()) {
      auto not_full =
          not_full_cv_.wait_for(lck, std::chrono::milliseconds(timeout_millis), [this]() { return !is_full(); });
      if (!not_full) {
        return false;
      }
    }

    queue_.push(t);
    not_empty_cv_.notify_one();
    ++size_;
    return true;
  }

  T get(int64_t wait_millis, bool *ok = nullptr) {
    std::unique_lock<std::mutex> lck(mutex_);
    if (wait_millis < 0) {
      // blocking forever
      if (ok) {
        *ok = true;
      }
      return get_blocking_forever_(lck);
    }
    if (wait_millis == 0) {
      // no blocking
      if (queue_.empty()) {
        if (ok) {
          *ok = false;
        }
        return T();
      }
      auto ret = queue_.front();
      if (ok) {
        *ok = true;
      }
      queue_.pop();
      --size_;
      not_full_cv_.notify_one();
      return ret;
    }
    // else blocking for millis-milliseconds at most
    auto is_not_empty =
        not_empty_cv_.wait_for(lck, std::chrono::milliseconds(wait_millis), [this]() { return !is_empty(); });
    if (!is_not_empty) {
      if (ok) {
        *ok = false;
      }
      return T();
    }
    if (ok) {
      *ok = true;
    }
    auto ret = queue_.front();
    queue_.pop();
    --size_;
    not_full_cv_.notify_one();
    return ret;
  }

  T get() {
    std::unique_lock<std::mutex> lck(mutex_);
    return get_blocking_forever_(lck);
  }

  size_t size() const { return size_; }

  size_t capacity() const { return capacity_; }

  bool is_empty() const { return size() == 0; }

  bool is_full() const { return size() >= capacity(); }

 private:
  T get_blocking_forever_(std::unique_lock<std::mutex> &lck) {
    if (is_empty()) {
      not_empty_cv_.wait(lck, [this]() { return !is_empty(); });
    }
    auto ret = queue_.front();
    queue_.pop();
    --size_;
    not_full_cv_.notify_one();
    return ret;
  }
  T put_blocking_forever_(std::unique_lock<std::mutex> &lck) {
    if (is_empty()) {
      not_empty_cv_.wait(lck, [this]() { return !is_empty(); });
    }
    auto ret = queue_.front();
    queue_.pop();
    --size_;
    not_full_cv_.notify_one();
    return ret;
  }

 private:
  std::mutex mutex_;
  std::condition_variable not_empty_cv_;
  std::condition_variable not_full_cv_;
  std::queue<T> queue_;

  size_t size_;

  size_t capacity_;
};
