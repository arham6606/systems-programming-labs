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
# Virtual Address Translation Simulator

A C++ simulator that models how a Memory Management Unit (MMU) translates virtual addresses into physical addresses using a Translation Lookaside Buffer (TLB), a page table, and simulated physical memory.

This project was built after studying the **Virtual Memory** chapter from *Computer Systems: A Programmer's Perspective (CS:APP)*. Rather than treating address translation as a static diagram, I wanted to implement the complete translation pipeline to better understand how these components interact.

---

## Why I Built This

Virtual memory is often introduced through diagrams that explain concepts such as virtual pages, page tables, TLBs, and physical memory. While those diagrams explain *what* happens, they don't provide the experience of implementing the translation process.

The goal of this project was to move beyond theory by building a simulator that models the behavior of a Memory Management Unit (MMU). Every memory access follows the same sequence used by the simulator, making the translation process easy to visualize and experiment with.

---

## Features

- Virtual to Physical Address Translation
- Translation Lookaside Buffer (TLB)
- TLB Hit and Miss Handling
- Page Table Lookup
- Page Fault Detection
- Simulated Physical Memory
- Read and Write Operations
- Configurable Memory Parameters
- Translation Statistics

---

# Architecture & Translation Flow

<p align="center">
  <img src="assets/VPN.jpg" alt="Virtual Address Translation Flow" width="900">
</p>

The simulator models the logical behavior of an MMU during address translation.

For each memory access:

- The CPU generates a virtual address.
- The MMU splits it into a Virtual Page Number (VPN) and Virtual Page Offset (VPO).
- The VPN is first searched in the TLB.
- On a TLB hit, the corresponding Physical Frame Number (PFN) is immediately returned.
- On a TLB miss, the page table is consulted.
- If a valid mapping exists, the PFN is returned and the TLB is updated.
- If no valid mapping exists, a page fault is reported.
- The PFN is combined with the unchanged offset to construct the physical address.
- The physical memory is then accessed for the requested read or write operation.

---

## Project Structure

```text
VirtualAddressTranslationSimulator/
│
├── include/
│   ├── MMU.h
│   ├── PageTable.h
│   ├── PhysicalMemory.h
│   └── TLB.h
│
├── src/
│   ├── MMU.cpp
│   ├── PageTable.cpp
│   ├── PhysicalMemory.cpp
│   └── TLB.cpp
│
├── main.cpp
├── CMakeLists.txt
└── README.md
```

---

## Building

Clone the repository.

```bash
git clone https://github.com/yourusername/VirtualAddressTranslationSimulator.git
```

Build the project.

```bash
mkdir build
cd build

cmake ..
make
```

Run the simulator.

```bash
./run.sh
```

---

## Design Decisions

This project intentionally models the **behavior** of an MMU instead of emulating a real processor architecture.

The simulator uses:

- A simplified page table
- A software-modeled TLB
- Simulated physical memory
- Configurable memory parameters

These design choices keep the implementation focused on understanding virtual address translation while avoiding architecture-specific details.

---

## Limitations

This simulator does **not** attempt to emulate:

- x86 page table formats
- Multi-level page tables
- CR3 register
- Hardware caches
- Kernel page fault handling
- Demand paging
- Disk-backed virtual memory
- Process context switching

Its purpose is to demonstrate the core translation algorithm used by a Memory Management Unit.

---

## References

- Randal E. Bryant
- David R. O'Hallaron

**Computer Systems: A Programmer's Perspective (3rd Edition)**

Chapter 9 — Virtual Memory