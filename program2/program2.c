#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/jiffies.h>
#include <linux/kmod.h>
#include <linux/fs.h>
#include <linux/signal.h>


MODULE_LICENSE("GPL");

struct wait_opts {
    enum pid_type wo_type; //It is defined in ‘/include/linux/pid.h’.
    int wo_flags; //Wait options. (0, WNOHANG, WEXITED, etc.)
    struct pid *wo_pid; //Kernel's internal notion of a process identifier. “Find_get_pid()”
    struct siginfo __user *wo_info; //Singal information.
    int __user *wo_stat; // Child process’s termination status
    struct rusage __user *wo_rusage; //Resource usage
    wait_queue_t child_wait; //Task wait queue
    int notask_error;
};

// extern shit
extern long do_wait(struct wait_opts *wo);
extern struct filename * getname(const char __user * filename);
extern long _do_fork(unsigned long clone_flags,unsigned long stack_start,unsigned long stack_size,int __user *parent_tidptr, int __user *child_tidptr,unsigned long tls);
extern int do_execve(struct filename *filename,const char __user *const __user *__argv, const char __user *const __user *__envp);

static struct task_struct *task;

int my_exec(void) {
    int result;
    const char path[] = "/mnt/hgfs/CSC3150/Project/CSC3150_Assignment_1/source/program2/test";
    const char *const argv[] = {path, NULL, NULL};
    const char *const envp[] = {"HOME=/", "PATH=/sbin:/user/sbin:/bin:/usr/bin", NULL};

    struct filename *my_filename = getname(path);

    /* execute a test program in child process */
    printk("[program2] : child process");

    result = do_execve(my_filename, argv, envp);

    if (!result) {
        return 0;
    } else {
        do_exit(result);
    }
}

int status;

void output_info(int exit){
    switch (exit) {
        case 1:
            printk("[program2] : get SIGHUP signal\n");
            printk("[program2] : child process is hung up\n");
            printk("[program2] : The return signal is 1\n");
            break;
        case 2:
            printk("[program2] : get SIGINT signal\n");
            printk("[program2] : terminal interrupt\n");
            printk("[program2] : The return signal is 2\n");
            break;
        case 131:
            printk("[program2] : get SIGQUIT signal\n");
            printk("[program2] : terminal quit\n");
            printk("[program2] : The return signal is 3\n");
            break;
        case 132:
            printk("[program2] : get SIGILL signal\n");
            printk("[program2] : child process has illegal instruction error\n");
            printk("[program2] : The return signal is 4\n");
            break;
        case 133:
            printk("[program2] : get SIGTRAP signal\n");
            printk("[program2] : child process has trap error\n");
            printk("[program2] : The return signal is 5\n");
            break;
        case 134:
            printk("[program2] : get SIGABRT signal\n");
            printk("[program2] : child process has abort error\n");
            printk("[program2] : The return signal is 6\n");
            break;
        case 135:
            printk("[program2] : get SIGBUS signal\n");
            printk("[program2] : child process has bus error\n");
            printk("[program2] : The return signal is 7\n");
            break;
        case 136:
            printk("[program2] : get SIGFPE signal\n");
            printk("[program2] : child process has float error\n");
            printk("[program2] : The return signal is 8\n");
            break;
        case 9:
            printk("[program2] : get SIGKILL signal\n");
            printk("[program2] : child process killed\n");
            printk("[program2] : The return signal is 9\n");
            break;
        case 139:
            printk("[program2] : get SIGSEGV signal\n");
            printk("[program2] : child process has segmentation fault error\n");
            printk("[program2] : The return signal is 11\n");
            break;
        case 13:
            printk("[program2] : get SIGPIPE signal\n");
            printk("[program2] : child process has pipe error\n");
            printk("[program2] : The return signal is 13\n");
            break;
        case 14:
            printk("[program2] : get SIGALARM signal\n");
            printk("[program2] : child process has alarm error\n");
            printk("[program2] : The return signal is 14\n");
            break;
        case 15:
            printk("[program2] : get SIGTERM signal\n");
            printk("[program2] : child process terminated\n");
            printk("[program2] : The return signal is 15\n");
            break;
        case 0:
            printk("[program2] : child process exit normally\n");
            printk("[program2] : The return signal is 0\n");
            break;
    }
    return;
}


void my_wait(pid_t pid) {
    struct wait_opts wo;
    struct pid *wo_pid = NULL;
    enum pid_type type;
    type = PIDTYPE_PID;
    wo_pid = find_get_pid(pid);

    wo.wo_type = type;
    wo.wo_pid = wo_pid;
    wo.wo_flags = WEXITED;
    wo.wo_info = NULL;
    wo.wo_stat = (int __user*)&status;
    wo.wo_rusage = NULL;

    int a;
    a = do_wait(&wo);

    output_info(status);


    put_pid(wo_pid);
    return;
}

//implement fork function
extern int my_fork(void *argc) {


    //set default sigaction for current process
    int i;
    struct k_sigaction *k_action = &current->sighand->action[0];
    for (i = 0; i < _NSIG; i++) {
        k_action->sa.sa_handler = SIG_DFL;
        k_action->sa.sa_flags = 0;
        k_action->sa.sa_restorer = NULL;
        sigemptyset(&k_action->sa.sa_mask);
        k_action++;
    }

    pid_t pid;
    /* fork a process using do_fork */
    pid = _do_fork(SIGCHLD, (unsigned long) &my_exec, 0, NULL, NULL, 0);

    printk("[program2] : The child process has pid= %d\n", pid);
    printk("[program2] : The parent process has pid= %d\n", (int) current->pid);

    my_wait(pid);

    return 0;
}

static int __init

program2_init(void) {

    printk("[program2] : Module init\n");

    /* write your code here */
    printk("[program2] : Module_init create kthread start\n");

    /* create a kernel thread to run my_fork */
//    task = kthread_create(&my_fork, NULL, "MyThread");
    task = kthread_create(&my_fork, NULL, "MyThread");
    //wake up new thread if ok
    if (!IS_ERR(task)) {
        printk("[program2] : Module_init kthread starts\n");
        wake_up_process(task);
    }


    return 0;
}

static void __exit

program2_exit(void) {
    printk("[program2] : Module exit\n");
}

module_init(program2_init);
module_exit(program2_exit);

