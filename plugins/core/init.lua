-- Uinta Engine Core Plugin
-- Provides default implementations for engine systems.

uinta.log.info("Uinta Core plugin loading...")

--------------------------------------------------------------------------------
-- Camera Provider (Placeholder)
--------------------------------------------------------------------------------

-- The camera system will be implemented in a future phase.
-- The core plugin will register a default view matrix provider that
-- CameraManager will call each frame to get the current view matrix.
--
-- Future API (not yet implemented):
--   uinta.camera.setViewMatrixProvider(function()
--       return viewMatrix  -- mat4
--   end)

--------------------------------------------------------------------------------
-- Initialization Complete
--------------------------------------------------------------------------------

uinta.log.info("Uinta Core plugin loaded successfully!")

--------------------------------------------------------------------------------
-- Cleanup
--------------------------------------------------------------------------------

function cleanup()
    uinta.log.info("Uinta Core plugin unloading...")
    -- Future: unregister camera provider, input handlers, etc.
end
