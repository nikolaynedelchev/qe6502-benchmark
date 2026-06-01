#pragma once

// Minimal ares prelude for compiling only the MOS6502 component.
// The full ares umbrella header pulls in emulator/platform/JIT subsystems that
// are not needed by this benchmark's CPU-only smoke target.
#include <nall/platform.hpp>
#include <nall/memory.hpp>
#include <nall/primitives.hpp>
#include <nall/maybe.hpp>
#include <nall/string.hpp>
#include <nall/serializer.hpp>

using namespace nall;
using namespace nall::primitives;

#include <ares/types.hpp>
