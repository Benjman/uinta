-- Example Plugin for Uinta Engine
-- Demonstrates the core Lua plugin API features.

--------------------------------------------------------------------------------
-- Logging
--------------------------------------------------------------------------------

uinta.log.info("Example plugin loading...")
uinta.log.debug("Debug messages only appear in debug builds")

--------------------------------------------------------------------------------
-- Vector Math
--------------------------------------------------------------------------------

-- Create vectors
local position = uinta.vec3(10.0, 5.0, 3.0)
local direction = uinta.vec3(1.0, 0.0, 0.0)
local scale = uinta.vec2(2.0, 2.0)

-- Vector operations
local moved = position + direction * 5.0
local length = direction:length()
local normalized = position:normalize()
local dot_product = position:dot(direction)

uinta.log.info("Position: " .. tostring(position.x) .. ", " .. tostring(position.y) .. ", " .. tostring(position.z))
uinta.log.info("Moved position: " .. tostring(moved.x) .. ", " .. tostring(moved.y) .. ", " .. tostring(moved.z))
uinta.log.info("Direction length: " .. tostring(length))
uinta.log.info("Dot product: " .. tostring(dot_product))

-- Cross product (vec3 only)
local up = uinta.vec3(0.0, 1.0, 0.0)
local right = direction:cross(up)
uinta.log.info(
	"Cross product (right vector): " .. tostring(right.x) .. ", " .. tostring(right.y) .. ", " .. tostring(right.z)
)

-- vec4 for colors/homogeneous coordinates
local color = uinta.vec4(1.0, 0.5, 0.2, 1.0)
uinta.log.info(
	"Color RGBA: "
		.. tostring(color.x)
		.. ", "
		.. tostring(color.y)
		.. ", "
		.. tostring(color.z)
		.. ", "
		.. tostring(color.w)
)

--------------------------------------------------------------------------------
-- Event System
--------------------------------------------------------------------------------

-- Define event handlers
local function on_game_start(data)
	uinta.log.info("Game started! Data: " .. tostring(data or "none"))
end

local function on_player_spawn(player_data)
	if player_data then
		uinta.log.info("Player spawned at position")
	else
		uinta.log.info("Player spawned (no data)")
	end
end

-- Subscribe to events
uinta.events.on("game.start", on_game_start)
uinta.events.on("player.spawn", on_player_spawn)

-- Example: emit an event (typically done by the engine or other plugins)
uinta.events.emit("game.start", "Hello from Lua!")

--------------------------------------------------------------------------------
-- Plugin Info
--------------------------------------------------------------------------------

-- Access current plugin information (when set by the loader)
if uinta.plugin then
	uinta.log.info("Plugin ID: " .. tostring(uinta.plugin.id or "unknown"))
	uinta.log.info("Plugin version: " .. tostring(uinta.plugin.version or "unknown"))
end

--------------------------------------------------------------------------------
-- Engine Info
--------------------------------------------------------------------------------

-- Access engine runtime information (when available)
if uinta.engine then
	if uinta.engine.fps then
		uinta.log.info("Current FPS: " .. tostring(uinta.engine.fps))
	end
	if uinta.engine.deltaTime then
		uinta.log.info("Delta time: " .. tostring(uinta.engine.deltaTime))
	end
end

--------------------------------------------------------------------------------
-- API Version
--------------------------------------------------------------------------------

if uinta.api_version then
	uinta.log.info(
		"API Version: "
			.. tostring(uinta.api_version.major)
			.. "."
			.. tostring(uinta.api_version.minor)
			.. "."
			.. tostring(uinta.api_version.patch)
	)
end

--------------------------------------------------------------------------------
-- Sandboxing Demonstration
--------------------------------------------------------------------------------

-- The following would fail due to sandboxing (commented out for safety):
-- os.execute("echo 'This would be dangerous!'")  -- BLOCKED: os.execute removed
-- io.open("/etc/passwd", "r")                    -- BLOCKED: io library removed
-- loadfile("malicious.lua")                      -- BLOCKED: loadfile removed
-- debug.getinfo(1)                               -- BLOCKED: debug library removed

uinta.log.info("Example plugin loaded successfully!")

--------------------------------------------------------------------------------
-- Cleanup function (called on plugin unload/reload)
--------------------------------------------------------------------------------

function cleanup()
	uinta.log.info("Example plugin unloading...")
	uinta.events.off("game.start", on_game_start)
	uinta.events.off("player.spawn", on_player_spawn)
end
