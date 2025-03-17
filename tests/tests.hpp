#pragma once
#include <gtest/gtest.h>
#include <memory>
#include <cassert>
#include <wire.hpp>
#include <gates.hpp>

extern "C" {
  void __ubsan_on_report() {
    FAIL() << "Encountered an undefined behavior sanitizer error";
  }
  
  void __asan_on_error() {
    FAIL() << "Encountered an address sanitizer error";
  }
  
  void __tsan_on_report() {
    FAIL() << "Encountered a thread sanitizer error";
  }
}  // extern "C"
