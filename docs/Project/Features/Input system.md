---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-02-25
status: complete
---

# Feature: Input System

## Overview

Comprehensive input management system with keyboard, mouse, and modifier support.

## Implementation Details

### Keyboard Support

**Full Keyboard:**

- QWERTY layout (SPACE through MENU keys)
- Function keys (F1-F25)
- Numpad keys (KP_0 through KP_EQUAL)
- Arrow keys and navigation (HOME, END, PAGE_UP, PAGE_DOWN)
- Modifier keys (SHIFT, CONTROL, ALT, SUPER)

**Key States:**

- `Down` - Key is currently pressed
- `Pressed` - Key was just pressed this frame
- `Released` - Key was just released this frame
- `Repeated` - Key is being held and repeating

### Mouse Support

**Mouse Buttons:**

- Standard buttons: LEFT, RIGHT, MIDDLE
- Extended buttons: 1-8 (for gaming mice)

**Mouse Tracking:**

- Position tracking (x, y)
- Position delta (movement since last frame)
- Scroll wheel support (x, y scroll)
- Cursor world point calculation (screen to world)

### Input Modifiers

**Modifier Detection:**

- MOD_SHIFT - Shift key pressed
- MOD_CONTROL - Control key pressed
- MOD_ALT - Alt key pressed
- MOD_SUPER - Super/Windows/Command key pressed

### Input Actions

**Action Types:**

- ACTION_PRESS - Input just activated
- ACTION_RELEASE - Input just deactivated
- ACTION_REPEAT - Input is repeating

### Utilities

**Input Functions:**

- Key/button name to string conversion
- Input state reset
- Modifier checking
