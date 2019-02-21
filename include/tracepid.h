/**
 * @file tracepid.h
 * 
 * @author Michele Campa aka s1ckb0y
 * @version 1.0
 * @date 2017-01-01
 */


#ifndef TRACEPID_H
#define TRACEPID_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>




#define BUILDMAPSPATH(x, len, pid) snprintf(x, len, "/proc/%d/maps", pid);

extern errno;

uint64_t getHeapAddr(uint64_t *, const pid_t);
int ptrace_attach(const pid_t);
int ptrace_detach(const pid_t);
int getHeapMem(const uint64_t, const uint64_t, const pid_t, uint64_t *);

#endif
