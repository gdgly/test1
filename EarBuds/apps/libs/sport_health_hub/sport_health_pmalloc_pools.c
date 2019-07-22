/* Copyright (c) 2018 Qualcomm Technologies International, Ltd. */
/*  */
/*!
  @file sport_health_pmalloc_pools.c
  @brief Creation of pmalloc pools used in Activity Monitoring configs only.
*/

#include "sport_health_hub_private.h"

#include <pmalloc.h>

_Pragma ("unitsuppress Unused")

_Pragma("datasection apppool") static const pmalloc_pool_config app_pools[] =
{
    {2048, 1}
};

/* The linker will discard any files in an archive library that do not contain
 * an externally-referenced symbol. This function is used to guarantee that the 
 * architecture-specific app_pools structure is included in the final executable
 * when needed.
 */
void sport_health_pmalloc_init(void)
{
    /* Do nothing */
}
