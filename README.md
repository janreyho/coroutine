It's an asymmetric coroutine library (like lua).

You can use coroutine_open to open a schedule first, and then create coroutine in that schedule. 

You should call coroutine_resume in the thread that you call coroutine_open, and you can't call it in a coroutine in the same schedule.

Coroutines in the same schedule share the stack , so you can create many coroutines without worry about memory.

But switching context will copy the stack the coroutine used.

Read source for detail.

Chinese blog : http://blog.codingnow.com/2012/07/c_coroutine.html



# 使用方法

``` c
void foo(struct schedule * S, void *ud){
  printf(coroutine_running(S)) 	//协程
  coroutine_yield(S);			//将当前运行协程恢复到主协程
}

int main(){
  struct schedule * S = coroutine_open();		//打开一个调度器
  int co1 = coroutine_new(S, foo, &arg1);		//创建协程,传给它调度器
  while (coroutine_status(S,co1)) {				//查看协程状态
      coroutine_resume(S,co1);		//恢复协程函数
  coroutine_close(S);				//关闭调度器
}
```