
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>

#include <kernel/syscall.h>
#include <kernel/driver.h>
#include <kernel/printk.h>
#include <kernel/vfs.h>

#include "api.h"
#include "process.h"
#include "interrupts.h"


extern void sh_task();


extern void init_mallocfs();
extern void init_minix();


extern struct driver tty_68681_driver;
extern struct vnode_ops mallocfs_vnode_ops;

struct driver *drivers[] = {
	&tty_68681_driver,
	NULL	// Null Termination of Driver List
};

extern struct process *current_proc;
extern void *current_proc_stack;


struct process *run_sh()
{
	int error = 0;

	struct process *proc = new_proc(SU_UID);
	if (!proc) {
		printk("Ran out of procs\n");
		return NULL;
	}

	current_proc = proc;

	// Open stdin
	int fd = do_open("tty", 0, 0);
	if (fd < 0) {
		printk("Error opening file tty %d\n", error);
		return NULL;
	}
	printk("FD: %d\n", fd);

	// Open stdout
	fd = do_open("tty", 0, 0);
	if (fd < 0) {
		printk("Error opening file tty %d\n", error);
		return NULL;
	}
	printk("FD: %d\n", fd);

	// Open stderr
	fd = do_open("tty", 0, 0);
	if (fd < 0) {
		printk("Error opening file tty %d\n", error);
		return NULL;
	}
	printk("FD: %d\n", fd);


	int stack_size = 0x2000;
	char *stack = malloc(stack_size);
	char *stack_p = stack + stack_size;
	printk("Sh Bottom: %x\n", stack);
	printk("Sh Stack: %x\n", stack_p);


 	stack_p = create_context(stack_p, sh_task);

	proc->map.segments[M_TEXT].base = NULL;
	proc->map.segments[M_TEXT].length = 0x10000;
	proc->map.segments[M_STACK].base = stack;
	proc->map.segments[M_STACK].length = stack_size;
	proc->sp = stack_p;

	printk("After: %x\n", stack_p);

	//dump(task, task_size);

	return proc;
}


int main()
{
	DISABLE_INTS();


	init_heap((void *) 0x110000, 0xD0000);

	init_interrupts();
	init_syscall();
	init_proc();

	init_vfs();
	init_mallocfs();

	// Initialize drivers
	for (char i = 0; drivers[i]; i++) {
		drivers[i]->init();
	}

	//init_minix();

	struct process *task = run_sh();

	//for (int i = 0; i < 0x2800; i++)
	//	asm volatile("");

	//printk("THINGS %x\n", current_proc);

	current_proc = task;
	current_proc_stack = task->sp;

	//panic("Panicking for good measure\n");

	// Start Multitasking
	asm("bra restore_context\n");
	//asm("stop #0x2700\n");


	// Force an address error
	//volatile uint16_t *data = (uint16_t *) 0x100001;
	//volatile uint16_t value = *data;

	//char *str = "Hey syscall, %d";
	//asm(
	//"move.l	#2, %%d0\n"
	//"move.l	%0, %%d1\n"
	//"move.l	#125, %%a0\n"
	//"trap	#1\n"
	//: : "r" (str)
	//);

}

