-- Input System Tests
-- These tests are NOT loaded at runtime. They are for manual testing
-- or future integration with a test runner.
--
-- To run manually, you can temporarily require() this from init.lua
-- or load it via the Lua console when that feature is available.

local M = {}

--------------------------------------------------------------------------------
-- Test Helpers
--------------------------------------------------------------------------------

local function log(msg)
    uinta.log.info("[InputTest] " .. msg)
end

--------------------------------------------------------------------------------
-- Keyboard Tests
--------------------------------------------------------------------------------

function M.setupKeyboardTests()
    log("Setting up keyboard tests...")

    -- Test W key press
    uinta.input.onKey(uinta.Key.W | uinta.Action.Press, function(key, action, mods)
        log("W key pressed!")
    end)

    -- Test Escape key press
    uinta.input.onKey(uinta.Key.Escape | uinta.Action.Press, function(key, action, mods)
        log("Escape key pressed!")
    end)

    -- Test key with modifier (Ctrl+S)
    uinta.input.onKey(uinta.Key.S | uinta.Action.Press | uinta.Mod.Control, function(key, action, mods)
        log("Ctrl+S pressed!")
    end)

    log("Keyboard tests ready. Press W, Escape, or Ctrl+S to test.")
end

--------------------------------------------------------------------------------
-- Mouse Button Tests
--------------------------------------------------------------------------------

function M.setupMouseButtonTests()
    log("Setting up mouse button tests...")

    -- Test left mouse button
    uinta.input.onMouseButton(uinta.MouseBtn.Left | uinta.Action.Press, function(btn, action, mods)
        log("Left mouse button pressed!")
    end)

    -- Test right mouse button
    uinta.input.onMouseButton(uinta.MouseBtn.Right | uinta.Action.Press, function(btn, action, mods)
        log("Right mouse button pressed!")
    end)

    -- Test middle mouse button
    uinta.input.onMouseButton(uinta.MouseBtn.Middle | uinta.Action.Press, function(btn, action, mods)
        log("Middle mouse button pressed!")
    end)

    log("Mouse button tests ready. Click any mouse button to test.")
end

--------------------------------------------------------------------------------
-- Mouse Movement Tests
--------------------------------------------------------------------------------

function M.setupMouseMoveTests()
    log("Setting up mouse movement tests...")

    uinta.input.onMouseMove(function(x, y, dx, dy)
        -- Only log when there's significant movement to avoid spam
        if math.abs(dx) > 5 or math.abs(dy) > 5 then
            log(string.format("Mouse: pos=(%.1f, %.1f) delta=(%.1f, %.1f)", x, y, dx, dy))
        end
    end)

    log("Mouse movement tests ready. Move the mouse to test.")
end

--------------------------------------------------------------------------------
-- Mouse Scroll Tests
--------------------------------------------------------------------------------

function M.setupScrollTests()
    log("Setting up scroll tests...")

    uinta.input.onMouseScroll(function(dx, dy)
        log(string.format("Scroll: dx=%.2f, dy=%.2f", dx, dy))
    end)

    log("Scroll tests ready. Use mouse wheel to test.")
end

--------------------------------------------------------------------------------
-- Validation Tests
-- These tests verify that invalid tokens are properly rejected
--------------------------------------------------------------------------------

function M.setupValidationTests()
    log("Setting up validation tests...")

    -- Valid subscriptions should succeed
    log("Testing valid key subscription...")
    local keyHandle = uinta.input.onKey(uinta.Key.W | uinta.Action.Press, function(key, action, mods)
        log("Valid key subscription triggered!")
    end)
    log("Valid key subscription created with handle ID: " .. keyHandle.id)

    log("Testing valid mouse button subscription...")
    local btnHandle = uinta.input.onMouseButton(uinta.MouseBtn.Left | uinta.Action.Press, function(btn, action, mods)
        log("Valid mouse button subscription triggered!")
    end)
    log("Valid mouse button subscription created with handle ID: " .. btnHandle.id)

    -- Test unsubscription logging
    log("Testing unsubscription...")
    uinta.input.off(keyHandle)
    log("Unsubscribed from key input")

    -- Note: Invalid tokens will cause LOG(FATAL) and terminate the application
    -- Uncomment to test:
    -- log("Testing invalid key token (no action)...")
    -- uinta.input.onKey(uinta.Key.W, function() end)  -- Missing Action - will fail!

    -- log("Testing invalid mouse button token (no action)...")
    -- uinta.input.onMouseButton(uinta.MouseBtn.Left, function() end)  -- Missing Action - will fail!

    log("Validation tests completed!")
end

--------------------------------------------------------------------------------
-- Run All Tests
--------------------------------------------------------------------------------

function M.setupAll()
    log("========================================")
    log("Input System Test Suite")
    log("========================================")

    M.setupKeyboardTests()
    M.setupMouseButtonTests()
    M.setupMouseMoveTests()
    M.setupScrollTests()
    M.setupValidationTests()

    log("========================================")
    log("All input tests configured!")
    log("========================================")
end

return M
