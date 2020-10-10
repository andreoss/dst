# dst

Dynamic system monitor for dwm (and other X11 window managers), based on libuv.

## Usage

`dst` periodically updates the name of the X11 root window with a report line.

For [dwm](https://dwm.suckless.org/ "dwm") it is enough to have a `dst` process
started.

For [stumpwm](https://stumpwm.github.io/ "stumpwm") `root-name` should be added
in `*screen-mode-line-format*` explicitly.

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
- libX11
- libxkbfile
- cmocka (tests)

## Build

With Nix:

```
nix build .
```

With CMake:

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Install

With Nix:

```
nix profile install .
```

With CMake:

```
cmake --install build
```
