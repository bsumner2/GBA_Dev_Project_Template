# GBA_Dev_Project_Template

Skeleton project for GBA Development. Requirement: DevkitPro gba-dev tools.

## Important Note For Clangd Language Server Config (./.clangd)

In order for clangd to use arm-none-eabi-g(cc/++) for queries, you gotta make sure
that clangd is being initialized with *this flag:* 
<b>--query-driver=<i>\<path to devkitARM folder\></i>/bin/arm-none-eabi*</b>

If you're using Neovim and LSPConfig, this is pretty easy. Just add the 
following line to your lspconfig setup lua script.

```lua
local lspcfg = require('lspconfig')

lspcfg.clangd.setup({
  cmd = {
    'clangd', 
    '--query-driver=' .. (os.getenv('DEVKITARM')) .. '/bin/arm-none-eabi*'
  },
  --[[ Insert the rest of you clangd setup table fields here ]]--
})
```

Doing this is important because otherwise, not only will any standard library
header files used be pulled from your computer's native architecture stdlib 
includes <i>(e.g.: Headers pulled from /usr/include instead of from 
$DEVKITARM/arm-none-eabi/include)</i>, but additionally, sizeof(type) will be
evaluated based on your PC's native C compiler's type size specs instead of the
GBA-specific type sizes spec'd by the arm-none-eabi-g(cc/++) compilers.

#### I.E:

```c
// Without setting up clangd to be able to use arm-none-eabi-g(cc/++) when 
// prompted to by project-scoped clangd config, the clangd hoverdoc will tell 
// you that the values of all of these constants are 8. In reality, the compiler
// devkitARM uses for GBA development, arm-none-eabi-gcc, treats pointers and
// long integers as 4 bytes as would be expected for a 32b architecture, 
// and not the 8 bytes that would only be expected for 64b architecture.
static const size_t vplen = sizeof(void*);  // x64 gcc: 8 | ARM32 gcc: 4
static const size_t lilen = sizeof(long int);  // x64 gcc: 8 | ARM32 gcc: 4
```

## TODO List

1. Implement my own ISR Switchboard.
    - So I have shamelessly, "borrowed," the Libtonc IRQ handling implementation
        because I loved the whole switchboard design of it. I think it was a very 
        clever design, and I would love to expand on it; however, as of the current 
        moment, I can't say I have the strongest grasp of ARM assembly, and 
        unfortunately the way the IRQ switchboard works is that it is used as the 
        master ISR function that gets invoked whenever an interruption occurs and 
        this function then acts as the liaison, finding the highest-priority callback 
        in the ISR cb table that corresponds to the source of the IRQ. That being said,
        if I'm interpreting his documentation of it correctly, these callbacks 
        can also be interruptible (meta I know), but in order for these callbacks
        to, as well, be interruptible, we have to set the CPU back into system mode
        and that requires *direct* access to the stack pointer, which is impossible in C.

