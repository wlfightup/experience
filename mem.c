/**
 * 连续用malloc分配10G内存，然后将前面的内存释放，留下最后一个内存块，最终会发现heap仍然是10G而不是1G
 * 连续用malloc分配10G内存，从后面往开始释放内存，会发现heap会在慢慢下降
 * 还有就是malloc后，竟然dirty也是满的，我都没有写0，竟然都dirty了，
 * 
 * 内存释放：
 * 当glibc发现堆顶有连续的128k的空间是空闲的时候，它就会通过brk或sbrk系统调用，来调整heap顶的位置，
 * 将占用的内存返回给系统。这时，内核会通过删除相应的线性区，来释放占用的物理内存。
 * 下面我要讲一个内存空洞的问题：
 * 一个场景，堆顶有一块正在使用的内存，而下面有很大的连续内存已经被释放掉了，那么这块内存是否能够被释放？其对应的物理内存是否能够被释放？
 * 很遗憾，不能。
 * 这也就是说，只要堆顶的部分申请内存还在占用，我在下面释放的内存再多，都不会被返回到系统中，仍然占用着物理内存。为什么会这样呢？
 * 这主要是与内核在处理堆的时候，过于简单，它只能通过调整堆顶指针的方式来调整调整程序占用的线性区；
 * 而又只能通过调整线性区的方式，来释放内存。所以只要堆顶不减小，占用的内存就不会释放。
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int **list;
int chunk = 1024;
int times = 1024 * 1024 * 10;

int main(int argc, char *argv) 
{
	list = (int **)malloc( sizeof(int)*times*2);
	int i;
	for(i=0; i < times; i++)
	{
		//printf("%d\n", i);
		list[i] = malloc(chunk);	
	}
	
	printf("times %d\n", times);

	printf("star free\n");
	//for(i=0; i < times-1; i++)
	//{
	//	free(list[i]);
	//}
	printf("end free\n");
	sleep(2);
	printf("terminal\n");
        free(list[i+1]);
	for(i=times; i>=0; i--) 
	{
		free(list[i]);
	}
	//free(list);
	while(1);
	return 1;
}