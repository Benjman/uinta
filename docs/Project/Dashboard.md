---
tags: [dashboard]
updated: 2026-02-25
---

# Uinta Engine Dashboard

## 🎯 Epic Overview

[[Epics/v0.1|Uinta Engine v0.1]] - C++ game engine in development

**Progress:** 24 core systems implemented (testing & refinement ongoing)

## 📊 Project Metrics

**Feature Status:**

```dataview
TABLE WITHOUT ID
  "" AS "🔄 In Progress",
  length(filter(rows, (r) => contains(r.status, "in-progress"))) AS "Count"
FROM "project/features"
WHERE contains(tags, "feature")
GROUP BY true
```

```dataview
TABLE WITHOUT ID
  "" AS "📋 Planned",
  length(filter(rows, (r) => contains(r.status, "planned"))) AS "Count"
FROM "project/features"
WHERE contains(tags, "feature")
GROUP BY true
```

```dataview
TABLE WITHOUT ID
  "" AS "🐛 Active Bugs",
  length(rows) AS "Count"
FROM "project/bugs"
WHERE contains(status, "open")
GROUP BY true
```

## ⚡ Quick Actions

- Create new feature from [[Feature Template]]
- Report new bug from [[Bug Template]]
