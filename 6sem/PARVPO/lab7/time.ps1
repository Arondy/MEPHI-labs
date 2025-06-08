# Список уровней оптимизации для тестирования
$OptimizationLevels = "O0", "O1", "O2", "O3", "Ofast", "Os", "Oz"

foreach ($Level in $OptimizationLevels) {

    if (Test-Path main.exe) {
        Remove-Item main.exe
    }

    # --- Сборка проекта ---
    $totalBuildTime = 0.0
    $buildStopwatch = [System.Diagnostics.Stopwatch]::StartNew()

    for ($i = 1; $i -le 5; $i++) {
        g++ main.cpp -o main -fopenmp -mavx -$Level
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
    Write-Host "$Level $formattedBuildTime $formattedRunTime $formattedFileSize"
}