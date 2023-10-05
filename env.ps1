~\qnx710\qnxsdp-env.bat | .{ process { 
    if ($_ -match 'set ([^=]+)=(.*)') {
        Write-Host $matches[1]
        New-Item -Path Env:\$($matches[1]) -Value $($matches[2])
    }
}}
