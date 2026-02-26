---
tags: [spec, documentation]
created: 2026-02-25
updated: 2026-03-01
---

# Project Management System for Uinta Engine

This document describes markdown-based project management system for the Uinta Engine project.

See [[feature-project-management-system|Feature: Project Management System]].

## Architecture

### Directory Structure

```
docs/
├── Project/                          # Project management root
│   ├── Dashboard.md                  # Live project overview
│   ├── readme.md                     # System overview and specification
│   ├── Epic Template.md              # Template for epics
│   ├── Feature Template.md           # Template for features
│   ├── Task Template.md              # Template for tasks
│   ├── Bug Template.md               # Template for bugs
│   ├── Epics/                        # Epic documents
│       ├── v1.0.md
│   │   └── ...
│   ├── Features/                     # Feature documents
│       ├── Input system.md
│   │   └── ...
│   ├── Tasks/                        # Task documents
│       ├── Add async texture loading.md
│   │   └── ...
│   └── Bugs/                         # Bug reports
│       ├── Crash on startup.md
│       └── ...
```

### Document Types

#### 1. Epic

- **Purpose**: High-level project milestone (e.g., "Uinta Engine v0.1")
- **Location**: `docs/Project/Epics/<Epic name>.md`
- **Content**: See [[Epic template]]

#### 2. Feature

- **Purpose**: Major functionality or system component
- **Location**: `docs/Project/Features/<Feature name>.md`
- **Content**: See [[Feature template]]

#### 3. Task

- **Purpose**: Concrete, implementable work unit that contributes to a feature
- **Scope**: Smaller than a feature, used for breaking down implementation work, enhancements, or technical debt
- **Location**: `docs/Project/Tasks/<task-name>.md`
- **Content**: See [[Task template]]

#### 4. Bug

- **Purpose**: Issue report linked to a specific feature
- **Location**: `docs/Project/Bugs/<bug-name>.md`
- **Content**: See [[Bug template]]

## Document Type Usage Guide

### When to Use Each Type

| Type | Use When | Example |
|------|----------|---------|
| **Epic** | Major version or project milestone | "v0.1 Release", "Graphics Overhaul" |
| **Feature** | Self-contained system or major capability | "Input System", "Particle System" |
| **Task** | Specific implementation work | "Add gamepad deadzone support", "Refactor texture cache" |
| **Bug** | Something is broken | "Crash when unplugging controller" |

### Task Relationships to Features

Tasks relate to features in three primary ways:

**1. Feature Breakdown** - Breaking initial implementation into work units:
```markdown
# Feature: Input System
## Implementation Tasks
- [[Tasks/Create input event queue]]
- [[Tasks/Implement keyboard handler]]
- [[Tasks/Add mouse support]]
```

**2. Feature Enhancement** - Adding capabilities post-completion:
```markdown
# Task: Add gamepad deadzone configuration
**Parent Feature:** [[Features/Input System]]  
**Type:** Enhancement
```

**3. Technical Debt / Refactoring** - Optimization or maintenance:
```markdown
# Task: Refactor input dispatch to reduce allocations
**Parent Feature:** [[Features/Input System]]  
**Type:** Optimization
```

## Conventions

### Naming Conventions

**Epic files:**

```
Epics/<version>.md
```

Example: `Epics/v0.1.md`, `Epics/v0.2.md`

**Feature files:**

```
Features/<Feature name>.md
```

Example: `Features/Rendering pipeline.md`, `Features/Physics system.md`

**Task files:**

```
Tasks/<Short description>.md
```

Example: `Tasks/Add async texture loading.md`, `Tasks/Refactor shader compilation.md`

**Bug files:**

```
Bugs/<Short description>.md
```

Example: `Bugs/Crash on startup.md`

### Status Labels

Use checkbox-style status indicators in document bodies:

- `[ ]` - Planned
- `[~]` - In Progress
- `[x]` - Complete
- `[!]` - Blocked

**Important:** `[x]` Implemented means working code exists, not that it's production-ready. Use `[✓]` to indicate a system is fully tested, optimized, documented, and ready for release.

### Tags System

**Document Type Tags:**

- `#epic` - Epic documents
- `#feature` - Feature specifications
- `#task` - Task documents
- `#bug` - Bug reports
- `#dashboard` - Dashboard/overview pages

### Bug Tagging System

Bugs use two independent axes for classification: **severity** and **priority**.

#### Bug Frontmatter

```yaml
---
tags: [bug]
feature: "[[Feature Name]]"
created: YYYY-MM-DD
updated: YYYY-MM-DD
status: [planned, in-progress, complete]
priority: [low, medium, high, critical]
severity: [low, medium, high, critical]
---
```

### YAML Frontmatter

All documents include YAML frontmatter for metadata:

**Epic:**

```yaml
---
tags: [epic]
created: YYYY-MM-DD
updated: YYYY-MM-DD
status: in-progress
target: YYYY-MM-DD or TBD
---
```

**Feature:**

```yaml
---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: YYYY-MM-DD
updated: YYYY-MM-DD
status: not-started
---
```

**Task:**

```yaml
---
tags: [task]
feature: "[[Features/Feature Name]]"
created: YYYY-MM-DD
updated: YYYY-MM-DD
status: planned
priority: medium
estimate: medium
---
```

**Bug:**

```yaml
---
tags: [bug]
feature: "[[Features/Feature Name]]"
reported: YYYY-MM-DD
updated: YYYY-MM-DD
status: open
priority: medium
---
```

### Linking

Use Obsidian-style wiki links for cross-referencing:

```markdown
[[Epics/v0.1|Uinta Engine v0.1]]
[[Rendering Pipeline]]
[[BUG-001 - Crash on startup]]
```