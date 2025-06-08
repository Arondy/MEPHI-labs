# Список уровней оптимизации
$OptimizationLevels = "-O0", "-O2", "-Oz"

# Список режимов LTO
$LtoModes = @(
    @{ Name = "no LTO"; Flags = "" }
    @{ Name = "thin LTO"; Flags = "-flto=thin" }
    @{ Name = "full LTO"; Flags = "-flto" }
)

foreach ($Level in $OptimizationLevels) {
    foreach ($Lto in $LtoModes) {

        # --- Подготовка ---
        $ltoName = $Lto.Name
        $extraFlags = $Lto.Flags

        Write-Host "Собираю: [$Level] + [$ltoName]"

        if (Test-Path main.exe) {
            Remove-Item main.exe
        }

        # --- Формируем флаги сборки ---
        $flags = @("-fopenmp", "-mavx", "-fuse-ld=lld", "--std=c++20")

        if ($Level -ne "") { $flags += $Level }
        if ($extraFlags -ne "") { $flags += $extraFlags }

        # --- Сборка проекта ---
        $totalBuildTime = 0.0
        $buildStopwatch = [System.Diagnostics.Stopwatch]::StartNew()

        for ($i = 1; $i -le 5; $i++) {
            clang++ main.cpp equation_solver.cpp -o main.exe $flags
        }

        $buildStopwatch.Stop()
        $totalBuildTime = $buildStopwatch.Elapsed.TotalSeconds
        $averageBuildTime = $totalBuildTime / 5
        $formattedBuildTime = "{0:F2}" -f $averageBuildTime

        # --- Запуск программы ---
        $totalRunTime = 0.0

        for ($i = 1; $i -le 3; $i++) {
            $runStopwatch = [System.Diagnostics.Stopwatch]::StartNew()
            .\main.exe
            $runStopwatch.Stop()
            $totalRunTime += $runStopwatch.Elapsed.TotalSeconds
        }

        $averageRunTime = $totalRunTime / 3
        $formattedRunTime = "{0:F2}" -f $averageRunTime

        # --- Размер файла ---
        $fileInfo = Get-Item main.exe
        $fileSizeKB = $fileInfo.Length / 1KB
        $formattedFileSize = "{0:F2}" -f $fileSizeKB

        # --- Вывод результатов ---
        Write-Host "$Level $ltoName $formattedBuildTime $formattedRunTime $formattedFileSize"
    }
}