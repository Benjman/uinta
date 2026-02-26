---
tags: [epic]
created: YYYY-MM-DD
updated: YYYY-MM-DD
status: [planned, in-progress, complete]
target: YYYY-MM-DD or TBD
---

# Epic: [Epic Name]

## Overview

[Brief description of the epic and its purpose. 1-2 sentences explaining the high-level goal.]

## Goals

- [Primary goal or objective]
- [Secondary goal or objective]
- [Additional goals as needed]

## Features

```dataview
TABLE status, created, updated
FROM "project/features"
WHERE contains(epic, "[[Epics/[Epic Name]]]")
SORT updated DESC
```
