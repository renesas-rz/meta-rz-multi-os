/*
 * Copyright (c) 2016 Xilinx, Inc. All rights reserved.
 * Copyright (c) 2020, eForce Co., Ltd. 
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 ****************************************************************************
 * @par     History
 *          - rev 1.0 (2020.10.27) Imada
 *            Initial version for RZ/G2.
 ****************************************************************************
 */

#ifndef PLATFORM_INFO_H_
#define PLATFORM_INFO_H_

#ifndef RPMSG_REMOTE_CORE
#define RPMSG_REMOTE_CORE 0
#endif

#include <openamp/rpmsg.h>
#include <openamp/remoteproc.h>
#include "OpenAMP_RPMsg_cfg.h"

// Macros for printf
#define LPRINTF(format, ...) (printf(format, ##__VA_ARGS__))
#define LPERROR(format, ...) (LPRINTF("ERROR: " format, ##__VA_ARGS__))

// Page size on Linux (Default: 4KB)
#define PAGE_SIZE (0x01000U) // 4KB page size as the dafault value

// Mailbox config
#define MBX_DEV_NAME    "802a0000.mbox-uio"
// Select the TX and RX channel (must be different)
#define MBX_TX_CH       (0x0U) /* Maibox TX channel (0, 1, ..., or 15 this program uses */
#define MBX_RX_CH       (0x1U) /* Maibox RX channel (0, 1, ..., or 15 this program uses */

// INTER-CPU related macros
#define ICU_INTER_CPU_IRQ_NS_SWINT_MASK (0x3FFF) /* SWINT channel 0, 1,..., 13 */
#define ICU_INTER_CPU_IRQ_S_SWINT_MASK  (0xC000) /* SWINT channel 14, 15 */
#define ICU_INTER_CPU_IRQ_S_SWINT_SHIFT (14U)

#define ICU_INTER_CPU_IRQ_ID(ch)        (0 + ch) /* SPI of INTER-CPU channel */

// Index of IO region. 0: NS_SWINT, 1: S_SWINT
#define ICU_INTER_CPU_IO_INDEX(ch) (((1 << ch) & ICU_INTER_CPU_IRQ_NS_SWINT_MASK) ? (0x0U) : (0x1U))

#define ICU_INTER_CPU_REG_OFFSET(ch)      (0x0U) /* Offset from base register */
#define ICU_INTER_CPU_REG_WRITE_VALUE(ch) (((1 << ch) & ICU_INTER_CPU_IRQ_NS_SWINT_MASK) ? (1 << ch) : ((1 << ch) >> ICU_INTER_CPU_IRQ_S_SWINT_SHIFT))
#define ICU_INTER_CPU_MAP_SIZE            (0x4U)

// Core specific setting
#define DEV_BUS_NAME            "platform"

// Macros for mailboxes
// Mailbox interrupt ID
#define MBX_INT_NUM             (ICU_INTER_CPU_IRQ_ID(MBX_RX_CH))

// The number of maximum mailbox channels on the mailbox cluster
#define MBX_MAX_CH (0xFU)

// Index of MBX IO region
#define MBX_IO_INDEX(ch)        (ICU_INTER_CPU_IO_INDEX(ch))

#define MBX_TX_OFFSET(ch)       (ICU_INTER_CPU_REG_OFFSET(ch))
#define MBX_TX_WRITE_VALUE(ch)  (ICU_INTER_CPU_REG_WRITE_VALUE(ch))

#define MBX_REG_START   (0x0U)
#define MBX_REG_END     (ICU_INTER_CPU_MAP_SIZE)

// Shared memory config
#if (RPMSG_REMOTE_CORE == 0)
#define SHM_DEV_NAME    "3e0001000.intercpu-shm"
#elif (RPMSG_REMOTE_CORE == 1)	
#define SHM_DEV_NAME    "206001000.intercpu-shm"
#endif

// Macros for shared memory
#define SHM_TX_OFFSET(ch) (0x04U * ch)
#define SHM_RX_OFFSET(ch) (0x04U * ch)

// The number of maximum remoteproc vdevs
#define RPVDEV_MAX_NUM (MBX_MAX_CH)

// Macro used for translating addr from CR to CA
#if (RPMSG_REMOTE_CORE == 0)
#define ADDRESS_CR_DDR_BASE     (0xE0000000)
#define ADDRESS_CR_DDR_SIZE     (0x3FFFFFFFF)
#define ADDRESS_CA_DDR_BASE     (0x300000000)
#elif (RPMSG_REMOTE_CORE == 1)	
#define ADDRESS_CR_DDR_BASE     (0xC0000000)
#define ADDRESS_CR_DDR_SIZE     (0x2FFFFFFF)
#define ADDRESS_CA_DDR_BASE     (0x200000000)
#endif

struct ipi_info {
    const char *name;
    const char *bus_name;
    struct metal_device *dev;
    struct metal_io_region *io;
    uintptr_t irq_info;
    int registered;
    unsigned int mbx_chn[CFG_RPMSG_SVCNO];
    unsigned int chn_mask; /**< IPI channel mask */
#ifdef __linux__
    atomic_flag sync;
    uint32_t notify_id;
#else
    ID ipi_sem_id[CFG_RPMSG_SVCNO];
#endif
};

struct shm_info {
    const char *name;
    const char *bus_name;
    struct metal_device *dev; /**< pointer to shared memory device */
    struct metal_io_region *io; /**< pointer to shared memory i/o region */
    struct remoteproc_mem mem; /**< shared memory */
};

struct vring_info {
    struct shm_info rsc;
    struct shm_info ctl;
    struct shm_info shm;
};

struct remoteproc_priv {
    unsigned int notify_id;
    unsigned int mbx_chn_id;
    struct vring_info *vr_info;
};

/**
 * platform_init - initialize the platform
 *
 * It will initialize the platform.
 *
 * @proc_id: processor id
 * @rsc_id: resource id
 * @platform: pointer to store the platform data pointer
 *
 * return 0 for success or negative value for failure
 */
int platform_init(unsigned long proc_id, unsigned long rsc_id, void **platform);

/**
 * platform_create_rpmsg_vdev - create rpmsg vdev
 *
 * It will create rpmsg virtio device, and returns the rpmsg virtio
 * device pointer.
 *
 * @platform: pointer to the private data
 * @vdev_index: index of the virtio device, there can more than one vdev
 *              on the platform.
 * @role: virtio master or virtio slave of the vdev
 * @rst_cb: virtio device reset callback
 * @ns_bind_cb: rpmsg name service bind callback
 *
 * return pointer to the rpmsg virtio device
 */
struct rpmsg_device *
platform_create_rpmsg_vdev(void *platform, unsigned int vdev_index,
               unsigned int role,
               void (*rst_cb)(struct virtio_device *vdev),
               rpmsg_ns_bind_cb ns_bind_cb);

/**
 * platform_poll - platform poll function
 *
 * @platform: pointer to the platform
 *
 * return negative value for errors, otherwise 0.
 */
int platform_poll(void *platform);

/**
 * platform_release_rpmsg_vdev - release rpmsg virtio device
 *
 * @platform: pointer to the platform
 * @rpdev: pointer to the rpmsg device
 */
void platform_release_rpmsg_vdev(void *platform, struct rpmsg_device *rpdev);

/**
 * platform_cleanup - clean up the platform resource
 *
 * @platform: pointer to the platform
 */
void platform_cleanup(void *platform);

#endif /* PLATFORM_INFO_H_ */
