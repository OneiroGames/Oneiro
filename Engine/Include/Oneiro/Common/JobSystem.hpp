//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Common.hpp"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <future>
#include <sstream>
#include <thread>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

namespace oe
{
	struct JobDispatchArgs
	{
		uint32_t jobIndex{};
		uint32_t groupIndex{};
	};

	template <typename T, size_t capacity>
	class ThreadSafeRingBuffer
	{
	public:
		inline bool push_back(const T& item)
		{
			bool result{};
			lock.lock();
			const auto next = (head + 1) % capacity;
			if (next != tail)
			{
				data[head] = item;
				head = next;
				result = true;
			}
			lock.unlock();
			return result;
		}

		inline bool pop_front(T& item)
		{
			bool result{};
			lock.lock();
			if (tail != head)
			{
				item = data[tail];
				tail = (tail + 1) % capacity;
				result = true;
			}
			lock.unlock();
			return result;
		}

	private:
		T data[capacity]{};
		size_t head{};
		size_t tail{};
		std::mutex lock{};
	};

	// Original: https://wickedengine.net/2018/11/24/simple-job-system-using-standard-c/
	class JobSystem
	{
	public:
		static void Init();

		static void Execute(const std::function<void()>& job);

		static void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job);

		static bool IsBusy();

		static void Wait();

		static void Poll();

		static void Shutdown();

	private:
		inline static uint32_t mNumThreads{};
		inline static ThreadSafeRingBuffer<std::function<void()>, 256> mJobPool;
		inline static std::condition_variable mWakeCondition{};
		inline static std::mutex mWakeMutex{};
		inline static uint64_t mCurrentLabel{};
		inline static std::atomic<uint64_t> mFinishedLabel{};
		inline static bool mIsShouldExit{};
	};

	class ScopedJob
	{
	public:
		explicit ScopedJob(std::future<void> future);

		~ScopedJob();

		static Ref<ScopedJob> Execute(const std::function<void()>& job);

		template <class... Args>
		static Ref<ScopedJob> Execute(const std::function<void(Args...)>& job, Args&&... args)
		{
			return CreateRef<ScopedJob>(std::async(std::launch::async, job, args...));
		}

		void Wait() noexcept;

		[[nodiscard]] bool IsBusy() const noexcept;

	private:
		std::shared_future<void> mFuture{};
	};
} // namespace oe
