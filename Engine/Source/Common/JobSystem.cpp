//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/JobSystem.hpp"

void oe::JobSystem::Init()
{
	mFinishedLabel.store(0);

	auto numCores = std::thread::hardware_concurrency();
	mNumThreads = std::max(1u, numCores);
	for (uint32_t threadID = 0; threadID < mNumThreads; ++threadID)
	{
		std::thread worker([] {
			std::function<void()> job;

			while (true)
			{
				if (mJobPool.pop_front(job))
				{
					job();
					mFinishedLabel.fetch_add(1);
				}
				else
				{
					std::unique_lock<std::mutex> lock(mWakeMutex);
					mWakeCondition.wait(lock);
					if (mIsShouldExit)
						return;
				}
			}
		});
		worker.detach();
	}
}

void oe::JobSystem::Execute(const std::function<void()>& job)
{
	mCurrentLabel += 1;
	while (!mJobPool.push_back(job))
		Poll();

	mWakeCondition.notify_one();
}

void oe::JobSystem::Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job)
{
	if (jobCount == 0 || groupSize == 0)
		return;

	const uint32_t groupCount = (jobCount + groupSize - 1) / groupSize;

	mCurrentLabel += groupCount;

	for (uint32_t groupIndex{}; groupIndex < groupCount; ++groupIndex)
	{
		const auto& jobGroup = [jobCount, groupSize, job, groupIndex]() {
			const uint32_t groupJobOffset = groupIndex * groupSize;
			const uint32_t groupJobEnd = std::min(groupJobOffset + groupSize, jobCount);

			JobDispatchArgs args;
			args.groupIndex = groupIndex;

			for (uint32_t i = groupJobOffset; i < groupJobEnd; ++i)
			{
				args.jobIndex = i;
				job(args);
			}
		};

		while (!mJobPool.push_back(jobGroup))
		{
			Poll();
		}

		mWakeCondition.notify_one();
	}
}

bool oe::JobSystem::IsBusy()
{
	return mFinishedLabel.load() < mCurrentLabel;
}

void oe::JobSystem::Wait()
{
	while (IsBusy())
		Poll();
}

void oe::JobSystem::Poll()
{
	mWakeCondition.notify_one();
	std::this_thread::yield();
}

void oe::JobSystem::Shutdown()
{
	mIsShouldExit = true;
	Wait();
	mWakeCondition.notify_all();
}

oe::ScopedJob::ScopedJob(std::future<void> future)
{
	mFuture = future.share();
}

oe::ScopedJob::~ScopedJob()
{
	if (IsBusy())
		Wait();
}

oe::Ref<oe::ScopedJob> oe::ScopedJob::Execute(const std::function<void()>& job)
{
	return CreateRef<ScopedJob>(std::async(std::launch::async, job));
}

void oe::ScopedJob::Wait() noexcept
{
	mFuture.wait();
}

bool oe::ScopedJob::IsBusy() const noexcept
{
	return mFuture.valid();
}
