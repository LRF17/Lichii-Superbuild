# Changelog

All releases are unofficial builds. See the GitHub releases for assets and checksums.

## v53

- **New: status-bar upscaler button (PR #12 by LRF17).** The window now shows
  which upscaler is drawing: `DLSS`, `FSR 3.1`, `FSR 4`, `GAME UPSCALER`,
  `DLSS (IDLE)` / `FSR 3.1 (IDLE)` when the setting is on but nothing was
  replaced, or `DLSS UNAVAILABLE`. The tooltip gives the sizes (for example
  "2560x1440 to 3840x2160") and, for Ocarina of Time, the reason the renderer
  fell back. This came from a real case: a game patch silently made the game's
  own upscaler draw while NVIDIA DLSS was ticked, and the log was the only clue.
- **Fixed: v51/v52 could over-allocate video memory in Ocarina of Time
  (upstream #1333).** Read-only native compute storage bindings reached to the
  end of their mapping (up to 1 GiB), so OoT's ~1 GiB heap became a single
  1 GiB device-local buffer. Build 150 logs measured a median buffer cache of
  1594 MiB against 509 MiB before, and 24 of 27 sessions with failed image
  allocations were on 8 GB AMD cards. Bindings now size from their size word
  again.
- **Fixed: right-clicking a game could abort Lichii on Windows (upstream
  #1258)** when the game path held non-ASCII characters (UTF-8 decoded as ANSI,
  `c0000409`). Also from upstream: NUP add-ons are registered from external
  content scans (#1257), duplicate stub registrations are dropped (#1252),
  `FlushCommands` no longer submits an empty command buffer (#1329), AMD
  drivers may overallocate device memory (#1335), and Ocarina of Time's guard
  house freeze gets the built-in Texture Pool x2 patch (#1313).
- Everything from v52 stays: LRF17's DLSS scene jitter and -1.58 texture LOD
  bias (PR #10), Help -> Check for Updates... and Open Releases Page..., #1234,
  #1216, #1210, Fritchii v5.6's zero-page trim, the one-shot saved OoT preset,
  #1199, #1180, #1155, #1129, #1110, Linux pipeline preloading, the DLSS/HDR
  work (PR #7/#8/#9), native HDR, DLSS presets E/F and the optional white-flash
  mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v52

- **PR #10 by LRF17: NVIDIA DLSS now gets Ocarina of Time's scene jitter and
  texture LOD bias.** The host adds a Halton sub-pixel jitter sequence for
  temporal upscalers, but DLSS was left with `{0, 0}` and an unjittered scene,
  so every frame sampled the same points and could never resolve detail finer
  than the rendered pixels. DLSS now receives the same offset as FSR (with
  FSR's sign; motion vectors stay unjittered), and mipmapped samplers get the
  -1.58 LOD bias NVIDIA's DLSS guide asks for, already used by FSR. Measured
  sharpness roughly doubles on distant grass, foliage and roofs, with a stable
  picture in motion. SDR and HDR, 1440p -> 4K, tested on an RTX 4090.
- Everything from v51 stays: the rebase on `nx2` `7dddff2d0e`, #1231, #1236,
  #959, #1245/#1246, the manual Help -> Check for Updates... and Open Releases
  Page..., #1234, #1216, Fritchii v5.6's zero-page trim, the one-shot saved OoT
  preset, #1199, #1180, #1155, #1129, #1110, Linux pipeline preloading,
  #1200/#1197/#1196/#1154, 4K HDR DLSS PR #9, frame-generation HDR PR #8, HDR
  DLSS PR #7, #1058, #1127, #1078, #1094/#1095, #1061, #1062, #1056, #1060,
  #1036, #1028, #874, native HDR, DLSS presets E/F, the safe Fritchii ports and
  the optional white-flash mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v51

- Rebased on the latest zortos `nx2` `7dddff2d0e` (16 upstream merges since
  `093e264c8a`). New from upstream:
  - **#1236** (white areas / white light columns) and **#1231** (MKW dark
    lighting) are now merged upstream — we dropped our local copies of them;
    #1231 is inherited for free.
  - **#959** (allocate a buffer over the memory budget instead of throwing) is
    upstream's own version of our long-carried Fritchii allocator port, with
    device-pressure accounting — we dropped ours.
  - **#1245/#1246** (jpkto): one command and one pair of barriers for a run of
    in-order uploads, and a dispatch's barrier recorded in the dispatch's own
    command.
  - **#941**: keep NVIDIA DLSS on when Streamline warns the VRAM budget is used
    up. **#853**: copy 2D-engine blits between 3D surfaces slice by slice.
    **#850**: size the sysmix queue to the host's callback. **#784**: swizzle
    and unswizzle block linear in 16-byte units. **#796**: show generated frames
    next to the FPS. **#1168**: do not wait for the time services when applying
    settings during loading. **#1128**, **#972**, **#1108**, **#1126**: build,
    kernel and patch-handling fixes.
- **Dropped two PRs upstream closed:** #1219 (the owner confirmed #1236, not
  the occlusion carry, was the white-light cause) and #1235 (a maintainer sweep
  flagged the time-based Equal-acquire recovery as able to unblock GPU work
  early and rewind semaphore progress). Both were in v50.
- Kept everything else from v50: #1234 (UI deadlock), #1216 (OoT fast draws),
  the manual Help -> Check for Updates... and Open Releases Page..., #1210,
  Fritchii v5.6's zero-page trim, the one-shot saved OoT preset, #1199, #1180,
  #1155, #1129, #1110, Linux pipeline preloading, #1200/#1197/#1196/#1154,
  4K HDR DLSS PR #9, frame-generation HDR PR #8, HDR DLSS PR #7, #1058, #1127,
  #1078, #1094/#1095, #1061, #1062, #1056, #1060, #1036, #1028, #874, native
  HDR, DLSS presets E/F, the safe Fritchii ports and the optional white-flash
  mitigation.
- README now states plainly that this is a hobby project, run for fun, with no
  donations or payment accepted; all donation links and traces were removed.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v50

- **#1236: fixed Ocarina of Time's white areas / white light columns
  (#849, #867).** Switch 2 device (SMMU) ranges are cached by
  `{cpu_address, size}` and were never released when sub 102 destroys the
  memory. When the game frees that memory and maps new pages at the same CPU
  address, the cached range was handed back, so the GPU kept reading the old
  pages — by then recycled for actor packs, BYAML and textures — producing NaN
  vertices, white areas behind scenery and missing objects that stayed until a
  restart. The range is now released on destroy, and reused ranges are refreshed
  to the CPU range's current pages before the GMMU map. This is zortos's own
  port of #1221, unconditional (no experiment switch), superseding the closed
  #1230.
- **#1234:** no longer deadlocks the UI thread when terminating a stuck
  emulation thread (#775).
- **#1235:** rescues a stuck Equal acquire on an NVN2 queue semaphore (#925,
  port of Eglantine #712), so boot no longer advances one ring round per two
  seconds.
- Based on the same zortos `nx2` `093e264c8a` as v49; everything from v49
  stays: the manual Help -> Check for Updates... and Open Releases Page...,
  #1210, #1219, #1216, Fritchii v5.6's zero-page trim, the one-shot saved OoT
  preset, #1199, #1180, #1155, #1129, #1110, Linux pipeline preloading,
  #1200/#1197/#1196/#1154, 4K HDR DLSS PR #9, frame-generation HDR PR #8, HDR
  DLSS PR #7, #1058, #1127, #1078, #1094/#1095, #1061, #1062, #1056, #1060,
  #1036, #1028, #874, native HDR, DLSS presets E/F, the safe Fritchii ports and
  the optional white-flash mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v49

- **New: Help -> Check for Updates...** runs an update check on demand and
  always shows the result. The old check ran only once at startup and stayed
  silent on a network failure, on "no update", and for 24 hours after a dialog
  was dismissed (or forever after "Skip this version") — which is why an update
  sometimes went unnoticed. The manual check bypasses those suppressions and
  reports rate limits, HTTP errors, offline failures and "you are up to date"
  in a dialog. A failed startup check now also retries once after two minutes.
- **New: Help -> Open Releases Page...** opens this repository's releases page
  directly, so an update can always be downloaded even when GitHub's API is
  rate limited.
- Everything from v48 stays: #1210, #1219, #1216, Fritchii v5.6's zero-page
  trim, the one-shot saved OoT preset, #1199, #1180, #1155, #1129, #1110,
  Linux pipeline preloading, #1200/#1197/#1196/#1154, 4K HDR DLSS PR #9,
  frame-generation HDR PR #8, HDR DLSS PR #7, #1058, #1127, #1078,
  #1094/#1095, #1061, #1062, #1056, #1060, #1036, #1028, #874, native HDR,
  DLSS presets E/F, the safe Fritchii ports and the optional white-flash
  mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v48

- Based on latest zortos `nx2` `093e264c8a`: **#1210** keeps the texture-cache
  collector thresholds within the host's commit budget, fixing an OoT
  out-of-memory crash on cards like the RX 7600 XT (Windows charges device
  memory to the system commit limit); **#1212** renames the recommended-settings
  parameter that shadowed `Settings::values` (the same MinGW/macOS build fix we
  carried locally, now upstream).
- **#1219: Ocarina of Time's white light.** Gameplay samples-query windows of
  1,025-1,110 slots were being dropped at the 1,024-slot bank bound; the running
  count is now carried into the next query instead of lost, so reports in the
  window read correctly.
- **#1216: more OoT fast draws.** Writes that change no draw state
  (`vertex_id_base`, MME memory addresses A/B, occlusion query methods) no
  longer break the fast-draw chain — about 36,000 draws a second were affected.
  Also keeps vertex-stream stride changes from ending it when dynamic strides
  are in use.
- **Fritchii v5.6:** the zero-page trim stops at the first non-zero line instead
  of reading the whole page, cutting the cost of scanning resident guest pages.
- Everything from v47 stays: the OoT recommended settings applied once without
  overwriting per-game edits, #1199, the expanded saved OoT preset, #1180 water
  ripples, #1155 descriptor-pool flush, #1129 texture-cache ceiling, #1110
  Bananza AMD crash fix, Linux pipeline preloading, #1200/#1197/#1196/#1154,
  4K HDR DLSS PR #9, frame-generation HDR PR #8, HDR DLSS PR #7, #1058, #1127,
  #1078, #1094/#1095, #1061, #1062, #1056, #1060, #1036, #1028, #874, native
  HDR, DLSS presets E/F, the safe Fritchii ports and the optional white-flash
  mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v47

- **Fixed: the Ocarina of Time recommended settings were overwriting changes
  made in the game's Properties.** v46 applied the full recommended table on
  every OoT launch and saved it to OoT's per-game config, so a value you set
  yourself — for example `Precache shaders before game start = Always` — was
  reset to the recommended `Off` at the next boot. The recommended settings
  are now offered and applied **once** (a saved flag), and the preset skips any
  setting that already has a per-game value, so manual edits stick. The prompt
  says so explicitly.
- Based on the same zortos `nx2` `bcb37c49f7` as v46; everything from v46
  stays: #1199, the expanded OoT preset saved per game, #1180 water ripples,
  #1155 descriptor-pool flush, #1129 texture-cache ceiling, #1110 Bananza AMD
  crash fix, Linux pipeline preloading, #1200/#1197/#1196/#1154, 4K HDR DLSS
  PR #9, frame-generation HDR PR #8, HDR DLSS PR #7, #1058, #1127, #1078,
  #1094/#1095, #1061, #1062, #1056, #1060, #1036, #1028, #874, native HDR,
  DLSS presets E/F, the safe Fritchii ports and the optional white-flash
  mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v46

- **The Ocarina of Time recommended-settings prompt now uses zortos's full
  recommended table and saves the result.** Accepting it applies the same
  46-setting developer baseline as the global recommended-settings offer
  (GPU accuracy Low, conservative VRAM, BC3 ASTC, asynchronous presentation and
  shaders, no memory-operation syncs, Bounded pipeline waits, pipeline caches
  on, CPU/System defaults and more) plus FSR 3 scaling and NVIDIA DLSS where
  available. The values are written to OoT's per-game configuration, so they
  persist; other games and global settings are untouched.
- Based on latest zortos `nx2` `bcb37c49f7`: #1199 unmaps every GPU range into
  memory that sub 102 destroys.
- Everything from v45 stays: water ripples #1180, descriptor-pool flush #1155,
  texture-cache ceiling #1129, Bananza AMD crash fix #1110, the Linux pipeline
  preloading parity, 4K HDR DLSS PR #9, frame-generation HDR PR #8, HDR DLSS
  PR #7, #1154/#1200/#1197/#1196, #1058, #1127, #1078, #1094/#1095, #1061,
  #1062, #1056, #1060, #1036, #1028, #874, native HDR, DLSS presets E/F, the
  safe Fritchii ports and the optional white-flash mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v45

- **Based on latest zortos `nx2` `610bd08731`:** #1154 (build fix after #1143,
  now upstream), #1200 (offers the developer's recommended settings once, with
  a way back in the Help menu), #1197 (stand-in for damaged .zs members of a
  NUP dump) and #1196 (ADPCM nibble-step sign — fixes the OoT ocarina / Song of
  Time crackle).
- **Water:** replaced the earlier ripple patch with #1180, which counts single
  ripple passes per frame and also covers Dynamic FPS, so ripples keep moving
  above 60 fps and the surface stops jumping with the camera. Dropped #1147:
  its Water Temple premise was withdrawn — the missing geometry came from an
  incomplete game-data container, not the occlusion cap.
- **Fewer pipeline stalls:** #1155 raises the texture-descriptor-pool flush cap
  to 4 MiB, so OoT's 1,058,176-byte pool lets pipeline stages translate on the
  worker threads instead of the GPU thread; #1129 scales the texture-cache
  ceiling with VRAM on 10+ GiB cards, so OoT stops evicting costly ASTC
  textures; #1110 fixes the AMD crash after Bananza's golden-banana loading
  screen.
- **Linux: pipeline preloading is now on by default**, matching Windows. That
  stops the Linux-only "shaders rebuilt every couple of minutes" behaviour
  (#747: pipelines evicted after 9,000 frames were recompiled from scratch on
  RADV).
- Everything from v43 stays: the shader-cache upgrade note (clear caches from
  v34 or earlier), the 4K HDR DLSS fix (PR #9), frame-generation HDR (PR #8),
  HDR DLSS (PR #7), #1058, #1127, #1078, #1094/#1095, #1061, #1062, #1056,
  #1060, #1036, #1028, #874, native HDR, DLSS presets E/F, the safe Fritchii
  ports and the optional white-flash mitigation. Precache shaders stays off in
  the OoT preset.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v43

- **Fixed the stutter regression in the removed v42 build.** That build dropped
  two performance presets that were doing real work: preloading every cached
  graphics pipeline at boot and skipping native draws whose pipeline is still
  building. Without them, pipelines were rebuilt while playing and frames
  stalled while builds finished. Both are restored, matching v41's smoothness.
- **Before playing: if your shader cache is from v34 or earlier, clear it.**
  v35 replaced the Vulkan pipeline cache with shader cache v2, and caches from
  older builds can cause wrong graphics or repeated rebuilds. If you enable
  Precache shaders, the first launch after that takes a few minutes on the
  loading screen while shaders are built — this is normal, there is a Skip
  button, and later launches only build what is new until the GPU driver
  changes.
- Based on the same zortos `nx2` `d2eaae4f3a` as v41, so everything from v41
  stays: the water fixes #1152/#1147, 4K HDR DLSS PR #9, frame-generation HDR
  PR #8, #1143/#1154, HDR DLSS PR #7, #1058, #1127, #1078, #1094/#1095, #1061,
  #1062, #1056, #1060, #1036, #1028, #874, native HDR, DLSS presets E/F, the
  safe Fritchii ports and the optional white-flash mitigation. Precache shaders
  stays off in the OoT preset.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.
## v41

- **Water fixes.** Two open upstream PRs taken ahead of merge because they
  address the water reports:
  - **OoT water ripples keep moving above 60 fps with FPS Unlock (#1152).**
    The game's water renderer ran its ripple simulation `trunc(step) x substeps`
    times per frame; above 60 fps the step is below 1, so it ran zero times and
    the rings Link makes stuck to the camera. A built-in patch adds an
    accumulator that carries the fractional step between frames (with a unit
    test on the patched instructions).
  - **Water Temple occlusion windows are no longer dropped (#1147).** The
    central chamber reports 1,025-1,035-slot windows; the 4-bank (1,024-slot)
    cap reset the counter mid-window and the chamber walls went missing
    (#1142). The bound is now 8 banks (2,048 slots), still far below the
    64-bank menu case, with Catch2 boundary tests.
- **Ocarina's 4K NVIDIA DLSS image now reaches the screen with HDR output
  (PR #9, LRF17).** HDR uses another last-pass shader and a 10-bit target, so
  the 3840x2160 DLSS result was being scaled back to the game's 1440p target.
  New HDR10 reference/composite shaders encode the game's own PQ (203 nits,
  BT.709->BT.2020) and the 4K composite is presented when its encoding matches
  the frame. SDR and FSR are unchanged.
- **Frame generation runs with HDR10 output, Optical Flow type (PR #8, LRF17).**
  Generated frames now carry the presented frame's colour space instead of
  being reset to sRGB, and the interpolation shader writes A2B10G10R10 as well
  as BGRA8. scRGB and Lossless Scaling stay paused in HDR.
- Based on latest zortos `nx2` `d2eaae4f3a`: **#1143** keeps an evicted
  graphics pipeline's storage buffer pointers when it is translated again.
- Everything from v40 stays: the HDR DLSS scene fix (#7), shutdown hang fix
  (#1058), nxformat delay-load (#1127), nxformat 1.0.1 (#1078), #1094/#1095
  OoT speedups, #1061 FSR jitter, #1062 FSR 4, #1056 back macros, #1060 OoT
  FSR 3.1, shader cache v2 #1036, async read-back #1028, #874, native HDR,
  DLSS presets E/F, the safe Fritchii ports and the optional white-flash
  mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v40

- **NVIDIA DLSS now takes Ocarina of Time's scene as HDR when native HDR is
  on (PR #7, contributed by LRF17).** With HDR on and DLSS replacing the
  game's upscaler, highlights used to stop at the HDR reference white (200 nits
  by default) instead of reaching the configured peak: the guest's
  square-root-encoded scene was handed to DLSS with `.hdr = false`, so DLSS
  clipped it as LDR at 1.0. The scene is now squared into linear HDR for DLSS,
  DLSS runs with HDR colour buffers and auto exposure, and its linear output is
  written back without squaring again. SDR and the FSR 3.1/FSR 4 paths are
  unchanged.
- Based on the same zortos `nx2` `f1932bc9a9` as v39 — `nx2` has not moved —
  so everything from v39 stays: the shutdown hang fix (#1058), the nxformat
  delay-load startup fix (#1127), nxformat 1.0.1 (#1078), the brighter
  pipeline-failure diagnostics (#807), the UI translations, #1094/#1095 OoT
  speedups, #1061 FSR jitter, #1062 FSR 4, #1056 back macros, #1060 OoT FSR
  3.1, shader cache v2 #1036, async read-back #1028, #874, native HDR, DLSS
  presets E/F, the safe Fritchii ports and the optional white-flash mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v39

- Based on latest zortos `nx2` `f1932bc9a9`, fifteen merged changes on top of
  v38: **shutdown no longer hangs (#1058)** when a guest thread is waiting on a
  GPU sync operation as the GPU threads stop — the queued requests are dropped
  and their waiters released (tested 30 of 30 runs; without it 5 of 5 hung);
  **Lichii starts again when Windows blocks `nxformat.dll` (#1127)** —
  Smart App Control and code-integrity policies refuse the unsigned DLL, so it
  is now delay-loaded with a clear error instead of stopping `lichii.exe`;
  **nxformat 1.0.1 (#1078)** reads a range across blocks in one go and uses the
  archive's binary search for directory lookups; out-of-host-memory pipeline
  failures now say why (#807); and the UI translations are built into releases
  and cover the nx2 contexts (#743, #819, #826, #827, #893).
- **Our own fixes are now upstream:** #1045 (stutter-trace shadow), #1098
  (DLSS presets E/F and the one-shot VRAM log) and #1101 (updater retries)
  merged into `nx2`, so the superbuild layer no longer carries them.
- Keeps v38's #1094 and #1095 OoT split speedups (command-list word waits and
  3D state-register writes), #1061 FSR jitter, #1079 texture-cache joins,
  #1062 FSR 4, #1056 back macros, #1060 OoT FSR 3.1, shader cache v2 #1036,
  async read-back #1028, #874, native HDR, DLSS presets E/F, the safe Fritchii
  ports and the optional white-flash mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v38

- **Big frame-rate gain.** Two OoT split-GPU-thread changes by jpkto, the
  author of #1028 and #1056, on top of everything v37 has:
  - **Command lists wait only for the words GPU work wrote (#1094).** The
    split's front spent 93-156 ms of every second in Hyrule Field
    (169-204 waits, 550-765 us each) waiting for the copy of a whole 4 KiB
    command-list page; at Death Mountain it was ~196 ms a second in ~270
    waits, leaving the front about 14 ms idle. The page now tracks exactly
    which words GPU work wrote, so a list elsewhere on the page reads guest
    memory and waits for nothing, and a list on some words takes only those.
    Also fixes `DMAClear` checking a quarter of its range, and a
    `DMACopy`/`DMAClear` case that left the page's copy stale.
  - **3D state registers written in one call (#1095).** The back writes about
    5 million 3D registers a second in OoT, about half of them state
    registers; each went through three out-of-line calls. The hot path now
    writes them in place, with every other method taking the old route.
- Full v37 feature set: #1061 FSR jitter, #1079 texture-cache joins, #1062
  FSR 4, #1056 back macros, #1060 OoT FSR 3.1, shader cache v2 #1036,
  async read-back #1028, #874, native HDR, DLSS presets E/F, the safe
  Fritchii ports and the optional white-flash mitigation.
- **Note:** #1094 and #1095 are open upstream PRs, merged here ahead of
  upstream at the maintainers' request; both are jpkto's and carry tests.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v37

- Based on latest zortos `nx2` `7afad5494e`, three safe merged changes on top of
  v36: **OoT FSR jitter and texture LOD bias (#1061)** — sub-pixel camera jitter
  so FSR gets real sub-pixel detail instead of sampling the same position every
  frame, which removes stair-stepping and shimmer on thin geometry; **texture
  cache join uploads skipped when layer copies overwrite the whole target
  (#1079)**; and **OoT AMD FSR 4 (#1062)** through AMD's official FidelityFX
  runtime, loaded at run time and falling back to FSR 3.1 when it is not there.
- Full v36 feature set stays in: #1056 macros on the split's back (the big OoT
  gain), #1060 OoT FSR 3.1, shader cache v2 #1036, async read-back #1028, #874,
  native HDR, DLSS presets E/F, the safe Fritchii ports and the optional
  `EDEN_NX2_FENCE_FLUSH_US=0` white-flash mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v36

- **Big frame-rate gain.** #1056 runs the macros that read memory on the split's
  back: Ocarina of Time's render-enable macro makes ~25,000 MME DMA reads a
  second in Hyrule Field, and each waited on the split's front — 252.8 ms of
  every second of a front that was 99.9% busy. Those macro calls now run on the
  back, at the point of the method stream the read needs. Command-list fetches
  also wait only for the work before the last host sync instead of every writer
  (~7,500 waits a second, 142 ms a second). Both are the largest OoT gains since
  #1028, on top of the ~12 fps #1028 + #874 already give.
- **Ocarina of Time: AMD FSR 3.1 (#1060)** replaces the game's own
  anti-aliasing network, with the game's camera and actor motion as input and
  the effects bias mask fed to FSR as its reactive and transparency masks.
  Optional, off by default. This upstreams our #635 glow/particle history fix
  and the R16F bias mask, so those are no longer local patches.
- Based on latest zortos `nx2` `c215653cc5`; shader cache v2 (#1036) and the
  per-frame stutter trace (#1039) from v35 stay in.
- Full v35 feature set included, including #874, native HDR, the DLSS presets
  E/F and the optional `EDEN_NX2_FENCE_FLUSH_US=0` white-flash mitigation.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.

## v35

- **Big frame-rate gain kept.** #1028 async read-back plus the #874 payload-release
  fix give OoT roughly 12 fps over builds without them (Kokiri Forest 28.2 ->
  32.9 fps measured on 5900X + RTX 3080).
- Based on latest zortos `nx2` `0ab7736d`: shader cache v2 becomes the Vulkan
  pipeline cache (#1036) — fewer and shorter shader stutters, pipelines built off
  the GPU thread, native programs from records, driver updates no longer discard
  the cache; plus per-frame stutter-trace zones (#1039).
- **Rare white flashes:** an optional `EDEN_NX2_FENCE_FLUSH_US=0` line is now
  documented in `nx2_switches.txt`. It is off by default: the upstream reporter
  did not open a PR for it and batching helps other titles. Uncomment to try it.
- Full v34 feature set included.
- Windows and Linux, standard and PGO. macOS arm64 builds remain on v34.
## v34

- **Big frame-rate gain.** #1028 async read-back measured at Ocarina of Time
  Kokiri Forest 28.2 -> 32.9 fps (+17%), p90 frame time 38.0 -> 32.8 ms on a
  5900X + RTX 3080; combined with the #874 payload-release fix from v29, OoT
  gains roughly 12 fps over builds without it.
- Also #965 GPU profiling diagnostics (GPU waits by zone, present/submit zones,
  spin-mutex contention), all off unless their env vars are set.
- Based on zortos `nx2` `0b83798b`, with #874 and the full superbuild set.
- Includes the selected safe Fritchii fixes: bounds-checked ASTC/unswizzle
  stores (AMD corruption), host-memory buffers over the memory budget, the
  Windows high-resolution waitable timer and updater download retries.
- v32 is no longer offered; it was based on an older nx2 and some AMD users
  reported broken graphics. v31 remains a fallback.
- Windows and Linux, standard and PGO; macOS arm64 `.zip` / `.dmg`.

## v32

- Based on zortos `nx2` `a551b808`: NX archive format support (#923), all
  semaphore regions kept instead of only the first 32 (#1014), HostTiming OS
  sleeps between events (#1016), drain-free semaphore acquires and descriptor
  replay (#949), Switch 2 semaphore mirror with direct-release shadowing (#979),
  all-zero guest page trimming (#961), per-device translation generations (#984),
  malformed-file parser hardening (#947), and memory/stutter trace tools.
- Keeps #874: fence-flagged payload releases stay synchronized at low accuracy /
  Default fences, restoring OoT performance after #337.
- Adds selected safe Fritchii fixes: bounds-checked ASTC decode and unswizzle
  stores (AMD corruption / device loss), host-memory buffers may exceed the
  memory budget instead of aborting (OoT town-square crash after long sessions),
  Windows high-resolution waitable timer for the guest clock, and updater
  download retries on 5xx/429/dropped connections.
- MinGW build fix for `PROCESS_MEMORY_COUNTERS_EX2`.
- Full v31 feature set included.
- Windows and Linux, standard and PGO.

## v31

- Based on zortos `nx2` `cbb512ee`: #932 fixes the macOS build type mismatch in
  `GetSubmappedDeviceRange`, and #918 makes `Log2Ceil(0)` use the smallest
  bucket instead of wrapping.
- Includes #874 payload-release fix and #910/#921 OoT crash fixes.
- Includes the full v30 feature set.
- Windows and Linux, standard and PGO.
## v30

- Based on zortos `nx2` `1c25fbdf`, including #921 (fast-draw and texture-memo
  paths now match a full resolve, closing the remaining #858 crash class) and
  #912 (dav1d keeps its default thread count).
- Includes #874: fence-flagged payload releases stay synchronized at low
  accuracy/Default fences, restoring OoT performance after #337.
- Includes the #910 OoT texture-memo crash fix and the full v29 feature set.
- Windows and Linux, standard and PGO; macOS arm64 `.zip`/`.dmg`.
## v29

- Adds #874: fence-flagged payload releases stay synchronized at low
  accuracy/Default fences, restoring Ocarina of Time performance after #337.
- Based on v28 / zortos `nx2` `c2e293ec`; includes the #910 OoT texture-memo
  crash fix and the official nx2 merge set.
- All v28 features included.
- Windows and Linux, standard and PGO.

## v28

- Based on zortos `nx2` `c2e293ec`, with the #910 Ocarina of Time texture-memo
  crash fix: null texture views are no longer prepared on a memo hit.
- Includes the official nx2 merge set: #858 (Bananza back-thread performance),
  #878 (precache memory), #879 (AV1 libdav1d), #880 (backable texture), #881
  (microphone silence), #885 (NUP zstd lazy decode) and #902 (SPIR-V
  clip-distance race).
- All v24 features included: native HDR,
  NVIDIA DLSS, Fritchii v4/v4.1 fixes and the full superbuild set.
- Windows and Linux, standard and PGO.

## v24

- Shader precache discovers every container the archive walker reads (#812),
  improving hit coverage before a game starts.
- Cached task/mesh pipelines are built at boot on GPUs without
  VK_EXT_mesh_shader (#841), avoiding first-use pipeline builds.
- All v23 features included: GPU-thread poll timer, pipeline-cache filtering,
  native HDR, NVIDIA DLSS and the full
  superbuild set.
- Windows and Linux, standard and PGO.
## v23

- Smoother GPU thread scheduling: the pending-channel poll timer resets when a
  channel makes progress, with shorter Windows waits (#621).
- Pipeline caches keep only undamaged, non-stale and non-duplicate records,
  avoiding slow loads and corrupt cache entries (#815).
- All v22 features included: shader
  precache, split GPU thread pinning, OoT FPS Unlock, native HDR, NVIDIA DLSS,
  the Fritchii fixes and the full superbuild set.
- Windows and Linux, standard and PGO.
## v22

- Ocarina of Time's recommended preset now also disables memory-operation
  synchronization and turns on NVIDIA DLSS where available (FSR 3.1 fallback).
- Latest `nx2` batch: shader precache before game start (#781), split GPU
  threads pinned to their own cores by default (#769), OoT FPS Unlock and
  Switch 2 frame-rate limit slider (#655), non-blocking channel waits (#752),
  fence-flagged report timing (#337), buffer read traps (#264), MKW water and
  waves (#690), deferred-clear render passes (#699), perspective-correct
  noperspective interpolation (#820), bounded native compute storage (#816),
  cheaper ASTC uploads (#296), stale-image upload fixes (#818/#817), Switch 2
  button icons (#691) and decoder/movie-clock fixes (#809).
- The rest of the superbuild set is unchanged.
- Windows and Linux, standard and PGO.

## v21

- Ocarina of Time's water no longer flickers at Zora's River and other water; a
  storage-buffer address mistaken for a texture handle now binds no texture.
- Upload staging buffers are kept and reused instead of being freed every 64
  frames, removing 2-68 ms setup stalls after pauses in Hyrule Field.
- Large video-memory cards may fill more of their budget before texture
  collection, and the texture-cache overrides are capped at the device budget.
- Ocarina of Time's movie network is replaced by an input copy, fixing its
  movie path.
- Faster GPU invalidation and query handling, fewer pipeline-cache evictions,
  motion-sensor fixes, NUP title fixes, LayeredFS mod-root guards and many more
  upstream fixes (zortos `nx2` `794602bf`, including #778).
- The full superbuild set is unchanged: OoT recommended-settings prompt,
  GPU-split and fence fixes, fence-flagged query reports, Bananza page-mark
  batching, mja00 work, Nolar's video/scheduler/texture-cache fixes, the
  `PRELOAD_ALL_PIPELINES` and `ASYNC_DISCARD` presets, NVIDIA DLSS on Windows,
  native HDR (opt-in) and the updater pointed at this repository.
- Windows and Linux, standard and PGO.

## v20

- **Native HDR output (opt-in).** Enable HDR in Graphics settings on a compatible
  Vulkan system and HDR display; reference white and peak luminance are adjustable.
  HDR uses RGB10A2/RGBA16F framebuffers and HDR swapchain color spaces, and skips
  SDR-only effects and frame generation while active. From the shortfuse HDR PR
  (upstream Eglantine #605, ported to Lichii as #180).
- HDR is off by default and requires an HDR-capable display; unsupported HDR
  presentation fails rather than silently falling back to SDR.
- The rest of the superbuild set is unchanged: OoT recommended-settings prompt,
  GPU-split and fence fixes, fence-flagged query reports, Bananza page-mark
  batching, mja00 uniform-bank/HMMA work, Nolar's video, scheduler and
  texture-cache fixes, the `PRELOAD_ALL_PIPELINES` and `ASYNC_DISCARD` presets,
  NVIDIA DLSS on Windows, and an updater pointed at this repository.
- Windows and Linux, standard and PGO.

## v19

- Mario Kart World no longer locks near 30 fps: GPU Clocks Boost and Overclock
  once again shrink Switch 2 GPU durations (upstream #664).
- A batch of AMD graphics fixes: occlusion query waits, non-Depth-ref compare
  samplers, blend key rebuilds, out-of-range texture handles, pipeline cache
  recovery after a driver rejection, uneven BCn mip handling and keeping
  preloaded pipelines resident (upstream #679).
- What this build adds over stock Lichii is unchanged: the OoT recommended-settings
  prompt, GPU-split and fence fixes, fence-flagged query reports, Bananza
  page-mark batching, the mja00 uniform-bank/HMMA work, Nolar's video,
  scheduler and texture-cache fixes, the `PRELOAD_ALL_PIPELINES` and
  `ASYNC_DISCARD` presets, and an updater pointed at this repository.
- Windows and Linux, standard and PGO.

## v18

- Movie video now stays in sync with its audio, with late-frame dropping, movie
  pacing and faster streaming for Switch 2 titles (upstream #659).
- Fixed an RDNA2 GPU crash caused by cooperative-matrix HMMA (upstream #657).
- Windows and Linux, standard and PGO.

## v17

- Ocarina of Time cutscenes now stay in sync with their audio below 60 fps: late
  movie frames are dropped, movie pacing is always on, and Switch 2 titles no
  longer pay the SD-card read throttle that delayed streamed video.
- NVIDIA DLSS for OoT: the effects mask is now handed to Streamline in a format
  it accepts, so the glow and particle fix works. DLSS presets E and F are now
  selectable.
- Windows and Linux, standard and PGO.

## v16

- NVIDIA DLSS is now available on Windows. Enable it in Graphics settings and
  use the download button under the option to fetch NVIDIA's Streamline and DLSS
  files. Requires a GeForce RTX card; AMD and Intel keep FSR 3.1.
- Option to bypass Ocarina of Time's sharpening anti-aliasing network.
- Fix for night-time smear and flicker from Navi's glow, fireflies and dust when
  DLSS is on.
- Windows and Linux, standard and PGO.

## v15

- Adds NVIDIA DLSS Super Resolution for Ocarina of Time and task/mesh pipelines
  through compute on GPUs without `VK_EXT_mesh_shader`.
- PGO builds verified and kept for Windows and Linux.
- Windows and Linux, standard and PGO.

## v14

- The OoT grass/foliage fix is now upstream: Hyrule Field's grass stays dense and
  Castle Town's near hedges keep their leaves.
- Cooperative-matrix HMMA for OoT (+12–24% standing/walking, RDNA4 41 → 60 fps),
  float16 support for LSFG frame generation, Dynamic FPS down to 10 fps, a 50 µs
  GPU-thread poll, texture-cache out-of-memory handling and framebuffer limits.
- Windows and Linux, standard and PGO.

## v13

- In-place AppImage updates with one rollback copy.
- Windows and Linux, standard and PGO.

## v12

- The built-in updater now points at this repository and finds `Lichii-vN-…`
  releases.
- Added device-loss diagnostics and frame-generation status in the status bar.
- Windows packages use bundled static FFmpeg.
- Windows and Linux, standard and PGO.

## v11

- Upstream fixes for OoT far-field sky haze, NaN handling, QMD release formats,
  microphone capture and NUP metadata.
- Windows packages use bundled static FFmpeg.
- Windows and Linux, standard and PGO.

## v10

- 43 upstream commits: 64-byte usage tracking, 32-bit global atomics, dynamic
  per-tap offsets, macro hand-back, Turing macro RAM, gathered-buffer lifetime,
  address-wrap rejection and game-list crash fixes.
- Windows and Linux, standard and PGO.

## 261106-v9

- Reviewed candidate fixes: NaN-defined min/max, QMD release formats,
  framebuffer limits, texture-cache OOM retries and mesh-shader logging.
- Windows and Linux, standard and PGO.

## 261106-v8

- Upstream OoT split-deadlock fix, GPU back bookkeeping, quad index growth,
  paired cbuf pointer and CPU-write download guard.
- Windows and Linux, standard and PGO.

## 261106-v7

- Opt-in desktop compatibility reports and Linux AppImage switch discovery.
- Windows and Linux, standard and PGO.

## 261106-v6

- Ocarina of Time now asks on launch whether to use recommended performance
  settings (FSR3, GPU accuracy Low, GPU NVDEC, async presentation and async
  shaders). Session-only; saved preferences are unchanged. New desktop configs
  receive those values as first-run defaults.
- GPU thread wake from the sharkiix work.
- Windows and Linux, standard and PGO.

## 261106-v4

- Async native draw discard, worker lost-wake fix, misaligned SSBO and compute
  write ordering fixes.
- GPU thread wake: queued commands wake the GPU thread immediately.
- Windows and Linux, standard and PGO.

## 261106-v3

- Async native discard stops draws stalling on unbuilt pipelines.
- Windows and Linux, standard and PGO.

## 261106-v2

- First release with the full curated superbuild fix set: Bananza page-mark
  batching, OoT reticle, GPU split, Frizti ports, mja00 fixes and AMD fixes.
- Windows and Linux, standard and PGO.
