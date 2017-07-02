#include <vector>
#include <thread>
#include <future>
#include <atomic>
#include <tuple>

template<typename... Args> class packed_function
{
public:
  using invoke_ptr_t = void(*)(Args...);
  using tuple_t = std::tuple<Args...>;

  template<typename F, typename... Ts> packed_function(F&& func, Ts&&... args) :
    invoke_ptr_(std::forward<F>(func)),
    args_(std::make_tuple(std::forward<Ts>(args)...))
  {}

  packed_function(const packed_function&) = default;
  packed_function(packed_function&&) = default;

  packed_function& operator=(const packed_function&) = default;
  packed_function& operator=(packed_function&&) = default;

  ~packed_function() = default;

  void operator() ()
  {
    invoke_impl(std::index_sequence_for<Args...>{});
  }
private:
  invoke_ptr_t invoke_ptr_;
  tuple_t args_;

  template<size_t... Is> inline void invoke_impl(std::index_sequence<Is>...)
  {
    invoke_ptr_(std::get<Is>(args_)...);
  }
};


class atomic_lock_guard
{
public:
  explicit atomic_lock_guard(std::atomic_flag& lock) noexcept : lock_(lock)
  {
    while(lock_.test_and_set(std::memory_order_acq_rel));
  }

  ~atomic_lock_guard()
  {
    lock_.clear(std::memory_order_release);
  }
private:
  std::atomic_flag& lock_;
};

namespace lock
{
  struct atomic{}; // guranteed lockfree but will spin
  struct mutex{}; // can sleep
}

template<typename Closure, typename Lock = lock::mutex>class task_queue
{
public:
  using closure_t = Closure;
  using tasks_t = std::vector<closure_t>;

  using is_atomic = std::is_same<Lock, lock::atomic>;
  using lock_t = std::conditional_t<is_atomic::value, std::atomic_flag, std::mutex>;
  using guard_t = std::conditional_t<is_atomic::value,
    atomic_lock_guard,
    std::lock_guard<std::mutex>
  >;

  explicit task_queue() : lock_(init_lock())
  {
    auto execution_loop = [&active = active_, &lock = lock_, &tasks = tasks_]
    {
      auto finish_remaining_tasks = [&]()
      {
        guard_t guard(lock);
        for (auto&& task : tasks)
          task();
      };

      while(active.test_and_set(std::memory_order_acq_rel))
      {
        tasks_t local_tasks;
        {
          guard_t guard(lock);

          if (tasks.empty())
            continue;

          local_tasks = std::move(tasks);
          tasks.clear();
        }
        for (auto& task : local_tasks)
          task();
      }
      finish_remaining_tasks();
    };

    active_.test_and_set(std::memory_order_acq_rel);
    future_ = std::async(std::launch::async, execution_loop);
  }

  task_queue(const task_queue&) = delete;
  task_queue(task_queue&&) = default;

  task_queue& operator= (const task_queue&) = delete;
  task_queue& operator= (task_queue&&) = default;

  ~task_queue()
  {
    active_.clear(std::memory_order_release);
    future_.wait();
  }

  template<typename... Ts> void add_task(Ts&&... args)
  {
    guard_t guard(lock_);
    tasks_.emplace_back(std::forward<Ts>(args)...);
  }

private:
  lock_t lock_;
  std::atomic_flag active_ = ATOMIC_FLAG_INIT;
  tasks_t tasks_;
  std::future<void> future_;

  auto init_lock() -> lock_t
  {
    if constexpr (is_atomic::value)
      return ATOMIC_FLAG_INIT;
    else
      return {};
  }
};


#include <iostream>
#include <functional>

int main(int argc, char* argv[])
{
  if (argc != 2)
    return 1;

  std::ios::sync_with_stdio(false);

  task_queue<std::function<void()>, lock::mutex> render_queue{};
  //task_queue<packed_function<int>> render_queue{};

  int i{};
  while(i < static_cast<int>(std::stoi(argv[1])))
  {
    render_queue.add_task([i] { std::cout << i; });
    ++i;
  }
}
