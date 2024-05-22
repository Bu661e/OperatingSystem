
#include <linux/bpf.h>
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/ip.h>

#define __section(NAME) \
    __attribute__((section(NAME), used))

// struct xdp_md *ctx 是XDP程序的上下文参数，包含了数据包的信息
__section("prog") int drop_tcp(struct xdp_md *ctx)
{
    // 获取数据包的起始位置
    void *data = (void *)(long)ctx->data;
    // 获取数据包的结束位置
    void *data_end = (void *)(long)ctx->data_end;

    // 检查以太网头部是否在数据包内
    if (data + sizeof(struct ethhdr) > data_end)
    {
        // XDP_ABORTED 是 XDP 程序返回的一种特殊值，表示程序由于某种错误而终止处理数据包
        return XDP_ABORTED;
    }

    // 定义一个指向IP头部的指针
    struct iphdr *ip;
    // struct ethhdr结构体来表示以太网帧的头部
    // 将指针 ip 移动到以太网头之后的位置，这里是IP头的开始位置
    ip = data + sizeof(struct ethhdr);

    // 检查IP头部是否在数据包内
    if ((void *)(ip + 1) > data_end)
    {
        return XDP_ABORTED;
    }

    // 检查IP头部中的协议字段是否为TCP协议
    if (ip->protocol == IPPROTO_TCP)
    {
        // 如果是TCP数据包，返回 XDP_drop，表示丢弃数据包
        return XDP_DROP;
    }
    // 如果不是TCP数据包，返回 XDP_PASS，表示允许数据包通过
    return XDP_PASS;
}

char _license[] __section("license") = "GPL";
