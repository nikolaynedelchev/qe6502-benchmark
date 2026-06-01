#include <at/atcore/scheduler.h>
#include <vd2/system/linearalloc.h>

#include <cstdlib>

VDLinearAllocator::VDLinearAllocator(uint32 blockSize)
    : mpBlocks(nullptr), mpAllocPtr(nullptr), mAllocLeft(0), mBlockSize(blockSize)
{
}

VDLinearAllocator::~VDLinearAllocator() = default;

void VDLinearAllocator::Clear() {}
void VDLinearAllocator::Swap(VDLinearAllocator&) {}
void VDLinearAllocator::Reset() {}
void* VDLinearAllocator::AllocateSlow(size_t) { std::abort(); }
void* VDLinearAllocator::AllocateSlow(size_t, size_t) { std::abort(); }
bool VDLinearAllocator::Contains(const void*) const { return false; }
size_t VDLinearAllocator::GetTotalAllocatedSize() const { return 0; }

ATScheduler::ATScheduler()
    : mNextEventCounter(0U - 1000)
    , mTimeBase(0xFFF00000 + 1000)
    , mActiveEvents{&mActiveEvents, &mActiveEvents}
    , mpFreeEvents(nullptr)
    , mbStopTimeValid(false)
    , mStopTime(0)
    , mTick64Floor(mTimeBase + mNextEventCounter)
    , mRate(1, 1)
    , mAllocator()
{
}

ATScheduler::~ATScheduler() = default;

void ATScheduler::ProcessNextEvent()
{
    std::abort();
}

void ATScheduler::SetEvent(uint32, IATSchedulerCallback*, uint32, ATEvent*&)
{
    std::abort();
}

void ATScheduler::UnsetEvent(ATEvent*&)
{
    std::abort();
}

ATEvent* ATScheduler::AddEvent(uint32, IATSchedulerCallback*, uint32)
{
    std::abort();
}

void ATScheduler::RemoveEvent(ATEvent*)
{
    std::abort();
}

int ATScheduler::GetTicksToEvent(ATEvent*) const
{
    return 0;
}

uint64 ATScheduler::GetTick64() const
{
    return mTick64Floor + (mTimeBase + mNextEventCounter - static_cast<uint32>(mTick64Floor));
}

void ATScheduler::UpdateTick64()
{
    mTick64Floor = GetTick64();
}

void ATScheduler::RebaseTick64(uint64 newTick)
{
    mTick64Floor = newTick;
}

uint32 ATScheduler::GetTicksToNextEvent() const
{
    return uint32(0) - mNextEventCounter;
}

void ATScheduler::SetStopTime(uint32 stopTime)
{
    mbStopTimeValid = true;
    mStopTime = stopTime;
    const uint32 current = mTimeBase + mNextEventCounter;
    const uint32 delta = stopTime - current;
    if (delta < uint32(0) - mNextEventCounter) {
        mTimeBase = current;
        mNextEventCounter = uint32(0) - delta;
        mTimeBase -= mNextEventCounter;
    }
}

void ATScheduler::ClearStopTime()
{
    mbStopTimeValid = false;
}
