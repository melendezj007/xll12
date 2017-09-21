// test.h - convenience macros for testing
// Copyright (c) KALX, LLC. All rights reserved. No warranty made.
#pragma once
#include <exception>
#include <functional>

namespace xll {
    struct test {
        test(const std::function<void(void)>& f)
        {
            try {
                f();
            }
            catch (const std::exception& ex) {
                MessageBoxA(0, ex.what(), "Failed", MB_OK);
            }
        }
    };
}