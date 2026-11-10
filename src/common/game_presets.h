// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "common/settings.h"

namespace Settings {

inline constexpr u64 MarioKartWorldProgramId = 0x0400C3F00006E000ULL;
inline constexpr u64 OcarinaOfTimeProgramId = 0x0400486024DEC000ULL;

inline void SetMarioKartWorldRecommendedSettings(Values& settings, bool global) {
    const auto session = [global](auto& setting, auto value) {
        setting.SetGlobal(global);
        setting.SetValue(value);
    };
    session(settings.renderer_backend, RendererBackend::Vulkan);
    session(settings.resolution_setup, ResolutionSetup::Res1X);
    session(settings.gpu_accuracy, GpuAccuracy::Low);
    session(settings.use_asynchronous_shaders, true);
    session(settings.astc_recompression, AstcRecompression::Bc3);
    session(settings.vram_usage_mode, VramUsageMode::Conservative);
    session(settings.use_reactive_flushing, false);
    session(settings.vsync_mode, VSyncMode::FifoRelaxed);
    session(settings.use_docked_mode, ConsoleMode::Handheld);
    session(settings.scaling_filter, ScalingFilter::ScaleForce);
    session(settings.anti_aliasing, AntiAliasing::Fxaa);
}

inline bool ApplyMarioKartWorldPreset(Values& settings, u64 program_id, bool accepted) {
    if (!accepted || program_id != MarioKartWorldProgramId) {
        return false;
    }
    SetMarioKartWorldRecommendedSettings(settings, false);
    return true;
}

inline void SetOcarinaOfTimeRecommendedSettings(Values& settings, bool global) {
    const auto session = [global](auto& setting, auto value) {
        setting.SetGlobal(global);
        setting.SetValue(value);
    };
    session(settings.scaling_filter, ScalingFilter::Fsr3);
    session(settings.gpu_accuracy, GpuAccuracy::Low);
    session(settings.nvdec_emulation, NvdecEmulation::Gpu);
    session(settings.async_presentation, true);
    session(settings.use_asynchronous_shaders, true);
    // A fence per synced command list stalls the GPU thread and OoT renders correctly without it.
    session(settings.sync_memory_operations, false);
    // Real DLSS on RTX cards; without Streamline's files or an RTX GPU this falls back to FSR 3.1.
    session(settings.nvidia_dlss, true);
    // Build the game's shaders on the loading screen rather than stutter on first use, and bound
    // the wait for unbuilt Switch 2 pipelines instead of skipping draws.
    session(settings.precache_shaders, PrecacheShaders::Always);
    session(settings.native_async_draws, NativeAsyncDraws::Bounded);
}

inline bool ApplyOcarinaOfTimePreset(Values& settings, u64 program_id, bool accepted) {
    if (!accepted || program_id != OcarinaOfTimeProgramId) {
        return false;
    }
    SetOcarinaOfTimeRecommendedSettings(settings, false);
    return true;
}

} // namespace Settings
