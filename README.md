# dst

Single-process scalable dynamic system monitor for dwm (and others) based upon libuv.

## Dependencies
- libuv
- xkbfile

## Compile
On Nix
```
nix-build .
```

Elsewhere
```
$ cmake .
$ make
$ make test
```

## Install

On Nix
```
nix-env -i dst-dev -f default.nix
```

Elsewhere
```
make install
```
