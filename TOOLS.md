# TOOLS.md - QA Tooling & Environment

## Project Paths

- **Main project:** `/root/.openclaw/workspace/ESPresense-companion/`
- **Backend tests:** `/root/.openclaw/workspace/ESPresense-companion/tests/ESPresense.Companion.Tests/`
- **Frontend tests:** `/root/.openclaw/workspace/ESPresense-companion/src/ui/tests/`
- **CI config:** `/root/.openclaw/workspace/ESPresense-companion/.github/workflows/ci.yml`
- **Regression plan:** `/root/.openclaw/workspace/REGRESSION_TESTS.md`
- **Architecture docs:** `/root/.openclaw/workspace/ESPresense-companion/ESPresense-companion-architecture.md`
- **Dev guide:** `/root/.openclaw/workspace/ESPresense-companion/CLAUDE.md`

## Test Commands

### Backend
```bash
# Full test suite
dotnet test /root/.openclaw/workspace/ESPresense-companion

# CI-mirror (skip long-running)
dotnet build /root/.openclaw/workspace/ESPresense-companion -c Release && \
dotnet test /root/.openclaw/workspace/ESPresense-companion -c Release --no-build --verbosity normal --filter "Category!=LongRunning"

# Single test class
dotnet test /root/.openclaw/workspace/ESPresense-companion --filter "ClassName=DeviceTrackerTests"
```

### Frontend
```bash
cd /root/.openclaw/workspace/ESPresense-companion/src/ui

pnpm test          # Playwright E2E
pnpm test:unit     # Vitest unit tests
pnpm check         # Type checking
pnpm lint          # ESLint + Prettier
```

### GitHub CLI
```bash
gh run list --repo DTTeraique/ESPresense-companion --limit 5
gh pr list --repo DTTeraique/ESPresense-companion
gh pr checks <number> --repo DTTeraique/ESPresense-companion
gh pr diff <number> --repo DTTeraique/ESPresense-companion
```

## Hardware Bank

- ESP32-C3: `/dev/serial/by-id/usb-1a86_USB_Single_Serial_54F7004746-if00`
- Power cycling: `uhubctl`
- OTA: `espota.py`
- Flashing: `pio run -e <env> -t upload`

## QA Scripts (in this workspace)

- `read_serial.py` — read serial output from a device (pass port as arg)
- `read_serial_nonblock.py` — non-blocking serial reader (hardcoded to ESP32-C3 port)
- `improv_setup.py` — SerialImprov WiFi provisioning over serial
- `monitor_stability.py` — monitor serial logs for crashes/reboots
- `github_auth.py` — GitHub App auth helper (JWT + installation tokens)

## Beads (Task Management)

To learn about beads and how to use them:
```bash
bd quickstart
```

This will show you how to work with beads - the task management system for tracking and organizing work.

