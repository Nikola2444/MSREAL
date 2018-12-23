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
	{ 0xeaf8b007, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x25cf29f4, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xfb939496, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x30e12a1, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x231a8687, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x9fa9f7dc, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x1371d7b0, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x79c5a9f0, __VMLINUX_SYMBOL_STR(ioremap) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x9416e1d8, __VMLINUX_SYMBOL_STR(__request_region) },
	{ 0xbe41f2c2, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xb64b084f, __VMLINUX_SYMBOL_STR(platform_get_resource) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x20000329, __VMLINUX_SYMBOL_STR(simple_strtoul) },
	{ 0x28cc25db, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0xcbc3f1f, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x77718060, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xaf26042f, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x90af9a4, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x85f74b00, __VMLINUX_SYMBOL_STR(iomem_resource) },
	{ 0x2ab3cc9d, __VMLINUX_SYMBOL_STR(__release_region) },
	{ 0xedc03953, __VMLINUX_SYMBOL_STR(iounmap) },
	{ 0x3cfc3e13, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x822137e2, __VMLINUX_SYMBOL_STR(arm_heavy_mb) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cdglnt,myled-1.00.a");
MODULE_ALIAS("of:N*T*Cdglnt,myled-1.00.aC*");
