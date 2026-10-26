# Handoff: Zed "Run current file" button, Mac port

Written 2026-10-26 by Claude on Ahad's Windows PC, for Claude on Ahad's Mac.
Ahad's preferences: keep chat replies short.

## What already works on Windows

Ahad wanted one button in Zed that compiles (if needed) and runs the current file, in any
language. On Windows it is done and tested:

- A global Zed task named **"Run current file"** (`windows-reference/tasks.json`) runs a runner
  script (`windows-reference/run-file.ps1`). The script reads the file to run from the
  `ZED_FILE` environment variable, which Zed sets for every task. The task passes no file
  argument because a task argument split paths that contain spaces ("Fall 2026").
- The task carries `tags` (`cpp-main`, `c-main`, `python-module-main-method`, `rust-main`,
  `go-main`, `bash-script`). These make Zed show its built-in gutter ▶ next to `main()`.
- **F5** (`windows-reference/keymap.json`) spawns the task, in context
  `Workspace && !debugger_stopped`, so F5 still means Continue while the debugger is paused.
- **Toolbar ▶ button:** Zed has no config or extension API for toolbar buttons, so Zed was
  rebuilt from source. `zed-run-button.patch` adds a PlayFilled button to the editor's
  quick action bar that dispatches `task::Spawn` by name for "Run current file". The patch is
  21 lines in `crates/zed/src/zed/quick_action_bar.rs` and is platform-independent. The base
  was Zed v1.25.3 stable, commit `8dd255c3eafb5231056fe4e6f601cedd011f1c93`. Auto-update is
  off on Windows (`"auto_update": false`) so the custom build isn't replaced.
- The Windows build also needed a patch to the `msvc_spectre_libs` crate. **That patch is
  Windows-only. Do not apply it on the Mac.**

## The job on the Mac

1. **Runner:** write `~/.config/zed/run-file.sh` (or zsh) that ports `run-file.ps1`. Keep the
   behavior listed below and make it executable.
2. **Task and key:** add the "Run current file" task to `~/.config/zed/tasks.json` and the key
   binding to `~/.config/zed/keymap.json`. **Merge into existing files; don't overwrite them.**
   Keep the same task label (the toolbar button looks it up by name), tags, `save: "all"`,
   `reveal: "always"`, `cwd: "$ZED_DIRNAME"`, and no file argument.
3. **Toolbar button (optional, ask Ahad first):** official Zed plus steps 1 and 2 already gives
   the gutter ▶ and F5. Only the toolbar ▶ needs a custom build, and it costs about 20 minutes
   of build time plus about 12 GB of disk. If Ahad wants it, follow the build section below.

## Runner behavior to keep

The spec is `run-file.ps1`; read it. In short:

- **Input:** the file comes from `$1`, else `$ZED_FILE`. Fail clearly if it is missing or
  unsaved. `cd` to the file's folder before running it.
- **Interactive programs:** run programs attached to the terminal, never through a pipe, so
  `cin` and `input()` work. Exit with the program's exit code.
- **C/C++ build:**
  - Find companion sources by following `#include "..."` breadth-first. For each local header
    included, add the same-stem `.cpp/.cc/.cxx/.c++` (or `.c`) beside it.
  - Flags: `-std=c++17` (C uses `-std=c17`), plus `-Wall -Wextra -g -fdiagnostics-color=always`.
  - Write the executable to a temp build dir, named stem + short hash of the full path.
  - Print the compile command with file names only, then the compiler output, then run.
- **Numbered class versions:** the course has v1/v2 pairs that share a class name and include
  guard (`Question.hpp` vs `Question1.hpp` + `Question1.cpp`). For each included header `X.h*`
  that has exactly one sibling `X<N>.h*` with its own `X<N>.cpp`:
  - If the entry file's stem ends in digits N (e.g. `QueueTest1.cpp`), use the `X<N>` variants
    from the start.
  - Otherwise try the plain build first. If it fails, retry with the variants: `-include
    X<N>.hpp` and compile `X<N>.cpp` in place of `X.cpp`. If the retry succeeds, print a
    yellow note ("Plain build failed; built with the numbered version instead (Question.hpp ->
    Question1.hpp)") and show only that attempt's output. If both fail, show the original
    errors.
  - Never compile both versions' `.cpp` files together; that is an ODR violation.
- **File with no `main()`:** look in the same folder for drivers that include `stem.h|hpp|hh|hxx`
  and have `main()`. Run the driver if there is exactly one. If there are several, list them and
  fail. If there are none, fail with a message.
- **Headers** (`.h .hpp .hh .hxx .tpp`): run the matching source file, which may in turn lead to
  its driver.
- **Other languages:**
  - Python: `python3`.
  - JS/TS: `node`.
  - Rust: `cargo run --manifest-path` when a `Cargo.toml` exists above the file (add `--bin` for
    `src/bin/x.rs`); otherwise `rustc --edition 2021`.
  - `bash` for `.sh` and for files that start with `#!`.
  - `open` for `.html`, `.htm` and `.svg`.
  - java, go, ruby, php, perl, lua, Rscript: if the tool is missing, print a clear "install X"
    message instead of crashing.
  - Drop the Windows-only cases: `.ps1` (unless `pwsh` exists), `.bat/.cmd`, `.cs`.
  - Unknown extension: "Don't know how to run '.xyz' files."
- **Colors:** dim gray for the command line, yellow for notes, red for errors.

## Mac-specific notes

- **Bash version:** `/bin/bash` on macOS is 3.2. Avoid `mapfile`, associative arrays,
  `${x,,}` and `globstar`, or write the script for zsh.
- **BSD tools:** grep and sed are the BSD versions. Use `grep -E`, not `grep -P`. Use
  `sed -i ''`. Hash with `md5 -q` or `shasum`.
- **Compiler:** use the Xcode Command Line Tools (`xcode-select --install`). There `g++` is
  clang, which is fine and supports `-include`.
- **Paths:** use `${TMPDIR:-/tmp}/zed-run` for the build dir. Executables get no `.exe`.
- **Task shell:** Zed runs tasks through the login shell (usually zsh). Check that the task
  doesn't print a shell banner and that Homebrew tools are on `PATH`.
- **F5 on Mac keyboards** is a media key, so it needs fn+F5 unless "Use F1, F2, etc. keys as
  standard function keys" is on. Ask Ahad whether to also add a second binding. Check
  conflicts first with `zed: open default keymap`.
- **After editing `tasks.json`, restart Zed.** Gutter ▶ icons cache the resolved task per
  buffer, so a stale icon keeps running the old task until restart.

## Optional custom build for the toolbar ▶

1. Clone Zed and check out the latest **stable** release tag rather than the old commit.
   - `git apply --3way zed-run-button.patch`.
   - If it conflicts, re-apply by hand. The patch is three small hunks: a const, a
     `run_button` before `assistant_button`, and `.children(run_button)` first in the
     `h_flex()` chain.
2. Follow `docs/src/development/macos.md` in the clone for prerequisites (full Xcode for Metal,
   rustup, and so on).
3. Build a release `.app`. Check `script/bundle-mac` in the clone for its current options.
   Build with the stable release channel.
4. Back up `/Applications/Zed.app` before replacing it. Add `"auto_update": false` to
   `~/.config/zed/settings.json`.
5. Every future Zed update means re-applying the patch and rebuilding.

## Test plan

`test-files/` is a copy of the course's `code/` folder. Don't edit these files:
`buffer/Buffer.cpp` leaks memory on purpose for a Valgrind lesson. For realism, copy them into
a folder whose name has a space, then run the runner on:

| File | Expected |
| --- | --- |
| `RectangleTest.cpp` | Plain build (with `Rectangle.cpp`), runs. |
| `QuestionTest.cpp` | Reads keyboard input interactively. |
| `QuestionChoiceQuestionTest.cpp`, `RectangleSquareTest.cpp` | Plain build fails, falls back to `Question1`/`Rectangle1` with the yellow note, runs. |
| `QueueTest1.cpp` | Uses `Queue1.hpp`/`Queue1.cpp` from the start. |
| `buffer/main.cpp` | Builds with `Buffer.cpp`, runs. |
| `buffer/Buffer.cpp` | No `main()`, so it runs `main.cpp`. |
| `Question.cpp` | No `main()` and two drivers, so it lists `QuestionChoiceQuestionTest.cpp` and `QuestionTest.cpp`. |
| `Question.hpp` | Goes to `Question.cpp`, which lists the same drivers. |
| `printReverse.cpp` | Runs. |
| a `.py` with `input()`, a `.sh` | Run interactively. |
| `ThankYouFade.java` | Runs, or prints "install a JDK". |

Then in Zed, check the gutter ▶ next to `int main()`, F5, and the toolbar ▶ if built.

When done, save a memory on the Mac describing the setup and how to undo it.
