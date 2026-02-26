---
tags: [feature, meta]
epic: "[[Epics/v0.1]]"
created: 2026-03-01
updated: 2026-03-01
status: in-progress
priority: medium
---

# Feature: Project Management System

## Notes

This is a meta feature - it documents the project management system within itself. The system is designed to be self-documenting and self-hosting, allowing it to track its own development progress.

The PM system prioritizes simplicity and developer experience over feature completeness. It's deliberately minimal to avoid becoming a maintenance burden while still providing structure for planning and tracking work.

## Extensibility plans

### Custom Scripts

Location: `scripts/pm/` (project management utilities)

**Example scripts:**

```bash
scripts/pm/new-feature.sh <name>      # Create feature from template
scripts/pm/new-bug.sh <feature> <desc> # Create bug report
scripts/pm/status.sh                  # Show project metrics
scripts/pm/archive.sh <feature>       # Move to CHANGELOG
```

### CI/CD Integration

**Validation (GitHub Actions):**

- Validate frontmatter syntax
- Check for broken wiki links
- Ensure bug-feature links are bidirectional
- Generate reports on PR

**Auto-updates:**

- Update epic metrics on feature changes
- Auto-increment bug numbers
