#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <stdbool.h>
#include <syscall-nr.h>
#include <list.h>
#include <ctype.h>
#include <stdint.h>

typedef int pid_t;

void syscall_init (void);
void syscall_halt(void);
void syscall_exit(int status);
int syscall_open(const char * file);
bool syscall_create(const char *file, unsigned initial_size);
pid_t syscall_exec(const char *cmd_line);
int syscall_wait(pid_t pid);
bool syscall_remove(const char *file);
int syscall_filesize(int fd);
int syscall_read(int fd, void *buffer, unsigned size);
int syscall_write(int fd, const void *buffer, unsigned size);
void syscall_seek(int fd, unsigned position);
unsigned syscall_tell(int fd);
void syscall_close(int fd);


#endif /* userprog/syscall.h */
