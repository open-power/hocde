/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/ppe_closed/sgpe/stop_gpe/p9_sgpe_irq.h $ */
/*                                                                        */
/* OpenPOWER HCODE Project                                                */
/*                                                                        */
/* COPYRIGHT 2015,2018                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2014
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file   p9_sgpe_irq.h
/// \brief  Shared and global definitions for SGPE H-codes.
/// \owner  Michael Olsen   Email: cmolsen@us.ibm.com
/// \owner  David Du        Email: daviddu@us.ibm.com

//-------------------------------------------------------------------//
//            DO NOT modify this file unless you're the owner        //
//-------------------------------------------------------------------//

// Notes:
// - The only define names that should be changed/added/removed
//   in this file are:
//   - IRQ_VEC_PRTY(n>0)_SGPE(x)
//   - IDX_PRTY_LVL_(task_abbr) and reflect in relevant H-code as well
//   - All other define names are used in H-codes
// - The variable names and actions in this file must perfectly match associated
//   definitions in p9_sgpe_irq.c

extern uint32_t G_OCB_OIMR0_CLR;
extern uint32_t G_OCB_OIMR0_OR;
extern uint32_t G_OCB_OIMR1_CLR;
extern uint32_t G_OCB_OIMR1_OR;

// Priority Levels
#define IDX_PRTY_LVL_HIPRTY         0
#define IDX_PRTY_LVL_IPI3_HIGH      1
#define IDX_PRTY_LVL_PIG_TYPE2      2
#define IDX_PRTY_LVL_PIG_TYPE6      3
#define IDX_PRTY_LVL_PIG_TYPE0      4
#define IDX_PRTY_LVL_PIG_TYPE3      5
#define IDX_PRTY_LVL_IPI3_LOW       6
#define IDX_PRTY_LVL_DISABLED       7
#define IDX_PRTY_VEC                0
#define IDX_MASK_VEC                1
#define NUM_EXT_IRQ_PRTY_LEVELS  (uint8_t)(8)
extern const uint64_t ext_irq_vectors_sgpe[NUM_EXT_IRQ_PRTY_LEVELS][2];

// Group0: Non-task hi-prty IRQs
#define IRQ_VEC_PRTY0_SGPE   (uint64_t)(0x0100800000000000)
// Group1: ipi3_high
#define IRQ_VEC_PRTY1_SGPE   (uint64_t)(0x0000000800000000)
// Group2: pig_type2
#define IRQ_VEC_PRTY2_SGPE   (uint64_t)(0x0000000000010000)
// Group3: pig_type6
#define IRQ_VEC_PRTY3_SGPE   (uint64_t)(0x0000000000001000)
// Group4: pig_type0
#define IRQ_VEC_PRTY4_SGPE   (uint64_t)(0x0000000000040000)
// Group5: pig_type3
#define IRQ_VEC_PRTY5_SGPE   (uint64_t)(0x0000000000008000)
// Group6: ipi3_low
#define IRQ_VEC_PRTY6_SGPE   (uint64_t)(0x0000000000000004)
// Group7: We should never detect these
#define OVERRIDE_OTHER_ENGINES_IRQS 0
#if OVERRIDE_OTHER_ENGINES_IRQS == 1
    #define IRQ_VEC_PRTY7_SGPE  (uint64_t)(0xFEFF7FF7FFFA6FFB) // Other instances' IRQs
#else
    #define IRQ_VEC_PRTY7_SGPE  (uint64_t)(0x0000000000000000) // Other instances' IRQs
#endif

#define IRQ_VEC_ALL_OUR_IRQS  ( IRQ_VEC_PRTY0_SGPE | \
                                IRQ_VEC_PRTY1_SGPE | \
                                IRQ_VEC_PRTY2_SGPE | \
                                IRQ_VEC_PRTY3_SGPE | \
                                IRQ_VEC_PRTY4_SGPE | \
                                IRQ_VEC_PRTY5_SGPE | \
                                IRQ_VEC_PRTY6_SGPE )      // Note, we do not incl PRTY7 here!

extern uint8_t       g_current_prty_level;
extern uint8_t       g_oimr_stack[NUM_EXT_IRQ_PRTY_LEVELS];
extern int           g_oimr_stack_ctr;
extern uint64_t      g_oimr_override_stack[NUM_EXT_IRQ_PRTY_LEVELS];
extern uint64_t      g_oimr_override;

/// Restore a vector of interrupts by overwriting EIMR.
UNLESS__PPE42_IRQ_CORE_C__(extern)
inline void
pk_irq_vec_restore(PkMachineContext* context)
{
    pk_critical_section_enter(context);

    if (g_oimr_stack_ctr >= 0)
    {

        out32(G_OCB_OIMR0_CLR, (uint32_t)((IRQ_VEC_ALL_OUR_IRQS |
                                           g_oimr_override_stack[g_oimr_stack_ctr]) >> 32));
        out32(G_OCB_OIMR1_CLR, (uint32_t)(IRQ_VEC_ALL_OUR_IRQS |
                                          g_oimr_override_stack[g_oimr_stack_ctr]));
        out32(G_OCB_OIMR0_OR,
              (uint32_t)((ext_irq_vectors_sgpe[g_oimr_stack[g_oimr_stack_ctr]][IDX_MASK_VEC] |
                          g_oimr_override) >> 32));
        out32(G_OCB_OIMR1_OR,
              (uint32_t)(ext_irq_vectors_sgpe[g_oimr_stack[g_oimr_stack_ctr]][IDX_MASK_VEC] |
                         g_oimr_override));

        // Restore the prty level tracker to the task that was interrupted, if any.
        g_current_prty_level = g_oimr_stack[g_oimr_stack_ctr];
        g_oimr_stack_ctr--;
    }
    else
    {
        PK_TRACE_ERR("ERROR: Messed up EIMR book keeping: g_oimr_stack_ctr=%d. HALT SGPE!",
                     g_oimr_stack_ctr);
        PK_PANIC(SGPE_UIH_EIMR_STACK_UNDERFLOW);
    }

    //pk_critical_section_exit(context);
}
