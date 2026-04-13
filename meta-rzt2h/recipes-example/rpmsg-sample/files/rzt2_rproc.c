/*
 * Copyright (c) 2014, Mentor Graphics Corporation
 * All rights reserved.
 * Copyright (c) 2017 Xilinx, Inc.
 * Copyright (c) 2020, eForce Co., Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**************************************************************************
 * FILE NAME
 *
 *       rzt2_rproc.c
 *
 * DESCRIPTION
 *
 *       This file defines RZ/G2 CA5X/CR7 remoteproc implementation.
 *
 * @par  History
 *       - rev 1.0 (2020.10.27) Imada
 *         Initial version.
 *
 **************************************************************************/

#include <metal/atomic.h>
#include <metal/assert.h>
#include <metal/device.h>
#include <metal/irq.h>
#include <metal/list.h>
#include <metal/utilities.h>
#include <openamp/rpmsg_virtio.h>
#include "platform_info.h"

extern struct ipi_info ipi;
extern struct shm_info shm;

/* Inline functions to add accessing address check to corresponding 
 * libmetal functions to avoid accessing a reserved region. 
 * They are mainly required because of larger (uio) mmap size due the 
 * 4KB page size on Verified Linux. They are also utilized for uC3 because 
 * accessing a reserved area of registers can cause system failure.
 */
static inline void metal_io_read32_with_check(struct metal_io_region *io, unsigned long offset, unsigned int *ret)
{
    if ((MBX_REG_START <= offset) && (offset < MBX_REG_END)) {
        *ret = metal_io_read32(io, offset);
    }
    else {
        LPERROR("Offset value larger than the register size in metal_io_read32.\n");
    }

    return ;
}

static inline void metal_io_write32_with_check(struct metal_io_region *io, unsigned long offset, uint64_t val)
{
    if ((MBX_REG_START <= offset) && (offset < MBX_REG_END)) {
        metal_io_write32(io, offset, val);
    }
    else {
        LPERROR("Offset value larger than the register size in metal_io_write32.\n");
    }
    return ;
}

static int init_memory_device(struct remoteproc *rproc, struct shm_info *info){
    struct metal_device *dev;
    metal_phys_addr_t mem_pa;
    int ret;

    ret = metal_device_open(info->bus_name, info->name, &dev);
    if (ret) {
        LPERROR("Failed to open uio device %s: %d.\n", info->name, ret);
        goto err;
    }
    LPRINTF("Successfully open uio device: %s.\n", info->name);
    
    info->dev = dev;
    info->io = metal_device_io_region(dev, 0x0U);
    if (!info->io) {
        goto err;
    }
    
    mem_pa = metal_io_phys(info->io, 0x0U);
    remoteproc_init_mem(&info->mem, info->name, mem_pa, mem_pa,
                metal_io_region_size(info->io),
                info->io);
    remoteproc_add_mem(rproc, &info->mem);
    LPRINTF("Successfully added memory device %s.\n", info->name);

    return 0;
err:
    if(info->dev)
        metal_device_close(info->dev);
    return ret;
}

static int rzt2_proc_irq_handler(int vect_id, void *data)
{
    unsigned int val = 0U;

    (void)vect_id;
    (void)data;

    /* Get a message from the mailbox */
    val = metal_io_read32(shm.io, SHM_RX_OFFSET(MBX_RX_CH));

    if (val >= RPVDEV_MAX_NUM) { /* val should have the notify_id of the sender */
        return METAL_IRQ_NOT_HANDLED; /* Invalid message arrived */
    }

#ifdef __linux__
    ipi.notify_id = val;
    atomic_flag_clear(&ipi.sync);
#else /* uC3 */
    if (ipi.ipi_sem_id[val] != E_ID) {
        isig_sem(ipi.ipi_sem_id[val]);
    }
    else
        return METAL_IRQ_NOT_HANDLED;
#endif

    return METAL_IRQ_HANDLED;
}

static int rzt2_enable_interrupt(struct remoteproc *rproc, struct metal_device *ipi_dev)
{
    unsigned int irq_vect;
    int ret = 0;

    if (!ipi.registered) {
        /* Register interrupt handler and enable interrupt for RZ/G2 CA5X or CR7 */
        irq_vect = (uintptr_t)ipi_dev->irq_info;
        ret = metal_irq_register((int)irq_vect, rzt2_proc_irq_handler, ipi_dev, rproc);
        if (ret) {
            LPRINTF("metal_irq_register() failed with %d", ret);
            return ret;
        }
        metal_irq_enable(irq_vect);
    }

    return ret;
}

static void rzt2_disable_interrupt(struct remoteproc *rproc)
{
    (void)rproc;
    struct metal_device *dev;
    int ret;

    dev = ipi.dev;
    if (dev) {
        metal_irq_disable((uintptr_t)dev->irq_info);
        ret = metal_irq_unregister((int)dev->irq_info, NULL, NULL, NULL);
        if (ret) {
            LPRINTF("metal_irq_unregister() failed with %d", ret);
            return ;
        }
        metal_device_close(dev);
        ipi.registered = 0;
    }
}

static struct remoteproc *
rzt2_proc_init(struct remoteproc *rproc,
            struct remoteproc_ops *ops, void *arg)
{
    struct remoteproc_priv *prproc = arg;
    struct metal_device *dev;
    int ret;

    if ((!rproc) || (!prproc) || (!ops))
        return NULL;
    rproc->priv = prproc;
    rproc->ops = ops;

    if (!ipi.registered) {
        /* Get an IPI device (Mailbox) */
        ret = metal_device_open(ipi.bus_name, ipi.name, &dev);
        if (ret) {
            LPERROR("Failed to open ipi device: %d.\n", ret);
            return NULL;
        }
        ipi.dev = dev;
        ipi.io = metal_device_io_region(dev, MBX_IO_INDEX(MBX_TX_CH));
        if (!ipi.io)
            goto err1;
#ifdef __linux__
        atomic_flag_test_and_set(&ipi.sync);
#endif
        LPRINTF("Successfully probed IPI device\n");

    }
    ret = rzt2_enable_interrupt(rproc, ipi.dev);
    if (ret) {
        LPERROR("Failed to register the interrupt handler.\n");
        goto err1;
    }
    ipi.registered++;

    /* Get the resource table device */
    if(init_memory_device(rproc, &prproc->vr_info->rsc))
        goto err1;
    /* Get VRING related devices */
    if(init_memory_device(rproc, &prproc->vr_info->ctl))
        goto err1;
    if(init_memory_device(rproc, &prproc->vr_info->shm))
        goto err1;
    /* Get shared memory device */
    if (init_memory_device(rproc, &shm))
        goto err1;

    return rproc;
err1:
    metal_device_close(ipi.dev);
    return NULL;
}

static void rzt2_proc_remove(struct remoteproc *rproc)
{
    if (!rproc)
        return;

    if (ipi.registered > 1) {
        ipi.registered--;
        return;
    }

    /* Disable interrupts */
    rzt2_disable_interrupt(rproc);

    return;
}

static int rzt2_proc_notify(struct remoteproc *rproc, uint32_t id)
{
    struct remoteproc_priv *prproc = rproc->priv;

    /* Put a message saying "This is the notify_id of mine!" */
    metal_io_write32(shm.io, SHM_TX_OFFSET(MBX_TX_CH), (uint64_t)prproc->notify_id);

    /* Send notification */
    metal_io_write32_with_check(ipi.io, MBX_TX_OFFSET(MBX_TX_CH), MBX_TX_WRITE_VALUE(MBX_TX_CH));

    return 0;
}

#ifdef __linux__
/* Inline funciton to translate DDR address from CR space to CA space */
static inline void rzt2_address_translate(metal_phys_addr_t *addr)
{
    if ((ADDRESS_CR_DDR_BASE <= *addr) && (*addr < (ADDRESS_CR_DDR_BASE + ADDRESS_CR_DDR_SIZE))) {
#if (RPMSG_REMOTE_CORE == 0)
		*addr = (*addr) + ADDRESS_CA_DDR_BASE;	
#elif (RPMSG_REMOTE_CORE == 1)	
        *addr = (*addr - ADDRESS_CR_DDR_BASE) + ADDRESS_CA_DDR_BASE;
#endif
    }
}

static void *
rzt2_proc_mmap(struct remoteproc *rproc,
            metal_phys_addr_t *pa, metal_phys_addr_t *da, size_t size,
            unsigned int attribute, struct metal_io_region **io)
{
    metal_phys_addr_t lpa, lda;
    struct metal_io_region *tmpio;
    struct remoteproc_priv *prproc;
    (void)attribute;
    (void)size;
    
    if (!rproc)
        return NULL;
    prproc = rproc->priv;

    rzt2_address_translate(pa);
    rzt2_address_translate(da);

    lpa = *pa;
    lda = *da;

    if (lpa == METAL_BAD_PHYS && lda == METAL_BAD_PHYS)
        return NULL;
    if (lpa == METAL_BAD_PHYS)
        lpa = lda;
    if (lda == METAL_BAD_PHYS)
        lda = lpa;
    tmpio = prproc->vr_info->ctl.io; /* We consider the resource table device only */
    if (!tmpio)
        return NULL;

    *pa = lpa;
    *da = lda;
    if (io)
        *io = tmpio;

    return metal_io_phys_to_virt(tmpio, lpa);
}
#endif

/* processor operations in rzt2. It defines
 * notification operation and remote processor managementi operations. */
struct remoteproc_ops rzt2_proc_ops = {
    .init = rzt2_proc_init,
    .remove = rzt2_proc_remove,
#ifdef __linux__
    .mmap = rzt2_proc_mmap,
#else
    .mmap = NULL,
#endif
    .notify = rzt2_proc_notify,
    .start = NULL,
    .stop = NULL,
    .shutdown = NULL,
};
