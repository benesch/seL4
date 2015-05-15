/*
 * Copyright 2015, Nikhil Benesch
 *
 * This file developed as part of schedul4.
 */

#ifndef __OBJECT_RUNQUEUE_H
#define __OBJECT_RUNQUEUE_H

#include <types.h>
// #include <api/failures.h>
// #include <object/structures.h>
// #include <arch/object/interrupt.h>
// #include <plat/machine.h>

exception_t decodeScheduleControlInvocation(word_t label, unsigned int length,
                                            extra_caps_t extraCaps, word_t *buffer);

exception_t decodeRunqueueInvocation(word_t label, unsigned int length, cap_t cap,
                                     extra_caps_t extraCaps, word_t *buffer);

#endif
