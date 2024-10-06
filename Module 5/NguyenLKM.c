/**
 *
 * An LKM that displays all the necessary information about specified processes. 
 * 
 * Completion time: 180 minutes
 *
 * @author Matthew Nguyen
 * @version 1.0
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/list.h>   
#include <linux/moduleparam.h>

static int inp_pid = 1;
//Takes user input and stores it
module_param(inp_pid, int, 0664);

//This function is called to print out the important information about a process in the correct format
void printInfo(struct task_struct *task){
	printk(KERN_INFO "%-20s %-10d %-10d %-10d %-10d %-10d\n", task->comm, task->pid, task->__state, task->prio, task->static_prio, task->normal_prio);
}

/* This function is called when the module is loaded. */
int find_init(void)
{
	printk(KERN_INFO "Loading Module\n");
	struct task_struct *task;
	struct list_head *head;
	struct task_struct *child;

	//Finds each processs who has a pid greater than the input pid
	for_each_process(task){
		if(task->pid > inp_pid){
			//Formats the columns
			printk(KERN_INFO "%-20s %-10s %-10s %-10s %-10s %-10s\n", "Process Name", "PID", "State", "Priority", "Static", "Normal");

			//Prints info of the process
			printInfo(task);

			//Finds child processes (if any) and prints it
			list_for_each(head, &task->children){
				child = list_entry(head, struct task_struct, sibling);
				printk(KERN_INFO "CHILD\n");
				printInfo(child);
			}
	
			//Prints the parent process of the current process
			printk(KERN_INFO "PARENT\n");
			printInfo(task->real_parent);

			printk(KERN_INFO "\n");
		}
	}

	return 0;
}

/* This function is called when the module is removed. */
void find_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( find_init );
module_exit( find_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NguyenLKM Module");
MODULE_AUTHOR("SGG");