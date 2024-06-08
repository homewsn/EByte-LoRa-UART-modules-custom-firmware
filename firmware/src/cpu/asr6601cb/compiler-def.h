#ifndef COMPILER_DEF_H_
#define COMPILER_DEF_H_

//-----------------------------------------------------------------------------
// GNU ARM Embedded
// https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain
#if defined __GNUC__ && !defined __CC_ARM

#define enableInterrupts() __ASM volatile ("cpsie i" : : : "memory");
#define disableInterrupts() __ASM volatile ("cpsid i" : : : "memory");

//-----------------------------------------------------------------------------
// IAR EW ARM
// http://www.iar.com
#elif defined __ICCARM__

#define enableInterrupts() __iar_builtin_enable_interrupt()
#define disableInterrupts() __iar_builtin_disable_interrupt()

#else
#error "Unsupported compiler."

#endif

#endif // COMPILER_DEF_H_
