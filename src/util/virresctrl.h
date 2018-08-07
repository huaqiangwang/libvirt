/*
 * virresctrl.h:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef __VIR_RESCTRL_H__
# define __VIR_RESCTRL_H__

# include "internal.h"

# include "virbitmap.h"
# include "virutil.h"


typedef enum {
    VIR_CACHE_TYPE_BOTH,
    VIR_CACHE_TYPE_CODE,
    VIR_CACHE_TYPE_DATA,

    VIR_CACHE_TYPE_LAST
} virCacheType;

VIR_ENUM_DECL(virCache);
VIR_ENUM_DECL(virCacheKernel);


typedef struct _virResctrlInfoPerCache virResctrlInfoPerCache;
typedef virResctrlInfoPerCache *virResctrlInfoPerCachePtr;
struct _virResctrlInfoPerCache {
    /* Smallest possible increase of the allocation size in bytes */
    unsigned long long granularity;
    /* Minimal allocatable size in bytes (if different from granularity) */
    unsigned long long min;
    /* Type of the allocation */
    virCacheType scope;
    /* Maximum number of simultaneous allocations */
    unsigned int max_allocation;
};

typedef struct _virResctrlInfoMemBWPerNode virResctrlInfoMemBWPerNode;
typedef virResctrlInfoMemBWPerNode *virResctrlInfoMemBWPerNodePtr;
struct _virResctrlInfoMemBWPerNode {
    /* Smallest possible increase of the allocation bandwidth in percentage */
    unsigned int granularity;
    /* Minimal allocatable bandwidth in percentage */
    unsigned int min;
    /* Maximum number of simultaneous allocations */
    unsigned int max_allocation;
};

typedef struct _virResctrlInfo virResctrlInfo;
typedef virResctrlInfo *virResctrlInfoPtr;

virResctrlInfoPtr
virResctrlInfoNew(void);

int
virResctrlInfoGetCache(virResctrlInfoPtr resctrl,
                       unsigned int level,
                       unsigned long long size,
                       size_t *ncontrols,
                       virResctrlInfoPerCachePtr **controls);

int
virResctrlInfoGetMemoryBandwidth(virResctrlInfoPtr resctrl,
                                 unsigned int level,
                                 virResctrlInfoMemBWPerNodePtr control);
/* resource group -related things */
typedef struct _virResctrlGroup virResctrlGroup;
typedef virResctrlGroup *virResctrlGroupPtr;

typedef int virResctrlAllocForeachCacheCallback(unsigned int level,
                                                virCacheType type,
                                                unsigned int cache,
                                                unsigned long long size,
                                                void *opaque);

typedef int virResctrlAllocForeachMemoryCallback(unsigned int id,
                                                 unsigned int size,
                                                 void *opaque);

virResctrlGroup
virResctrlAllocNew(void);

bool
virResctrlAllocIsEmpty(virResctrlGroupPtr group);

int
virResctrlAllocSetCacheSize(virResctrlGroupPtr group,
                            unsigned int level,
                            virCacheType type,
                            unsigned int cache,
                            unsigned long long size);

int
virResctrlAllocForeachCache(virResctrlGroupPtr group,
                            virResctrlAllocForeachCacheCallback cb,
                            void *opaque);

int
virResctrlAllocSetMemoryBandwidth(virResctrlGroupPtr group,
                                  unsigned int id,
                                  unsigned int memory_bandwidth);

int
virResctrlAllocForeachMemory(virResctrlGroupPtr resctrl,
                             virResctrlAllocForeachMemoryCallback cb,
                             void *opaque);

int
virResctrlAllocSetID(virResctrlGroupPtr group,
                     const char *id);
const char *
virResctrlAllocGetID(virResctrlGroupPtr group);

char *
virResctrlAllocFormat(virResctrlGroupPtr group);

int
virResctrlAllocDeterminePath(virResctrlGroupPtr group,
                             const char *machinename);

int
virResctrlAllocCreate(virResctrlInfoPtr r_info,
                      virResctrlGroupPtr group,
                      const char *machinename);

int
virResctrlAllocAddPID(virResctrlGroupPtr group,
                      pid_t pid);

int
virResctrlAllocRemove(virResctrlGroupPtr group);

#endif /*  __VIR_RESCTRL_H__ */
