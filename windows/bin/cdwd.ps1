if ($args.Count -gt 1) {
  Write-Error "Incorrect Usage"
  Write-Output "Usage: $PSCommandPath [name]"
  Write-Output "Change working directory to the saved path with name <name> (default 'unnamed')."
  exit 1
}

$wdmgr_dir = "$Env:USERPROFILE\.wdmgr"

if ($args.Count -gt 0) {
  $name = $args[0]
}
else {
  $name = "unnamed"
}

if (Test-Path "$wdmgr_dir\$name") {
  $dest = Get-Content "$wdmgr_dir\$name"
}
else {
  Write-Error "No path saved with name '$name'"
  exit 1
}

cd $dest
