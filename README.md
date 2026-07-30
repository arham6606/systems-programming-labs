# Systems Programming Labs

A collection of small C++ projects built to understand how core operating system and hardware mechanisms actually work — by implementing them, not just reading about them.

Each project in this repo was built after studying a specific chapter of **Computer Systems: A Programmer's Perspective (CS:APP, 3rd Edition)** by Randal E. Bryant and David R. O'Hallaron. The goal isn't to reproduce production-grade systems — it's to strip each concept down to its core algorithm and implement it from scratch, so the underlying mechanism stops being a black box.

---

## Why This Repo Exists

Most systems concepts — virtual memory, address translation, dynamic allocation — are taught through diagrams and prose. That's enough to pass an exam, but it doesn't give you the experience of actually building the thing. Something always gets skipped over in a diagram: how the offset is preserved during translation, how a header hides itself from the caller, how a free block gets reclaimed and merged back in.

This repo is where I close that gap — by implementing the mechanisms directly, dry-running them, and testing them against edge cases, instead of trusting that I understood the diagram correctly.

---

## Projects

### 1. [Virtual Address Translation Simulator](chapter_9_virtual_memory/VA_translation_simulator/)

Models how a Memory Management Unit (MMU) translates virtual addresses into physical addresses using a TLB, a page table, and simulated physical memory. Covers the full pipeline: VPN/VPO splitting, TLB hit/miss handling, page table lookup, page fault detection, and physical address reconstruction.

### 2. [Toy Memory Allocator](chapter_9_virtual_memory/toy_malloc/)
A simplified implementation of the core algorithms behind `malloc()`: an implicit free list, First-Fit allocation, block splitting, and adjacent block coalescing. Focused on how a heap tracks free memory and reclaims it on `free()`, without relying on the system allocator.

---

## Common Design Philosophy

Both projects follow the same principles:

- **Model the algorithm, not the hardware.** Neither project emulates real x86 page table formats or glibc-level allocator internals — the point is to get the core logic right, not to compete with production systems.
- **No external frameworks.** Plain C++, close to the system, so nothing hides how the pieces fit together.
- **Built from the reference text.** Each project maps directly to a CS:APP chapter and intentionally scopes out anything beyond what that chapter covers (see each project's own README for exact limitations).

---

## Reference

- Randal E. Bryant, David R. O'Hallaron — **Computer Systems: A Programmer's Perspective (3rd Edition)**, Chapter 9 — Virtual Memory