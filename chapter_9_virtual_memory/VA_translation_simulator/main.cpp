#include "MMU.hpp"
#include <bitset>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

// Helper function to print binary representation
void printBinary(uint16_t value, int bits = 16) {
  std::cout << std::bitset<16>(value);
}

// Helper to print address translation details
void printAddressTranslation(uint16_t virtual_addr, uint8_t vpn,
                             uint8_t offset) {
  std::cout << "\n┌─────────────────────────────────────────────────────┐\n";
  std::cout << "│          ADDRESS TRANSLATION DETAILS              │\n";
  std::cout << "├─────────────────────────────────────────────────────┤\n";
  std::cout << "│ Virtual Address:  0x" << std::hex << std::setw(4)
            << std::setfill('0') << virtual_addr << std::dec
            << std::setfill(' ') << "\n";
  std::cout << "│  Binary:          " << std::bitset<16>(virtual_addr) << "\n";
  std::cout << "│  VPN (upper 8):  0x" << std::hex << (int)vpn << std::dec
            << " (" << std::bitset<8>(vpn) << ")\n";
  std::cout << "│  Offset (lower 8):0x" << std::hex << (int)offset << std::dec
            << " (" << std::bitset<8>(offset) << ")\n";
  std::cout << "└─────────────────────────────────────────────────────┘\n";
}

// Structure to track access statistics
struct AccessStats {
  size_t total_accesses = 0;
  size_t tlb_hits = 0;
  size_t tlb_misses = 0;
  size_t page_faults = 0;
  size_t reads = 0;
  size_t writes = 0;

  double getHitRate() const {
    size_t total = tlb_hits + tlb_misses;
    if (total == 0)
      return 0.0;
    return (static_cast<double>(tlb_hits) / total) * 100.0;
  }
};

// Generate random virtual address with optional bias toward recent addresses
uint16_t generateRandomAddress(std::mt19937 &rng,
                               std::uniform_int_distribution<uint16_t> &dist) {
  return dist(rng);
}

// Print access result with detailed info
void printAccessResult(int access_num, uint16_t vaddr, uint8_t vpn,
                       uint8_t offset, bool is_read, bool success,
                       uint8_t value, const AccessStats &stats,
                       bool show_detail = false) {
  std::cout << "[" << std::setw(3) << access_num << "] ";
  std::cout << (is_read ? "READ" : "WRITE") << " 0x" << std::hex << std::setw(4)
            << std::setfill('0') << vaddr << std::dec << std::setfill(' ');
  std::cout << " | VPN=0x" << std::hex << (int)vpn << std::dec;
  std::cout << " Offset=0x" << std::hex << (int)offset << std::dec;

  if (!success) {
    std::cout << " | ❌ PAGE FAULT!";
  } else {
    std::cout << " | " << (is_read ? "Read" : "Wrote") << " 0x" << std::hex
              << (int)value << std::dec;
  }

  std::cout << " | TLB: ";
  if (stats.tlb_hits + stats.tlb_misses > 0) {
    // We need to check if this access was a hit or miss
    // This is a simplification - in real code you'd track per-access
  }
  std::cout << "\n";
}

int main() {
  std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
  std::cout << "║     VIRTUAL MEMORY SIMULATOR - 100 REQUESTS        ║\n";
  std::cout << "╚══════════════════════════════════════════════════════╝\n\n";

  // ============================================================
  // PART 1: Create MMU with configuration
  // ============================================================
  std::cout << "📋 MMU CONFIGURATION:\n";
  std::cout << "  • Virtual Pages:   256 (VPNs 0x00 - 0xFF)\n";
  std::cout << "  • Physical Frames: 64  (Frames 0 - 63)\n";
  std::cout << "  • Page Size:       256 bytes (8-bit VPN + 8-bit offset)\n";
  std::cout << "  • TLB Entries:     4\n\n";

  MMU mmu(256, 64, 256, 4);

  // ============================================================
  // PART 2: Setup page table mappings (OS does this)
  // ============================================================
  std::cout << "📝 PAGE TABLE SETUP (OS creates mappings):\n";

  // Map 20 random VPNs to frames (to have some variety)
  std::vector<uint8_t> mapped_vpns;
  std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<uint8_t> vpn_dist(0x00, 0xFF);
  std::uniform_int_distribution<uint8_t> frame_dist(0, 63);
  std::uniform_int_distribution<uint16_t> addr_dist(0x0000, 0xFFFF);

  // Map some initial VPNs
  for (int i = 0; i < 20; i++) {
    uint8_t vpn = vpn_dist(rng);
    uint8_t frame = frame_dist(rng);
    mmu.getPageTable().map(vpn, frame);
    mapped_vpns.push_back(vpn);
    std::cout << "  VPN 0x" << std::hex << (int)vpn << std::dec << " → Frame "
              << (int)frame << "\n";
  }
  std::cout << "\n";

  // ============================================================
  // PART 3: Write test data to physical memory
  // ============================================================
  std::cout << "💾 WRITING TEST DATA TO PHYSICAL MEMORY:\n";

  // Write data to some of the mapped pages
  for (size_t i = 0; i < std::min((size_t)10, mapped_vpns.size()); i++) {
    uint8_t vpn = mapped_vpns[i];
    uint8_t offset = vpn_dist(rng) & 0xFF; // Random offset
    uint16_t vaddr = (static_cast<uint16_t>(vpn) << 8) | offset;
    uint8_t data = static_cast<uint8_t>(vpn_dist(rng));
    mmu.write(vaddr, data, false);
    std::cout << "  Wrote 0x" << std::hex << (int)data << std::dec
              << " at VA 0x" << std::hex << vaddr << std::dec << " (VPN 0x"
              << std::hex << (int)vpn << std::dec << ", offset 0x" << std::hex
              << (int)offset << std::dec << ")\n";
  }
  std::cout << "\n";

  // ============================================================
  // PART 4: Generate 100 random CPU requests
  // ============================================================
  std::cout << "🔄 GENERATING 100 RANDOM CPU REQUESTS...\n\n";

  AccessStats stats;
  std::vector<uint16_t> addresses;
  std::vector<bool> is_reads;

  // Generate 100 random addresses with some locality (20% chance to repeat)
  for (int i = 0; i < 100; i++) {
    uint16_t addr;
    // 30% chance to repeat a recent address (simulating locality)
    if (!addresses.empty() &&
        std::uniform_int_distribution<int>(0, 100)(rng) < 30) {
      // Pick a random address from the last 10
      int idx = std::uniform_int_distribution<size_t>(
          0, std::min((size_t)9, addresses.size() - 1))(rng);
      addr = addresses[addresses.size() - 1 - idx];
    } else {
      addr = addr_dist(rng);
    }
    addresses.push_back(addr);

    // 60% reads, 40% writes
    is_reads.push_back(std::uniform_int_distribution<int>(0, 100)(rng) < 60);
  }

  // ============================================================
  // PART 5: Process all requests
  // ============================================================
  std::cout << "════════════════════════════════════════════════════════\n";
  std::cout << "  PROCESSING 100 REQUESTS\n";
  std::cout << "════════════════════════════════════════════════════════\n\n";

  // Track per-access TLB hit/miss (we need to check TLB state before each
  // access) This is a simplified tracking
  size_t access_hits = 0;
  size_t access_misses = 0;

  for (int i = 0; i < 100; i++) {
    uint16_t vaddr = addresses[i];
    uint8_t vpn = vaddr >> 8;
    uint8_t offset = vaddr & 0x00FF;
    bool is_read = is_reads[i];

    // Get current TLB stats before access to detect hit/miss
    size_t hits_before = mmu.getTlbHits();
    size_t misses_before = mmu.getTlbMisses();

    bool success;
    uint8_t value = 0;

    if (is_read) {
      success = mmu.read(vaddr, value, false);
      stats.reads++;
    } else {
      uint8_t write_data = static_cast<uint8_t>(vpn_dist(rng));
      success = mmu.write(vaddr, write_data, false);
      stats.writes++;
      value = write_data;
    }

    // Determine if this access was a hit or miss
    size_t hits_after = mmu.getTlbHits();
    size_t misses_after = mmu.getTlbMisses();

    if (hits_after > hits_before) {
      stats.tlb_hits++;
      access_hits++;
    } else if (misses_after > misses_before) {
      stats.tlb_misses++;
      access_misses++;
    }

    if (!success) {
      stats.page_faults++;
    }

    stats.total_accesses++;

    // Print every 5th access in detail, or all if fewer
    if (i % 5 == 0 || i == 99) {
      std::cout << "[" << std::setw(3) << (i + 1) << "] ";
      std::cout << (is_read ? "READ " : "WRITE") << " 0x" << std::hex
                << std::setw(4) << std::setfill('0') << vaddr << std::dec
                << std::setfill(' ');
      std::cout << " | VPN=0x" << std::hex << (int)vpn << std::dec;
      std::cout << " Offset=0x" << std::hex << (int)offset << std::dec;

      if (!success) {
        std::cout << " | ❌ PAGE FAULT!";
      } else {
        std::cout << " | " << (is_read ? "Read" : "Wrote") << " 0x" << std::hex
                  << std::setw(2) << std::setfill('0') << (int)value << std::dec
                  << std::setfill(' ');
      }

      // Show TLB result
      if (hits_after > hits_before) {
        std::cout << " | ✅ TLB HIT";
      } else if (misses_after > misses_before) {
        std::cout << " | ❌ TLB MISS";
      } else {
        std::cout << " | ⚠️  No TLB change";
      }

      std::cout << "\n";
    }

    // Print current stats every 20 accesses
    if ((i + 1) % 20 == 0) {
      std::cout << "\n📊 [Checkpoint] After " << (i + 1) << " accesses:\n";
      std::cout << "   Hits: " << stats.tlb_hits
                << ", Misses: " << stats.tlb_misses
                << ", Hit Rate: " << std::fixed << std::setprecision(1)
                << stats.getHitRate() << "%\n";
      std::cout << "   Page Faults: " << stats.page_faults << "\n\n";
    }
  }

  // ============================================================
  // PART 6: Final Summary
  // ============================================================
  std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
  std::cout << "║              FINAL SUMMARY                          ║\n";
  std::cout << "╚══════════════════════════════════════════════════════╝\n";

  std::cout << "\n📊 TOTAL STATISTICS:\n";
  std::cout << "  • Total Accesses:   " << stats.total_accesses << "\n";
  std::cout << "  • Reads:            " << stats.reads << "\n";
  std::cout << "  • Writes:           " << stats.writes << "\n";
  std::cout << "  • TLB Hits:         " << stats.tlb_hits << "\n";
  std::cout << "  • TLB Misses:       " << stats.tlb_misses << "\n";
  std::cout << "  • Hit Rate:         " << std::fixed << std::setprecision(2)
            << stats.getHitRate() << "%\n";
  std::cout << "  • Page Faults:      " << stats.page_faults << "\n";

  std::cout << "\n📈 PERFORMANCE ANALYSIS:\n";
  double miss_rate = 100.0 - stats.getHitRate();
  std::cout << "  • TLB accelerated " << stats.tlb_hits << " out of "
            << (stats.tlb_hits + stats.tlb_misses) << " translations\n";
  std::cout << "  • Page table walks avoided: " << stats.tlb_hits << "\n";
  std::cout << "  • Page table walks performed: " << stats.tlb_misses << "\n";

  if (stats.page_faults > 0) {
    std::cout << "  ⚠️  " << stats.page_faults
              << " page faults occurred (OS would load from disk)\n";
  }

  std::cout << "\n💡 KEY OBSERVATIONS:\n";
  if (stats.getHitRate() > 80) {
    std::cout
        << "  ✅ Excellent TLB hit rate! Good spatial/temporal locality.\n";
  } else if (stats.getHitRate() > 50) {
    std::cout << "  👍 Moderate TLB hit rate. Some locality present.\n";
  } else {
    std::cout << "  ⚠️  Low TLB hit rate. Consider increasing TLB size.\n";
  }

  std::cout << "\n📌 REAL SYSTEM COMPARISON:\n";
  std::cout << "  • Typical TLB hit rate in real systems: 95-99%\n";
  std::cout << "  • Your hit rate: " << std::fixed << std::setprecision(1)
            << stats.getHitRate() << "%\n";
  std::cout
      << "  • TLB size (4 entries) is very small vs real systems (32-64)\n";

  std::cout << "\n✅ Simulation complete!\n";

  return 0;
}