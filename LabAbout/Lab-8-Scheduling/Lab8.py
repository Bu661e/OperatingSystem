import copy
import argparse
import random

class PCB:
    """
    进程控制块（PCB）结构
    """
    def __init__(self, name, pri, round_time, cpu_time=0, need_time=None, state='Ready', next=None):
        self.name = name      # 进程名
        self.pri = pri         # 进程优先数
        self.round_time = round_time  # 时间片轮转的轮转时间片
        self.cpu_time = cpu_time   # 已使用的CPU时间
        self.need_time = need_time if need_time is not None else int('50')  # 进程还需运行时间
        self.state = state      # 进程状态，默认为'Ready'
        self.next = next        # 链表指针


class ProcessQueue:
    """
    进程列表，存储就绪PCB和已完成的PCB
    """

    def __init__(self, status):
        self.head = None
        self.tail = None
        self.status = status

    def add_process_to_queue(self, pcb):
        pcb.next = None
        if not self.head:
            self.head = self.tail = pcb
        else:
            self.tail.next = pcb
            self.tail = pcb

    def display_process_ready_queue(self):
        current = self.head
        print("    ", end=" ")
        while current:
            print(f"{current.name} (Need_time: {current.need_time} pri: {current.pri})",  end=' -> ')
            current = current.next
        print('None')  # 表示队列的末尾

    def display_process_finished_queue(self):
        current = self.head
        print("    ", end=" ")
        while current:
            print(f"{current.name} (Cpu_time: {current.cpu_time} pri: {current.pri})", end=' -> ')
            current = current.next

        print('None')  # 表示队列的末尾


    # 我要写一个函数传入一个PCB对象，在队列中找到该对象，并把它放到队列的末尾
    def move_process_to_tail(self, pcb):
        if not self.head:
            return
        if self.tail == self.head:
            return
        current = self.head
        previous = None
        while current:
            if current.name == pcb.name:
                if current == self.tail:
                    return
                if previous:
                    previous.next = current.next
                else:
                    self.head = current.next
                if not self.head:
                    self.tail = None
                # 添加以下判断，防止在self.tail为None时访问self.tail.next
                if self.tail is not None:
                    self.tail.next = current
                self.tail = current
                current.next = None
                break  # 找到后跳出循环
            previous = current
            current = current.next


    # 我要写一个函数代替remove_process_from_queue函数,传入一个PCB对象，在队列中删除该元素
    def remove_process_from_queue(self, pcb):
        if not self.head:
            return None
        current = self.head
        previous = None
        while current:
            if current.name == pcb.name:
                if previous:
                    previous.next = current.next
                else:
                    self.head = current.next
                if not current.next:
                    self.tail = previous
                return current
            previous = current
            current = current.next


    # 我要写一个函数，找到队列中优先级最大的元素并返回
    def find_max_priority_process(self):
        if not self.head:
            return None
        max_priority = self.head.pri
        max_priority_process = self.head
        current = self.head
        while current:
            if current.pri > max_priority:
                max_priority = current.pri
                max_priority_process = current
            current = current.next
        return max_priority_process


def round_robin_scheduling(ready_queue, quantum, finished_queue):
    """
    时间片轮转调度算法实现
    :param ready_queue: 就绪队列（ProcessQueue实例）
    :param quantum: 时间片长度
    :param finished_queue: 完成队列（ProcessQueue实例）
    """
    print("\n--- Init ---\n")
    print("Ready Queue:")
    ready_queue.display_process_ready_queue()
    print("Finished Queue:")
    finished_queue.display_process_ready_queue()

    while ready_queue.head:  # 当就绪队列不为空时继续调度
        current_process = ready_queue.head

        # 模拟进程执行
        if current_process.need_time <= quantum:  # 如果剩余时间小于等于时间片
            current_process.cpu_time += current_process.need_time
            current_process.need_time = 0  # 设置进程完成
            current_process.state = 'Finished'
            ready_queue.remove_process_from_queue(current_process)
            # print(type(has_finished_process))
            finished_queue.add_process_to_queue(current_process)
        else:
            current_process.cpu_time += quantum
            current_process.need_time -= quantum  # 减少进程的剩余执行时间
            ready_queue.move_process_to_tail(current_process)

        print("\n--- Next Round ---\n")
        print("Ready Queue:")
        ready_queue.display_process_ready_queue()
        print("Finished Queue:")
        finished_queue.display_process_finished_queue()

def dynamic_priority_scheduling(ready_queue, quantum, finished_queue):
    while ready_queue.head:  # 当就绪队列不为空时继续调度
        current_process = ready_queue.find_max_priority_process()
        print(f"当前进程为：{current_process.name}")

        # 模拟进程执行
        if current_process.need_time <= quantum:  # 如果剩余时间小于等于时间片
            current_process.cpu_time += current_process.need_time
            current_process.need_time = 0  # 设置进程完成
            current_process.state = 'Finished'
            ready_queue.remove_process_from_queue(current_process)
            finished_queue.add_process_to_queue(current_process)
        else:
            current_process.cpu_time += quantum
            current_process.need_time -= quantum  # 减少进程的剩余执行时间
            ready_queue.move_process_to_tail(current_process)   # 本来就在队尾再移动到队尾就会抱错



        print("\n--- Next Round ---\n")
        print("Ready Queue:")
        ready_queue.display_process_ready_queue()
        print("Finished Queue:")
        finished_queue.display_process_finished_queue()

    print("\n--- Init ---\n")
    print("Ready Queue:")
    ready_queue.display_process_ready_queue()
    print("Finished Queue:")
    finished_queue.display_process_ready_queue()


if __name__ == "__main__":


    parser = argparse.ArgumentParser(description="Process scheduling simulation")
    parser.add_argument("-c", "--process_count", type=int, help="Number of processes")
    parser.add_argument("-s", "--scheduler", choices=["rr", "dp"],
                        help="Scheduling algorithm: Round Robin or Dynamic Priority")
    parser.add_argument("-q", "--quantum", type=int, help="Time of  slice")
    args = parser.parse_args()

    # 使用解析的参数
    process_count = args.process_count
    scheduler = args.scheduler
    quantum = args.quantum

    # 初始化就绪队列和完成队列
    ready_queue = ProcessQueue('Ready')
    finished_queue = ProcessQueue('Finished')
    i = 1
    for _ in range(args.process_count):
        pcb = PCB(f"P{i}", i, quantum, 0, random.randint(1, 100))
        ready_queue.add_process_to_queue(pcb)
        i += 1

    # 根据调度算法执行相应的逻辑
    if scheduler == "rr":
        round_robin_scheduling(ready_queue, quantum, finished_queue)
    elif scheduler == "dp":
        dynamic_priority_scheduling(ready_queue, quantum, finished_queue)
    else:
        print(f"Invalid scheduler: {scheduler}. Please choose 'rr' or 'dp'.")
