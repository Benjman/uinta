---@meta
-- Uinta Engine Lua API Type Definitions
-- This file provides EmmyLua annotations for IDE support.
-- It is not executed at runtime.

--------------------------------------------------------------------------------
-- Vector Types
--------------------------------------------------------------------------------

---@class uinta.vec2
---@field x number X component
---@field y number Y component
---@operator add(uinta.vec2): uinta.vec2
---@operator sub(uinta.vec2): uinta.vec2
---@operator mul(number): uinta.vec2
---@operator mul(uinta.vec2): uinta.vec2
---@operator div(number): uinta.vec2
---@operator unm: uinta.vec2
local vec2 = {}

---Returns the length (magnitude) of the vector.
---@return number
function vec2:length() end

---Returns a normalized copy of the vector (unit length).
---@return uinta.vec2
function vec2:normalize() end

---Computes the dot product with another vector.
---@param other uinta.vec2
---@return number
function vec2:dot(other) end

---@class uinta.vec3
---@field x number X component
---@field y number Y component
---@field z number Z component
---@operator add(uinta.vec3): uinta.vec3
---@operator sub(uinta.vec3): uinta.vec3
---@operator mul(number): uinta.vec3
---@operator mul(uinta.vec3): uinta.vec3
---@operator div(number): uinta.vec3
---@operator unm: uinta.vec3
local vec3 = {}

---Returns the length (magnitude) of the vector.
---@return number
function vec3:length() end

---Returns a normalized copy of the vector (unit length).
---@return uinta.vec3
function vec3:normalize() end

---Computes the dot product with another vector.
---@param other uinta.vec3
---@return number
function vec3:dot(other) end

---Computes the cross product with another vector.
---@param other uinta.vec3
---@return uinta.vec3
function vec3:cross(other) end

---@class uinta.vec4
---@field x number X component
---@field y number Y component
---@field z number Z component
---@field w number W component
---@operator add(uinta.vec4): uinta.vec4
---@operator sub(uinta.vec4): uinta.vec4
---@operator mul(number): uinta.vec4
---@operator mul(uinta.vec4): uinta.vec4
---@operator div(number): uinta.vec4
---@operator unm: uinta.vec4
local vec4 = {}

---Returns the length (magnitude) of the vector.
---@return number
function vec4:length() end

---Returns a normalized copy of the vector (unit length).
---@return uinta.vec4
function vec4:normalize() end

---Computes the dot product with another vector.
---@param other uinta.vec4
---@return number
function vec4:dot(other) end

---@class uinta.mat4
---4x4 matrix for transformations (view, projection, model matrices).
---Note: Not yet implemented. Coming in a future phase.
local mat4 = {}

--------------------------------------------------------------------------------
-- Logging
--------------------------------------------------------------------------------

---@class uinta.log
---Logging utilities for plugin output.
local log = {}

---Log an info message.
---@param msg string The message to log
function log.info(msg) end

---Log a warning message.
---@param msg string The message to log
function log.warn(msg) end

---Log an error message.
---@param msg string The message to log
function log.error(msg) end

---Log a debug message (only appears in debug builds).
---@param msg string The message to log
function log.debug(msg) end

--------------------------------------------------------------------------------
-- Events
--------------------------------------------------------------------------------

---@alias uinta.event_callback fun(...: any)

---@class uinta.events
---Event system for inter-plugin and engine communication.
local events = {}

---Subscribe to an event.
---@param name string The event name to listen for
---@param callback uinta.event_callback The function to call when the event fires
function events.on(name, callback) end

---Unsubscribe from an event.
---@param name string The event name
---@param callback uinta.event_callback The callback to remove
function events.off(name, callback) end

---Emit an event with optional data.
---@param name string The event name to emit
---@param ... any Data to pass to event handlers
function events.emit(name, ...) end

--------------------------------------------------------------------------------
-- Engine Info
--------------------------------------------------------------------------------

---@class uinta.engine
---Runtime information about the engine.
---@field fps number Current frames per second
---@field deltaTime number Time elapsed since last frame (seconds)
---@field runtime number Total engine runtime (seconds)
local engine = {}

--------------------------------------------------------------------------------
-- Plugin Info
--------------------------------------------------------------------------------

---@class uinta.plugin
---Information about the currently executing plugin.
---@field id string Plugin identifier
---@field version string Plugin version string
---@field path string Path to the plugin directory
local plugin = {}

--------------------------------------------------------------------------------
-- API Version
--------------------------------------------------------------------------------

---@class uinta.api_version
---The engine's Lua API version for compatibility checking.
---@field major integer Major version (breaking changes)
---@field minor integer Minor version (new features, backward compatible)
---@field patch integer Patch version (bug fixes)
local api_version = {}

--------------------------------------------------------------------------------
-- Config Bindings
--------------------------------------------------------------------------------

---@class uinta.config
---Application configuration access (when enabled by feature flags).
---Read functions are only available if configReadEnabled is set.
---Write functions are only available if configWriteEnabled is set.
local config = {}

---Get a string configuration value.
---@param key string The configuration key
---@return string|nil value The value, or nil if not found
function config.getString(key) end

---Get an integer configuration value.
---@param key string The configuration key
---@return integer|nil value The value, or nil if not found
function config.getInt(key) end

---Get a float configuration value.
---@param key string The configuration key
---@return number|nil value The value, or nil if not found
function config.getFloat(key) end

---Get a boolean configuration value.
---@param key string The configuration key
---@return boolean|nil value The value, or nil if not found
function config.getBoolean(key) end

---Set a string configuration value.
---@param key string The configuration key
---@param value string The value to set
---@return boolean success True if the value was set successfully
function config.setString(key, value) end

---Set an integer configuration value.
---@param key string The configuration key
---@param value integer The value to set
---@return boolean success True if the value was set successfully
function config.setInt(key, value) end

---Set a float configuration value.
---@param key string The configuration key
---@param value number The value to set
---@return boolean success True if the value was set successfully
function config.setFloat(key, value) end

---Set a boolean configuration value.
---@param key string The configuration key
---@param value boolean The value to set
---@return boolean success True if the value was set successfully
function config.setBoolean(key, value) end

---Flush configuration changes to disk.
function config.flush() end

--------------------------------------------------------------------------------
-- UI Bindings (Stubbed)
--------------------------------------------------------------------------------

---@class uinta.ui
---UI widget bindings (stubbed - design TBD).
local ui = {}

---Create a button widget (stub).
---@param label string Button label
---@return any
function ui.button(label) end

---Create a label widget (stub).
---@param text string Label text
---@return any
function ui.label(text) end

---Create a slider widget (stub).
---@param label string Slider label
---@param value number Current value
---@param min number Minimum value
---@param max number Maximum value
---@return any
function ui.slider(label, value, min, max) end

---Create a checkbox widget (stub).
---@param label string Checkbox label
---@param checked boolean Current state
---@return any
function ui.checkbox(label, checked) end

--------------------------------------------------------------------------------
-- Camera Bindings (Future - Phase 2+)
--------------------------------------------------------------------------------

---@alias uinta.view_matrix_provider fun(): uinta.mat4

---@class uinta.camera
---Camera system bindings for view matrix management.
---Note: Not yet implemented. Coming in a future phase.
local camera = {}

---Register a view matrix provider callback.
---The CameraManager will call this function each frame to get the view matrix.
---@param provider uinta.view_matrix_provider Function that returns a mat4 view matrix
function camera.setViewMatrixProvider(provider) end

---Unregister the current view matrix provider.
function camera.clearViewMatrixProvider() end

--------------------------------------------------------------------------------
-- Input Bindings
--------------------------------------------------------------------------------

---@class uinta.SubscriptionHandle
---Opaque handle for unsubscribing from input events.
---@field id integer Handle ID
local SubscriptionHandle = {}

---@alias uinta.key_callback fun(key: integer, action: integer, mods: integer)
---@alias uinta.mouse_button_callback fun(button: integer, action: integer, mods: integer)
---@alias uinta.mouse_move_callback fun(x: number, y: number, dx: number, dy: number)
---@alias uinta.mouse_scroll_callback fun(dx: number, dy: number)

---@class uinta.Key
---Keyboard key codes.
local Key = {}
Key.Space = 32
Key.Apostrophe = 39
Key.Comma = 44
Key.Minus = 45
Key.Period = 46
Key.Slash = 47
Key.Num0 = 48
Key.Num1 = 49
Key.Num2 = 50
Key.Num3 = 51
Key.Num4 = 52
Key.Num5 = 53
Key.Num6 = 54
Key.Num7 = 55
Key.Num8 = 56
Key.Num9 = 57
Key.Semicolon = 59
Key.Equal = 61
Key.A = 65
Key.B = 66
Key.C = 67
Key.D = 68
Key.E = 69
Key.F = 70
Key.G = 71
Key.H = 72
Key.I = 73
Key.J = 74
Key.K = 75
Key.L = 76
Key.M = 77
Key.N = 78
Key.O = 79
Key.P = 80
Key.Q = 81
Key.R = 82
Key.S = 83
Key.T = 84
Key.U = 85
Key.V = 86
Key.W = 87
Key.X = 88
Key.Y = 89
Key.Z = 90
Key.LeftBracket = 91
Key.Backslash = 92
Key.RightBracket = 93
Key.GraveAccent = 96

Key.Escape = 256
Key.Enter = 257
Key.Tab = 258
Key.Backspace = 259
Key.Insert = 260
Key.Delete = 261
Key.Right = 262
Key.Left = 263
Key.Down = 264
Key.Up = 265
Key.PageUp = 266
Key.PageDown = 267
Key.Home = 268
Key.End = 269
Key.CapsLock = 280
Key.ScrollLock = 281
Key.NumLock = 282
Key.PrintScreen = 283
Key.Pause = 284
Key.F1 = 290
Key.F2 = 291
Key.F3 = 292
Key.F4 = 293
Key.F5 = 294
Key.F6 = 295
Key.F7 = 296
Key.F8 = 297
Key.F9 = 298
Key.F10 = 299
Key.F11 = 300
Key.F12 = 301
Key.F13 = 302
Key.F14 = 303
Key.F15 = 304
Key.F16 = 305
Key.F17 = 306
Key.F18 = 307
Key.F19 = 308
Key.F20 = 309
Key.F21 = 310
Key.F22 = 311
Key.F23 = 312
Key.F24 = 313
Key.F25 = 314

Key.Kp0 = 320
Key.Kp1 = 321
Key.Kp2 = 322
Key.Kp3 = 323
Key.Kp4 = 324
Key.Kp5 = 325
Key.Kp6 = 326
Key.Kp7 = 327
Key.Kp8 = 328
Key.Kp9 = 329
Key.KpDecimal = 330
Key.KpDivide = 331
Key.KpMultiply = 332
Key.KpSubtract = 333
Key.KpAdd = 334
Key.KpEnter = 335
Key.KpEqual = 336

Key.LeftShift = 340
Key.LeftControl = 341
Key.LeftAlt = 342
Key.LeftSuper = 343
Key.RightShift = 344
Key.RightControl = 345
Key.RightAlt = 346
Key.RightSuper = 347
Key.Menu = 348

---@class uinta.Action
---Input action codes (bitwise flags).
local Action = {}
Action.Press = 0x00010000
Action.Release = 0x00020000
Action.Repeat = 0x00040000

---@class uinta.Mod
---Modifier key flags (bitwise).
local Mod = {}
Mod.Shift = 0x00100000
Mod.Control = 0x00200000
Mod.Alt = 0x00400000
Mod.Super = 0x00800000

---@class uinta.MouseBtn
---Mouse button codes.
local MouseBtn = {}
MouseBtn.Left = 0
MouseBtn.Right = 1
MouseBtn.Middle = 2

---@class uinta.input
---Input system bindings for keyboard and mouse events.
local input = {}

---Subscribe to keyboard input.
---@param token integer Key | Action | Mod token (use bitwise OR)
---@param callback uinta.key_callback Function to call when key event occurs
---@return uinta.SubscriptionHandle Handle for unsubscription
function input.onKey(token, callback) end

---Subscribe to mouse button input.
---@param token integer MouseBtn | Action | Mod token (use bitwise OR)
---@param callback uinta.mouse_button_callback Function to call when button event occurs
---@return uinta.SubscriptionHandle Handle for unsubscription
function input.onMouseButton(token, callback) end

---Subscribe to mouse movement.
---@param callback uinta.mouse_move_callback Function to call when mouse moves
---@return uinta.SubscriptionHandle Handle for unsubscription
function input.onMouseMove(callback) end

---Subscribe to mouse scroll.
---@param callback uinta.mouse_scroll_callback Function to call when scrolling
---@return uinta.SubscriptionHandle Handle for unsubscription
function input.onMouseScroll(callback) end

---Unsubscribe from an input event.
---@param handle uinta.SubscriptionHandle The handle returned by a subscription function
function input.off(handle) end

--------------------------------------------------------------------------------
-- Main uinta Global
--------------------------------------------------------------------------------

---@class uinta
---The Uinta engine Lua API.
---@field log uinta.log Logging utilities
---@field events uinta.events Event system
---@field engine uinta.engine Engine runtime info
---@field plugin uinta.plugin Current plugin info
---@field api_version uinta.api_version API version info
---@field config uinta.config Application config access
---@field ui uinta.ui UI widgets (stubbed)
---@field camera uinta.camera Camera system (future)
---@field input uinta.input Input system bindings
---@field Key uinta.Key Keyboard key codes
---@field Action uinta.Action Input action codes
---@field Mod uinta.Mod Modifier key flags
---@field MouseBtn uinta.MouseBtn Mouse button codes
uinta = {}

---Create a 2D vector.
---@param x number X component
---@param y number Y component
---@return uinta.vec2
function uinta.vec2(x, y) end

---Create a 3D vector.
---@param x number X component
---@param y number Y component
---@param z number Z component
---@return uinta.vec3
function uinta.vec3(x, y, z) end

---Create a 4D vector.
---@param x number X component
---@param y number Y component
---@param z number Z component
---@param w number W component
---@return uinta.vec4
function uinta.vec4(x, y, z, w) end

--------------------------------------------------------------------------------
-- Engine Stage Callbacks
--------------------------------------------------------------------------------

---@alias uinta.stage_callback fun(dt: number)

---Register a callback for the PreTick stage.
---@param callback uinta.stage_callback Called before tick processing
function uinta.onPreTick(callback) end

---Unregister a PreTick callback.
---@param callback uinta.stage_callback The callback to remove
function uinta.offPreTick(callback) end

---Register a callback for the Tick stage.
---@param callback uinta.stage_callback Called during main tick processing
function uinta.onTick(callback) end

---Unregister a Tick callback.
---@param callback uinta.stage_callback The callback to remove
function uinta.offTick(callback) end

---Register a callback for the PostTick stage.
---@param callback uinta.stage_callback Called after tick processing
function uinta.onPostTick(callback) end

---Unregister a PostTick callback.
---@param callback uinta.stage_callback The callback to remove
function uinta.offPostTick(callback) end

---Register a callback for the PreRender stage.
---@param callback uinta.stage_callback Called before rendering
function uinta.onPreRender(callback) end

---Unregister a PreRender callback.
---@param callback uinta.stage_callback The callback to remove
function uinta.offPreRender(callback) end

---Register a callback for the Render stage.
---@param callback uinta.stage_callback Called during rendering
function uinta.onRender(callback) end

---Unregister a Render callback.
---@param callback uinta.stage_callback The callback to remove
function uinta.offRender(callback) end

---Register a callback for the PostRender stage.
---@param callback uinta.stage_callback Called after rendering
function uinta.onPostRender(callback) end

---Unregister a PostRender callback.
---@param callback uinta.stage_callback The callback to remove
function uinta.offPostRender(callback) end
