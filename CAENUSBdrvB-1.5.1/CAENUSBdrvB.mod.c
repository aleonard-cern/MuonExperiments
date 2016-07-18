#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x6c15661c, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x77477780, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0xe6d30606, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xcf2085cf, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x8ca05263, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xc5092e89, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0x9775d5f4, __VMLINUX_SYMBOL_STR(usb_register_dev) },
	{ 0xb4f22726, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x3a40aaab, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x68badb88, __VMLINUX_SYMBOL_STR(usb_deregister_dev) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x7b01d1d4, __VMLINUX_SYMBOL_STR(usb_clear_halt) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x714ad75e, __VMLINUX_SYMBOL_STR(usb_bulk_msg) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xb713117a, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x5f37af31, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x9e88526, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x78e739aa, __VMLINUX_SYMBOL_STR(up) },
	{ 0x6dc6dd56, __VMLINUX_SYMBOL_STR(down) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v0547p1002d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v21E1p0000d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v21E1p0001d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v21E1p0005d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "8D9E1150BB67F8447FD6216");
