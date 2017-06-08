#include "coroutine.h"
#include <stdio.h>

struct args {
	int n;
};

static void
foo(struct schedule * S, void *ud) {
	struct args * arg = ud;
	int start = arg->n;
	int i;
	for (i=0;i<5;i++) {
		printf("coroutine %d : %d\n",coroutine_running(S) , start + i);
		coroutine_yield(S);		//将当前运行协程恢复到主协程
	}
}

static void
test(struct schedule *S) {
	struct args arg1 = { 0 };
	struct args arg2 = { 100 };

	int co1 = coroutine_new(S, foo, &arg1);		//创建协程,传给它调度器
	int co2 = coroutine_new(S, foo, &arg2);
	printf("main start\n");
	while (coroutine_status(S,co1) && coroutine_status(S,co2)) {	//查看协程状态
		//首先恢复协程1运行，协程1阶段性完成后回到主协程，恢复协程2运行，协程2阶段性完成后回到主协程继续循环
		coroutine_resume(S,co1);	//恢复协程函数
		coroutine_resume(S,co2);
	} 
	printf("main end\n");
}

int 
main() {
	struct schedule * S = coroutine_open();		//打开一个调度器
	test(S);
	coroutine_close(S);		//关闭调度器
	
	return 0;
}

