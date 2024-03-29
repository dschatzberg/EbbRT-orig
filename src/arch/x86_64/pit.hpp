#ifndef EBBRT_ARCH_X86_64_PIT_HPP
#define EBBRT_ARCH_X86_64_PIT_HPP

#include "arch/io.hpp"

namespace {
  const uint16_t PIT_CHANNEL_0 = 0x40;
  const uint16_t PIT_COMMAND = 0x43;
};

namespace ebbrt {
  namespace pit {
    inline void
    disable() {
      //set the counter to 0
      outb(PIT_CHANNEL_0, 0x00);
      //affecting channel 0, write to both hi/lo bytes, operate in mode
      //0, binary
      outb(PIT_COMMAND, 0x30);
    }
  }
}
#endif
