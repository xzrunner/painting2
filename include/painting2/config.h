#pragma once

#define PT2_EDITOR

#ifdef PT2_EDITOR
#define PT2_FILTER_FULL
#endif // PT2_EDITOR

#ifdef NDEBUG
#define PT2_DISABLE_STATISTICS
#endif // NDEBUG

#define PT2_DISABLE_DEFERRED