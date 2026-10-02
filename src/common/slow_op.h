// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <chrono>

#include "common/logging/log.h"

namespace Common {

/// Logs a warning when the enclosing scope takes longer than a threshold. Used to find the
/// operations behind frame time spikes (stutter) without a profiler attached.
class SlowOpTimer {
public:
    explicit SlowOpTimer(const char* name_, double threshold_ms_ = 15.0)
        : name{name_}, threshold_ms{threshold_ms_}, start{std::chrono::steady_clock::now()} {}

    ~SlowOpTimer() {
        const double ms =
            std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start)
                .count();
        if (ms > threshold_ms) {
            LOG_WARNING(Render, "Slow operation: {} took {:.1f} ms", name, ms);
        }
    }

    SlowOpTimer(const SlowOpTimer&) = delete;
    SlowOpTimer& operator=(const SlowOpTimer&) = delete;

private:
    const char* name;
    double threshold_ms;
    std::chrono::steady_clock::time_point start;
};

} // namespace Common

#define SLOW_OP_CONCAT_IMPL(a, b) a##b
#define SLOW_OP_CONCAT(a, b) SLOW_OP_CONCAT_IMPL(a, b)
#define SLOW_OP_TIMER(name) ::Common::SlowOpTimer SLOW_OP_CONCAT(slow_op_timer_, __LINE__){name}
