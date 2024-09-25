#include "platforms.h"
#include <stdio.h>

static bool _modern_platform_failed = false;

bool impl::is_modern_platform_failed()
{
    return _modern_platform_failed;
}

void impl::modern_platform_failed()
{
    fprintf(stderr, "Modern platform (GLFW / OPENGL3) failed. Fallback to legacy (SDL2 / SDL2 RENDERER)\n");
    _modern_platform_failed = true;
}