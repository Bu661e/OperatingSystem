#include <linux/bpf.h>


#define __section(NAME) \
 __attribute__((section(NAME), used))
__section("prog")
int drop_all(struct xdp_md *ctx)
{
 return XDP_DROP;
}
char __license[] __section("license") = "GPL";
