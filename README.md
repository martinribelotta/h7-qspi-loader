# STM32H7 QSPI flash loader

Binary project for run in RAM and load QSPI memory via semihosting using openocd

This project provides a firmware that can load in ram and expect file name (via semihosting command line) and offset in flash for read from host (via semihosting) and burn in QSPI flash.

![](docs/arch.png)

The control flow of burner is like this:

![](docs/flow.png)

To perform a burn in QSPI you need this project as a subproject of your program (by example in a
`burner/` directory) and do this:

```sh
make burn -C burner/ BURN_ADDR=0x00000000 BURN_FILE=path/to/my/binary-file.bin
```

This build, load in ram and run burner with `BURN_ADDR` (for flash address to store file) and
`BURN_FILE` as source file to burn in flash.
