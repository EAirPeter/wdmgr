if ($args.Count -ne 0) {
  Write-Error "Incorrect Usage"
  Write-Output "Usage: $PSCommandPath [name]"
  Write-Output "  Remove all saved paths."
  exit 1
}

$wdmgr_dir = "$Env:USERPROFILE\.wdmgr"

if ($args.Count -gt 0) {
  $name = $args[0]
}
else {
  $name = "unnamed"
}

if (Test-Path $wdmgr_dir) {
  Remove-Item $wdmgr_dir -Recurse
}
