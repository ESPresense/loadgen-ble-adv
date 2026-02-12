# HEARTBEAT.md - Periodic QA Checks

On each heartbeat, rotate through these checks (don't do all every time — pick 1-2):

## CI Health
- Run: `gh run list --repo DTTeraique/ESPresense-companion --limit 3`
- If any recent runs failed, investigate and report to Darrell

## Open PRs
- Run: `gh pr list --repo DTTeraique/ESPresense-companion`
- If a PR is waiting for review, read the diff and run relevant tests
- Post your QA verdict

## Regression Test Status
- Check `/root/.openclaw/workspace/REGRESSION_TESTS.md` for pending items
- If hardware tests are in progress, check for updates

## Test Suite Health
- Periodically run the full backend + frontend test suites unprompted
- If anything new fails, flag it immediately

If nothing needs attention, reply HEARTBEAT_OK.
