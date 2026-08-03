#ifndef BUTTERBOTCTRL_FIRMWARE_LV_CONF_EXT_H
#define BUTTERBOTCTRL_FIRMWARE_LV_CONF_EXT_H

/*
 * Pulled into every LVGL translation unit via CONFIG_LV_ASSERT_HANDLER_INCLUDE
 * (see lv_assert.h, which includes this file right after lv_conf_internal.h).
 * Overrides lv_conf.h options that are not reachable through Kconfig when
 * CONFIG_LV_CONF_SKIP is set. The directory holding this header is added to
 * every component's include path in the top-level CMakeLists.txt.
 */

#include <stdlib.h>
#include <esp_heap_caps.h>

/*
 * LVGL's stock assert handler is `while(1);`. On an lv_malloc failure
 * (LV_USE_ASSERT_MALLOC is enabled) the LVGL task spins forever: the screen
 * and all input freeze, and the task watchdog only logs warnings since it is
 * not configured to panic. Abort instead, so the failure produces a backtrace
 * and the controller reboots rather than staying bricked until power-off.
 */
#undef LV_ASSERT_HANDLER
#define LV_ASSERT_HANDLER abort();

/*
 * Allocate the lv_malloc() pool (LV_MEM_SIZE) from PSRAM instead of a static
 * internal-RAM array. Used by lv_mem_core_builtin.c, which sees this macro
 * through its lv_assert.h include. The display canvas already lives in PSRAM,
 * so pool accesses are no slower than rendering itself, and this frees the
 * internal RAM the static pool used to occupy.
 */
#undef LV_MEM_POOL_ALLOC
#define LV_MEM_POOL_ALLOC(size) heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT)

#endif //BUTTERBOTCTRL_FIRMWARE_LV_CONF_EXT_H
