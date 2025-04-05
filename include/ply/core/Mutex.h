#pragma once

#include <mutex>
#include <shared_mutex>

namespace ply {

typedef std::shared_mutex SharedMutex;
typedef std::unique_lock<std::shared_mutex> WriteLock;
typedef std::shared_lock<std::shared_mutex> ReadLock;

}  // namespace ply
