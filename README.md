# dst
[![Build Status](https://travis-ci.com/andreoss/dst.svg?branch=master)](https://travis-ci.com/andreoss/dst)

Single-pid scalable dynamic system monitor for dwm (and others) based upon libuv.

## Usage

dst periodically updates name of X11's root window with a report line.

For [dwm](https://dwm.suckless.org/ "dwm") it's just enough to have a dst process started.

For [stumpwm](https://stumpwm.github.io/ "stumpwm") `root-name` should be added in `*screen-mode-line-format*` explicitly.

```lisp
(setf *screen-mode-line-format*
      (list ".... ^>" '(:eval (root-name))))
(defun root-name ()
  (let* ((screen (current-screen))
         (selwin (screen-focus-window (current-screen)))
         (root (screen-root screen)))
    (utf8-to-string
     (xlib:get-property root
                        :wm_name
                        :result-type '(vector (unsigned-byte 8))))))
```

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
