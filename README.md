wdmgr - Working Directory Manager
=================================

## Usage
* Put `<os>/bin/*` into some directory in your `PATH` environment variable. Or add `<os>/bin` to
  `PATH`.
* Add content of `unix/bashrc` to your bash profile (typically `~/.bashrc`) if you are using
  unix-like operating system.
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
