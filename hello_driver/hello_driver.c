// Copyright [2018] <nikola>
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");  //  govori da modul koristi besplatnu licencu

static int __init hello_init(void) {
   printk(KERN_ALERT "Hello, world\n");  //  printk je funkcija koja omogucava stampanje na konzoli, ona je neophodna jer drajver iz kernela nema pristup c bibliotekama.
                                        //KERN_ALERT je makro koji ispisuje prioritet poruke 
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
  //  exit i _init su makroi koji definisu ulogu fukcije
