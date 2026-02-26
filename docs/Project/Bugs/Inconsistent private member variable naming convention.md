---
tags: [bug, refactoring]
feature: "[[Code Quality]]"
created: 2026-02-28
updated: 2026-02-28
status: open
priority: low
severity: low
---

# Bug: Inconsistent Private Member Variable Naming Convention

## Summary

Refactor the codebase to convert all private variables with trailing underscores to be prepended with underscores.

## Expected Behavior

All private members use leading underscore convention: `_vao`, `_indexCount`, `_updateEconomy`

## Actual Behavior

Mixed convention: variables use trailing (`vao_`), methods use leading (`_updateEconomy`)

## Proposed Solution

Codebase-wide refactor of private member variables from trailing to leading underscores. This should easily be done by an AI agent.
