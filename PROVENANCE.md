# Source provenance

Original local directories were left unchanged. This directory was seeded from
local Better Blur DX commit `7a0e3a6` (2.0.0), then the rendering, window-management,
configuration and build sources were updated from upstream's main branch,
reporting version 2.6.0:
https://github.com/xarblu/kwin-effects-better-blur-dx

GitHub DNS access from the shell was unavailable. Source was retrieved through
the web connector on 2026-11-05. A full tagged Git checkout and exact upstream
commit could not be obtained, so this is **not an exact, pinned upstream tree**.
Cached web files may come from different upstream revisions. In particular the
cache header was reconciled against the retrieved implementation, missing
utilities and the rounded-corner header were implemented against installed
KWin 6.8.2, and core shader variants were generated where needed. The refraction
core shader was recovered from the GitHub source view after the raw web response
flattened its newlines. Active shaders are compiled by the EGL test.

This fork intentionally limits its build to Wayland KWin 6.8.2. Obsolete 2.0
sources and packaging were removed. A future upstream rebase should use a normal
Git checkout, preserve these feature changes, and repeat compositor validation.
