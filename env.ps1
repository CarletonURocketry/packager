~\qnx710\qnxsdp-env.bat | .{ process { 
    if ($_ -match 'set ([^=]+)=(.*)') {
        try {
            New-Item -Path Env:\$($matches[1]) -Value $($matches[2]) -ErrorAction Stop
        }
        catch {
            Write-Host $($matches[1])=$($matches[2])
            Set-Item -Path Env:\$($matches[1]) -Value $($matches[2])
        }
    }
}}
