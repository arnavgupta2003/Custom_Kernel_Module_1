#include<linux/module.h>
#include <asm/current.h>
#include<linux/sched/signal.h>
#include<linux/pid_namespace.h>
#include<linux/cdev.h>
#include<linux/proc_fs.h>
#include<linux/slab.h>


static int pid =0;
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arnav Gupta");
MODULE_DESCRIPTION("A Simple module to print the task_struct of a program corresponding to the given process.");

module_param(pid,int,0660);


struct task_struct *tocheck;
struct pid *pid_struct;

void code_memory(void){
  pid_struct=find_get_pid(pid);
  tocheck=pid_task(pid_struct,PIDTYPE_PID);
  struct task_struct *thechild = tocheck;
  int flag=0;
  for_each_process(thechild){

    if(thechild->pid==pid){
      printk(KERN_INFO "-------------------");
      printk(KERN_INFO "PID:          %d ",thechild->pid);
      printk(KERN_INFO "UID:          %d",thechild->cred->uid);
      printk(KERN_INFO "PGID:         %d",pid_vnr(task_pgrp(thechild)));
      printk(KERN_INFO "Command Path: %s",thechild->comm);
      printk(KERN_INFO "-------------------\n");
      flag=1;
      break;
    }
  }
  if(flag==0){
    printk(KERN_INFO "PID not found");
  }
}

int proc_init (void) {
  printk(KERN_INFO "\n\n");
  printk(KERN_INFO "--TASK DESC kernel Module--\n");
  printk(KERN_INFO "PID Recieved: %d",pid);
  code_memory();
  printk(KERN_INFO "\n\n");
  return 0;
}

void proc_cleanup(void) {
  printk(KERN_INFO "--TASK DESC kernel Module UNLOADED--\n");
}


module_init(proc_init);
module_exit(proc_cleanup);

