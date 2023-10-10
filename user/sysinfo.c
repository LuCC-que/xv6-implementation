#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int main(void) {
  struct sysinfo info;
  if (sysinfo(&info) < 0) {
    printf("FAIL: sysinfo failed\n");
    exit(1);
  }
  printf("freemem: %d\n", info.freemem);
  printf("nproc: %d\n", info.nproc);
  exit(0);
}