/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/
#ifndef DEMO_DEBUG_H
#define DEMO_DEBUG_H

#include <stdio.h>

#define ERROR(a,...) printf("%s(): "  a,__func__,## __VA_ARGS__)

#ifdef DEBUG
#define INFO(...) printf(__VA_ARGS__)
#else
#define INFO(...) {}
#endif

#endif