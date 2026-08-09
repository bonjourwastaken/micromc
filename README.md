# micromc
Lightweight and self-contained Minecraft clone written in C23 using OpenGL
## Build instructions
Run the following commands in order to build the project:
```bash
  nix develop
  meson setup build --buildtype release
  ninja -C build
```
A binary called ``micromc`` will then be located in the ``build/`` directory.

## License
This project is licensed under the zlib license. Please see [LICENSE.md](LICENSE.md) for more information.
