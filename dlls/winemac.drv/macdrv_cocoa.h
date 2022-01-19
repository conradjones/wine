/*
 * MACDRV Cocoa interface declarations
 *
 * Copyright 2011, 2012, 2013 Ken Thomases for CodeWeavers Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* This header serves as a C interface between the Wine parts of MACDRV
 * and the Objective-C parts.  It should restrict itself to C and the C-based
 * Mac APIs, avoiding both Wine and Objective-C/Cocoa features, so that it can
 * be included by both sides without contaminating either.
 */

#ifndef __WINE_MACDRV_COCOA_H
#define __WINE_MACDRV_COCOA_H


#define GetCurrentProcess MacGetCurrentProcess
#define GetCurrentThread MacGetCurrentThread
#define LoadResource MacLoadResource
#define AnimatePalette MacAnimatePalette
#define EqualRgn MacEqualRgn
#define FillRgn MacFillRgn
#define FrameRgn MacFrameRgn
#define GetPixel MacGetPixel
#define InvertRgn MacInvertRgn
#define LineTo MacLineTo
#define OffsetRgn MacOffsetRgn
#define PaintRgn MacPaintRgn
#define Polygon MacPolygon
#define ResizePalette MacResizePalette
#define SetRectRgn MacSetRectRgn
#define EqualRect MacEqualRect
#define FillRect MacFillRect
#define FrameRect MacFrameRect
#define GetCursor MacGetCursor
#define InvertRect MacInvertRect
#define OffsetRect MacOffsetRect
#define PtInRect MacPtInRect
#define SetCursor MacSetCursor
#define SetRect MacSetRect
#define ShowCursor MacShowCursor
#define UnionRect MacUnionRect

#include <ApplicationServices/ApplicationServices.h>

#undef GetCurrentProcess
#undef GetCurrentThread
#undef LoadResource
#undef AnimatePalette
#undef EqualRgn
#undef FillRgn
#undef FrameRgn
#undef GetPixel
#undef InvertRgn
#undef LineTo
#undef OffsetRgn
#undef PaintRgn
#undef Polygon
#undef ResizePalette
#undef SetRectRgn
#undef EqualRect
#undef FillRect
#undef FrameRect
#undef GetCursor
#undef InvertRect
#undef OffsetRect
#undef PtInRect
#undef SetCursor
#undef SetRect
#undef ShowCursor
#undef UnionRect

#include <pthread.h>


#ifndef DECLSPEC_HIDDEN
# if defined(_MSC_VER) || defined(__MINGW32__) || defined(__CYGWIN__)
#  define DECLSPEC_HIDDEN
# elif defined(__GNUC__) && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)))
#  define DECLSPEC_HIDDEN __attribute__((visibility ("hidden")))
# else
#  define DECLSPEC_HIDDEN
# endif
#endif


#include "macdrv_res.h"


/* Must match the values of Cocoa's NSDragOperation enum. */
enum {
    DRAG_OP_NONE    = 0,
    DRAG_OP_COPY    = 1,
    DRAG_OP_LINK    = 2,
    DRAG_OP_GENERIC = 4,
    DRAG_OP_PRIVATE = 8,
    DRAG_OP_MOVE    = 16,
    DRAG_OP_DELETE  = 32,
    DRAG_OP_EVERY   = UINT32_MAX
};

enum {
    TOPMOST_FLOAT_INACTIVE_NONE,
    TOPMOST_FLOAT_INACTIVE_NONFULLSCREEN,
    TOPMOST_FLOAT_INACTIVE_ALL,
};

enum {
    GL_SURFACE_IN_FRONT_OPAQUE,
    GL_SURFACE_IN_FRONT_TRANSPARENT,
    GL_SURFACE_BEHIND,
};

enum {
    MACDRV_HOTKEY_SUCCESS,
    MACDRV_HOTKEY_ALREADY_REGISTERED,
    MACDRV_HOTKEY_FAILURE,
};

typedef struct __TISInputSource *TISInputSourceRef;

typedef struct macdrv_opaque_window* macdrv_window;
typedef struct macdrv_opaque_event_queue* macdrv_event_queue;
typedef struct macdrv_opaque_view* macdrv_view;
typedef struct macdrv_opaque_opengl_context* macdrv_opengl_context;
#ifdef HAVE_METAL_METAL_H
typedef struct macdrv_opaque_metal_device* macdrv_metal_device;
typedef struct macdrv_opaque_metal_view* macdrv_metal_view;
typedef struct macdrv_opaque_metal_layer* macdrv_metal_layer;
#endif
typedef struct macdrv_opaque_status_item* macdrv_status_item;
struct macdrv_event;
struct macdrv_query;

struct macdrv_display {
    CGDirectDisplayID displayID;
    CGRect frame;
    CGRect work_frame;
};


/* main */
extern int macdrv_err_on;
extern int topmost_float_inactive DECLSPEC_HIDDEN;
extern int capture_displays_for_fullscreen DECLSPEC_HIDDEN;
extern int left_option_is_alt DECLSPEC_HIDDEN;
extern int right_option_is_alt DECLSPEC_HIDDEN;
extern int left_command_is_ctrl DECLSPEC_HIDDEN;
extern int right_command_is_ctrl DECLSPEC_HIDDEN;
extern int allow_immovable_windows DECLSPEC_HIDDEN;
extern int cursor_clipping_locks_windows DECLSPEC_HIDDEN;
extern int use_precise_scrolling DECLSPEC_HIDDEN;
extern int gl_surface_mode DECLSPEC_HIDDEN;
extern CFDictionaryRef localized_strings DECLSPEC_HIDDEN;
extern int retina_enabled DECLSPEC_HIDDEN;  /* Whether Retina mode is enabled via registry setting. */
extern int retina_on DECLSPEC_HIDDEN;       /* Whether Retina mode is currently active (enabled and display is in default mode). */
extern int enable_app_nap DECLSPEC_HIDDEN;

static inline CGRect cgrect_mac_from_win(CGRect rect)
{
    if (retina_on)
    {
        rect.origin.x /= 2;
        rect.origin.y /= 2;
        rect.size.width /= 2;
        rect.size.height /= 2;
    }

    return rect;
}

static inline CGRect cgrect_win_from_mac(CGRect rect)
{
    if (retina_on)
    {
        rect.origin.x *= 2;
        rect.origin.y *= 2;
        rect.size.width *= 2;
        rect.size.height *= 2;
    }

    return rect;
}

static inline CGSize cgsize_mac_from_win(CGSize size)
{
    if (retina_on)
    {
        size.width /= 2;
        size.height /= 2;
    }

    return size;
}

static inline CGSize cgsize_win_from_mac(CGSize size)
{
    if (retina_on)
    {
        size.width *= 2;
        size.height *= 2;
    }

    return size;
}

static inline CGPoint cgpoint_mac_from_win(CGPoint point)
{
    if (retina_on)
    {
        point.x /= 2;
        point.y /= 2;
    }

    return point;
}

static inline CGPoint cgpoint_win_from_mac(CGPoint point)
{
    if (retina_on)
    {
        point.x *= 2;
        point.y *= 2;
    }

    return point;
}

extern int macdrv_start_cocoa_app(unsigned long long tickcount) DECLSPEC_HIDDEN;
extern void macdrv_window_rejected_focus(const struct macdrv_event *event) DECLSPEC_HIDDEN;
extern void macdrv_beep(void) DECLSPEC_HIDDEN;
extern void macdrv_set_application_icon(CFArrayRef images) DECLSPEC_HIDDEN;
extern void macdrv_quit_reply(int reply) DECLSPEC_HIDDEN;
extern int macdrv_using_input_method(void) DECLSPEC_HIDDEN;
extern void macdrv_set_mouse_capture_window(macdrv_window window) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_retina_mode(int new_mode) DECLSPEC_HIDDEN;


/* cursor */
extern void macdrv_set_cursor(CFStringRef name, CFArrayRef frames) DECLSPEC_HIDDEN;
extern int macdrv_get_cursor_position(CGPoint *pos) DECLSPEC_HIDDEN;
extern int macdrv_set_cursor_position(CGPoint pos) DECLSPEC_HIDDEN;
extern int macdrv_clip_cursor(CGRect rect) DECLSPEC_HIDDEN;


/* display */

/* Used DISPLAY_DEVICE.StateFlags for adapters */
#define DISPLAY_DEVICE_ATTACHED_TO_DESKTOP      0x00000001
#define DISPLAY_DEVICE_PRIMARY_DEVICE           0x00000004
/* Used DISPLAY_DEVICE.StateFlags for monitors */
#define DISPLAY_DEVICE_ACTIVE                   0x00000001
#define DISPLAY_DEVICE_ATTACHED                 0x00000002

/* Represent a physical GPU in the PCI slots */
struct macdrv_gpu
{
    /* PCI GPU io registry entry id */
    uint64_t id;
    /* Name, in UTF-8 encoding */
    char name[128];
    /* PCI ID */
    uint32_t vendor_id;
    uint32_t device_id;
    uint32_t subsys_id;
    uint32_t revision_id;
};

/* Represent an adapter in EnumDisplayDevices context */
struct macdrv_adapter
{
    /* ID to uniquely identify an adapter. Currently it's a CGDirectDisplayID */
    uint32_t id;
    /* as StateFlags in DISPLAY_DEVICE struct */
    uint32_t state_flags;
};

/* Represent a monitor in EnumDisplayDevices context */
struct macdrv_monitor
{
    /* Name, in UTF-8 encoding */
    char name[128];
    /* as RcMonitor in MONITORINFO struct after conversion by rect_from_cgrect */
    CGRect rc_monitor;
    /* as RcWork in MONITORINFO struct after conversion by rect_from_cgrect */
    CGRect rc_work;
    /* StateFlags in DISPLAY_DEVICE struct */
    uint32_t state_flags;
};

extern int macdrv_get_displays(struct macdrv_display** displays, int* count) DECLSPEC_HIDDEN;
extern void macdrv_free_displays(struct macdrv_display* displays) DECLSPEC_HIDDEN;
extern int macdrv_set_display_mode(const struct macdrv_display* display,
                                   CGDisplayModeRef display_mode) DECLSPEC_HIDDEN;
extern int macdrv_get_gpus(struct macdrv_gpu** gpus, int* count) DECLSPEC_HIDDEN;
extern void macdrv_free_gpus(struct macdrv_gpu* gpus) DECLSPEC_HIDDEN;
extern int macdrv_get_adapters(uint64_t gpu_id, struct macdrv_adapter** adapters, int* count) DECLSPEC_HIDDEN;
extern void macdrv_free_adapters(struct macdrv_adapter* adapters) DECLSPEC_HIDDEN;
extern int macdrv_get_monitors(uint32_t adapter_id, struct macdrv_monitor** monitors, int* count) DECLSPEC_HIDDEN;
extern void macdrv_free_monitors(struct macdrv_monitor* monitors) DECLSPEC_HIDDEN;


/* event */
enum {
    APP_ACTIVATED,
    APP_DEACTIVATED,
    APP_QUIT_REQUESTED,
    DISPLAYS_CHANGED,
    HOTKEY_PRESS,
    IM_SET_TEXT,
    KEY_PRESS,
    KEY_RELEASE,
    KEYBOARD_CHANGED,
    LOST_PASTEBOARD_OWNERSHIP,
    MOUSE_BUTTON,
    MOUSE_MOVED,
    MOUSE_MOVED_ABSOLUTE,
    MOUSE_SCROLL,
    QUERY_EVENT,
    QUERY_EVENT_NO_PREEMPT_WAIT,
    REASSERT_WINDOW_POSITION,
    RELEASE_CAPTURE,
    SENT_TEXT_INPUT,
    STATUS_ITEM_MOUSE_BUTTON,
    STATUS_ITEM_MOUSE_MOVE,
    WINDOW_BROUGHT_FORWARD,
    WINDOW_CLOSE_REQUESTED,
    WINDOW_DID_MINIMIZE,
    WINDOW_DID_UNMINIMIZE,
    WINDOW_DRAG_BEGIN,
    WINDOW_DRAG_END,
    WINDOW_FRAME_CHANGED,
    WINDOW_GOT_FOCUS,
    WINDOW_LOST_FOCUS,
    WINDOW_MAXIMIZE_REQUESTED,
    WINDOW_MINIMIZE_REQUESTED,
    WINDOW_RESIZE_ENDED,
    WINDOW_RESTORE_REQUESTED,
    NUM_EVENT_TYPES
};

enum {
    QUIT_REASON_NONE,
    QUIT_REASON_LOGOUT,
    QUIT_REASON_RESTART,
    QUIT_REASON_SHUTDOWN,
};

typedef uint64_t macdrv_event_mask;

typedef struct macdrv_event {
    int                 refs;
    int                 deliver;
    int                 type;
    macdrv_window       window;
    union {
        struct {
            int reason;
        }                                           app_quit_requested;
        struct {
            int activating;
        }                                           displays_changed;
        struct {
            unsigned int    vkey;
            unsigned int    mod_flags;
            unsigned int    keycode;
            unsigned long   time_ms;
        }                                           hotkey_press;
        struct {
            void           *data;
            CFStringRef     text;       /* new text or NULL if just completing existing text */
            unsigned int    cursor_pos;
            unsigned int    complete;   /* is completing text? */
        }                                           im_set_text;
        struct {
            CGKeyCode                   keycode;
            CGEventFlags                modifiers;
            unsigned long               time_ms;
        }                                           key;
        struct {
            CFDataRef                   uchr;
            CGEventSourceKeyboardType   keyboard_type;
            int                         iso_keyboard;
            TISInputSourceRef           input_source;
        }                                           keyboard_changed;
        struct {
            int             button;
            int             pressed;
            int             x;
            int             y;
            unsigned long   time_ms;
        }                                           mouse_button;
        struct {
            int             x;
            int             y;
            int             drag;
            unsigned long   time_ms;
        }                                           mouse_moved;
        struct {
            int             x_scroll;
            int             y_scroll;
            int             x;
            int             y;
            unsigned long   time_ms;
        }                                           mouse_scroll;
        struct {
            struct macdrv_query *query;
        }                                           query_event;
        struct {
            int handled;
            int *done;
        }                                           sent_text_input;
        struct {
            macdrv_status_item  item;
            int                 button;
            int                 down;
            int                 count;
            int                 x;
            int                 y;
        }                                           status_item_mouse_button;
        struct {
            macdrv_status_item  item;
            int                 x;
            int                 y;
        }                                           status_item_mouse_move;
        struct {
            int no_activate;
        }                                           window_drag_begin;
        struct {
            CGRect  frame;
            int     fullscreen;
            int     in_resize;
            int     skip_size_move_loop;
        }                                           window_frame_changed;
        struct {
            unsigned long   serial;
            void           *tried_windows;
        }                                           window_got_focus;
        struct {
            int     keep_frame;
            CGRect  frame;
        }                                           window_restore_requested;
    };
} macdrv_event;

enum {
    QUERY_DRAG_DROP,
    QUERY_DRAG_EXITED,
    QUERY_DRAG_OPERATION,
    QUERY_IME_CHAR_RECT,
    QUERY_PASTEBOARD_DATA,
    QUERY_RESIZE_SIZE,
    QUERY_RESIZE_START,
    QUERY_MIN_MAX_INFO,
    NUM_QUERY_TYPES
};

typedef struct macdrv_query {
    int                 refs;
    int                 type;
    macdrv_window       window;
    int                 status;
    int                 done;
    union {
        struct {
            int                 x;
            int                 y;
            uint32_t            op;
            CFTypeRef           pasteboard;
        }                                           drag_drop;
        struct {
            int                 x;
            int                 y;
            uint32_t            offered_ops;
            uint32_t            accepted_op;
            CFTypeRef           pasteboard;
        }                                           drag_operation;
        struct {
            void   *data;
            CFRange range;
            CGRect  rect;
        }                                           ime_char_rect;
        struct {
            CFStringRef type;
        }                                           pasteboard_data;
        struct {
            CGRect          rect;
            unsigned int    from_left : 1;
            unsigned int    from_top : 1;
        }                                           resize_size;
    };
} macdrv_query;

static inline macdrv_event_mask event_mask_for_type(int type)
{
    return ((macdrv_event_mask)1 << type);
}

typedef void (*macdrv_event_handler)(const macdrv_event *event);

extern macdrv_event_queue macdrv_create_event_queue(macdrv_event_handler handler) DECLSPEC_HIDDEN;
extern void macdrv_destroy_event_queue(macdrv_event_queue queue) DECLSPEC_HIDDEN;
extern int macdrv_get_event_queue_fd(macdrv_event_queue queue) DECLSPEC_HIDDEN;

extern int macdrv_copy_event_from_queue(macdrv_event_queue queue,
        macdrv_event_mask mask, macdrv_event **event) DECLSPEC_HIDDEN;
extern void macdrv_release_event(macdrv_event *event) DECLSPEC_HIDDEN;

extern macdrv_query* macdrv_create_query(void) DECLSPEC_HIDDEN;
extern macdrv_query* macdrv_retain_query(macdrv_query *query) DECLSPEC_HIDDEN;
extern void macdrv_release_query(macdrv_query *query) DECLSPEC_HIDDEN;
extern void macdrv_set_query_done(macdrv_query *query) DECLSPEC_HIDDEN;
extern int macdrv_register_hot_key(macdrv_event_queue q, unsigned int vkey, unsigned int mod_flags,
                                   unsigned int keycode, unsigned int modifiers) DECLSPEC_HIDDEN;
extern void macdrv_unregister_hot_key(macdrv_event_queue q, unsigned int vkey, unsigned int mod_flags) DECLSPEC_HIDDEN;


/* window */
struct macdrv_window_features {
    unsigned int    title_bar:1;
    unsigned int    close_button:1;
    unsigned int    minimize_button:1;
    unsigned int    resizable:1;
    unsigned int    maximize_button:1;
    unsigned int    utility:1;
    unsigned int    shadow:1;
    unsigned int    prevents_app_activation:1;
};

struct macdrv_window_state {
    unsigned int    disabled:1;
    unsigned int    no_foreground:1;
    unsigned int    floating:1;
    unsigned int    excluded_by_expose:1;
    unsigned int    excluded_by_cycle:1;
    unsigned int    minimized:1;
    unsigned int    minimized_valid:1;
    unsigned int    maximized:1;
};

extern macdrv_window macdrv_create_cocoa_window(const struct macdrv_window_features* wf,
        CGRect frame, void* hwnd, macdrv_event_queue queue) DECLSPEC_HIDDEN;
extern void macdrv_destroy_cocoa_window(macdrv_window w) DECLSPEC_HIDDEN;
extern void* macdrv_get_window_hwnd(macdrv_window w) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_window_features(macdrv_window w,
        const struct macdrv_window_features* wf) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_window_state(macdrv_window w,
        const struct macdrv_window_state* state) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_window_title(macdrv_window w, const UniChar* title,
        size_t length) DECLSPEC_HIDDEN;
extern void macdrv_order_cocoa_window(macdrv_window w, macdrv_window prev,
        macdrv_window next, int activate) DECLSPEC_HIDDEN;
extern void macdrv_hide_cocoa_window(macdrv_window w) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_window_frame(macdrv_window w, const CGRect* new_frame) DECLSPEC_HIDDEN;
extern void macdrv_get_cocoa_window_frame(macdrv_window w, CGRect* out_frame) DECLSPEC_HIDDEN;
extern void macdrv_set_cocoa_parent_window(macdrv_window w, macdrv_window parent) DECLSPEC_HIDDEN;
extern void macdrv_set_window_surface(macdrv_window w, void *surface, pthread_mutex_t *mutex) DECLSPEC_HIDDEN;
extern CGImageRef create_surface_image(void *window_surface, CGRect *rect, int copy_data, int color_keyed,
        CGFloat key_red, CGFloat key_green, CGFloat key_blue) DECLSPEC_HIDDEN;
extern int get_surface_blit_rects(void *window_surface, const CGRect **rects, int *count) DECLSPEC_HIDDEN;
extern void macdrv_window_needs_display(macdrv_window w, CGRect rect) DECLSPEC_HIDDEN;
extern void macdrv_set_window_shape(macdrv_window w, const CGRect *rects, int count) DECLSPEC_HIDDEN;
extern void macdrv_set_window_alpha(macdrv_window w, CGFloat alpha) DECLSPEC_HIDDEN;
extern void macdrv_set_window_color_key(macdrv_window w, CGFloat keyRed, CGFloat keyGreen,
                                        CGFloat keyBlue) DECLSPEC_HIDDEN;
extern void macdrv_clear_window_color_key(macdrv_window w) DECLSPEC_HIDDEN;
extern void macdrv_window_use_per_pixel_alpha(macdrv_window w, int use_per_pixel_alpha) DECLSPEC_HIDDEN;
extern void macdrv_give_cocoa_window_focus(macdrv_window w, int activate) DECLSPEC_HIDDEN;
extern void macdrv_set_window_min_max_sizes(macdrv_window w, CGSize min_size, CGSize max_size) DECLSPEC_HIDDEN;
extern macdrv_view macdrv_create_view(CGRect rect) DECLSPEC_HIDDEN;
extern void macdrv_dispose_view(macdrv_view v) DECLSPEC_HIDDEN;
extern void macdrv_set_view_frame(macdrv_view v, CGRect rect) DECLSPEC_HIDDEN;
extern void macdrv_set_view_superview(macdrv_view v, macdrv_view s, macdrv_window w, macdrv_view p, macdrv_view n) DECLSPEC_HIDDEN;
extern void macdrv_set_view_hidden(macdrv_view v, int hidden) DECLSPEC_HIDDEN;
extern void macdrv_add_view_opengl_context(macdrv_view v, macdrv_opengl_context c) DECLSPEC_HIDDEN;
extern void macdrv_remove_view_opengl_context(macdrv_view v, macdrv_opengl_context c) DECLSPEC_HIDDEN;
#ifdef HAVE_METAL_METAL_H
extern macdrv_metal_device macdrv_create_metal_device(void) DECLSPEC_HIDDEN;
extern void macdrv_release_metal_device(macdrv_metal_device d) DECLSPEC_HIDDEN;
extern macdrv_metal_view macdrv_view_create_metal_view(macdrv_view v, macdrv_metal_device d) DECLSPEC_HIDDEN;
extern macdrv_metal_layer macdrv_view_get_metal_layer(macdrv_metal_view v) DECLSPEC_HIDDEN;
extern void macdrv_view_release_metal_view(macdrv_metal_view v) DECLSPEC_HIDDEN;
#endif
extern int macdrv_get_view_backing_size(macdrv_view v, int backing_size[2]) DECLSPEC_HIDDEN;
extern void macdrv_set_view_backing_size(macdrv_view v, const int backing_size[2]) DECLSPEC_HIDDEN;
extern uint32_t macdrv_window_background_color(void) DECLSPEC_HIDDEN;
extern void macdrv_send_text_input_event(int pressed, unsigned int flags, int repeat, int keyc,
                                         void* data, int* done) DECLSPEC_HIDDEN;
extern int macdrv_is_any_wine_window_visible(void) DECLSPEC_HIDDEN;


/* keyboard */
extern void macdrv_get_input_source_info(CFDataRef* uchr,CGEventSourceKeyboardType* keyboard_type, int* is_iso,
                                         TISInputSourceRef* input_source) DECLSPEC_HIDDEN;
extern CFArrayRef macdrv_create_input_source_list(void) DECLSPEC_HIDDEN;
extern int macdrv_select_input_source(TISInputSourceRef input_source) DECLSPEC_HIDDEN;
extern const CFStringRef macdrv_input_source_input_key DECLSPEC_HIDDEN;
extern const CFStringRef macdrv_input_source_type_key DECLSPEC_HIDDEN;
extern const CFStringRef macdrv_input_source_lang_key DECLSPEC_HIDDEN;
extern int macdrv_layout_list_needs_update DECLSPEC_HIDDEN;


/* clipboard */
extern CFArrayRef macdrv_copy_pasteboard_types(CFTypeRef pasteboard) DECLSPEC_HIDDEN;
extern CFDataRef macdrv_copy_pasteboard_data(CFTypeRef pasteboard, CFStringRef type) DECLSPEC_HIDDEN;
extern int macdrv_is_pasteboard_owner(macdrv_window w) DECLSPEC_HIDDEN;
extern int macdrv_has_pasteboard_changed(void) DECLSPEC_HIDDEN;
extern void macdrv_clear_pasteboard(macdrv_window w) DECLSPEC_HIDDEN;
extern int macdrv_set_pasteboard_data(CFStringRef type, CFDataRef data, macdrv_window w) DECLSPEC_HIDDEN;


/* opengl */
extern macdrv_opengl_context macdrv_create_opengl_context(void* cglctx) DECLSPEC_HIDDEN;
extern void macdrv_dispose_opengl_context(macdrv_opengl_context c) DECLSPEC_HIDDEN;
extern void macdrv_make_context_current(macdrv_opengl_context c, macdrv_view v, CGRect r) DECLSPEC_HIDDEN;
extern void macdrv_update_opengl_context(macdrv_opengl_context c) DECLSPEC_HIDDEN;
extern void macdrv_flush_opengl_context(macdrv_opengl_context c) DECLSPEC_HIDDEN;


/* systray / status item */
extern macdrv_status_item macdrv_create_status_item(macdrv_event_queue q) DECLSPEC_HIDDEN;
extern void macdrv_destroy_status_item(macdrv_status_item s) DECLSPEC_HIDDEN;
extern void macdrv_set_status_item_image(macdrv_status_item s, CGImageRef cgimage) DECLSPEC_HIDDEN;
extern void macdrv_set_status_item_tooltip(macdrv_status_item s, CFStringRef cftip) DECLSPEC_HIDDEN;

extern void macdrv_clear_ime_text(void) DECLSPEC_HIDDEN;

#endif  /* __WINE_MACDRV_COCOA_H */
