wdmgr - Working Directory Manager
=================================

## Why?
* You may get tired of:
  * Every time when you open up a new shell window/tab, you need to type a very long `cd` command
    (or many decomposed `cd`'s) to get to what you previously worked at.
  * Every time when you want to switch to different working directory (developing, then viewing
    document, then testing, then developing, then ...), you need to type very annoying `cd`
    commands.
* So I made these scripts to save your time and mood to do those `cd`'s.
  * It basically saves working directories with names you want and allow you to `cd` them later.

## Usage
* Put `<os>/bin/*` into some directory in your `PATH` environment variable. Or add `<os>/bin` to
  `PATH`.
* Add content of `unix/bashrc` to your bash profile (typically `~/.bashrc`) if you are using
  UNIX-like operating system.
* Command line:
```
cdwd [name]
  Change working directory to the saved path with name <name> (default 'unnamed').
shwd [name]
  Show the path with <name> (default 'unnamed').
svwd [name]
  Save the current working directory as <name> (default 'unnamed').
lswd
  List all saved directories.
rmwd [name]
  Remove the saved path <name>.
rmallwd
  Remove all saved paths.
```

## Notes
* Currently only `bash`, `CMD` and `Powershell` are supported.
* __No other dependencies!__ All you need is your shell itself, no other interpreters or compilers.
* The saved paths are located in `$HOME/.wdmgr` for UNIX-like systems and `%USERPROFILE%\_wdmgr`
  for Windows.
