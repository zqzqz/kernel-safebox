
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <linux/gfp.h>
#include <linux/err.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/highmem.h>

struct crypto_tfm *tfm;
#if 1
char *code = "Hello everyone,I'm Richardhesidu"
             "Hello everyone,I'm Richardhesidu"
             "Hello everyone,I'm Richardhesidu";
char *key = "00112233445566778899aabbccddeeff";
#endif



static int __init test_init(void)
{

    int ret, templen, keylen, codelen;
    struct scatterlist sg[1];
    char *result;
    char *temp;

    keylen = 16;
    codelen = strlen(code) / 2;

    /* Allocate transform for AES ECB mode */

    tfm = crypto_alloc_tfm("aes", CRYPTO_TFM_MODE_ECB);
    if (IS_ERR(tfm))
    {

        printk("<1>failed to load transform for aes ECB mode !\n");
        return 0;
    }
    ret = crypto_cipher_setkey(tfm, key, keylen);
    if (ret)
    {
        printk("<1>failed to setkey \n");
        goto failed1;
    }
    sg_init_one(sg, code, codelen);
    /* start encrypt */

    ret = crypto_cipher_encrypt(tfm, sg, sg, codelen);
    if (ret)
    {
        printk("<1>encrypt failed \n");
        goto failed1;
    }

    temp = kmap(sg[0].page) + sg[0].offset;
    /* start dencrypt */
    templen = strlen(temp) / 2;
    sg_init_one(sg, temp, templen);
    ret = crypto_cipher_decrypt(tfm, sg, sg, templen);
    if (ret)
    {
        printk("<1>dencrypt failed \n");
        goto failed1;
    }

    result = kmap(sg[0].page) + sg[0].offset;
    printk("<1>%s\n", result);

failed1:

    crypto_free_tfm(tfm);
    return 0;
}