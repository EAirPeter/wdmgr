if ($args.Count -gt 1) {
  Write-Error "Incorrect Usage"
  Write-Output "Usage: $PSCommandPath [name]"
  Write-Output "Save the current working directory as <name> (default 'unnamed')."
  exit 1
}

$wdmgr_dir = "$Env:USERPROFILE\.wdmgr"

if ($args.Count -gt 0) {
  $name = $args[0]
}
else {
  $name = "unnamed"
}

if (!(Test-Path $wdmgr_dir)) {
  (New-Item -ItemType Directory -Force -Path $wdmgr_dir) -and
    ("$(Get-Location)" | Out-File -Encoding oem "$wdmgr_dir\$name")
}
else {
  ("$(Get-Location)" | Out-File -Encoding oem "$wdmgr_dir\$name")
}
