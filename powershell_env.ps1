~\qnx710\qnxsdp-env.bat | .{ process { 
    if ($_ -match 'set ([^=]+)=(.*)') {
        Write-Host $matches[1]
        setx $matches[1] $matches[2];
    }
}}
