/*#define _GNU_SOURCE
#define __need_IOV_MAX
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define LEN 2
#define TOTAL (1*1024*1024)
#define IOVECS IOV_MAX
int main() {
    char yes[LEN] = {'y', '\n'};
    char *buf = malloc(TOTAL);
    int bufused = 0;
    int i;
    struct iovec iov[IOVECS];
    while (bufused < TOTAL) {
        memcpy(buf+bufused, yes, LEN);
        bufused += LEN;
    }
    for (i = 0; i < IOVECS; i++) {
        iov[i].iov_base = buf;
        iov[i].iov_len = TOTAL;
    }
    while(vmsplice(1, iov, IOVECS, 0));
    return 1;
}*/


#include <stdio.h>
#include <fmt.hpp>

#include <string>

int main(int argc, char* argv[])
{
  if (argc != 2)
    return 1;

  uint32_t i{};
  const auto max = static_cast<uint32_t>(std::stoi(argv[1]));
  while(i < max)
  {
    fixed::fixed_uint32_t fi{i++};
    std::fwrite(
      fi.data(),
      sizeof(fixed::fixed_uint32_t::buff_value_t),
      fi.size(),
      stdout
    );
  }
}
