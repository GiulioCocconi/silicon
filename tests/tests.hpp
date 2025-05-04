#pragma once
#include <gtest/gtest.h>
#include <memory>
#include <cassert>
#include <core/wire.hpp>
#include <core/gates.hpp>

extern "C" {
inline void __ubsan_on_report()
{
  FAIL() << "Encountered an undefined behavior sanitizer error";
}

inline void __asan_on_error()
{
  FAIL() << "Encountered an address sanitizer error";
}

inline void __tsan_on_report() {
    FAIL() << "Encountered a thread sanitizer error";
  }
}  // extern "C"
