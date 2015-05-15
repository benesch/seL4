/*
 * Copyright 2015, Nikhil Benesch
 *
 * This file developed as part of schedul4.
 */

#include <assert.h>
#include <types.h>
#include <model/statedata.h>
#include <object/runqueue.h>

exception_t decodeScheduleControlInvocation(word_t label, unsigned int length,
                                            extra_caps_t extraCaps, word_t *buffer)
{
    cap_t runqueueCap;

    if (label != ScheduleControlStartRunqueue) {
        userError("Root runqueue cap: Illegal operation attempted.");
        current_syscall_error.type = seL4_IllegalOperation;
        return EXCEPTION_SYSCALL_ERROR;
    }

    if (extraCaps.excaprefs[0] == NULL) {
        current_syscall_error.type = seL4_TruncatedMessage;
        return EXCEPTION_SYSCALL_ERROR;
    }

    runqueueCap = extraCaps.excaprefs[0]->cap;

    if (cap_get_capType(runqueueCap) != cap_runqueue_cap) {
        userError("ScheduleControlStartRunqueue: provided cap is not a runqueue");
        current_syscall_error.type = seL4_InvalidCapability;
        current_syscall_error.invalidCapNumber = 0;
        return EXCEPTION_SYSCALL_ERROR;
    }

    ksRunqueue = RUNQUEUE_PTR(cap_runqueue_cap_get_capRunqueuePtr(runqueueCap));
    ksRunqueueIndex = 0;

    return EXCEPTION_NONE;
}

exception_t decodeRunqueueInvocation(word_t label, unsigned int length, cap_t cap,
                                     extra_caps_t extraCaps, word_t *buffer)
{
    cap_t tcbCap;
    tcb_t *tcb;
    runqueue_t *runqueue;
    uint32_t slot;

    if (label != RunqueueSetSlot) {
        userError("Runqueue cap: Illegal operation attempted.");
        current_syscall_error.type = seL4_IllegalOperation;
        return EXCEPTION_SYSCALL_ERROR;
    }

    if (extraCaps.excaprefs[0] == NULL) {
        current_syscall_error.type = seL4_TruncatedMessage;
        return EXCEPTION_SYSCALL_ERROR;
    }

    tcbCap = extraCaps.excaprefs[0]->cap;

    if (cap_get_capType(tcbCap) != cap_thread_cap) {
        userError("ScheduleControlStartRunqueue: provided cap is not a TCB");
        current_syscall_error.type = seL4_InvalidCapability;
        current_syscall_error.invalidCapNumber = 0;
        return EXCEPTION_SYSCALL_ERROR;
    }

    tcb = TCB_PTR(cap_thread_cap_get_capTCBPtr(tcbCap));
    runqueue = RUNQUEUE_PTR(cap_runqueue_cap_get_capRunqueuePtr(cap));

    slot = getSyscallArg(0, buffer);

    if (slot >= 1024) {
        userError("ScheduleControlStartRunqueue: slot too large");
        current_syscall_error.type = seL4_IllegalOperation;
        return EXCEPTION_SYSCALL_ERROR;
    }

    runqueue->runqueueSlots[slot] = tcb;

    return EXCEPTION_NONE;
}
