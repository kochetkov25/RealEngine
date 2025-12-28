# RealEngine Build Script for Windows (PowerShell)
# Usage:
#   .\build.ps1                    # Build with default settings (Release)
#   .\build.ps1 -Config Debug      # Build in Debug mode
#   .\build.ps1 -Clean             # Clean build directory before building
#   .\build.ps1 -ConfigureOnly     # Only run CMake configuration, don't build
#   .\build.ps1 -VerboseBuild      # Show verbose build output

param(
    [Parameter()]
    [ValidateSet("Debug", "Release", "RelWithDebInfo", "MinSizeRel")]
    [string]$Config = "Release",
    
    [switch]$Clean,
    [switch]$ConfigureOnly,
    [switch]$VerboseBuild
)

$ErrorActionPreference = "Stop"

$BUILD_DIR = "build"
$SCRIPT_DIR = Split-Path -Parent $MyInvocation.MyCommand.Path

# Change to script directory
Set-Location $SCRIPT_DIR

Write-Host "=== RealEngine Build Script ===" -ForegroundColor Cyan
Write-Host "Build Type: $Config" -ForegroundColor Yellow
Write-Host "Build Directory: $BUILD_DIR" -ForegroundColor Yellow
Write-Host ""

# Clean build directory if requested
if ($Clean) {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    if (Test-Path $BUILD_DIR) {
        Remove-Item -Recurse -Force $BUILD_DIR
        Write-Host "Build directory cleaned." -ForegroundColor Green
    } else {
        Write-Host "Build directory does not exist, skipping clean." -ForegroundColor Gray
    }
    Write-Host ""
}

# Get CPU count for parallel builds
$CPU_COUNT = [Environment]::ProcessorCount
Write-Host "Using $CPU_COUNT parallel jobs" -ForegroundColor Gray
Write-Host ""

# Configure CMake
Write-Host "Configuring CMake..." -ForegroundColor Cyan

# Check if Ninja is available (preferred for compile_commands.json)
$useNinja = $false
if (Get-Command ninja -ErrorAction SilentlyContinue) {
    $useNinja = $true
    Write-Host "Using Ninja generator (better for compile_commands.json)" -ForegroundColor Gray
} else {
    Write-Host "Note: Ninja not found. Visual Studio generator will be used." -ForegroundColor Yellow
    Write-Host "      compile_commands.json may not be generated with Visual Studio generator." -ForegroundColor Yellow
}

$cmakeArgs = @(
    "-S", ".",
    "-B", $BUILD_DIR,
    "-DCMAKE_BUILD_TYPE=$Config",
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
)

# Use Ninja generator if available
if ($useNinja) {
    $cmakeArgs += "-G", "Ninja"
}

# Run CMake - use try-catch to handle warnings gracefully
$oldErrorAction = $ErrorActionPreference
$ErrorActionPreference = "Continue"

try {
    # Run cmake and let output flow normally
    & cmake @cmakeArgs
    $cmakeExitCode = $LASTEXITCODE
} catch {
    # If an exception was thrown, it's likely just a warning
    # Check the actual exit code instead
    $cmakeExitCode = $LASTEXITCODE
    if ($cmakeExitCode -eq 0) {
        # Exit code is 0, so it was just a warning - continue
        $cmakeExitCode = 0
    }
} finally {
    $ErrorActionPreference = $oldErrorAction
}

# Check exit code - only fail on actual errors, not warnings
if ($cmakeExitCode -ne 0) {
    Write-Host "CMake configuration failed with exit code $cmakeExitCode!" -ForegroundColor Red
    exit $cmakeExitCode
}

Write-Host "CMake configuration completed successfully." -ForegroundColor Green

# Copy compile_commands.json to project root for clangd
$compileCommandsBuild = Join-Path $BUILD_DIR "compile_commands.json"
$compileCommandsRoot = Join-Path $SCRIPT_DIR "compile_commands.json"

if (Test-Path $compileCommandsBuild) {
    Copy-Item -Path $compileCommandsBuild -Destination $compileCommandsRoot -Force
    Write-Host "Copied compile_commands.json to project root for clangd." -ForegroundColor Gray
} else {
    Write-Host "Warning: compile_commands.json not found in build directory." -ForegroundColor Yellow
    Write-Host "         This is normal for Visual Studio generators." -ForegroundColor Yellow
    Write-Host "         To generate compile_commands.json, install Ninja and rebuild:" -ForegroundColor Yellow
    Write-Host "         choco install ninja  (or: winget install Ninja-build.Ninja)" -ForegroundColor Cyan
    Write-Host "         Then run: .\build.ps1 -Clean" -ForegroundColor Cyan
}

Write-Host ""

# Build if not configure-only
if (-not $ConfigureOnly) {
    Write-Host "Building project..." -ForegroundColor Cyan
    
    $buildArgs = @(
        "--build", $BUILD_DIR,
        "--parallel", $CPU_COUNT,
        "--config", $Config
    )
    
    if ($VerboseBuild) {
        $buildArgs += "--verbose"
    }
    
    # Run build - use try-catch to handle warnings gracefully
    $oldErrorAction = $ErrorActionPreference
    $ErrorActionPreference = "Continue"
    
    try {
        # Run cmake build and let output flow normally
        & cmake @buildArgs
        $buildExitCode = $LASTEXITCODE
    } catch {
        # If an exception was thrown, it's likely just a warning
        # Check the actual exit code instead
        $buildExitCode = $LASTEXITCODE
        if ($buildExitCode -eq 0) {
            # Exit code is 0, so it was just a warning - continue
            $buildExitCode = 0
        }
    } finally {
        $ErrorActionPreference = $oldErrorAction
    }
    
    # Check exit code - only fail on actual errors, not warnings
    if ($buildExitCode -ne 0) {
        Write-Host "Build failed with exit code $buildExitCode!" -ForegroundColor Red
        exit $buildExitCode
    }
    
    Write-Host ""
    Write-Host "Build completed successfully!" -ForegroundColor Green
    
    # Try to find the executable (location varies by generator)
    $exePaths = @(
        "$BUILD_DIR\bin\$Config\RealEngine.exe",
        "$BUILD_DIR\bin\RealEngine.exe",
        "$BUILD_DIR\$Config\RealEngine.exe"
    )
    
    $exeFound = $false
    foreach ($path in $exePaths) {
        if (Test-Path $path) {
            Write-Host "Executable location: $path" -ForegroundColor Cyan
            $exeFound = $true
            break
        }
    }
    
    if (-not $exeFound) {
        Write-Host "Note: Executable location may vary by build generator" -ForegroundColor Gray
    }
} else {
    Write-Host "Configuration only mode - skipping build." -ForegroundColor Yellow
}

Write-Host ""
Write-Host "=== Build Script Complete ===" -ForegroundColor Cyan
