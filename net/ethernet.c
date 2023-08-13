#include <net/ethernet.h>
#include <kernel/printk.h>

static const char *eth_type(u16 type) {
  switch (type) {
    case 0x0806:
      return "ARP";
    case 0x0800:
      return "IPv4";
    case 0x86DD:
      return "IPv6";
    default:
      return "Unknown";
  }
}

void ethernet_parse(unsigned char *data, size_t size) {
  u16 type = (data[12] << 8) | data[13];

  printk(
    "Ethernet frame:\n"
    " desc: %02x:%02x:%02x:%02x:%02x:%02x\n"
    " src : %02x:%02x:%02x:%02x:%02x:%02x\n"
    " type: %04x (%s)\n"
    " data: ",
      data[0], data[1], data[2], data[3], data[4], data[5],
      data[6], data[7], data[8], data[9], data[10], data[11],
      type, eth_type(type)
  );

  for (size_t i = 14; i < size; i++) {
    size_t j = i - 14;
    if (j > 0 && j % 16 == 0) {
      printk("\n       ");
    }

    printk("%02x ", data[i]);
  }

  printk("\n\n");
}

