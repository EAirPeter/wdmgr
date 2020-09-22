if ($args.Count -ne 0) {
  Write-Error "Incorrect Usage"
  Write-Output "Usage: $PSCommandPath [name]"
  Write-Output "  List all saved directories."
  exit 1
}

$wdmgr_dir = "$Env:USERPROFILE\_wdmgr"

if ($args.Count -gt 0) {
  $name = $args[0]
}
else {
  $name = "unnamed"
}

$names = @()
$contents = @()
$maxlen = 0

if (Test-Path $wdmgr_dir) {
  foreach ($file in Get-ChildItem $wdmgr_dir) {
    $name = $file.Name
    $names += $name
    $contents += Get-Content $file.FullName
    if ($name.Length -gt $maxlen) {
      $maxlen = $name.Length
    }
  }
}

if ($names.Length -gt 0) {
  $fmt = "{{0,{0}}} {{1}}" -f $maxlen
  for ($i = 0; $i -lt $names.Count; ++$i) {
    Write-Output ($fmt -f $names[$i], $contents[$i])
  }
}
else {
  Write-Output "(Empty)"
}
