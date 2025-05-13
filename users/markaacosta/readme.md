# qmk userspace

> shared userspace / functionality for qmk-supported boards

as seen [here](https://docs.qmk.fm/feature_userspace#userspace-sharing-code-between-keymaps), qmk supports a shared userspace for different boards / maps

## how it works

first, assume a relative project directory structure under `/users/<someuser>` like this:

```bash
├── ansi_encoder -> ../../keyboards/keychron/q1v2/ansi_encoder/
│   ├── ansi_encoder.c
│   ├── config.h
│   ├── keyboard.json
│   ├── keymaps
│   │   ├── default
│   │   │   ├── keymap.c
│   │   │   └── rules.mk
│   │   └── keychron-encoder-custom
│   │       ├── keymap.c
│   │       └── rules.mk
│   └── rules.mk
├── .clangd -> ../../keyboards/keychron/q1v2/.clangd
├── config.h
├── markaacosta.c
├── markaacosta.h
├── readme.md
└── rules.mk

5 directories, 14 files
```

### breakdown

> note - breakdown compares general userspace files to any *given* keyboard; in this case, the keychron

#### .clangd

(symlink to) file for the `clangd` lsp file, so it'll ignore diagnostics

#### ansi_encoder/config.h

has some (high-level) config for boards in that scope. otherwise not relevant

#### ansi_encoder/keymaps/keychron-encoder-custom/keymap.c

keymap for a particular board + layout. non-shared / unique configuration

#### ansi_encoder/keymaps/keychron-encoder-custom/rules.mk

rules for a particular board + layout. non-shared / unique configuration

#### config.h

shared definitions for static [configuration options](https://docs.qmk.fm/config_options)

#### markaacosta.h

shared configuration for static, personal settings like:

- enums (e.g. layer names)

#### markaacosta.c

shared configuration for functions / actual implementations of shared features

#### rules.mk

shared configuration for rules common to all your user boards

## caveats / pitfalls / caution

shared userspace is great means maintainable logic, values, and functions, while still having per-keyboard layout config; however, the tradeoff is more awareness of building general solutions in the userspace. As an example, if all boards share userspace, they must share layers as well; to get this to work, it was necessary to "get rid of" the first two (Mac) layers on the keychron, so that both / all boards simply start on main layer. This caused issues originally, since the k310 doesn't have any "sub-modes" before its main layer; without the removal of those bottom two, k310 and this config meant it wasn't possible to type anything at all on the k310, even if it compiled and could be flashed
