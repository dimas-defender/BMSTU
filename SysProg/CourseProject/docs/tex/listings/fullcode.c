#define pr_fmt(fmt) "my_hook: " fmt

#include <linux/init.h>
#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/syscalls.h>
#include <linux/kprobes.h>

MODULE_DESCRIPTION("Syscalls hook");
MODULE_AUTHOR("Dmitriy Zhabin");
MODULE_LICENSE("GPL");

/* Two ways of preventing recursive loops when hooking:
 * - detect recusion using function return address (USE_FENTRY_OFFSET = 0)
 * - avoid recusion by jumping over the ftrace call (USE_FENTRY_OFFSET = 1)*/
#define USE_FENTRY_OFFSET 0

struct ftrace_hook {
	const char *name;
	void *function;
	void *original;

	unsigned long address;
	struct ftrace_ops ops;
};

static unsigned long lookup_name(const char *name)
{
	struct kprobe kp = {
		.symbol_name = name
	};
	unsigned long retval;

	if (register_kprobe(&kp) < 0) return 0;
	retval = (unsigned long) kp.addr;
	unregister_kprobe(&kp);
	return retval;
}

static int resolve_hook_address(struct ftrace_hook *hook)
{
	hook->address = lookup_name(hook->name);

	if (!hook->address) {
		pr_debug("unresolved symbol: %s\n", hook->name);
		return -ENOENT;
	}

#if USE_FENTRY_OFFSET
	*((unsigned long*) hook->original) = hook->address + MCOUNT_INSN_SIZE;
#else
	*((unsigned long*) hook->original) = hook->address;
#endif

	return 0;
}

static void notrace ftrace_thunk(unsigned long ip, unsigned long parent_ip,
		struct ftrace_ops *ops, struct ftrace_regs *fregs)
{
	struct pt_regs *regs = ftrace_get_regs(fregs);
	struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);

#if USE_FENTRY_OFFSET
	regs->ip = (unsigned long)hook->function;
#else
	if (!within_module(parent_ip, THIS_MODULE))
		regs->ip = (unsigned long)hook->function;
#endif
}

int install_hook(struct ftrace_hook *hook)
{
	int err = resolve_hook_address(hook);
	
	if (err)
		return err;
	
	hook->ops.func = ftrace_thunk;
	hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS
	                | FTRACE_OPS_FL_RECURSION
	                | FTRACE_OPS_FL_IPMODIFY;

    /* enable ftrace for function */
	err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
	if (err)
	{
		pr_debug("ftrace_set_filter_ip() failed: %d\n", err);
		return err;
	}

    /* let ftrace invoke callback */
	err = register_ftrace_function(&hook->ops);
	if (err)
	{
		pr_debug("register_ftrace_function() failed: %d\n", err);
		ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
		return err;
	}

	return 0;
}

void remove_hook(struct ftrace_hook *hook)
{
	int err = unregister_ftrace_function(&hook->ops);
	if (err)
		pr_debug("unregister_ftrace_function() failed: %d\n", err);

	err = ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
	if (err)
		pr_debug("ftrace_set_filter_ip() failed: %d\n", err);
}

int install_hooks(struct ftrace_hook *hooks, size_t count)
{
	int err;
	size_t i;

	for (i = 0; i < count; i++)
	{
		err = install_hook(&hooks[i]);
		if (err)
			goto error;
	}
	return 0;

error:
	while (i > 0)
		remove_hook(&hooks[--i]);

	return err;
}

void remove_hooks(struct ftrace_hook *hooks, size_t count)
{
    size_t i;
	for (i = 0; i < count; i++)
		remove_hook(&hooks[i]);
}

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

#if !USE_FENTRY_OFFSET
#pragma GCC optimize("-fno-optimize-sibling-calls")
#endif

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*orig_sys_clone)(struct pt_regs *regs);

static asmlinkage long hook_sys_clone(struct pt_regs *regs)
{
	long ret;

	//pr_info("clone() before\n");

	ret = orig_sys_clone(regs);

	pr_info("clone(): %ld\n", ret);

	return ret;
}
#else
static asmlinkage long (*orig_sys_clone)(unsigned long clone_flags,
		unsigned long newsp, int __user *parent_tidptr,
		int __user *child_tidptr, unsigned long tls);

static asmlinkage long hook_sys_clone(unsigned long clone_flags,
		unsigned long newsp, int __user *parent_tidptr,
		int __user *child_tidptr, unsigned long tls)
{
	long ret;
	//pr_info("clone() before\n");

	ret = orig_sys_clone(clone_flags, newsp, parent_tidptr,
		child_tidptr, tls);

	pr_info("clone(): %ld\n", ret);

	return ret;
}
#endif

static char *duplicate_filename(const char __user *filename)
{
	char *kernel_filename = kmalloc(4096, GFP_KERNEL);
	if (!kernel_filename)
		return NULL;

	if (strncpy_from_user(kernel_filename, filename, 4096) < 0)
	{
		kfree(kernel_filename);
		return NULL;
	}
	return kernel_filename;
}

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*orig_sys_execve)(struct pt_regs *regs);

static asmlinkage long hook_sys_execve(struct pt_regs *regs)
{
	long ret;
	char *kernel_filename;

	kernel_filename = duplicate_filename((void*) regs->di);
	pr_info("execve(): %s\n", kernel_filename);

	kfree(kernel_filename);

	ret = orig_sys_execve(regs);
	//pr_info("execve(): %ld\n", ret);

	return ret;
}
#else
static asmlinkage long (*orig_sys_execve)(const char __user *filename,
		const char __user *const __user *argv,
		const char __user *const __user *envp);

static asmlinkage long hook_sys_execve(const char __user *filename,
		const char __user *const __user *argv,
		const char __user *const __user *envp)
{
	long ret;
	char *kernel_filename;

	kernel_filename = duplicate_filename(filename);
	pr_info("execve(): %s\n", kernel_filename);

	kfree(kernel_filename);

	ret = orig_sys_execve(filename, argv, envp);
	//pr_info("execve(): %ld\n", ret);
	
	return ret;
}
#endif

#define HOOK(_name, _function, _original)	\
	{					\
		.name = (_name),	\
		.function = (_function),	\
		.original = (_original),	\
	}
static struct ftrace_hook my_hooks[] = {
    HOOK("__x64_sys_clone",  hook_sys_clone,  &orig_sys_clone),
    HOOK("__x64_sys_execve", hook_sys_execve, &orig_sys_execve),
};

static int __init hook_module_init(void)
{
	int err = install_hooks(my_hooks, ARRAY_SIZE(my_hooks));
	if (err)
		return err;

	pr_info("my_hook module loaded\n");
	return 0;
}

static void __exit hook_module_exit(void)
{
	remove_hooks(my_hooks, ARRAY_SIZE(my_hooks));
	pr_info("my_hook module unloaded\n");
}

module_init(hook_module_init);
module_exit(hook_module_exit);
