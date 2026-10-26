# run-file.ps1 - compile (if needed) and run one file, for Zed's "Run current file" task.
# Usage: powershell -NoProfile -ExecutionPolicy Bypass -File run-file.ps1 [path-to-file]
# With no path it runs $env:ZED_FILE, which Zed sets for tasks (passing the path as a task
# argument breaks on folders with spaces, like "Fall 2026").
#
# C/C++: compiles the file plus the matching .cpp/.c of every local header it #includes
# (QuestionTest.cpp -> Question.cpp). If that build fails and a header has a numbered
# variant (Question.hpp -> Question1.hpp + Question1.cpp), it retries with the variant
# force-included. A file without main() runs the one driver in its folder that includes it.
# Executables go to %TEMP%\zed-run; programs run from the source file's folder.
param([string]$File = $env:ZED_FILE)

$ErrorActionPreference = 'Stop'
$BuildDir = Join-Path $env:TEMP 'zed-run'
$CSources = @('.c')
$CppSources = @('.cpp', '.cc', '.cxx', '.c++')
$MainPattern = '(?m)^[^/\r\n]*\b(int|auto|void)\s+main\s*\('
$ExitCode = 0

function Say([string]$Text, [string]$Color = 'DarkGray') { Write-Host $Text -ForegroundColor $Color }
function Fail([string]$Text, [int]$Code = 1) { Say $Text 'Red'; exit $Code }
function Has([string]$Command) { [bool](Get-Command $Command -ErrorAction SilentlyContinue) }
function Require([string]$Command, [string]$Hint) {
    if (-not (Has $Command)) { Fail "'$Command' is not on PATH. $Hint" }
}

function Quote([string[]]$Items) {
    ($Items | ForEach-Object { if ($_ -match '\s') { "`"$_`"" } else { $_ } }) -join ' '
}

# Runs a program attached to this console (so cin/input() work) and records its exit code.
# Call it as a statement: capturing its output would turn the program's stdout into a pipe.
function Invoke-Program([string]$Program, [string[]]$Arguments = @(), [switch]$Quiet) {
    if (-not $Quiet) { Say ('> ' + (Quote (@((Split-Path $Program -Leaf)) + $Arguments))) }
    & $Program @Arguments
    $script:ExitCode = $LASTEXITCODE
}

function Get-LocalIncludes([string]$Path) {
    $dir = Split-Path $Path
    $text = Get-Content -LiteralPath $Path -Raw
    if (-not $text) { return }
    foreach ($m in [regex]::Matches($text, '(?m)^\s*#\s*include\s*"([^"]+)"')) {
        $candidate = Join-Path $dir $m.Groups[1].Value
        if (Test-Path -LiteralPath $candidate) { (Resolve-Path -LiteralPath $candidate).Path }
    }
}

# Entry file plus the source file that implements each local header it (transitively) includes.
function Get-CompanionSources([string]$Entry, [string[]]$SourceExts) {
    $sources = [System.Collections.Generic.List[string]]::new()
    $seen = [System.Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
    $queue = [System.Collections.Generic.Queue[string]]::new()
    $sources.Add($Entry); [void]$seen.Add($Entry); $queue.Enqueue($Entry)
    while ($queue.Count -gt 0) {
        foreach ($header in @(Get-LocalIncludes $queue.Dequeue())) {
            if (-not $seen.Add($header)) { continue }
            $queue.Enqueue($header)
            $base = Join-Path (Split-Path $header) ([IO.Path]::GetFileNameWithoutExtension($header))
            foreach ($ext in $SourceExts) {
                $source = $base + $ext
                if ((Test-Path -LiteralPath $source) -and $seen.Add($source)) {
                    $sources.Add($source); $queue.Enqueue($source)
                }
            }
        }
    }
    return , $sources.ToArray()
}

# For each included header X.hpp with exactly one sibling X<N>.hpp that has its own X<N>.cpp:
# the variant header to force-include and which source file it replaces.
function Get-HeaderVariants([string[]]$Sources, [string[]]$SourceExts, [string]$Digits) {
    $suffix = if ($Digits) { [regex]::Escape($Digits) } else { '\d+' }
    $variants = @()
    foreach ($header in @($Sources | ForEach-Object { Get-LocalIncludes $_ } | Select-Object -Unique)) {
        $dir = Split-Path $header
        $stem = [IO.Path]::GetFileNameWithoutExtension($header)
        $ext = [IO.Path]::GetExtension($header)
        $found = @(Get-ChildItem -LiteralPath $dir -File | Where-Object {
                $_.Extension -eq $ext -and $_.BaseName -match ('^' + [regex]::Escape($stem) + $suffix + '$')
            })
        if ($found.Count -ne 1) { continue }
        $variantBase = Join-Path $dir $found[0].BaseName
        $variantSource = $SourceExts | ForEach-Object { $variantBase + $_ } |
            Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
        if ($variantSource) {
            $variants += [pscustomobject]@{ Header = $found[0].FullName; Source = $variantSource; Original = Join-Path $dir $stem }
        }
    }
    return , $variants
}

# Compiles $Sources to $Exe with each variant header force-included in place of the original
# header's source file. Returns the command line shown to the user, exit code and output.
function Invoke-Compiler([string]$Compiler, [string[]]$Flags, [string[]]$Sources, [object[]]$Variants, [string]$Exe) {
    $ccArgs = @() + $Flags
    $list = @($Sources)
    foreach ($v in $Variants) {
        $ccArgs += @('-include', $v.Header)
        $originals = @($CppSources + $CSources | ForEach-Object { $v.Original + $_ })
        $list = @($list | Where-Object { $originals -notcontains $_ })
        if ($list -notcontains $v.Source) { $list += $v.Source }
    }
    $psi = [Diagnostics.ProcessStartInfo]::new((Get-Command $Compiler).Source, (Quote ($ccArgs + $list + @('-o', $Exe))))
    $psi.UseShellExecute = $false
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError = $true
    $psi.StandardOutputEncoding = [Text.Encoding]::UTF8
    $psi.StandardErrorEncoding = [Text.Encoding]::UTF8
    $process = [Diagnostics.Process]::Start($psi)
    $stdout = $process.StandardOutput.ReadToEndAsync()
    $stderr = $process.StandardError.ReadToEnd()
    $process.WaitForExit()
    [pscustomobject]@{
        Command = '> ' + (Quote (@($Compiler) + $ccArgs + @($list | ForEach-Object { Split-Path $_ -Leaf })))
        Code    = $process.ExitCode
        Output  = ($stdout.Result + $stderr).TrimEnd()
    }
}

function Build-C([string]$Entry, [bool]$IsCpp) {
    $sourceExts = if ($IsCpp) { $CppSources } else { $CSources }
    $compiler = if ($IsCpp) { 'g++' } else { 'gcc' }
    $std = if ($IsCpp) { '-std=c++17' } else { '-std=c17' }
    Require $compiler 'Install MSYS2 g++/gcc and add its bin folder to PATH.'

    if ((Get-Content -LiteralPath $Entry -Raw) -notmatch $MainPattern) {
        # No main(): run the single driver in this folder that includes this file's header.
        $leaf = Split-Path $Entry -Leaf
        $include = '(?m)^\s*#\s*include\s*"' + [regex]::Escape([IO.Path]::GetFileNameWithoutExtension($Entry)) + '\.(h|hpp|hh|hxx)"'
        $drivers = @(Get-ChildItem -LiteralPath (Split-Path $Entry) -File | Where-Object {
                $sourceExts -contains $_.Extension.ToLower() -and $_.FullName -ne $Entry
            } | Where-Object {
                $text = Get-Content -LiteralPath $_.FullName -Raw
                $text -match $include -and $text -match $MainPattern
            })
        if ($drivers.Count -eq 0) { Fail "$leaf has no main() and no file in this folder that includes it does." }
        if ($drivers.Count -gt 1) { Fail ("$leaf has no main(). Run one of its drivers instead: " + (($drivers | ForEach-Object Name) -join ', ')) }
        Say "$leaf has no main(); running $($drivers[0].Name)" 'Yellow'
        $Entry = $drivers[0].FullName
    }

    New-Item -ItemType Directory -Force $BuildDir | Out-Null
    $stem = [IO.Path]::GetFileNameWithoutExtension($Entry)
    $bytes = [Security.Cryptography.MD5]::Create().ComputeHash([Text.Encoding]::UTF8.GetBytes($Entry.ToLower()))
    $exe = Join-Path $BuildDir ("$stem-" + (-join ($bytes[0..3] | ForEach-Object { $_.ToString('x2') })) + '.exe')
    $flags = @($std, '-Wall', '-Wextra', '-g', '-fdiagnostics-color=always')
    $sources = Get-CompanionSources $Entry $sourceExts

    # QueueTest1.cpp uses the "1" variants (Queue1.hpp/.cpp) from the start; others only when
    # the plain build fails (ChoiceQuestion's `override` needs the virtual Question1.hpp).
    $variants = @()
    $fallback = @()
    if ($stem -match '(\d+)$') { $variants = Get-HeaderVariants $sources $sourceExts $Matches[1] }
    if ($variants.Count -eq 0) { $fallback = Get-HeaderVariants $sources $sourceExts '' }
    $build = Invoke-Compiler $compiler $flags $sources $variants $exe
    if ($build.Code -ne 0 -and $fallback.Count -gt 0) {
        $retry = Invoke-Compiler $compiler $flags $sources $fallback $exe
        if ($retry.Code -eq 0) {
            $swapped = ($fallback | ForEach-Object { "$(Split-Path $_.Original -Leaf)$([IO.Path]::GetExtension($_.Header)) -> $(Split-Path $_.Header -Leaf)" }) -join ', '
            Say "Plain build failed; built with the numbered version instead ($swapped)" 'Yellow'
            $build = $retry
        }
    }
    Say $build.Command
    if ($build.Output) { Write-Host $build.Output }
    if ($build.Code -ne 0) { Fail 'Build failed.' $build.Code }
    Write-Host ''
    Invoke-Program $exe -Quiet
    exit $script:ExitCode
}

function Find-Up([string]$Start, [string]$Name) {
    $dir = $Start
    while ($dir) {
        $candidate = Join-Path $dir $Name
        if (Test-Path -LiteralPath $candidate) { return $candidate }
        $parent = Split-Path $dir
        if (-not $parent -or $parent -eq $dir) { break }
        $dir = $parent
    }
    return $null
}

function Get-Bash {
    foreach ($path in "$env:ProgramFiles\Git\bin\bash.exe", 'C:\msys64\usr\bin\bash.exe') {
        if (Test-Path -LiteralPath $path) { return $path }
    }
    Fail 'No Git Bash or MSYS2 bash found to run shell scripts.'
}

# Runs an interpreter on the file, or explains what to install.
function Invoke-Interpreter([string]$Command, [string]$Hint, [string[]]$Arguments) {
    Require $Command $Hint
    Invoke-Program $Command $Arguments
    exit $script:ExitCode
}

if (-not $File) { Fail 'No file to run: pass a path, or run this from Zed with a file open.' }
if (-not (Test-Path -LiteralPath $File -PathType Leaf)) { Fail "No such file: $File (is it saved?)" }
$File = (Resolve-Path -LiteralPath $File).Path
$dir = Split-Path $File
$name = Split-Path $File -Leaf
$ext = [IO.Path]::GetExtension($File).ToLower()
Set-Location -LiteralPath $dir

switch -Regex ($ext) {
    '^\.(cpp|cc|cxx|c\+\+)$' { Build-C $File $true }
    '^\.c$' { Build-C $File $false }
    '^\.(h|hpp|hh|hxx|tpp)$' {
        # A header can't run by itself; run the source file (or its driver) that goes with it.
        $base = Join-Path $dir ([IO.Path]::GetFileNameWithoutExtension($File))
        $source = @($CppSources + $CSources) | ForEach-Object { $base + $_ } |
            Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
        if (-not $source) { Fail "$name is a header with no matching source file to run." }
        Build-C $source ($CppSources -contains [IO.Path]::GetExtension($source).ToLower())
    }
    '^\.pyw?$' {
        $python = if (Has 'python') { 'python' } else { 'py' }
        Invoke-Interpreter $python 'Install Python to run .py files.' @($name)
    }
    '^\.(js|mjs|cjs|ts|mts|cts)$' { Invoke-Interpreter 'node' 'Install Node.js to run JavaScript/TypeScript.' @($name) }
    '^\.rs$' {
        $manifest = Find-Up $dir 'Cargo.toml'
        if ($manifest) {
            $cargoArgs = @('run', '--manifest-path', $manifest)
            if ((Split-Path $dir -Leaf) -eq 'bin') { $cargoArgs += @('--bin', [IO.Path]::GetFileNameWithoutExtension($File)) }
            Invoke-Interpreter 'cargo' 'Install Rust (rustup) to run .rs files.' $cargoArgs
        }
        Require 'rustc' 'Install Rust (rustup) to run .rs files.'
        New-Item -ItemType Directory -Force $BuildDir | Out-Null
        $exe = Join-Path $BuildDir ([IO.Path]::GetFileNameWithoutExtension($File) + '.exe')
        Invoke-Program 'rustc' @('--edition', '2021', $name, '-o', $exe)
        if ($ExitCode -ne 0) { Fail 'Build failed.' $ExitCode }
        Write-Host ''
        Invoke-Program $exe -Quiet
        exit $ExitCode
    }
    '^\.java$' { Invoke-Interpreter 'java' 'Install a JDK (11+) to run .java files.' @($name) }
    '^\.cs$' {
        if (-not (Has 'dotnet') -or -not (& dotnet --list-sdks)) { Fail 'Running .cs files needs the .NET 10 SDK (only the .NET runtime is installed).' }
        Invoke-Interpreter 'dotnet' '' @('run', $name)
    }
    '^\.go$' { Invoke-Interpreter 'go' 'Install Go to run .go files.' @('run', $name) }
    '^\.rb$' { Invoke-Interpreter 'ruby' 'Install Ruby to run .rb files.' @($name) }
    '^\.php$' { Invoke-Interpreter 'php' 'Install PHP to run .php files.' @($name) }
    '^\.pl$' { Invoke-Interpreter 'perl' 'Install Perl to run .pl files.' @($name) }
    '^\.lua$' { Invoke-Interpreter 'lua' 'Install Lua to run .lua files.' @($name) }
    '^\.r$' { Invoke-Interpreter 'Rscript' 'Install R to run .R files.' @($name) }
    '^\.sh$' { Invoke-Interpreter (Get-Bash) '' @($name) }
    '^\.ps1$' { Invoke-Interpreter 'powershell.exe' '' @('-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', $File) }
    '^\.(bat|cmd)$' { Invoke-Interpreter 'cmd.exe' '' @('/c', $File) }
    '^\.(html?|svg)$' { Say "Opening $name in your browser"; Start-Process -FilePath $File; exit 0 }
    default {
        if ((Get-Content -LiteralPath $File -TotalCount 1) -match '^#!') { Invoke-Interpreter (Get-Bash) '' @($name) }
        Fail "Don't know how to run '$ext' files."
    }
}
