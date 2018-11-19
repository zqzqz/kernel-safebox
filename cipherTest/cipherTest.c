
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <linux/gfp.h>
#include <linux/err.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/highmem.h>
 

/* tie all data structures together */
struct skcipher_def {
    struct scatterlist sg;
    struct crypto_skcipher *tfm;
    struct skcipher_request *req;
    struct crypto_wait wait;
};

/* Perform cipher operation */
static unsigned int test_skcipher_encdec(struct skcipher_def *sk,
                     int enc)
{
    int rc;

    if (enc)
        rc = crypto_wait_req(crypto_skcipher_encrypt(sk->req), &sk->wait);
    else
        rc = crypto_wait_req(crypto_skcipher_decrypt(sk->req), &sk->wait);

    if (rc)
            printk("skcipher encrypt returned with result %d\n", rc);

    return rc;
}

/* Initialize and trigger cipher operation */
static int test_skcipher(void)
{
    struct skcipher_def sk;
    struct crypto_skcipher *skcipher = NULL;
    struct skcipher_request *req = NULL;
    char *scratchpad = NULL;
    char *ivdata = NULL;
    unsigned char key[32];
    int ret = -EFAULT;

    skcipher = crypto_alloc_skcipher("cbc-aes-aesni", 0, 0);
    if (IS_ERR(skcipher)) {
        printk("could not allocate skcipher handle\n");
        return PTR_ERR(skcipher);
    }

    req = skcipher_request_alloc(skcipher, GFP_KERNEL);
    if (!req) {
        printk("could not allocate skcipher request\n");
        ret = -ENOMEM;
        goto out;
    }

    skcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
                      crypto_req_done,
                      &sk.wait);

    /* AES 256 with random key */
    get_random_bytes(&key, 32);
    if (crypto_skcipher_setkey(skcipher, key, 32)) {
        printk("key could not be set\n");
        ret = -EAGAIN;
        goto out;
    }

    /* IV will be random */
    ivdata = kmalloc(16, GFP_KERNEL);
    if (!ivdata) {
        printk("could not allocate ivdata\n");
        goto out;
    }
    get_random_bytes(ivdata, 16);

    /* Input data will be random */
    scratchpad = kmalloc(16, GFP_KERNEL);
    if (!scratchpad) {
        printk("could not allocate scratchpad\n");
        goto out;
    }
    get_random_bytes(scratchpad, 16);

    sk.tfm = skcipher;
    sk.req = req;

    /* We encrypt one block */
    sg_init_one(&sk.sg, scratchpad, 16);
    skcipher_request_set_crypt(req, &sk.sg, &sk.sg, 16, ivdata);
    crypto_init_wait(&sk.wait);

    /* encrypt data */
    ret = test_skcipher_encdec(&sk, 1);
    if (ret)
        goto out;

    printk("Encryption triggered successfully\n");

out:
    if (skcipher)
        crypto_free_skcipher(skcipher);
    if (req)
        skcipher_request_free(req);
    if (ivdata)
        kfree(ivdata);
    if (scratchpad)
        kfree(scratchpad);
    return ret;
}

static int __init test_init(void) {
    test_skcipher();
    return 0;
}

// struct crypto_tfm *tfm;
// #if 1
// char *code = "Hello everyone,I'm Richardhesidu"
//         "Hello everyone,I'm Richardhesidu"
//             "Hello everyone,I'm Richardhesidu";
// char *key = "00112233445566778899aabbccddeeff";
// #endif


// static inline  void hexdump(unsigned char *buf,unsigned int len) {
//     while(len--)printk("%02x",*buf++);
//     printk("\n");
// }

 

// static int __init test_init(void) {

//     int ret,templen,keylen,codelen;
//     struct scatterlist sg[1];
//     char *result;
//     char *temp;
 

//     keylen = 16;
//     codelen = strlen(code)/2;

//     /* Allocate transform for AES ECB mode */
  
//     tfm = crypto_alloc_tfm("aes",CRYPTO_TFM_MODE_ECB);
//         if(IS_ERR(tfm)) {

//         printk("<1>failed to load transform for aes ECB mode !\n");
//                 return 0;
//     }
//     ret = crypto_cipher_setkey(tfm,key,keylen);
//     if(ret) {
//         printk("<1>failed to setkey \n");
//         goto failed1;
//     }   
//     sg_init_one(sg,code,codelen);
//     /* start encrypt */

//     ret = crypto_cipher_encrypt(tfm,sg,sg,codelen);
//     if(ret) {
//         printk("<1>encrypt failed \n");
//         goto failed1;
//     }

//     temp = kmap(sg[0].page) + sg[0].offset;
//     hexdump(temp,sg[0].length);
//           /* start dencrypt */
//     templen = strlen(temp)/2;
//     sg_init_one(sg,temp,templen);
//     ret = crypto_cipher_decrypt(tfm,sg,sg,templen);
//         if(ret) {
//              printk("<1>dencrypt failed \n");
//              goto failed1;
//         }

//     result = kmap(sg[0].page) + sg[0].offset;
//     printk("<1>%s\n",result);
// //        hexdump(result,sg[0].length);

// failed1:

//            crypto_free_tfm(tfm);
//     return 0;
// }

static void __exit test_exit(void)
{

}

 

module_init(test_init);
module_exit(test_exit);
