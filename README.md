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
