#pragma once

#ifndef seccomp
int seccomp(unsigned int operation, unsigned int flags, void *args);
#endif
